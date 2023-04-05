#pragma once

/**
    * \file
    * \brief IRangeElementExtension classes provide access information in the Filters that are included in the Kernel library.
    */

// A short description precedes each, and generally the access functions
//   are self-explanatory.

#include "base/src/baseqt.h"
#include "module/interface/dim.h"
#include "module/interface/irangeelement.h"


namespace itascaxd {
    /// \brief A filter element described as a sphere in space.
    /** It can also represent a spherical annulus between a minimum and maximum radius.\n
        * If not an annulus, you should leave Radius1()=0.0 and specify Radius2().
        *  \ingroup kernel
        */ 
    class IRangeElementSphere { // name = "sphere"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bad;
        /// Returns the sphere center, see setCenter
        virtual DVect          getCenter() const=0;
        /// Returns the spherical annulus inner radius, see setRadius1()
        virtual double         getRadius1() const=0;
        /// Returns the spherical (outer) radius, , see setRadius2()
        virtual double         getRadius2() const=0;
        /// Returns true if object represents an spherical annulus, , see setRadius1()
        virtual bool           isAnnulus() const=0;
        /// Sets the sphere center
        virtual void           setCenter(const DVect &)=0;
        /// Sets the spherical annulus inner radius
        virtual void           setRadius1(const double &d)=0;
        /// Sets the spherical (outer) radius
        virtual void           setRadius2(const double &d)=0;
        /// Returns a pointer to the IRangeElement object
        virtual IRangeElement *getIRangeElement()=0;

        // SIGNALS
        /// Signal emittted when the center of the sphere has changed, , see setCenter()
        virtual void centerChanged(const DVect &center)=0;
        /// Signal emittted when the spherical annulus inner radius has changed, , see setRadius1()
        //virtual void radius1Changed(const double &rad1)=0;
        /// Signal emittted when the spherical radius has changed, , see setRadius2()
        //virtual void radius2Changed(const double &rad2)=0;
    };

    /// \brief A filter element specified as a geometric region inside a capped cylinder, defined by a segment [begin-end] of radius Rad2.
    /** It can also represent a cylindrical annulus between two radii.
        *  \ingroup kernel
        */ 
    class IRangeElementCylinder { // name = "cylinder"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815baf;
        /// Returns the central point where the cylinder starts, see setBegin()
        virtual DVect          getBegin() const=0;
        /// Returns the central point where the cylinder ends, see setEnd()
        virtual DVect          getEnd() const=0;
        /// Returns the inner radius of a cylindrical annulus, see setRadius1()
        virtual double         getRadius1() const=0;
        /// Returns the cylinder radius, see setRadius2()
        virtual double         getRadius2() const=0;
        /// Sets the central point where the cylinder starts
        virtual void           setBegin(const DVect &v)=0;
        /// Sets the central point where the cylinder ends
        virtual void           setEnd(const DVect &v)=0;
        /// Sets the cylinder radii. For a cylindrical annulus, \b d1 must be greater than zero and less than \b d2
        virtual void           setRadius(const double &d1,const double &d2)=0;
        /// Returns a pointer to the IRangeElement object
        virtual IRangeElement *getIRangeElement()=0;

        // SIGNALS
        /// Signal emitted when the central point where the cylinder starts has changed
        virtual void beginChanged(const DVect &dv)=0;
        /// Signal emitted when the central point where the cylinder ends has changed
        virtual void endChanged(const DVect &dv)=0;
        /// Signal emitted when the inner cylinder radius has changed
        virtual void radius1Changed(const double &r1)=0;
        /// Signal emitted when the (outer) cylinder radius has changed
        virtual void radius2Changed(const double &r2)=0;
    };

    /// \brief A filter element specified as a geometric region inside an axis aligned box.
    /** 
        *  \ingroup kernel
        */ 
    class IRangeElementPosition { // name = "box"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x57222304;
        /// Returns the lower bound
        virtual DVect          getLowerBound() const=0;
        /// Returns the upper bound
        virtual DVect          getUpperBound() const=0;
        /// Returns the tolerance
        virtual DVect          getTolerance() const=0;
        /// Returns the center
        virtual DVect          getCenter() const=0;
        /// Sets the lower bound
        virtual void           setLowerBound(const DVect &v)=0;
        /// Sets the upper bound
        virtual void           setUpperBound(const DVect &v)=0;
        /// Sets the tolernace
        virtual void           setTolerance(const DVect &v)=0;
        /// Sets the center
        virtual void           setCenter(const DVect &v)=0;
        /// Returns a pointer to the IRangeElement object
        virtual IRangeElement *getIRangeElement()=0;

        // SIGNALS
        /// Signal emitted when the lower bound has changed
        virtual void lowerBoundChanged(const DVect &dv)=0;
        /// Signal emitted when the upper bound has changed
        virtual void upperBoundChanged(const DVect &dv)=0;
        /// Signal emitted when the tolerance has changed
        virtual void toleranceChanged(const DVect &dv)=0;
        /// Signal emitted when the center bound has changed
        virtual void centerChanged(const DVect &dv)=0;
    };

    /// \brief Interface to a RangeElement of a plane in space, where you want objects above, below, or within a certain distance of the filter.
    /// \ingroup kernel
    class IRangeElementPlane { // name = "plane"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bb1;
        /// Possible object location
        enum Kind { 
            kAbove=0,    /*!< The object is located above the plane. */
            kBelow=1,    /*!< The object is located below the plane. */
            kDist=2      /*!< The object is distant from the plane. */
        };
        /// Returns the plane Orientation, see setOrientation()
        virtual Orientation getOrientation() const=0;
        /// Returns the plane origin, see setOrigin()
        virtual DVect       getOrigin() const=0;
        /// Returns if the objects should be above, below, or within a certain distance of the filter, see setKind()
        virtual Kind        getKind() const=0;
        /// Returns the object distance from the filter.
        virtual double      getDistance() const=0;
        /// Sets the plane Orientation
        virtual void        setOrientation(const Orientation &o)=0;
        /// Sets the plane origin
        virtual void        setOrigin(const DVect &o)=0;
        /// Sets where the object will be located related to plane, above, below, within a given distance, see getKind()
        virtual void        setKind(Kind k)=0;
        /// Sets the object distance to the filter.
        virtual void        setDistance(const double &d)=0;
        // SIGNALS
        /// Signal emitted when the plane orientation has changed, see setOrientation()
        virtual void normalChanged(const DVect &normal)=0;
        /// Signal emitted when the plane origin has changed, see setOrigin()
        virtual void originChanged(const DVect &origin)=0;
        /// Signal emitted when the objecte location related to the plane has changed, see setKind()
        virtual void kindChanged(int kind)=0;
        /// Signal emitted when the plane distance has changed, see setDistance()
        virtual void distanceChanged(const double &dis)=0;
    };

    /// \brief Interface to a Range Element that is actually a named filter.
    /** This can be used to create complex aggregates of filters, more complex
        * than is allowed by the simple union/intersection option in a single Range.
        */ 
    /// \ingroup kernel
    class IRangeElementNamed { // name = "named"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bb3;
        /// Returns the name of the named filter element
        virtual QString getFilterName() const=0;
        /// Sets the name of the named filter element
        virtual void    setFilterName(const QString &s)=0;

        //SIGNALS
        /// Signal emitted when the name of the filter element has changed, see setFilterName()
        virtual void nameChanged(const QString &s)=0;
    };
} // namespace itascaxd
// EoF
