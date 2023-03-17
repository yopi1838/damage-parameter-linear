#pragma once
/** \file quat.h
* \brief 2D and 3D quaternion utility classes.
* \addtogroup Base Base interface specification 
* 
*/

#include "avect.h"
#include "matrix.h"
#include "vect.h"
#include <cmath>
#include <cassert>

/** \brief 2D quaternion-like utility class. In this case only the angle (in radians) is stored 
*  as opposed to the complex number. Multiplication of quaternions is supposed to result in 
*  a combined rotation which, in 2D, is the addition of the angles. This is for consitency with 
*  the Quat3 class.
*/

class Quat2 {
public:
    /// Default constructor.
    Quat2(): ang_(0) { }
    /// Constructor initializing the angle.
    Quat2(const double &a): ang_(a) { }
    /// Destructor.
    ~Quat2() { }
    /// Copy constructor.
    Quat2(const Quat2 &q): ang_(q.ang_) { }
    /// Set the angle.
    void setQuat(const double &a){ ang_ = a; }

    /// Access the angle.
    const double &ang() const { return ang_; }

    /// Access a reference to the angle.
    double &rang() { return ang_; }

    /// Reset the angle to 0.
    void reset() { ang_=0.0; }
    /// Reset to the identity.
    void ident() { ang_ = 0.0; }
    /// Return the identity quaternion.
    static BASE_EXPORT Quat2 identity() { Quat2 ret(0.0);  return ret;}

    /// Convert the quaternion to a rotation matrix.
    BASE_EXPORT DMatrix<2,2> quatToMatrix() const;
    /// Convert a DVect2 into a quaternion. This assumes a rotation from the positive x axis.
    static BASE_EXPORT Quat2 fromVect(const DVect2 &v);
    /// Convert a matrix into a quaternion
    BASE_EXPORT void fromMatrix(const DMatrix<2,2> &);
    /// Take an axis-angle representation and convert it to a quaternion where the angle is in radians.
    /// In this case the axis is ignored.
    BASE_EXPORT void fromAxisAngle(const DVect2 &,const double &);
    /// Increment by this rotation. In this case the axis is ignored.
    BASE_EXPORT void incrementAxisAngle(const DVect2 &,const double &);
    /// Just spin about the Z axis
    BASE_EXPORT void spinAboutZ(const double &);
    /// Rotate a DVect2 by the quaternian to get a new DVect2
    BASE_EXPORT DVect2 rotate(const DVect2 &v) const;
    /// Take 1 orthogonal axis for an axis system and convert to a quaternion. The second axis is 
    /// ignored.
    BASE_EXPORT void fromUnitAxes(const DVect2 &,const DVect2 &);
    /// Increment the quaternion due to a rotation about Z by the DAVect.
    BASE_EXPORT void increment(const DAVect2 &);

    /// Convert to the conjugate quaternion - rotation by the opposite sign.
    void conj() { ang_*=-1; }
    /// Return the conjugate of the quaternion.
    Quat2 getConj() const { Quat2 q(-ang_); return q; }

    /// Multiplication by a double.
    Quat2 operator *(const double &d) const { Quat2 q(ang_*d); return q; }
    /// Division by a double.
    Quat2 operator /(const double &d) const { Quat2 q(ang_/d); return q; }
    /// Multiplication of a double times a quaternion.
    friend Quat2 operator *(const double &d,const Quat2 &p2) { Quat2 q(p2*d); return q;}
    /// Multiplication of two quaternions. This is the composition of rotations which corresponds to 
    /// the addition of the angles.
    Quat2 operator *(const Quat2 &v) const { Quat2 q(ang_+v.ang_); return q; }
    /// Addition of quaterions. This is also the composition of rotations in 2D.
    Quat2 operator +(const Quat2 &v) const { Quat2 q(ang_+v.ang_); return q; }

    /// In place multiplaction by a double.
    const Quat2 & operator *=(const double &d) { ang_ *= d; return *this; }
    /// In place division by a double.
    const Quat2 & operator /=(const double &d) { ang_ /= d; return *this; }
    /// In place multiplication of two quaternions. This is the composition of rotations which 
    /// corresponds to the addition of the angles.
    const Quat2 & operator *=(const Quat2 &v) { ang_ += v.ang_; return *this; }
    /// In place addition of two quaternions. This is the composition of rotations which 
    /// corresponds to the addition of the angles.
    const Quat2 & operator +=(const Quat2 &v) { ang_ += v.ang_; return *this; }
    /// Euality operator
    bool operator ==(const Quat2 &v) const { return (ang_ == v.ang_); }

private:
    double ang_; //the angle of rotation
};

/** \brief 3D quaternion utility class.
*/
class Quat3 { 
public:
    /// Default constructor.
    Quat3(): w_(0), i_(0), j_(0), k_(0) { }
    /// Constructor taking 4 double arguments, the first is the real part and the remaining three are the 
    /// imaginary parts.
    Quat3(const double &w1,const double &x,const double &y,const double &z) { w_ = w1; i_ = x; j_ = y; k_ = z; }
    /// Default destructor.
    ~Quat3() { }
    /// Copy constructor.
    Quat3(const Quat3 &q) { w_ = q.w_; i_ = q.i_; j_ = q.j_; k_ = q.k_; }
    /// Set the real and imaginary parts.
    void setQuat(const double &w1,const double &x,const double &y,const double &z){ w_ = w1; i_ = x; j_ = y; k_ = z; }

    /// Access the real part.
    const double &w() const { return w_; }
    /// Access the first imaginary part.
    const double &i() const { return i_; }
    /// Access the second imaginary part.
    const double &j() const { return j_; }
    /// Access the third imaginary part.
    const double &k() const { return k_; }

    /// Access a reference to the real part.
    double &rw() { return w_; }
    /// Access a reference to the first imaginary part.
    double &ri() { return i_; }
    /// Access a reference to the second imaginary part.
    double &rj() { return j_; }
    /// Access a reference to the third imaginary part.
    double &rk() { return k_; }

    /// Reset the quaternion.
    void reset() { w_=0.0; i_=0.0; j_=0.0; k_=0.0; }
    /// Reset the quaternion to the identity.
    void ident() { w_=1.0; i_=0.0; j_=0.0; k_=0.0; }
    /// Returns the identity quaternion.
    static BASE_EXPORT Quat3 identity() { Quat3 ret(1.0,0.0,0.0,0.0);  return ret;}
    /// Returns a boolean indicating whether or not the quaternion is null
    BASE_EXPORT bool isNull() const { return (abs(w_) > std::numeric_limits<double>::epsilon() || abs(i_) > std::numeric_limits<double>::epsilon() || abs(j_) > std::numeric_limits<double>::epsilon() || abs(k_) > std::numeric_limits<double>::epsilon()) ? false : true; }
    /// Convert the quaternion to a rotation matrix.
    BASE_EXPORT DMatrix<3,3> quatToMatrix() const;
    /// Convert to Axis Angle where the magnitude of the axis is the angle in radians
    BASE_EXPORT DVect3 quatToAAngle() const;
    /// Convert a DVect3 into a quaternion
    static BASE_EXPORT Quat3 fromVect(const DVect3 &v);
    /// Convert a matrix into a quaternion
    BASE_EXPORT void fromMatrix(const DMatrix<3,3> &);
    /// Take an axis-angle representation and convert it to a quaternion where the angle is in radians
    BASE_EXPORT void fromAxisAngle(const DVect3 &,const double &);
    /// Take the Euler representation and convert it to a quaternion (angles in DEGREES). This assumes an
    /// X,Y,Z rotation (really Bryant angles)
    BASE_EXPORT void fromEuler(const DVect3 &);
    /// Increment by this rotation
    BASE_EXPORT void incrementAxisAngle(const DVect3 &,const double &);
    /// Just spin about the X axis
    BASE_EXPORT void spinAboutX(const double &);
    /// Just spin about the Y axis
    BASE_EXPORT void spinAboutY(const double &);
    /// Just spin about the Z axis
    BASE_EXPORT void spinAboutZ(const double &);
    /// Rotate a DVect3 by the quaternian to get a new DVect3
    BASE_EXPORT DVect3 rotate(const DVect3 &v) const;
    /// Return the "real" vector part of the quaternian.
    BASE_EXPORT DVect3 real() const { return DVect3(i_,j_,k_); }

    /// Take 2 orthogonal axis for an axis system and convert to a quaternion
    BASE_EXPORT void fromUnitAxes(const DVect3 &,const DVect3 &);
    /// Increment the quaternion due to a rotation about the x, y, and z axes by the DAVect
    BASE_EXPORT void increment(const DAVect3 &);
    /// Get component e1 of the rotation matrix
    BASE_EXPORT DVect3 e1() const;
    /// Get component e2 of the rotation matrix
    BASE_EXPORT DVect3 e2() const;
    /// Get component e3 of the rotation matrix
    BASE_EXPORT DVect3 e3() const;
    /// Convert the quaternion into euler angles in degrees. This assumes and X,Y,Z order (really Bryant 
    /// angles)
    BASE_EXPORT DVect3 quatToEuler() const;

    /// Convert the quaternion to the conjugate.
    void conj() { i_=-i_; j_=-j_; k_=-k_; }
    /// Return the conjugate of the quaternion. 
    Quat3 getConj() const { Quat3 q(w_,-i_,-j_,-k_); return q; }
    /// Normalize the quaternion.
    BASE_EXPORT void normalize() { double m=1.0/std::sqrt(w_*w_+i_*i_+j_*j_+k_*k_); w_*=m; i_*=m; j_*=m; k_*=m; }

    /// Multiplication by a double.
    Quat3 operator *(const double &d) const { Quat3 q(w_*d,i_*d,j_*d,k_*d); return q; }
    /// Division by a double.
    Quat3 operator /(const double &d) const { Quat3 q(w_/d,i_/d,j_/d,k_/d); return q; }
    /// Product of a double and a quaternion.
    friend Quat3 operator *(const double &d,const Quat3 &p2) {Quat3 q(p2*d); return q;}
    /// Product of two quaternions.
    Quat3 operator *(const Quat3 &v) const { Quat3 q(w_*v.w_-i_*v.i_-j_*v.j_-k_*v.k_,w_*v.i_+i_*v.w_+j_*v.k_-k_*v.j_,w_*v.j_-i_*v.k_+j_*v.w_+k_*v.i_,w_*v.k_+i_*v.j_-j_*v.i_+k_*v.w_); return q; }
    /// Addition of two quaternions.
    Quat3 operator +(const Quat3 &v) const { Quat3 q(w_+v.w_,i_+v.i_,j_+v.j_,k_+v.k_); return q; }
    /// DVect3 times a quaternion, in that order.
    Quat3 friend operator *(const DVect3 &v,const Quat3 &q) {Quat3 q2(0.0,v.x(),v.y(),v.z()); return q2*q;} 
    /// Quaternion times a DVect3, in that order.
    Quat3 operator *(const DVect3 &v) const {Quat3 q2(0.0,v.x(),v.y(),v.z()); return *this*q2;} 

    /// In place multiplaction by a double.
    const Quat3 & operator *=(const double &d) { w_ *= d; i_ *= d; j_ *= d; k_ *= d; return *this; }
    /// In place division by a double.
    const Quat3 & operator /=(const double &d) { w_ /= d; i_ /= d; j_ /= d; k_ /= d; return *this; }
    /// In place multiplication of two quaternions.
    const Quat3 & operator *=(const Quat3 &v) { double tw(w_*v.w_-i_*v.i_-j_*v.j_-k_*v.k_);
    double ti(w_*v.i_+i_*v.w_+j_*v.k_-k_*v.j_);
    double tj(w_*v.j_-i_*v.k_+j_*v.w_+k_*v.i_);
    double tk(w_*v.k_+i_*v.j_-j_*v.i_+k_*v.w_);
    //double tw(v.w_*w_-v.i_*i_-v.j_*j_-v.k_*k_);
    //double ti(v.w_*i_+v.i_*w_+v.j_*k_-v.k_*j_);
    //double tj(v.w_*j_-v.i_*k_+v.j_*w_+v.k_*i_);
    //double tk(v.w_*k_+v.i_*j_-v.j_*i_+v.k_*w_);
    w_ = tw; i_ = ti; j_ = tj; k_ = tk;
    return *this; }
    /// In place addition of two quaternions.
    const Quat3 & operator +=(const Quat3 &v) { w_ += v.w_; i_ += v.i_; j_ += v.j_; k_ += v.k_; return *this; }
    /// Euality operator
    bool operator ==(const Quat3 &v) const { return (w_ == v.w_ && i_==v.i_ && j_==v.j_ && k_==v.k_); }

private:
    double w_; //the real amplitude part
    double i_,j_,k_; //the imaginary parts
};
