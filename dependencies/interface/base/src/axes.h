#pragma once
/** \file axes.h
* \brief 2D and 3D cartesian Axes systems.
* \addtogroup Base Base interface specification 
* @{
*/

#include "vect.h"
#include "avect.h"
#include "orientation.h"

/// \brief Class for specifying a particular 2D cartesian axes system, and converting to and from it.

/** This class is allows one to specify an alternal cartesian coordinate system 
* (including origin translation) and convert from "local" (to this Axes object)
* to "global" the default cartesian space.
* This class is a 2D version of Axes, and a 3D one exists as well.
* To the extent possible, the API has been made the same for both.
*/
class Axes2D {
public:
    /// Default construction, no initialization of data.
    BASE_EXPORT Axes2D() { }
    /// Copy constructor
    BASE_EXPORT Axes2D(const Axes2D &a) : c_(a.c_), d1_(a.d1_), d2_(a.d2_) { }
    /// Explicit constructor from two unit vectors
    /// \warning No checking is done to ensure vx and vy form an orthonormal basis!
    /// \param vx The local x direction
    /// \param vy The local y direction
    /// \param c The axes origin
    BASE_EXPORT Axes2D(const DVect2 &vx,const DVect2 &vy,const DVect2 &c=DVect2(0));
    /// Explicit contructor from dip
    /// \param dip In radians.  See the Orientation2 class for a defintion of dip.
    /// \param c The axes origin
    BASE_EXPORT explicit Axes2D(const Double &dip,const DVect2 &c=DVect2(0)) { setOrientation(dip); setOrigin(c); }
    /// Explicit contructor from Orientation2
    /// \param o Orientation of axes relative to global.
    /// \param c The axes origin.
    BASE_EXPORT explicit Axes2D(const Orientation2 &o,const DVect2 &c=DVect2(0)) { setOrientation(o);  setOrigin(c); }

    /// Equality operator
    BASE_EXPORT const Axes2D &operator=(const Axes2D &a) { d1_=a.d1_; d2_=a.d2_; c_=a.c_; return *this; }
    /// Comparison operator
    BASE_EXPORT bool operator==(const Axes2D &a) const;
    /// Comparison operator
    BASE_EXPORT bool operator!=(const Axes2D &a) const { return !operator==(a); }

    /// Specifies a new origin for the axes system.
    BASE_EXPORT void setOrigin(const DVect2 &c) { c_=c; }
    /// Specifies a new orientation for the axes, by dip (in radians).
    BASE_EXPORT void setOrientation(const Double &dip); 
    /// Specifies a new orientation for the axes, by orientation.
    BASE_EXPORT void setOrientation(const Orientation2 &o);

    /// The origin of the axes system.
    BASE_EXPORT const DVect2 &c() const { return c_; }
    /// The x unit vector.
    BASE_EXPORT const DVect2 &e1() const { return d1_; }
    /// The y unit vector
    BASE_EXPORT const DVect2 &e2() const { return d2_; }
    //   Note:  If you use these - no checks are made to ensure vectors are orthonormal!

    /// Reference access to the axes origin.
    BASE_EXPORT DVect2 &rc() { return c_; }
    /// Reference access to the axis x-direction.
    /// \warning If you use this, no check is made to ensure e1 and e2 are orthonormal!
    BASE_EXPORT DVect2 &re1() { return d1_; }
    /// Reference access to the axis y-direction.
    /// \warning If you use this, no check is made to ensure e1 and e2 are orthonormal!
    BASE_EXPORT DVect2 &re2() { return d2_; }

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
    BASE_EXPORT void   setDir(unsigned int dof,const DVect2 &v);
    BASE_EXPORT void   setDirAuto(unsigned int dof,unsigned int keepDOF=0xFFFFFFFF);
    /// Returns the equivalent orientation of degree-of-freedom dof (0-1)
    BASE_EXPORT Orientation2 getOrientation(unsigned int dof=0) const;

    // Static
    /// Creates an Axes using vector y in the +y direction (need not be unit).
    static BASE_EXPORT Axes2D make(const DVect2 &y);
    /// Creates an Axes using vector x in the +x direction (need not be unit).
    static BASE_EXPORT Axes2D makeX(const DVect2 &x);
    /// Creates an Axes using orientaion o for the +y direction.
    static BASE_EXPORT Axes2D make(const Orientation2 &o);
    /// Creates an Axes using orientaion o for the +x direction.
    static BASE_EXPORT Axes2D makeX(const Orientation2 &o);
    // Makes a default axes assuming dir only is specified.
    static BASE_EXPORT Axes2D make(const DVect2 &dir,unsigned dof);
    static BASE_EXPORT Axes2D makeDef() { Axes2D a(DVect2(1,0),DVect2(0,1));  return a; }
    /// Sets the tolerance used for comparison and detecting "near enough to zero".
    /// Defaults to 1e-6.  A single tolerance is used for all 2D *and* 3D Axes objects.
    static BASE_EXPORT void setTolerance(const double &d);
    /// Returns the tolerance used for comparison and detecting "near enough to zero".
    static BASE_EXPORT double getTolerance();
    static BASE_EXPORT Axes2D global() { return Axes2D(DVect2(1,0),DVect2(0,1)); }
private:
    DVect2 c_;
    DVect2 d1_;
    DVect2 d2_;
};



/// \brief Class for specifying a particular 3D cartesian axes system, and converting to and from it.

/** This class provides a cartesian coordinate system (3 orthogonal axes plus center), and
* supports mappings between local and global space.
* This class is a 3D version of Axes, and a 2D one exists as well.
* To the extent possible, the API has been made the same for both.
*/
class Axes3D {
public:
    /// Default constructor, no initialization of data.
    BASE_EXPORT Axes3D() { }
    /// Copy constructor
    BASE_EXPORT Axes3D(const Axes3D &a) : c_(a.c_), d1_(a.d1_), d2_(a.d2_), d3_(a.d3_) { }
    /// Explicit construction from Axes2D.  e3() = (0,0,1).
    BASE_EXPORT explicit Axes3D(const Axes2D &a) : c_(toVect3(a.c())), d1_(toVect3(a.e1())), d2_(toVect3(a.e2())), d3_(0,0,1) { }
    /// Explicit construction from three unit vectors. 
    /// \warning No checking is done to ensure vx,vy,vz form an orthonormal basis!
    BASE_EXPORT Axes3D(const DVect3 &vx, const DVect3 &vy, const DVect3 &vz, const DVect3 &c=DVect3(0));
    /// Explicit construction from dip, dip direction, and roll (all in radians)
    /// See Orientation3 for a definition of dip and dip direction.
    /// Roll defines the last degree-of-freedom, and is defined as the angle of +y on the xy plane
    ///   from +z.  If the intersection of the xy plane with +z is zero (dip 0 or 180)
    ///   the angle is from +y.
    BASE_EXPORT Axes3D(const Double &dip, const Double &ddir, const Double &roll=0.0, const DVect3 &c=DVect3(0)) { setOrientation(dip,ddir,roll); setOrigin(c); }
    /// Explicit construction from Orientation3, and roll (all in radians)
    /// See Axes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT explicit Axes3D(const Orientation3 &o,const Double &roll=0.0,const DVect3 &c=DVect3(0)) { setOrientation(o,roll); setOrigin(c); }

    /// Equality operator
    BASE_EXPORT const Axes3D &operator=(const Axes3D &a) { d1_=a.d1_; d2_=a.d2_; d3_=a.d3_; c_=a.c_; return *this; }
    /// Comparison operator
    BASE_EXPORT bool operator==(const Axes3D &a) const;
    /// Comparison operator
    BASE_EXPORT bool operator!=(const Axes3D &a) const { return !operator==(a); }

    /// Change the origin of the axes sytem.
    BASE_EXPORT void setOrigin(const DVect3 &c) { c_=c; }
    /// Change the orientation of the axes sytem, using dip, dd, and roll (all in radians)
    /// See Orientation3 for a definition of dip and dip direction.
    /// See Axes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT void setOrientation(const Double &dip, const Double &ddir, const Double &roll=0.0);  // radians
    /// Change the orientation of the axes sytem, using dip, dd, and roll (all in radians)
    /// See Axes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT void setOrientation(const Orientation3 &o,const Double &roll=0.0);

    // These return the unit vectors e1,e2,e3, defining an orthonormal basis....
    /// The origin of the axes system.
    BASE_EXPORT const DVect3 &c() const { return c_; }
    /// The x unit direction.
    BASE_EXPORT const DVect3 &e1() const { return d1_; }
    /// The y unit direction.
    BASE_EXPORT const DVect3 &e2() const { return d2_; }
    /// The z unit direction.
    BASE_EXPORT const DVect3 &e3() const { return d3_; }

    /// Reference access to the origin of the axes system.
    BASE_EXPORT DVect3 &rc() { return c_; }
    /// Reference access to the x direction.
    /// \warning If you use these - no checks are made to ensure the system remains orthonormal!
    BASE_EXPORT DVect3 &re1() { return d1_; }
    /// Reference access to the y direction.
    /// \warning If you use these - no checks are made to ensure the system remains orthonormal!
    BASE_EXPORT DVect3 &re2() { return d2_; }
    /// Reference access to the z direction.
    /// \warning If you use these - no checks are made to ensure the system remains orthonormal!
    BASE_EXPORT DVect3 &re3() { return d3_; }
    BASE_EXPORT DVect3 &rdof(unsigned dof) { if (!dof) return d1_; else if (dof==1) return d2_; else if (dof==2) return d3_;  else return c_; }

    /// Converts a vector from "global" (cartesian) system to this system.
    BASE_EXPORT DVect3 toLocal(const DVect3 &v) const;
    /// Converts a vector from this system to a "global" system.
    BASE_EXPORT DVect3 toGlobal(const DVect3 &v) const;
    /// Return e1, e2, or e3 based on degree-of-freedom (0-2)
    BASE_EXPORT DVect3 getDir(unsigned int dof) const;
    BASE_EXPORT void   setDir(unsigned int dof,const DVect3 &v);
    BASE_EXPORT void   setDirAuto(unsigned int dof,unsigned int keepDOF=0xFFFFFFFF);
    /// Returns equivalent orientation of e1, e2, or e2 based on degree-of-freedom (0-2)
    BASE_EXPORT Orientation3 getOrientation(unsigned int dof=0) const;
    /// Returns the effective roll of the current axes system.
    /// See Axes3D(const Double &dip,const Double &ddir,const Double &roll) for a defintion of roll.
    BASE_EXPORT Double getRoll() const;

    // Static
    /// Creates an Axes3D using vector z in the +z direction (need not be unit).
    /// By default, x will be dip of the plane defined using z as a normal,
    ///   and y will be the strike.
    /// Roll defines a rotation about the z axis (clockwise with vector facing towards you).
    static BASE_EXPORT Axes3D make(const DVect3 &z,const Double &roll=0.0);
    /// Same as make(const DVect3 &z,const Double &roll=0.0), except given vector is x.
    /// By default, then, Y will be dip and Z will be strike.
    static BASE_EXPORT Axes3D makeX(const DVect3 &x,const Double &roll=0.0);
    /// Creates an Axes3D using orientation o in the +z direction.
    /// By default, x will be dip of the plane defined using o as a normal,
    ///   and y will be the strike.
    /// Roll defines a rotation about the z axis (clockwise with vector facing towards you).
    static BASE_EXPORT Axes3D make(const Orientation3 &o,const Double &roll=0.0);
    /// Same as make(const Orientation3 &o,const Double &roll=0.0), except given vector is x.
    /// By default, then, Y will be dip and Z will be strike.
    static BASE_EXPORT Axes3D makeX(const Orientation3 &o,const Double &roll=0.0);
    /// Same as make(z,roll), but instead of using roll to determine the x,y orientation it uses a provided x direction.
    /// Note that the X vector will be corrected (made unit and perpendicular to z).
    static BASE_EXPORT Axes3D make(const DVect3 &z,const DVect3 &x);
    // Makes a default axes assuming dir only is specified.
    static BASE_EXPORT Axes3D make(const DVect3 &dir,unsigned dof);
    static BASE_EXPORT Axes3D makeDef() { Axes3D a(DVect3(1,0,0),DVect3(0,1,0),DVect3(0,0,1));  return a; }
    /// Tolerance used for comparison and detecting "near enough to zero".
    /// Defaults to 1e-6.  A single tolerance is used for all 2D *and* 3D Axes objects.
    static BASE_EXPORT void setTolerance(const double &d); // Defaults to 1e-6
    /// Returns the Tolerance used for comparison and detecting "near enough to zero".
    static BASE_EXPORT double getTolerance();
    static BASE_EXPORT Axes3D global() { return Axes3D(DVect3(1,0,0),DVect3(0,1,0),DVect3(0,0,1)); }
private:
    DVect3 c_;
    DVect3 d1_;
    DVect3 d2_;
    DVect3 d3_;
};

/// @}
// EOF
