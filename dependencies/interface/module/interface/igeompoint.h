#pragma once

/**
    * \file
    * \brief Interface for a user-defined geometric point
    */

#include "ilinktype.h"

/** \brief Itasca Library standard namespace, specific to 2D or 3D */
namespace itascaxd {
    /// \addtogroup kernel
    /// @{

    class IGeomEdge;
    class GeomBase;
    /// \brief Interface for a user-defined geometric point.
    class IGeomPoint {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bc7;

        /// Return an IThing interface for this object.
        virtual IThing *       getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing * getIThing() const=0;

        /// Returns the position associated with the point.
        virtual DVect getPosition() const=0;
        virtual DVect3 getPosition3() const=0;
        /// Sets the position of the point, causes pointMoved() signal on the 
        ///   owning object (like an IGeomSet), which will propagate to owning 
        ///   objects (lines, polygons, etc).
        virtual void  setPosition(const DVect &pos)=0;
        virtual void  setPosition3(const DVect3 &pos3)=0;

        /// Returns an interface to a CLinkType<> object, a const pointer to the first object in the linked list.
        virtual CLinkType<IGeomEdge> getStart() const=0;
        /// Returns an interface to a LinkType<> object, a pointer to the first object in the linked list.
        virtual LinkType<IGeomEdge>  getStart()=0;
        /// Return const GeomBase 
        virtual const GeomBase * getGeomBase() const=0;
    };
    /// @}
} // namespace itascaxd
// EoF
