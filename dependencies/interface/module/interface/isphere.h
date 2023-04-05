#pragma once


#include "base/src/base.h"
#include "dim.h"

namespace itascaxd {
    /// \brief Interface for the definition of a Sphere
    /// \ingroup module
    class ISphere {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4e93301b;
        /// Returns the center of the sphere
        virtual DVect       getPosition() const=0;
        /// Returns the sphere radius
        virtual double      getRadius() const=0;
        //virtual double             getDistance(const DVect &v) const=0;
        /// Sets the center of the sphere
        virtual void setPosition(const DVect &v)=0;
        /// sets the sphere radius
        virtual void setRadius(const double &v,bool lock=false)=0;
    };
} // namespace itascaxd
// EoF
