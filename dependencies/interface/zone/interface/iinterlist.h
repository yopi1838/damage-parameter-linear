#pragma once

#include "utility/interface/icontainer.h"

namespace zone {
    class IZone;
    class IInterface;

    /// \brief Class provides access to the global list of all IIinterface objects in the model.
    /// \ingroup Zone
    class IInterfaceList {
    public:
        /// returns a const IThing pointer
        virtual const IThing *           getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *                 getIThing()=0;
        virtual const IContainer *       getIContainer() const=0;
        virtual IContainer *             getIContainer()=0;
        /// Return a const pointer to the first interface stored. IInterface::getNext() can be used to iterate on the list.
        virtual const IInterface *       getFirstInterface() const=0;
        /// Return a pointer to the first interface stored. IInterface::getNext() can be used to iterate on the list.
        virtual IInterface *             getFirstInterface()=0;
        /// Given a interface \a id, returns const a pointer to the interface, or 0 if \a id does not exist.
        virtual const IInterface *       findInterfaceWithName(QString id) const=0;
        /// Given a interface \a id, returns a pointer to the interface, or 0 if \a id does not exist.
        virtual IInterface *             findInterfaceWithName(QString id)=0;
        /// Returns the general type for the interface elements
        virtual quint32                  getInterfaceElementType() const=0;
        /// Returns the general type for the interface nodes
        virtual quint32                  getInterfaceNodeType() const=0;
        /// Returns a flag number that indicates that the interface geometry has changed.
        virtual uint                     getInterfaceGeometryNotice() const=0;
        /// Returns a flag number that indicates that the number of interfaces in the global interface list has changed.
        virtual uint                     getInterfaceQuantityNotice() const=0;
        /// One or two interface elements are created, depending on whether the zone face is a quadrilateral or a triangle.\n
        /// Returns 1 or 2, indicating whether one or two elements were created.
        /// \brief Creates interface element(s) for interface \a id on zone \a zone face \a face.
        virtual quint64                 createElemWithFISH(const IZone *zone,quint32 face,QString name)=0;
        /// Returns a signal that get's called every time a node is removed from any interface in the list.
        virtual IContainer::signal_type *getNodeRemovedSignal()=0;
        /// Returns a signal that get's called every time an element is removed from any interface in the list.
        virtual IContainer::signal_type *getElemRemovedSignal()=0;
        /// Returns the tag for the interface node extra variable change notice
        virtual uint               getInterfaceNodeExtraNotice() const=0; 
        /// Returns the tag for the interface elem extra variable change notice
        virtual uint               getInterfaceElemExtraNotice() const=0; 
        /// Returns the tag for the interface elem group change notice
        virtual uint               getInterfaceElemGroupNotice() const=0;
        /// Returns the tag for the interface elem group change notice
        virtual uint               getInterfaceNodeGroupNotice() const=0;
        /// Returns the tag for the interface elem group change notice
        virtual uint               getInterfaceGroupNotice() const=0;
    };
} // namespace zone
// EoF