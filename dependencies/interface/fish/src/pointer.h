#pragma once

#include "exportfish.h"
#include "utility/src/thing.h"
#include "utility/src/tptr.h"

namespace fish {
    using itasca::Archive2;
    using itasca::IThing;
    using itasca::Thing;
    using itasca::TPtr;

    class Pointer : public itasca::IDeleteNotice {
    public:
        Pointer() { }
        Pointer(const Pointer &fp) { assignTPtr(fp.ptr_,fp.mapped_); handle_ = fp.handle_; }
        explicit Pointer(TPtr<IThing> &tp) { assignTPtr(tp,true); }
        explicit Pointer(IThing *t) { assign(t); }
        virtual inline ~Pointer() final;
        inline const Pointer &operator=(const Pointer &fp);
        inline const Pointer &operator=(IThing *t);
        inline const Pointer &operator=(TPtr<IThing> &tp);
        inline bool operator==(const Pointer &p) const;
        inline bool operator<(const Pointer &p) const;

#ifdef _DEBUG
        inline Pointer(IThing *t,unsigned line,const char *label);
        inline const Pointer &set(IThing *t,unsigned line,const char *label);
#endif

        const IThing *get() const { assert(mapped_);  return ptr_; }  ///< \copydoc get
        IThing *get() { assert(mapped_);  return ptr_; }  ///< \copydoc get
        operator const IThing *() const { return get(); } ///< \copydoc get
        operator IThing *()  { return get(); } ///< \copydoc get
        const IThing *operator->() const { return get(); }
        IThing *operator->() { return get(); }
        const Pointer &set(IThing *t) { if (t!=ptr_.get()) { remove(); assign(t); } return *this; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
        void restoreOld(Archive2 &a);
        void fixDroppedPointer(quint64 *missed);
        //void savePointerDetails() const;

        void clear() { remove(); }
        FISH_EXPORT virtual void onDelete(IThing *ptr) override;
        FISH_EXPORT virtual void changeToNewPointer(IThing *oldPnt,IThing *newPnt) override;

        // If true, on delete pointers become "dropped" (type and id remain) instead of null.
        static void clearToDropped(bool b) { clearToDropped_ = b; }
        class ClearToDropped {
        public:
            ClearToDropped() { Pointer::clearToDropped(true); }
            ~ClearToDropped() { Pointer::clearToDropped(false); }
        };

    private:
        explicit Pointer(int) {  }
        inline void assign(const IThing *t);
        inline void assignTPtr(const TPtr<IThing> &t,bool mapped);
        inline void remove();
        void drop();

    private:
        FISH_EXPORT static bool clearToDropped_;

        TPtr<IThing>   ptr_ = INIPTR(TPtr<IThing>,nullptr);
        bool           mapped_ = true; // Pointers have to explicitly keep track of their mapped status.  Ugly but necessary.
        mutable uint64 handle_ = 0;
    };

    inline Pointer::~Pointer() {
        remove();
    }

    inline const Pointer &Pointer::operator=(const Pointer &fp) {
        if (fp.ptr_.data()!=ptr_.data()) {
            remove();
            assignTPtr(fp.ptr_,fp.mapped_);
        }
        handle_ = fp.handle_;
        return *this;
    }

    inline const Pointer &Pointer::operator=(IThing *t) {
        if (ptr_.data()==t) return *this;
        // Remove
        if (mapped_) {
            auto tO = get();
            if (tO) tO->removeDeleteNotice(this);
        }
        handle_ = 0;
        // Assign
        assert(Thing::isMapped()); // ??
        ptr_ = t;
        if (t)
            t->addDeleteNotice(this);
        return *this;
    }

    inline const Pointer &Pointer::operator=(TPtr<IThing> &tp) {
        if (ptr_.data()==tp.data()) return *this;
        // Remove
        if (mapped_) {
            auto t = get();
            if (t) t->removeDeleteNotice(this);
        }
        handle_ = 0;
        // Assign
        ptr_ = tp;
        mapped_ = true;
        if (ptr_.data())
            ptr_.get()->addDeleteNotice(this); // Note - order is important!
        return *this;
    }

#ifdef _DEBUG
    inline Pointer::Pointer(IThing *t,unsigned line,const char *label)
        : ptr_(nullptr,line,label) {
        assign(t);
        ptr_.setLine(line);
        ptr_.setLabel(label);
    }

    inline const Pointer &Pointer::set(IThing *t,unsigned line,const char *label) {
        remove();
        assign(t);
        ptr_.setLine(line);
        ptr_.setLabel(label);
        return *this;
    }
#endif

    inline bool Pointer::operator==(const Pointer &p) const {
        if (mapped_!=p.mapped_) return false;
        if (!mapped_) return ptr_.data()==p.ptr_.data();
        if (!ptr_.get()) {
            if (p.ptr_.get())
                return false;
            return true;
        } else if (!p.ptr_.get())
            return true;
        if (ptr_->getType()!=p.ptr_->getType())
            return false;
        if (ptr_->getCollectionID()!=p.ptr_->getCollectionID())
            return false;
        return ptr_->getID() == p.ptr_->getID();
    }

    inline bool Pointer::operator<(const Pointer &p) const {
        if (mapped_!=p.mapped_) return mapped_;
        if (!mapped_) return ptr_.data() < p.ptr_.data();
        if (!ptr_.get()) {
            if (p.ptr_.get())
                return true;
            return false;
        } else if (!p.ptr_.get())
            return false;
        if (ptr_->getType()!=p.ptr_->getType())
            return ptr_->getType() < p.ptr_->getType();
        if (ptr_->getCollectionID()!=p.ptr_->getCollectionID())
            return ptr_->getCollectionID() < p.ptr_->getCollectionID();
        return ptr_->getID() < p.ptr_->getID();
    }

    inline void Pointer::assign(const IThing *t) {
        assert(Thing::isMapped()); // ??
        auto t2 = const_cast<IThing *>(t);
        ptr_ = t2;
        if (t2)
            t2->addDeleteNotice(this);
    }

    inline void Pointer::assignTPtr(const TPtr<IThing> &ptr,bool mapped) {
        ptr_ = ptr;
        mapped_ = mapped;
        if (ptr_.data() && mapped_)
            ptr_.get()->addDeleteNotice(this); // Note - order is important!
    }

    inline void Pointer::remove() {
        if (!mapped_) {
            ptr_ = nullptr;
            handle_ = 0;
            return;
        }
        auto t = get();
        if (!t) return;
        t->removeDeleteNotice(this); // Order is important!
        ptr_ = nullptr;
        handle_ = 0;
    }

} // namespace fish

Q_DECLARE_METATYPE(fish::Pointer);

// EoF

