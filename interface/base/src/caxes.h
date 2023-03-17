#pragma once
/** \file caxes.h
* \brief 2D and 3D cartesian Axes systems that are compact.
* \addtogroup Base Base interface specification 
* @{
*/

#include "vect.h"
#include "avect.h"
#include "orientation.h"

/// \brief Class for specifying a particular 2D cartesian axes system, and converting to and from it.

/** This class is allows one to specify a cartesian coordinate system 
* and convert from "local" (to this CAxes object)
* to "global" in cartesian space. 
* This class is a 2D version of CAxes, and a 3D one exists as well.
* This is being introduced so that a coordinate system can exist without
* the added overhead of storing a center and an extra vector (as in the CAxes2D 
* and 3D classes - the extra vector is obtained on the fly at the added expense of some computational overhead.
* The API has basically the same functionality as the Axes API without the access to the center
* of the coordinate system.
*/

class CAxes2D {
public:
    /// Default construction, no initialization of data.
    BASE_EXPORT CAxes2D() { }
    /// Copy constructor
    BASE_EXPORT CAxes2D(const CAxes2D &a) : d1_(a.d1_) { }
    /// Explicit constructor from two unit vectors
    /// \param vx The local x direction
    BASE_EXPORT CAxes2D(const DVect2 &vx);
    /// Explicit contructor from dip
    /// \param dip In radians.  See the Orientation2 class for a defintion of dip.
    BASE_EXPORT explicit CAxes2D(const Double &dip) { setOrientation(dip); }
    /// Explicit contructor from Orientation2
    /// \param o Orientation of axes relative to global.
    BASE_EXPORT explicit CAxes2D(const Orientation2 &o) { setOrientation(o); }

    /// Equality operator
    BASE_EXPORT const CAxes2D &operator=(const CAxes2D &a) { d1_=a.d1_; return *this; }
    /// Comparison operator
    BASE_EXPORT bool operator==(const CAxes2D &a) const;
    /// Comparison operator
    BASE_EXPORT bool operator!=(const CAxes2D &a) const { return !operator==(a); }

    /// Specifies a new orientation for the axes, by dip (in radians).
    BASE_EXPORT void setOrientation(const Double &dip); 
    /// Specifies a new orientation for the axes, by orientation.
    BASE_EXPORT void setOrientation(const Orientation2 &o);

    /// The x unit vector.
    BASE_EXPORT const DVect2 &e1() const { return d1_; }
    /// The y unit vector
    BASE_EXPORT DVect2 e2() const { return DVect2(-d1_.y(),d1_.x()); }

    /// Reference access to the axis x-direction.
    /// \warning If you use this, no check is made to ensure e1 and e2 are orthonormal!
    BASE_EXPORT DVect2 &re1() { return d1_; }

    /// Convert Dvect2 \a v from "global" system to this system.
    BASE_EXPORT DVect2 toLocal(const DVect2 &v) const;
    /// Convert DAVect2 \a v from "global" system to this system (identity transformation).
    BASE_EXPORT const DAVect2 &toLocal(const DAVect2 &v) const { return v; }
    /// Convert DVect2 \a v from this system to a "global" system.
    BASE_EXPORT DVect2 toGlobal(const DVect2 &v) const;
    /// Convert DAVect2 \a v from this system to a "global" system (identity transformation).
    BASE_EXPORT const DAVect2 &toGlobal(const DAVect2 &v) const { return v; }
    /// Return e1 or e2 given degree-of-freedom dof (0-1)
    BASE_EXPORT DVect2 getDir(unsigned int dof) const;
    /// Returns the equivalent orientation of degree-of-freedom dof (0-1)
    BASE_EXPORT Orientation2 getOrientation(unsigned int dof=0) const;

    // Static
    /// Creates an Axes using vector y in the +y direction (need not be unit).
    static BASE_EXPORT CAxes2D make(const DVect2 &y);
    /// Creates an Axes using vector x in the +x direction (need not be unit).
    static BASE_EXPORT CAxes2D makeX(const DVect2 &x);
    /// Creates an Axes using orientaion o for the +y direction.
    static BASE_EXPORT CAxes2D make(const Orientation2 &o);
    /// Creates an Axes using orientaion o for the +x direction.
    static BASE_EXPORT CAxes2D makeX(const Orientation2 &o);
    /// Tolerance used for comparison and detecting "near enough to zero".
    /// Defaults to 1e-6.  A single tolerance is used for all 2D *and* 3D CompactAxes and Axes objects.
    static BASE_EXPORT void setTolerance(const double &d);
private:
    DVect2 d1_;
};

/// \brief Class for specifying a particular 3D cartesian axes system, and converting to and from it.

/** This class is allows one to specify a cartesian coordinate system 
* and convert from "local" (to this CAxes object)
* to "global" in cartesian space. 
* This class is a 3D version of CAxes, and a 3D one exists as well.
* This is being introduced so that a coordinate system can exist without
* the added overhead of storing a center and an extra vector (as in the CAxes2D 
* and 3D classes - the extra vector is obtained on the fly) at the added expense of some computational overhead.
* The API has basically the same functionality as the Axes API without the access to the center
* of the coordinate system.
*/
class CAxes3D {
public:
    /// Default constructor, no initialization of data.
    BASE_EXPORT CAxes3D() { }
    /// Copy constructor
    BASE_EXPORT CAxes3D(const CAxes3D &a) : d1_(a.d1_), d2_(a.d2_) { }
    /// Explicit construction from CAxes2D.  e3() = (0,0,1).
    BASE_EXPORT explicit CAxes3D(const CAxes2D &a) : d1_(toVect3(a.e1())), d2_(toVect3(a.e2())) { }
    /// Explicit construction from three unit vectors. 
    /// \warning No checking is done to ensure vx,vy are orthogonal!
    BASE_EXPORT CAxes3D(const DVect3 &vx, const DVect3 &vy);
    /// Explicit construction from dip, dip direction, and roll (all in radians)
    /// See Orientation3 for a definition of dip and dip direction.
    /// Roll defines the last degree-of-freedom, and is defined as the angle of +y on the xy plane
    ///   from +z.  If the intersection of the xy plane with +z is zero (dip 0 or 180)
    ///   the angle is from +y.
    BASE_EXPORT CAxes3D(const Double &dip, const Double &ddir, const Double &roll=0.0) { setOrientation(dip,ddir,roll); }
    /// Explicit construction from Orientation3, and roll (all in radians)
    /// See CAxes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT explicit CAxes3D(const Orientation3 &o,const Double &roll=0.0) { setOrientation(o,roll); }

    /// Equality operator
    BASE_EXPORT const CAxes3D &operator=(const CAxes3D &a) { d1_=a.d1_; d2_=a.d2_; return *this; }
    /// Comparison operator
    BASE_EXPORT bool operator==(const CAxes3D &a) const;
    /// Comparison operator
    BASE_EXPORT bool operator!=(const CAxes3D &a) const { return !operator==(a); }

    /// Change the orientation of the axes sytem, using dip, dd, and roll (all in radians)
    /// See Orientation3 for a definition of dip and dip direction.
    /// See CAxes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT void setOrientation(const Double &dip, const Double &ddir, const Double &roll=0.0);  // radians
    /// Change the orientation of the axes sytem, using dip, dd, and roll (all in radians)
    /// See CAxes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT void setOrientation(const Orientation3 &o,const Double &roll=0.0);

    /// The x unit direction.
    BASE_EXPORT const DVect3 &e1() const { return d1_; }
    /// The y unit direction.
    BASE_EXPORT const DVect3 &e2() const { return d2_; }
    /// The z unit direction.
    BASE_EXPORT DVect3 e3() const { return d1_&d2_; }

    /// Reference access to the x direction.
    /// \warning If you use these - no checks are made to ensure the system remains orthonormal!
    BASE_EXPORT DVect3 &re1() { return d1_; }
    /// Reference access to the y direction.
    /// \warning If you use these - no checks are made to ensure the system remains orthonormal!
    BASE_EXPORT DVect3 &re2() { return d2_; }

    /// Converts a vector from "global" (cartesian) system to this system.
    BASE_EXPORT DVect3 toLocal(const DVect3 &v) const;
    /// Converts a vector from this system to a "global" system.
    BASE_EXPORT DVect3 toGlobal(const DVect3 &v) const;
    /// Return e1, e2, or e3 based on degree-of-freedom (0-2)
    BASE_EXPORT DVect3 getDir(unsigned int dof) const;
    /// Returns equivalent orientation of e1, e2, or e2 based on degree-of-freedom (0-2)
    BASE_EXPORT Orientation3 getOrientation(unsigned int dof=0) const;
    /// Returns the effective roll of the current axes system.
    /// See CAxes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT Double getRoll() const;

    // Static
    /// Creates an CAxes3D using vector z in the +z direction (need not be unit).
    /// By default, x will be dip of the plane defined using z as a normal,
    ///   and y will be the strike.
    /// Roll defines a rotation about the z axis (clockwise with vector facing towards you).
    static BASE_EXPORT CAxes3D make(const DVect3 &z,const Double &roll=0.0);
    /// Same as make(const DVect3 &z,const Double &roll=0.0), except given vector is x.
    /// By default, then, Y will be dip and Z will be strike.
    static BASE_EXPORT CAxes3D makeX(const DVect3 &x,const Double &roll=0.0);
    /// Creates an CAxes3D using orientation o in the +z direction.
    /// By default, x will be dip of the plane defined using o as a normal,
    ///   and y will be the strike.
    /// Roll defines a rotation about the z axis (clockwise with vector facing towards you).
    static BASE_EXPORT CAxes3D make(const Orientation3 &o,const Double &roll=0.0);
    /// Same as make(const Orientation3 &o,const Double &roll=0.0), except given vector is x.
    /// By default, then, Y will be dip and Z will be strike.
    static BASE_EXPORT CAxes3D makeX(const Orientation3 &o,const Double &roll=0.0);
    /// Tolerance used for comparison and detecting "near enough to zero".
    /// Defaults to 1e-6.  A single tolerance is used for all 2D *and* 3D Axes objects.
    static BASE_EXPORT void setTolerance(const double &d); // Defaults to 1e-6
private:
    DVect3 d1_;
    DVect3 d2_;
};

/// @}
// EOF
