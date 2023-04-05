#pragma once

// This is a pointer that compbines "safe" pointer (QPointer, or set to null when object deleted) with
//   "shared" pointers (object deleted when no one is referring to it).
// So the object will be deleted when lost, but *also* can be deleted explicitly in which case
//   all the pointers to it will be set to nullptr.
// There is also some support for save/restore, using the acrhive instance to track what has been done already.
// Does NOT automatically detach.  That must be done man

// Also note there are a LOT of shenanigans here in support of deeply nested data structures.
// You can't just chase them programmatically, otherwise a linked-list of 4 million objects
//    will blow the stack out.  And the "create a new thread with a new stack" trick
//    just does not scale that high.  So we play games to avoid it.
// Each SafeCountData is stored in a global list (staticData_.idMap_) so they can be
//    saved/restored/remapped without recursively following the nest heirarchy.
// Destruction is a separate (thorny) problem - the idea here to to use the
//    FISH utility thread to defer deletion of aggregate objects so
//    nested structures are deleted in a separate thread sequentially.

#include "executionstack.h"
#include "exportfish.h"
#include "savedepth.h"
#include "shared/src/archive2.h"
#include "base/src/spinlock.h"

namespace fish {
    using namespace itasca;

    template <class T>
    class SafeCountPointer;

    template <class T>
    struct SafeCountLink {
        SafeCountLink(T *data=nullptr) : data_(data) { }
        SafeCountLink(const SafeCountLink<T> &l) : data_(l.data_), id_(l.id_) { }
        // Move operations not OK because a pointer to the original object will still be in the linked list.
        SafeCountLink<T> *prev_ = nullptr;
        SafeCountLink<T> *next_ = nullptr;
        T *data_{nullptr};
        quint64 id_{0};
    };

    template <class T>
    class SafeCountData {
        friend class SafeCountPointer<T>;
    public:
        SafeCountData(bool autoID=false);
        virtual ~SafeCountData();

        quint32 archiveInstance() const { return archiveInstance_; }
        void    archiveInstance(quint32 i) const { archiveInstance_ = i; }

        // ID's are necessary to uniquely identify shared objects by something storeable
        quint64 id() const { return id_; }
        void id(quint64 i);

        inline void claim(SafeCountLink<T> *l) const;
        inline bool release(SafeCountLink<T> *l) const;
        bool shared() const { return (base_.next_!=&base_) && (base_.next_->next_==&base_); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);

        static void reset();
        static void saveAll(Archive2 &a);
        static void restoreAll(Archive2 &a);
        static void remapAll(Archive2 &a);
        static void nextID(quint64 id) { staticData_.nextID_.store(id);  }

    private:
        struct StaticData {
            std::atomic<quint64>             nextID_{1};
            std::mutex                       lock_;
            std::unordered_map<quint64, T *> idMap_;
            quint64                          archiveInstance_{limits<quint32>::max()};
        };
        static StaticData staticData_;

        quint64 id_{0};
        mutable SafeCountLink<T> base_;
        mutable spinlock         lock_;
        mutable quint32          archiveInstance_{limits<quint32>::max()};
    };

    template <class T>
    class SafeCountPointer : public SafeCountLink<T> {
        friend class SafeCountData<T>;
    public:
        inline SafeCountPointer() {  }
        inline ~SafeCountPointer() { if (d() && d()->release(this)) deleteInBackground(); }
        inline SafeCountPointer(const SafeCountPointer<T> &o) : SafeCountLink<T>(o) { if (d()) d()->claim(this); }
        // Move operations not OK because pointer to original SafeCountLink is in the linked list!
        inline explicit SafeCountPointer(T *data) : SafeCountLink<T>(data) { if (d()) d()->claim(this); }

        inline SafeCountPointer<T> & operator=(const SafeCountPointer<T> &o);
        inline SafeCountPointer &operator=(T *o);
        inline SafeCountPointer<T> &operator=(SafeCountPointer<T> &&other);

        inline void detach() { if (d() && d()->shared()) detachPrivate(); }

        inline T &operator*() { return *d(); }
        inline const T &operator*() const { return *d(); }
        inline T *operator->() { return d(); }
        inline const T *operator->() const { return d(); }
        inline operator T *() { return d(); }
        inline operator const T *() const { return d(); }
        inline T *data() { return d(); }
        inline const T *data() const { return d(); }
        inline const T *constData() const { return d(); }

        inline bool operator==(const SafeCountPointer<T> &other) const { return d() == other.d(); }
        inline bool operator!=(const SafeCountPointer<T> &other) const { return d() != other.d(); }
        inline bool operator!() const { return !d(); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);

    private:
        void detachPrivate();
        void deleteInBackground();

        T *&d() { return reinterpret_cast<T *&>(this->data_); }
        T * const &d() const { return reinterpret_cast<T * const &>(this->data_); }
    };

    template <class T>
    SafeCountData<T>::SafeCountData(bool autoID) {
        base_.next_ = base_.prev_ = &base_;
        if (autoID)
            id(staticData_.nextID_.fetch_add(1));
    }

    template <class T>
    SafeCountData<T>::~SafeCountData() {
        if (id_) {
            std::lock_guard<std::mutex> lock(staticData_.lock_);
            auto it = staticData_.idMap_.find(id_);
            // Only if it is this thing,
            //  another thread could have created a new one now that we have deferred deletion.
            if (it!=staticData_.idMap_.end() &&
                it->second==(T *)this)
                staticData_.idMap_.erase(it);
        }
        std::lock_guard<spinlock> lock(lock_);
        while (base_.next_!=&base_) {
            {   reverse_lock<spinlock> brk(lock_);
                auto *l = base_.next_;
                release(l);
                l->data_ = nullptr;
            }
        }
    }

    template <class T>
    void SafeCountData<T>::id(quint64 i) {
        while (staticData_.nextID_.load()<id_+1)
            staticData_.nextID_.store(id_+1);
        quint64 oldID = id_;
        id_ = i;
        if (id_ || oldID) {
            std::lock_guard<std::mutex> lock(staticData_.lock_);
            if (oldID)
                staticData_.idMap_.erase(oldID);
            if (id_)
                staticData_.idMap_.insert(std::make_pair(id_, (T *)this));
        }
    }

    template <class T>
    void SafeCountData<T>::save(Archive2 &a) const {
        archiveInstance_ = a.instance();
        a.save("ID",id());

    }

    template <class T>
    bool SafeCountData<T>::restore(Archive2 &a,quint64 label) {
        if (label==enc("ID")) {
            id(a.restore<quint64>());
            return true;
        }
        return false;
    }

    template <class T>
    void SafeCountData<T>::remap(Archive2 &a) {
        a;
    }

    template <class T>
    void SafeCountData<T>::claim(SafeCountLink<T> *l) const {
        assert(l->next_==nullptr);   assert(l->prev_==nullptr);
        std::lock_guard<spinlock> lock(lock_);
        l->next_ = base_.next_;
        l->prev_ = &base_;
        l->next_->prev_ = l;
        base_.next_ = l;
        l->id_ = id();
    }

    template <class T>
    bool SafeCountData<T>::release(SafeCountLink<T> *l) const {
        assert(l->next_);  assert(l->prev_);
        l->id_ = 0;
        std::lock_guard<spinlock> lock(lock_);
        l->next_->prev_ = l->prev_;
        l->prev_->next_ = l->next_;
        l->next_ = l->prev_ = nullptr;
        return base_.next_ == &base_;
    }

    template <class T>
    void SafeCountData<T>::reset() {
        std::lock_guard<std::mutex> lock(staticData_.lock_);
        staticData_.nextID_.store(1);
        while (staticData_.idMap_.size()) {
            auto p = staticData_.idMap_.begin();
            T *t = p->second;
            staticData_.idMap_.erase(p);
            reverse_lock rlock(staticData_.lock_);
            delete t;
        }
    }

    template <class T>
    void SafeCountData<T>::saveAll(Archive2 &a) {
        struct SaveCountAll {};
        a.startObjectSave(typeid(SaveCountAll));
            // Note idMap_ gets filled on object creation, so we can't use the
            //   standard container save/restore.
            // First need to count how many haven't been saved already, and tag
            //   them as being saved already so recursive save doesn't start up again.
            std::vector<T *> toBeSaved;
            for (auto &p : staticData_.idMap_) {
                if (p.second->archiveInstance_!=a.instance()) {
                    // Check if these are somehow lost, some error happened and they aren't referred to anywhere.
                    if (p.second->base_.next_ == &p.second->base_) continue;
                    toBeSaved.push_back(p.second);
                    p.second->archiveInstance_ = a.instance();
                }
            }
            a.saveLabel("IdMap");
                a.startArraySave(toBeSaved.size(),Archive2::Type::Object);
                for (auto &p : toBeSaved) {
                    struct SafeCountDataStore {};
                    a.startObjectSave(typeid(SafeCountDataStore));
                        a.save("ID", p->id_);
                        a.saveObj("Value", *p);
                    a.stopObjectSave();
                }
                a.stopArray();
            a.save("NextID", staticData_.nextID_.load());
        a.stopObjectSave();
    }

    template <class T>
    void SafeCountData<T>::restoreAll(Archive2 &a) {
        a.startObjectRestore();
        for (quint64 label=a.restoreLabel();label!=Archive2::finish_;label=a.restoreLabel()) {
            switch (label) {
            case enc("IdMap"):
                a.startArrayRestore();
                while (a.checkArray()) {
                    quint64 id{0};
                    a.startObjectRestore();
                    for (quint64 label2 = a.restoreLabel(); label2!=Archive2::finish_; label2 = a.restoreLabel()) {
                        switch (label2) {
                        case enc("ID"):  id = a.restore<quint64>();  break;
                        case enc("Value"): {
                                T *t = new T(false);
                                a.restoreObj(*t);
                                assert(t->id()==id);
                            }
                            break;
                        default:  a.skipValue();  break;
                        }
                    }
                    a.stopObjectRestore();
                }
                break;
            case enc("NextID"):  staticData_.nextID_.store(a.restore<quint64>());  break;
            default:  a.skipValue();  break;
            }
        }
        a.stopObjectRestore();
    }

    template <class T>
    void SafeCountData<T>::remapAll(Archive2 &a) {
        for (auto &p : staticData_.idMap_)
            a.remapObj(*p.second);
    }

    template <class T>
    SafeCountPointer<T> &SafeCountPointer<T>::operator=(const SafeCountPointer<T> &o) {
        if (o.d() != d()) {
            if (d() && d()->release(this))
                deleteInBackground();
            d() = o.d();
            if (o.d())
                o.d()->claim(this);
        }
        return *this;
    }

    template <class T>
    SafeCountPointer<T> &SafeCountPointer<T>::operator=(T *o) {
        if (o != d()) {
            if (d() && d()->release(this))
                deleteInBackground();
            d() = o;
            if (d())
                d()->claim(this);
        }
        return *this;
    }

    template <class T>
    SafeCountPointer<T> &SafeCountPointer<T>::operator=(SafeCountPointer<T> &&other) {
        if (this==&other) return *this;
        if (d() && d()->release(this)) deleteInBackground();
        d() = other.d();
        if (d()) {
            d()->claim(this);
            d()->release(&other);
            other.d() = nullptr;
        }
        return *this;
    }

    template <class T>
    void SafeCountPointer<T>::save(Archive2 &a) const {
        a.save("ID",d() ? d()->id(): 0);
        bool saveNow = true;
        if (!d()) saveNow = false; // Nothing to save
        else if (d()->archiveInstance_==a.instance()) saveNow = false; // Already saved
        else if (SaveDepth::depth(a.instance()) > SaveDepth::limit_) {
            if (a.isBuffer())  // BUffers won't save the global list separately, so have to abort
                throw Exception("FISH data structure nest level too high (larger than %1)."
                                " The current format does not allow this level of nesting.",SaveDepth::limit_);
            saveNow = false;
        }
        a.saveLabel("Array");
        a.startArraySave(saveNow ? 1 : 0, Archive2::Type::Object);
            if (saveNow)
                a.saveObj(nullptr, *d());
        a.stopArray();
    }

    template <class T>
    bool SafeCountPointer<T>::restore(Archive2 &a,quint64 label) {
        if (label==itasca::enc("ID")) {
            this->id_ = a.restore<quint64>();
            return true;
        }
        // Backwards compatibility
        if (label==itasca::enc("Array")) {
            a.startArrayRestore();
            while (a.checkArray()) {
                T *t = NEWC(T(false));
                a.restoreObj(*t);
                assert(SafeCountPointer<T>::id_==t->id_);
                operator=(t);
            }
            return true;
        }
        return false;
    }

    template <class T>
    void SafeCountPointer<T>::remap(Archive2 &) {
        if (this->id_) {
            auto it = SafeCountData<T>::staticData_.idMap_.find(this->id_);
            if (it!=SafeCountData<T>::staticData_.idMap_.end())
                operator=(it->second);
        }
        // remapAll remaps the actual objects! (avoiding recursion issues in nested data structures).
    }

    template <class T>
    void SafeCountPointer<T>::detachPrivate() {
        T *x = NEWC(T());
        x->claim(this);
        if (d()->release(this))
            deleteInBackground();
        d() = x;
    }

    template <class T>
    void SafeCountPointer<T>::deleteInBackground() {
        auto *pnt = d();
        executeWithStack([pnt] { delete pnt; });
    }

} // namespace fish
// EoF
