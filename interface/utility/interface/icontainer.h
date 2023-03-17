#pragma once
/// \file icontainer.h
/// \brief Interface for container of IThings.

#include "base/src/baseqt.h"
#include "isignalbase.h"
#include "ithing.h"


namespace itasca {
    class IThing;

    /** \brief Interface for containers of IThings.

    * A container, derived from IThing, contains a set of IThings.  
    * This interface provides methods to add, remove and erase an IThing.  
    * In addition, one can search for IThings via their names (as defined by the implementation) 
    * or unique integer IDs.  A container holds a small granularity callback utility so that other 
    * methods can be invoked whenever an IThing is added, removed or erased.   
    * \ingroup utility
    */

    class IContainer  {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x51cdf4ed;

        /// Defines the Signal2<> type used for removed, added, and destroy signals sent from the container.
        typedef Signal2<const IThing *,const IContainer *> signal_type;

        /// Interface class for iterating through the container that has a null implementation.
        class IOpaqueIterator { };

        /// Returns the IThing pointer of the container. 
        virtual IThing *      getIThing()=0;
        /// Returns the const IThing pointer of the container. 
        virtual const IThing *getIThing() const=0;
        /// Returns the IThing pointer of the container. 
        virtual IContainer *      getIContainer()=0;
        /// Returns the const IThing pointer of the container. 
        virtual const IContainer *getIContainer() const=0;
        /// Returns TRUE if the container is value based, or pointer based.
        virtual bool          getValueBased() const=0;
        /// Returns the IThing count or number of IThings held in the container.
        virtual quint64      getSize() const=0;
        /// Returns the number of IThings that have getHidden()==true.
        virtual quint64      getNumberHidden() const=0;
        /// Returns the number of IThings that have getSelected()==true.
        virtual quint64      getNumberSelected() const=0;
        /// Returns an ID one larger than the largest ID of any IThing currently in the container.
        /// This ID will be assigned automatically to the next added IThing, if it does not have an ID already.
        virtual quint64      getNextID() const=0;
        /// Set the maximum ID. This should only be used on a restore if manipulating the container under the hood. 
        virtual void         setNextID(quint64)=0;
        /// Remove the IThing \a t from the container.  A pointer to \a t is returned.  Any
        /// method or Signal2 attached to the Signal2 returned by getRemovedSignal() will be executed when remove() is invoked.
        /// \param t Pointer to an IThing that will be removed from the container.
        /// This function is not allowed on a value based container, and will automatically trigger an exception.
        virtual IThing *      remove(IThing *t)=0;
        /// Erase the IThing \a t from the container.  This invokes the remove() function and subsequently
        /// deletes \a t.  Any method or Signal2 attached to the Signal2 returned by getRemovedSignal() will be executed 
        /// when erase() is invoked.
        /// \param t Pointer to an IThing that will be erased from the container.
        virtual void          erase(IThing *t)=0;
        /// Add the IThing \a t to the container.  A pointer to \a t is returned.  Any
        /// method or Signal2 attached to the Signal2 returned by getAddedSignal() will be executed when add() is invoked.
        /// \param t Pointer to an IThing that will be added to the container.
        /// If the container is value based, a *copy* of \a t is placed in the container, and the return value is *not* the same as the argument.
        virtual IThing *      add(IThing *t,quint64 id=0)=0;
        /// Delete each IThing held in the container.  Note that remove() is not invoked.
        /// Will execute getRemovedSignal() with a null pointer if there is anything in the container.
        virtual void          clear()=0;
        /// Returns the IThing pointer to the IThing with the unique integer ID \a id.
        /// Returns 0 if not found.
        /// \param id Unique integer ID corresponding to the IThing to be found.
        virtual IThing *      findWithID(quint64 id)=0;
        /// Returns the const IThing pointer to the IThing with the unique integer ID \a id.
        /// Returns 0 if not found.
        /// \param id Unique integer ID corresponding to the IThing to be found.
        virtual const IThing *findWithID(quint64 id) const=0;
        /// Return instance count associated with \a id, this is always 0 if not a value based container.
        virtual quint64       getInstance(quint64 id) const=0;
        /// Returns the IThing pointer to the IThing with the name \a s.  The exact meaning of the QString \a s is defined by the container and the implementing class.
        /// Returns 0 if no match is found.
        /// \param s QString name of the IThing to be found.
        /// \param cs Qt CaseSensitivity object to specify case sensitivity when searching for the IThing with name \a s.
        virtual IThing *      findWithName(const QString &s)=0;
        /// Returns the const IThing pointer to the IThing with the name \a s.  The exact meaning of the QString \a s is defined by the container and the implementing class.
        /// Returns 0 if no match is found.
        /// \param s QString name of the IThing to be found.
        /// \param cs Qt CaseSensitivity object to specify case sensitivity when searching for the IThing with name \a s.
        virtual const IThing *findWithName(const QString &s) const=0;
        /// Returns the small granularity callback Signal2 object (templatized with IThing * and 
        /// IContainer * arguments) held in the container to which methods or Signal2 objects are attached that 
        /// will be executed whenever an IThing is added to the container via the add() function.
        /// If the first argument (const IThing *) is null, that indicates that the list has just been restored and that
        ///    ALL elements of the list have effectively been added.
        virtual signal_type *getAddedSignal() const=0;
        /// Returns the small granularity callback Signal2 object (templatized with IThing * and 
        /// IContainer * arguments) held in the container to which methods or Signal2 objects are attached that 
        /// will be executed whenever an IThing is removed or erased from the container via the remove() or erase() functions.
        /// If the first argument (const IThing *) is null, that indicates that the list is about to be cleared of ALL elements.
        virtual signal_type *getRemovedSignal() const=0;
        /// Returns the small granularity callback Signal2 object (templatized with IContainer arguments)
        /// held in the container to which methods or Signal2 objects are attached that will be executed
        /// just before the container itself is destroyed, allowing objects that depend on it (and hold iterators to it)
        /// to clear themselves.
        virtual signal_type *getDestroySignal() const=0;

        /// Acquire a new IOpaqueIterator for use iterating through the container.
        virtual IOpaqueIterator *acquire() const=0;
        /// Release a previously acquired IOpaqueIterator.
        /// \param i Pointer to IOpaqueIterator.
        virtual void             release(IOpaqueIterator *i) const=0;
        /// Return true if at the end of the container, false otherwise.
        /// \param i Pointer to IOpaqueIterator.
        virtual bool             atEnd(IOpaqueIterator *i) const=0;
        /// Return a pointer to the IThing the iterator \a i refers to. 
        /// \param i Pointer to IOpaqueIterator.
        virtual IThing *         get(IOpaqueIterator *i)=0;
        /// Return a const pointer to the IThing the iterator \a i refers to.
        /// \param i Pointer to IOpaqueIterator.
        virtual const IThing *   get(IOpaqueIterator *i) const=0;
        /// Increments the iterator if possible.
        /// Returns true if the iterator is incremented (e.g., if there is a next IThing in the container) else false. 
        /// \param i Pointer to IOpaqueIterator.
        virtual bool             increment(IOpaqueIterator *i) const=0;
        /// Copy the IOpaqueIterator \a iSrc to \a iDst.  Returns true if this is successful.
        /// \param iSrc Pointer to IOpaqueIterator that is the source to be copied.
        /// \param iDst Pointer to IOpaqueIterator that is the destination of the copy.
        virtual bool             copy(IOpaqueIterator *iSrc,IOpaqueIterator *iDst) const=0;
        /// Silently remove \a t. This means that no events are called. DO NOT USE THIS UNLESS YOU 
        /// KNOW THAT IT IS NECESSARY!!
        virtual IThing *         doSilentRemove(IThing *t)=0;
    };

    /** \brief Const iterator for an IContainer.

    * A const iterator for generically iterating through an IContainer.  This should not 
    * be used in time sensitive operations.

    * \ingroup utility

    */
    template <class T> class ICIterator {
    public:
        /// Constructor for the const iterator.
        /// \param c Pointer to the IContainer.
        ICIterator(const IContainer *c) : c_(c), i_(nullptr) { if (c_) i_ = c_->acquire(); }
        /// Destructor for the const iterator.
        ~ICIterator() { if (c_&&i_) c_->release(i_); }
        /// Associate this with a different container
        void setContainer(const IContainer *c) { if (c_&&i_) c_->release(i_);  i_ = 0; c_ = c;  if (c_) i_ = c_->acquire(); }
        /// Get the IContainer pointer that this iterator is associated with.
        const IContainer *getContainer() const { return c_; }
        /// Returns true if at the end of the IContainer.
        bool atEnd() const { if (!c_ || !i_) return true; return c_->atEnd(i_); }
        /// Increment the const iterator to the next entry in the IContainer.
        const ICIterator<T> &operator++() { if (c_&&i_) c_->increment(i_); return *this; }

        /// Increment the iterator by the specified amount. The implementation runs 
        /// through a linked list rather than doing math.
        //const ICIterator<T> &operator+=(int i) { if (c_&&i_) c_->incrementBy(i_, i); return *this; }
        /// subtract position of earlier iterator from position of this iterator to return the 
        /// difference as a number; there's no error check to see if earlier iterator is really 
        /// earlier than later iterator, and it's implemented by running through a linked list
        //quint64 operator-(const ICIterator& earlier) { if (c_&&i_) return c_->subtract(this, &earlier); return 0; }

        /// Return a const pointer to T.
        const T *get() const { if (!c_ || !i_) return 0;  return convert_cast<T>(c_->get(i_)); }
        /// Return a const pointer to T.
        const T *operator->() const { return get(); }
        /// Return a const reference to T.
        const T &operator*() const { return *get(); }
        /// Return a const pointer to T.
        operator const T *() const { return get(); }
        /// Copy another iterator
        void copy(const ICIterator<T> &c) { if (c_&&i_) c_->release(i_);  i_ = 0; c_ = c.c_;  if (c_) i_ = c_->acquire(); if (c_&&c.i_) c_->copy(c.i_,i_); }
    private:
        ICIterator(const ICIterator &);     // DISALLOWED
        void operator=(const ICIterator &); // DISALLOWED
        const IContainer *           c_;
        IContainer::IOpaqueIterator *i_;
    };

    /** \brief Iterator for an IContainer.

    * An iterator for generically iterating through an IContainer.  This should not 
    * be used in time sensitive operations.

    * \ingroup utility

    */
    template <class T> class IIterator : public ICIterator<T> {
    public:
        /// Constructor for the iterator.
        /// \param c Pointer to the IContainer.
        IIterator(IContainer *c) : ICIterator<T>(c) { }
        /// Increment the iterator to the next entry in the IContainer.
        const IIterator<T> &operator++() { ICIterator<T>::operator++();  return *this; }
        /// Associate this with a different container
        void setContainer(IContainer *c) { ICIterator<T>::setContainer(c); }
        /// Return a pointer to T.
        T *get() const { return const_cast<T *>(ICIterator<T>::get()); }
        /// Return a pointer to T.
        T *operator->() const { return get(); }
        /// Return a referenc to T.
        T &operator*() const { return *get(); }
        /// Return a pointer to T.
        operator T *() const { return get(); }
    private:
        IIterator(const IIterator &);     // DISALLOWED
        void operator=(const IIterator &); // DISALLOWED
    };

} /// namespace itasca
// EoF
