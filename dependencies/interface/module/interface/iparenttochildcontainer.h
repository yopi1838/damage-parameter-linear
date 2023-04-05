#pragma once
// iparenttochildcontainer.h
#include "utility/interface/icontainer.h"

namespace itasca {
    class IThing;

    /** \brief Provides a class to use as the parent to a child container, or the main container
        * of all of the children.
        *
        * Provides a class to use as the parent to a child container, or the main container of all of the children.
        * \ingroup module
        */
    class IParentToChildContainer {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4e95af7a;

        ///  Returns a callback Signal2 object. This signal will be executed whenever a child is added to the 
        /// container, see IContainer 
        virtual IContainer::signal_type     *getAddedSignalChild() const=0;
        ///  Returns a callback Signal2 object. This signal will be executed whenever a child is about to be
        /// removed or erased from the container, see IContainer 
        virtual IContainer::signal_type     *getRemovedSignalChild() const=0;
        ///  Returns a callback Signal2 object. This signal will be executed whenever a child is about to be
        /// destroyed, see IContainer 
        virtual IContainer::signal_type     *getDestroySignalChild() const=0;
        /// Adds child to the front of the list
        virtual void                         pushFrontChild(IThing *)=0;
        /// Adds child to the back of the list
        virtual void                         pushBackChild(IThing *)=0;
        /// Returns an opaque interator pointer to an available (or new) object in the container   
        virtual IContainer::IOpaqueIterator *acquireChild(IThing *) const=0;
        /// Make the opaque iterator availble 
        virtual void                         releaseChild(IContainer::IOpaqueIterator *i) const=0;
        /// Returns true if this child is the last one in the list
        virtual bool                         atEndChild(IContainer::IOpaqueIterator *i) const=0;
        /// Returns a pointer to the child of the given iterator, or NULL if the iterator does not have a child
        virtual IThing                      *getChild(IContainer::IOpaqueIterator *i)=0;
        /// Returns a constant pointer to the child of the given iterator, or NULL if the iterator does not have a child
        virtual const IThing                *getChild(IContainer::IOpaqueIterator *i) const=0; 
        /// Moves child pointer to the next child, incrementing the iterator
        virtual bool                         incrementChild(IContainer::IOpaqueIterator *i) const=0;
        /// Child shallow copy.
        virtual bool                         copyChild(IContainer::IOpaqueIterator *iSrc,IContainer::IOpaqueIterator *iDst) const=0;
        /// Clean up the iterators upon deletion
        virtual void                         cleanIterators(IThing *)=0;

    };
} /// namespace itasca
// EoF
