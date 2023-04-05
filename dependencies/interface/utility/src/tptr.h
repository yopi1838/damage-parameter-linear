#pragma once
/** \file tptr.h
* \brief Utility for Tracking Pointers to IThings (or others that conform to the interface).
*
* \addtogroup utilitysrc
*/

#include "thing.h"
#include "shared/src/archive2.h"

#include "utility/interface/ithing.h"
#ifdef _WIN32
#  pragma warning(disable:4311)
#  pragma warning(disable:4312)
#endif

namespace itasca {
    /** \brief Utility for Tracking Pointers to IThings (or others that conform to the interface).
    *
    * TPtrs allow for pointer tracking in debug mode and help to simplify save/restore of pointers.
    * A TPtr or a CTPtr can be saved to an Archive & syntax, and a handle will be stored.
    * When restore with & from an Archive, the Index is loaded and stored in the pointer temporarily.
    * It is then remapped using the Archive.  Hopefully all this is done in the archive() member function.
    * TPtr and CTPtr can be used largely transparent just as if they were pointers.
    * \a T must conform to the following interface: \code  class Interface
    * {
    * public:
    *   virtual quint64 remapIndex(itasca::Archive2 &)=0;
        inline void addDeleteNotice(IDeleteNotice *delnot);
        inline void removeDeleteNotice(IDeleteNotice *delnot);
    * }; \endcode
    * See RangeElementNamed in the filterelement2.h as an example.
    * Use CTPtr<class> for a constant pointer, and TPtr<class> for a normal pointer.
    * \ingroup utilitysrc
    * @{
    */
    template <class T> class CTPtr;

    template <class T> uint32 qHash(const CTPtr<T> &in, uint32 seed = 0);

#ifdef _DEBUG
    template <class T> class CTPtr : public IDeleteNotice {
        friend class archive::Traits<CTPtr<T>>;
    public:
        /// Default constructor
        CTPtr() : ptr_(nullptr), isIndex_(false), line_(0), label_(nullptr) { }
        /// Creator taking a const pointer \a t to \a T and a pointer to character \a label.
        CTPtr(const T *t,unsigned line,const char *label);
        /// Creator taking a const pointer \a t to \a T and a pointer to character \a label.
        CTPtr(const T *t);
        /// Copy constructor, taking a const reference \a p to a tracking pointer.
        CTPtr(const CTPtr<T> &p);
        /// Destructor assures that all of the memory has been released.
        ~CTPtr() { release(); }
        /// Assignment operator taking \a p, a const reference to CTPtr<T>.
        const CTPtr<T> &operator=(const CTPtr<T> &p);
        /// Reset the CTPtr<T>.
        void reset() { release(); ptr_ = nullptr;  isIndex_=false; }
        /// Assignment operator taking \a p, a const pointer to \a T.
        const CTPtr<T> &operator=(const T *p) { release(); assign(const_cast<T *>(p));  return *this; }
        const CTPtr<T> &set(const T *p,unsigned line,const char *label);
        const CTPtr<T> &set(const T *p) { release(); assign(const_cast<T *>(p)); return *this; }
        /// Change the label designation to \a label, a const chracter pointer.
        void setLabel(const char *label) { label_=label; }
        void setLine(unsigned line) { line_=line; }

        /// Access the const pointer to \a T held in the CTPtr<T> without an assert. 
        const T * data()               const { return ptr_; }
        /// Access the const pointer to \a T held in the CTPtr<T> with an assert of the index. 
        const T * get()                const { assert(!isIndex_); return ptr_; }  ///< \copydoc get
        operator const T *() const { assert(!isIndex_); return ptr_; } ///< \copydoc get
        /// Return true if the pointer is not NULL else false.
        bool      operator!()          const { assert(!isIndex_); return ptr_ ? false : true; }
        /// Access the const pointer to \a T held in the CTPtr<T> with an assert of the index and the pointer. 
        const T * operator->()         const { assert(!isIndex_); assert(ptr_); return ptr_; }
        /// Access the const reference to \a T held in the CTPtr<T> with an assert of the index and the pointer. 
        const T & operator*()          const { assert(!isIndex_); assert(ptr_); return *ptr_; }
        bool operator==(const CTPtr<T> &in) const { return ptr_ == in.ptr_; }
        bool operator==(const T *in) const { assert(!isIndex_);  return ptr_ == in; }
        bool operator==(T *in) const { assert(!isIndex_);  return ptr_ == in; }
        bool operator==(const std::nullptr_t t) const { return ptr_ == t;  }
        bool operator<(const CTPtr<T> &in) const { return ptr_ < in.ptr_; }


        /// Archive the CTPtr<T>.
        virtual void onDelete(IThing *ptr) override;
        virtual void changeToNewPointer(IThing *oldPnt,IThing *newPnt) override;

        bool isIndex() const { return isIndex_; }
    private:
        void assign(T *t);
        void release();
        union {
            T *     ptr_;
            quint64 handle_;
        };
        bool isIndex_;
        unsigned int line_;
        const char *label_;
    };

    template <class T> 
    CTPtr<T>::CTPtr(const T *t,unsigned line,const char *label) 
                               : ptr_(nullptr), isIndex_(false), line_(line), label_(label) { 
        assign(const_cast<T *>(t)); 
    }

    /// Creator taking a const pointer \a t to \a T and a pointer to character \a label.
    template <class T> 
    CTPtr<T>::CTPtr(const T *t) 
                               : ptr_(nullptr), isIndex_(false), line_(0), label_(nullptr) { 
        assign(const_cast<T *>(t)); 
    }

    /// Copy constructor, taking a const reference \a p to a tracking pointer.
    template <class T> CTPtr<T>::CTPtr(const CTPtr<T> &p) 
                               : ptr_(nullptr), isIndex_(p.isIndex_), line_(p.line_), label_(p.label_) {
        assign(p.ptr_);
    }

    template <class T> const CTPtr<T> &CTPtr<T>::operator=(const CTPtr<T> &p) {
        T *pp = p.ptr_;  
        release(); 
        isIndex_=p.isIndex_;
        if (!label_) label_=p.label_;
        if (!line_) line_=p.line_;
        assign(pp); 
        return *this; 
    }

    template <class T> const CTPtr<T> &CTPtr<T>::set(const T *p,unsigned line,const char *label) { 
        release(); 
        if (!label_) label_ = label; 
        if (!line_) line_ = line; 
        assign(const_cast<T *>(p)); 
        return *this; 
    }

    template <class T> 
    void CTPtr<T>::onDelete(IThing *ptr) {
        assert(ptr==(IThing *)ptr_);
        qDebug() << "Tracking Pointer Failure:  Pointer " 
                 << QString::number(reinterpret_cast<quint64>(this),16)
                 << " with line " << line_
                 << " label " << label_;
        throw Exception("Tracking Pointer Failure: Pointer %1 with line %2 label %3.",QString::number(reinterpret_cast<quint64>(this),16),line_,label_);
    }

    template <class T>
    void CTPtr<T>::changeToNewPointer(IThing *,IThing *) {
        // Do nothign deliberately, because in release mode it isn't even using IDeleteNotice
    }

    template <class T> void CTPtr<T>::assign(T *t) {
        ptr_ = t;
        if (ptr_ && !isIndex_) ptr_->addDeleteNotice(this);
    }

    template <class T> void CTPtr<T>::release() {
        if (isIndex_) return;
        if (ptr_) ptr_->removeDeleteNotice(this);
        ptr_ = nullptr;
    }

#else // #ifdef _DEBUG

    template <class T> class CTPtr {
        friend class archive::Traits<CTPtr<T>>;
    public:
        /// Default constructor
        CTPtr() : ptr_(nullptr) { }
        /// Creator taking a const pointer \a t to \a T and a pointer to character \a label.
        CTPtr(const T *t) : ptr_(const_cast<T *>(t)) { }
        /// Copy constructor, taking a const reference \a p to a tracking pointer.
        CTPtr(const CTPtr<T> &p) : ptr_(p.ptr_) { }
        /// Destructor assures that all of the memory has been released.
        ~CTPtr() { }
        /// Assignment operator taking \a p, a const reference to CTPtr<T>.
        const CTPtr<T> &operator=(const CTPtr<T> &p) { ptr_ = p.ptr_; return *this; }
        /// Reset the CTPtr<T>.
        void reset() { ptr_ = nullptr; }
        /// Assignment operator taking \a p, a const pointer to \a T.
        const CTPtr<T> &operator=(const T *p) { ptr_ = const_cast<T *>(p); return *this; }
        const CTPtr<T> &set(const T *p) { ptr_ = const_cast<T *>(p); return *this; }
        //bool operator==(const CTPtr<T> &in) const;
        //bool operator<(const CTPtr<T> &in) const;
        /// Change the label designation to \a label, a const chracter pointer.
        //void setLabel(const char *) { }
        //void setLine(unsigned) { }

        /// Access the const pointer to \a T held in the CTPtr<T> without an assert. 
        const T * data()               const { return ptr_; }
        /// Access the const pointer to \a T held in the CTPtr<T> with an assert of the index. 
        const T * get()                const { return ptr_; }  ///< \copydoc get
        operator const T *() const { return ptr_; } ///< \copydoc get
        /// Return true if the pointer is not NULL else false.
        bool      operator!()          const { return ptr_ ? false : true; }
        /// Access the const pointer to \a T held in the CTPtr<T> with an assert of the index and the pointer. 
        const T * operator->()         const { return ptr_; }
        /// Access the const reference to \a T held in the CTPtr<T> with an assert of the index and the pointer. 
        const T & operator*()          const { return *ptr_; }
    private:
        union {
            T *     ptr_;
            quint64 handle_;
        };
    };
    /// @}
#endif

    /** \brief Utility for storing pointers to IThings (or others that conform to the interface).
    *
    * TPtrs allow for pointer tracking in debug mode and help to simplify save/restore of pointers.
    * A TPtr or a CTPtr can be saved to an Archive & syntax, and an Index will be stored.
    * When restore with & from an Archive, the Index is loaded and stored in the pointer temporarily.
    * It is then remapped using the Archive.  Hopefully all this is done in the archive() member function.
    * TPtr and CTPtr can be used largely transparent just as if they were pointers.
    * \a T must conform to the following interface: \code  class Interface
    * {
    * public:
    *   virtual void claim(IThingTracker *tt)=0;
    *   virtual void release(IThingTracke *tt)=0;
    * }; \endcode
    * See RangeElementNamed in the filterelement2.h as an example.
    * Use CTPtr<class> for a constant pointer, and TPtr<class> for a normal pointer.
    * \ingroup utilitysrc
    * @{
    */

    template <class T> class TPtr : public CTPtr<T> {
    public:
        /// Default contructor
        TPtr() { } 
#ifdef _DEBUG
        /// Creator taking a pointer \a t and a pointer to character \a label.
        /// \param t Pointer to \a T.
        /// \param label Const character pointer to label the TPtr<T>.
        TPtr(T *t,unsigned line,const char *label) : CTPtr<T>(t,line,label) { }
#endif
        /// Creator taking a pointer \a t and a pointer to character \a label.
        /// \param t Pointer to \a T.
        /// \param label Const character pointer to label the TPtr<T>.
        TPtr(T *t) : CTPtr<T>(t) { }
        /// Copy constructor, taking a const reference \a p to a tracking pointer.
        /// \param p Const reference to a TPtr<T>.
        TPtr(const TPtr<T> &p) : CTPtr<T>(p) { }
        /// Assignment operator taking a const reference to \a T.
        /// \param p Const reference to \a T.
        const TPtr<T> &operator=(const TPtr<T> &p) { CTPtr<T>::operator=(p); return *this; }
        /// Assignment operator taking a pointer to \a T.
        /// \param p Pointer to \a T.
        const TPtr<T> &operator=(T *p) { CTPtr<T>::operator=(p);  return *this; }
#ifdef _DEBUG
        const TPtr<T> &set(T *p,unsigned line,const char *label) { CTPtr<T>::set(p,line,label); return *this; }
#endif
        const TPtr<T> &set(T *p) { CTPtr<T>::set(p); return *this; }

        /// Access the pointer to \a T held in the TPtr<T> with an assert of the index. 
        T *      get()                      { return const_cast<T *>(CTPtr<T>::get()); }
        /// Access the const pointer to \a T held in the TPtr<T> with an assert of the index. 
        const T *get()                const { return CTPtr<T>::get(); } 
        /// Access the const pointer to \a T held in the TPtr<T> with an assert of the index. 
        operator const T *() const { return get();  } 
        /// Access the const pointer to \a T held in the TPtr<T> with an assert of the index. 
        operator T *()             { return get();  } 
        /// Access the pointer to \a T held in the TPtr<T> with an assert of the index. 
        T *      operator->()               { return get();  }
        /// Access the const pointer to \a T held in the TPtr<T> with an assert of the index and the pointer. 
        const T *operator->()         const { return CTPtr<T>::operator->(); }
        /// Access the reference to \a T held in the TPtr<T> with an assert of the index. 
        T &      operator*()                { return *get(); }
        /// Access the const reference to \a T held in the TPtr<T> with an assert of the index and the pointer. 
        const T &operator*()          const { return CTPtr<T>::operator*(); }
    };

    /// @}
    template <class T> 
    uint32 qHash(const CTPtr<T> &in,uint32 seed) { return ::qHash((void *)in.data(),seed); }
} // namespace itasca
namespace archive {
    template <typename T> 
    class Traits<itasca::CTPtr<T>> {
    public:
        static void save(itasca::Archive2 &a, const itasca::CTPtr<T> &t);
        static bool restore(itasca::Archive2 &a, itasca::CTPtr<T> &t);
        static void remap(itasca::Archive2 &a, itasca::CTPtr<T> &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::QUInt64;  }
    };

    template <typename T>
    class Traits<itasca::TPtr<T>> {
    public:
        static void save(itasca::Archive2 &a, const itasca::TPtr<T> &t);
        static bool restore(itasca::Archive2 &a, itasca::TPtr<T> &t);
        static void remap(itasca::Archive2 &a, itasca::TPtr<T> &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::QUInt64;  }
    };

    template <typename T> 
    void Traits<itasca::CTPtr<T>>::save(itasca::Archive2 &a,const itasca::CTPtr<T> &ptr) {
        uint64 index(0);
        assert(a.allowRemap());
        if (ptr && a.allowRemap()) 
            index = ptr->getRemapHandle(a);
        Traits<uint64>::save(a,index);
    }

    template <typename T> 
    bool Traits<itasca::CTPtr<T>>::restore(itasca::Archive2 &a,itasca::CTPtr<T> &ptr) {
        assert(a.allowRemap());
        Traits<uint64>::restore(a,ptr.handle_);
        ptr.handle_ = itasca::Thing::adjustHandle(ptr.handle_);
#ifdef _DEBUG
        ptr.isIndex_ = true;
#endif
        return true;
    }

#ifdef _DEBUG
    template <typename T> 
    void Traits<itasca::CTPtr<T>>::remap(itasca::Archive2 &a,itasca::CTPtr<T> &ptr) {
        assert(a.allowRemap());
        if (!a.allowRemap()) {
            ptr.isIndex_ = false;
            ptr.assign(nullptr);
            return;
        }
        if (ptr.ptr_) assert(ptr.isIndex_);
        itasca::Thing *v = itasca::Thing::lookupHandle(ptr.handle_);
        if (ptr.handle_ && !v)
            a.warning("Failed pointer handle lookup!");
        ptr.isIndex_=false;
        T *t = itasca::convert_cast<T>(v);
        if (t==nullptr && ptr.handle_>0)
            a.warning("Pointer handle lookup resulted in incompatible class! (%1) (%2)",
                      QString::number(T::type_,16),QString::number(v ? v->getType() : 0,16));
        ptr.assign(t);
    }
#else
    template <typename T> 
    void Traits<itasca::CTPtr<T>>::remap(itasca::Archive2 &a,itasca::CTPtr<T> &ptr) {
        itasca::Thing *v = itasca::Thing::lookupHandle(ptr.handle_);
        if (ptr.handle_ && !v)
            a.warning("Failed pointer index lookup!");
        T *t = itasca::convert_cast<T>(v);
        if (ptr.handle_ && t==nullptr)
            a.warning("Pointer handle lookup resulted in incompatible class!");
        ptr = t;
    }
#endif


    template <typename T> 
    void Traits<itasca::TPtr<T>>::save(itasca::Archive2 &a,const itasca::TPtr<T> &ptr) {
        Traits<itasca::CTPtr<T>>::save(a,static_cast<const itasca::CTPtr<T> &>(ptr));
    }

    template <typename T> 
    bool Traits<itasca::TPtr<T>>::restore(itasca::Archive2 &a,itasca::TPtr<T> &ptr) {
        return Traits<itasca::CTPtr<T>>::restore(a,static_cast<itasca::CTPtr<T> &>(ptr));
    }

    template <typename T> 
    void Traits<itasca::TPtr<T>>::remap(itasca::Archive2 &a,itasca::TPtr<T> &ptr) {
        Traits<itasca::CTPtr<T>>::remap(a,static_cast<itasca::CTPtr<T> &>(ptr));
    }

}

#ifndef DOXYGEN

Q_DECLARE_METATYPE(itasca::TPtr<itasca::IThing>);

namespace std {
    template <class T>
    struct hash<itasca::CTPtr<T>> {
        size_t operator()(const itasca::CTPtr<T> &in) const { std::hash<const T *> h;  return h(in.data()); }
    };
}

#endif

#ifdef _DEBUG
#  define INIPTR(x,v) x(v,__LINE__,__FILE__)
#  define SETPTR(x,v) x.set(v,__LINE__,__FILE__)
#else
#  define INIPTR(x,v) x(v)
#  define SETPTR(x,v) x.set(v)
#endif

// EoF
