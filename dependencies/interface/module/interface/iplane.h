#pragma once

/** \file iplane.h
    * \brief Interface for the definition of a Plane in 3D space.
    * 
    */


#include "base/src/base.h"
#include "dim.h"

namespace itascaxd {
    // NOTE:  Might want to add distance checking, etc functions to the interface later.
    // NOTE:  This class IS used!

    /// \brief Interface for the definition of a Plane in 3D space.
    /** 
        This is the interface for the definition of a Plane in 3D space
        Either Normal or Dip an DipDirection may be specified to indication the planes orientation.
        Function \b wasDefinedByNormal() may be called to determine which was used,
        based on whether setNormal() or setDip(), setDipDirection() was called last.

        In a Right Handed system:
            Dip is defined by the angle between the normal vector and the Z axis.
            Dip-Direction is defined as the angle of the projection of the normal vector onto the XY 
                  plane with the Y axis (clockwise position looking from +Z).
     
        \ingroup module
    */
    class IPlane {
    public:
        /// Possible intersection types
        enum IntersectCode {
            icFront=0x01,             /*!< On the same side as the normal vector is pointing */
            icBehind=0x02,            /*!< On the opposide side as the normal vector is pointing. */
            icIntersect1=0x04,        /*!< Intersection with beginning on backside and end on frontside. */
            icIntersect2=0x08         /*!< Intersection with beginning on frontside and end on backside. */
        };

        /// Returns the plane origin.
        virtual const DVect       &getOrigin() const=0;
        /// Returns the plane orientation.
        virtual const Orientation &getOrientation() const=0;
        /// Returns the distance of the point v from the plane, with a negative value indicating
        ///   that it is on the side facing away from the normal vector.
        virtual double             getDistance(const DVect &v) const=0;
        /// Calculates the intersection of the plane with a line segment (if any).
        /// The location of the intersection if present is placed in \b loc.
        /// \b dist is filled with the distance from begin to the intersection point.
        virtual IntersectCode      getIntersection(const DVect &begin,const DVect &end,DVect *loc=0,double *dist=0) const=0;
        /// Returns the closest point in the plane to \b v
        virtual DVect              getClosestPoint(const DVect &v) const=0;
        /// Sets the plane origin
        virtual void setOrigin(const DVect &v)=0;
        /// Sets the plane orientation
        virtual void setOrientation(const Orientation &o)=0;
    };
} // namespace itascaxd
// EoF
