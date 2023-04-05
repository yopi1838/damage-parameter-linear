#pragma once

#include "module/interface/ilinktype.h"

namespace itasca {
    class IThing;
};
namespace itascaxd {
    class IGeomPoint;
}

namespace extruder {
    using namespace itasca;
    using namespace itascaxd;

    class IExtrudeEdge;

    /// \brief Interface for IExtrudePoint objects. 
    /// \ingroup FLAC3D
    class IExtrudePoint {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4cb610e5;

        /// Return a const IThing interface for this object.
        virtual const IThing *     getIThing() const=0;
        /// Return an IThing interface for this object.
        virtual IThing *           getIThing()=0;
        /// Return a const IGeomPoint interface for this object.
        virtual const IGeomPoint * getIGeomPoint() const=0;
        /// Return an IGeomPoint interface for this object.
        virtual IGeomPoint *       getIGeomPoint()=0;
        /// Return true if point is attached to any edge.
        virtual const bool         isConnected()=0;

        /// Returns an interface to a CLinkType<> object, a const pointer to the first object in the linked list.
        virtual CLinkType<IExtrudeEdge> getStartExtrude() const=0;
        /// Returns an interface to a LinkType<> object, a pointer to the first object in the linked list.
        virtual LinkType<IExtrudeEdge>  getStartExtrude()=0;
    };
} // namespace f3
// EoF
