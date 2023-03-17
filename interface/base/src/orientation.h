#pragma once
/** \file orientation.h
*  \brief Class for storing an "orientation", or a direction in 2D or 3D space.
* \addtogroup Base Base interface specification 
* @{
*/

#include "vect.h"

/// Define pi and pi/180
#ifndef _PI_DEFINED
#define _PI_DEFINED
static const double dPi = 3.141592653589793238462643383279502884197169399;
#endif
static const double dDegrad = 0.01745329251994329576923690768488;  // pi/180

/** \brief Class for storing an "orientation", or a direction in 2D or 3D space.
*
* Orientations can be viewed as defining either a normal direction, or
*    a plane in space normal to that direction.\n
* 2D orientations are specified by either a plane dip angle (clockwise from +X) or
*    a normal vector.\n
* 3D orientations are specified by a dip angle (clockwise from XY plane) and 
*    a dip direction (steepest Z descent line projected onto XY plane, clockwise from +Y).

* 3D orientation objects remember if they were specified with dip/dd or normal.

* The original data specified (non-normalized direction vector, dip/dd outside range) is
*    retained, so that it can be changed/updated by the user on a component basis.

* For historical (3DEC) reasons, an orientation has a global Left Handed System
*    mode --- which changes the 3D definition of dip and dip direction.

* An Orientation can also return a "joint angle", which is defined as the intersection of the plane
*    with the xy plane, measured positive counter-clockwise from x. In 2D the joint angle is the negative of the dip.
* Note that "joint angle" can not fully specify a normal vector, because it has no concept of a side; thus,
*    a joint angle of 45 is equivalent to a joint angle of 225.  Joint angles are always returned in the range 0 to 180.

* An "Azimuth" of a 3D normal vector is the clockwise-angle from +Y projected onto the XY plane.
*    In a LHS system, the YZ axes are swapped.\n
* A "Plunge" of a 3D normal vector is the angle between the vector and the XY plane.
*    In a LHS system, the YZ axes are swapped.\n
* \note Orientation2 relies on the default (compiler supplied) copy contructor and = operator.
*/
class Orientation2 {
public:
    /// Default constructor -- defaults to +Y is (0,1), defined by normal.
    BASE_EXPORT Orientation2() : normal_(0.0,1.0), normalUser_(0,1.0), definedByNormal_(true)  { }
    /// Explicit constructor, defines the normal.
    /// \a dv2 need not be a unit vector.
    BASE_EXPORT explicit Orientation2(const DVect2 &dv2) : normal_(dv2.unit()), normalUser_(dv2) { }
    /// Explicit constructor, orientation defined by \a dip (in radians).
    BASE_EXPORT explicit Orientation2(const double &dip) : normal_(getNormFromDip(dip)), normalUser_(getNormFromDip(dip)) { }
    /// Equality operator
    BASE_EXPORT bool operator==(const Orientation2 &o) const { return (normal_==o.normal_); }
    BASE_EXPORT bool operator!=(const Orientation2 &o) const { return !operator==(o); }

    /// Returns the \b unit normal vector of the orientation.
    BASE_EXPORT const DVect2 &getNormal() const { return normal_; }
    /// Returns the normal vector as specified by the user, not necessarily of unit lengths.
    BASE_EXPORT const DVect2 &getNormalUser() const { return normalUser_; }
    /// Returns the equivalent dip angle (in radians).
    BASE_EXPORT double        getDip() const { return getDipFromNorm(normal_); }
    /// Returs true if the orientation was specific by a normal vector, false if it was specified by dip.
    BASE_EXPORT bool          wasDefinedByNormal() const { return definedByNormal_; }
    /// Returns the joing angle of this orientation, in radians.
    BASE_EXPORT double        getJointAngle() const { return getJointAngleFromNorm(normal_); }

    /// Sets the orientation by normal.  \a norm need not be a unit vector.
    BASE_EXPORT void setNormal(const DVect2 &norm) { normal_ = norm.unit(); normalUser_ = norm; definedByNormal_ = true; }
    /// Sets the orientaion by dip (in radians).
    BASE_EXPORT void setDip(const double &dip) { normalUser_ = normal_ = getNormFromDip(dip); definedByNormal_ = false; }
    /// Can be used to set a full normal vector, but returned joint angle is still always between 0 and 180.
    BASE_EXPORT void setJointAngle(const double &ja) { normalUser_ = normal_ = getNormFromJointAngle(ja); }

    /// Converts from a dip (in radians) to a normal vector.
    static BASE_EXPORT DVect2 getNormFromDip(const double &dip);
    /// Converts from a normal vector (need not be unit) to a dip in radians.
    static BASE_EXPORT double getDipFromNorm(const DVect2 &norm);
    /// Converts from a joint angle (in radians) to a normal vector.
    static BASE_EXPORT DVect2 getNormFromJointAngle(const double &ja);
    /// Converts from a normal vector (need not be unit) to a joint angle in radians.
    static BASE_EXPORT double getJointAngleFromNorm(const DVect2 &norm);

private:
    DVect2 normal_;
    DVect2 normalUser_;
    bool   definedByNormal_ = false;
};

/** \brief Class for storing an "orientation", or a direction in 2D or 3D space.
* \copydoc Orientation2
*/
class Orientation3 {
public:
    /// Default 3D orientation is +Z normal vector.
    BASE_EXPORT Orientation3();
    /// Copy constructor
    BASE_EXPORT Orientation3(const Orientation3 &o);
    /// Explicit constructor by normal vector, \a dv3 need not be a unit vector.
    BASE_EXPORT explicit Orientation3(const DVect3 &dv3);
    /// Explicit constructor by dip and dip direction, in radians
    BASE_EXPORT explicit Orientation3(const double &dip,const double &dd=0.0);
    /// Equality operator
    BASE_EXPORT const Orientation3 &operator=(const Orientation3 &o);
    /// Comparison operator, by normal vector (unit, not user).
    BASE_EXPORT bool operator==(const Orientation3 &o) const { return(normal_==o.normal_); }
    BASE_EXPORT bool operator!=(const Orientation3 &o) const { return !operator==(o); }

    /// Returns the unit normal of this orientation.
    BASE_EXPORT const DVect3 &getNormal() const { return(normal_); }
    /// Returns the normal vector specified by the user, which may not be a unit vector.
    BASE_EXPORT const DVect3 &getNormalUser() const { return(normalUser_); }
    /// Returns the dip and dip direction (in radians) encoded in a DVect2.  x = dip, y = dd.
    BASE_EXPORT DVect2        getDipDD() const { return(dipdd_); }
    /// Returns the dip in radians.
    BASE_EXPORT double        getDip() const { return(dipdd_.x()); }
    /// Returns the dip direction in radians.
    BASE_EXPORT double        getDipDirection() const { return(dipdd_.y()); }
    /// Returns true if the orientation was defined by a normal vector -- setNormal(.
    /// Returns false if it was defined by dip/dd or joint angle.
    BASE_EXPORT bool          wasDefinedByNormal() const { return(definedByNormal_); }
    /// Returns joint angle in radians.
    BASE_EXPORT double        getJointAngle() const { return getJointAngleFromNorm(normal_); }
    /// Returns azimuth in radians.
    BASE_EXPORT double        getAzimuth() const { return getAzimuthFromNorm(normal_); }
    /// Returns plunge in radians.
    BASE_EXPORT double        getPlunge() const { return getPlungeFromNorm(normal_); }

    /// Sets the normal vector -- this value need not be a unit vector, and it is recoverable in getNormalUser().
    BASE_EXPORT void setNormal(const DVect3 &norm);
    /// Sets the X-component of the normal \b user vector, the other components are held constant.
    BASE_EXPORT void setNormalX(const double &x);
    /// Sets the Y-component of the normal \b user vector, the other components are held constant.
    BASE_EXPORT void setNormalY(const double &y);
    /// Sets the Z-component of the normal \b user vector, the other components are held constant.
    BASE_EXPORT void setNormalZ(const double &z);
    /// Sets the dip and dip direction, in radians.
    BASE_EXPORT void setDipDD(const double &dip,const double &dd);
    /// Sets dip in radians, the dip direction is held constant.
    BASE_EXPORT void setDip(const double &dip);
    /// Sets the dip direction in radians, the dip is held constant.
    BASE_EXPORT void setDipDirection(const double &dd);
    /// Sets the joint angle, in radians.
    /// Dip is always set to 90 deg, Dip Direction is negative Joint Angle.
    /// If in LHS, then Dip Direction is set to 90 deg, Dip is negative Joint Angle.
    BASE_EXPORT void setJointAngle(const double &angle);
    /// Sets the orientation in terms of azimuth and plunge.
    /// For definitions see getAzimuthFromNorm and getPlungeFromNorm.
    BASE_EXPORT void setAzimuthPlunge(const double &azimuth,const double &plunge);
    /// Utility class, returns TRUE if normal is within tolerance of this orientation
    BASE_EXPORT bool isIn(const DVect3 &normal,const DVect2 &tol) const;

    // Handy dip/dd to normal 3D conversion routines.
    /// Converts from a dip and dip direction (in radians) to a normal vector.
    static BASE_EXPORT DVect3 getNormFromDipDD(const double &dip,const double &dd);
    /// Converts from azimuth and plunge (in radians) to the corresponding vector.
    static BASE_EXPORT DVect3 getVectorFromAzimuthPlunge(const double &az,const double &plunge);
    // Handy dip/dd to normal 3D conversion routines.
    /// Converts from a dip and dip direction (in radians) to a normal vector.
    static BASE_EXPORT DVect3 getNormFromAzimuthPlunge(const double &azimuth,const double &plunge);
    /// Converts from a normal vector (need not be unit) to a dip and dip direction encoded in a DVect2.
    /// x = dip, y = dip direction.
    static BASE_EXPORT DVect2 getDipDDFromNorm(const DVect3 &norm);
    /// Converts a normal vector (need not be unit) to a joint angle in radians.
    static BASE_EXPORT Double getJointAngleFromNorm(const DVect3 &norm);
    /// Converts from a normal vector (need not be unit) to an azimuth in radians.
    /// Azimuth is the angle betweent he projection of the vector in the XY plane makes with the +y Axes, clockwise.
    static BASE_EXPORT double getAzimuthFromNorm(const DVect3 &norm);
    /// Converts from a normal vector (need not be unit) to a plunge in radians.
    /// The plunge is the angle between the normal vector and the horizontal plane, with positive being below the plane.
    static BASE_EXPORT double getPlungeFromNorm(const DVect3 &norm);
    /// These functions specify if a Left Handed coordinate system is being used.
    /// This changes the way that dip/dd is converted into a normal vector.
    static BASE_EXPORT bool   getLHS() { return lhs_; }
    static BASE_EXPORT void   setLHS(bool b) { lhs_ = b; } ///< \copydoc getLHS

private:
    DVect3      normal_;
    DVect3      normalUser_;
    DVect2      dipdd_;
    bool        definedByNormal_;
    BASE_EXPORT static bool lhs_;
};

/// @}
// EOF
