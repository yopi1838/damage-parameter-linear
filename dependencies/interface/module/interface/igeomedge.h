#pragma once

/**
    * \file
    * \brief Interface for a user-defined geometric point
    */

#include "base/src/baseqt.h"
#include "ilinktype.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IGeomPoint;
    class IGeomPoly;
    class GeomBase;

    /// \brief Interface for a user-defined geometric edge, defined as the line between two IGeomPoint objects.
    /// \ingroup kernel
    class IGeomEdge {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bcc;

        /// Return an IThing interface for this object.
        virtual IThing *         getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *   getIThing() const=0;

        /// Returns a pointer to the IGeomPoint associated with \a end 0 or 1.  Returns 0 if \a end > 1.  May be null if the edge is currently invalid.
        virtual IGeomPoint *      getPoint(quint32 end)=0;
        /// Returns a pointer to the IGeomPoint associated with \a end 0 or 1.  Returns 0 if \a end > 1.  May be null if the edge is currently invalid.
        virtual const IGeomPoint *getPoint(quint32 end) const=0;
        /// Sets the point associated with \a end 0 or 1,  has no effect if \a end > 1.  \a point may be null, rendering the edge invalid.
        /// This will cause an edgeMoved signal in the containing IGeomSet, if applicable.
        virtual void setPoint(IGeomPoint *point,quint32 end)=0;
        /// Returns true if the edge is currently valid, that is if both ends (0 and 1) are assigned to a non-null IGeomPoint object.
        virtual bool   isValid() const=0;
        /// Returns true if the edge is fake meaning that the edge exists and valid but user should not be able to see and interact with it
        virtual bool   isFake() const=0;
        /// Returns the distance between the two IGeomPoint objects, return 0.0 if not valid.
        virtual double getLength() const=0;
        /// Returns the unit vector from end 0 to end 1.  Returns DVect(0) if not valid.
        virtual DVect  getDirection() const=0;
        /// Returns the vector from end 0 to end 1.  Return DVect(0) if not valid.
        virtual DVect  getVector() const=0;
        /// Returns the point on the edge closes to the point \a pos.  Returns DVect(0) if not valid.
        virtual DVect  getClosestPoint(const DVect &pos) const=0;

        /// Returns an interface to a CLinkType<> object, a const pointer to the next object in the linked list.
        virtual CLinkType<IGeomEdge> getNext(quint32 end) const=0;
        /// Returns an interface to a LinkType<> object, a pointer to the next object in the linked list.
        virtual LinkType<IGeomEdge>  getNext(quint32 end)=0;
        /// Returns an interface to a CLinkType<> object, a const pointer to the first object in the linked list.
        virtual CLinkType<IGeomPoly> getStart() const=0;
        /// Returns an interface to a LinkType<> object, a pointer to the first object in the linked list.
        virtual LinkType<IGeomPoly>  getStart()=0;    
        /// Returns number of polygon faces attached.
        virtual int  getPolyOrder() const=0;
        /// Return const GeomBase 
        virtual const GeomBase * getGeomBase() const=0;
        /// Convert to global coordinate the point on edge. If u = 0.0 it will be position of point(0), if u=1.0 -- position of point(1). Returns zero vector if edge is not valid.
        virtual DVect convertToGlobal(double u) const =0;
        /// Convert o local edge coordinate
        virtual double convertToLocal(const DVect &pos) const =0;
    };
} // namespace itascaxd
// EoF
