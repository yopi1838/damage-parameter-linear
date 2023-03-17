#pragma once
/** \file
* \brief A Symmetric 2nd order tensor.
* \addtogroup Base Base interface specification 
* @{
*/

#include "axes.h"
#include <cassert>
#include <limits>
#include <algorithm>

class SymTensorInfo;

/** \brief A symmetric 2nd order tensor.
*
* Includes helper functions for converting to and from eigenvalues and eigenvectors.
*/
class BASE_EXPORT SymTensor {  
public:
    /// Default constructor, no data initialization.
#ifdef _DEBUG
    SymTensor() { d11_ = d22_ = d33_ = d12_ = d13_ = d23_ = initVal<double>(); }
#else
#pragma warning(push)
#pragma warning(disable:26495) // Init warning- not initializing on purpose
    SymTensor() { }
#pragma warning(pop)
#endif
    /// Copy constructor.
    SymTensor(const SymTensor &s) { operator=(s); }
    /// Explicit constructor, from the six components.\n
    /// The diagonal components are optional.
    explicit SymTensor(const double &i11,const double &i22=0.0,const double &i33=0.0,
                       const double &i12=0.0,const double &i13=0.0,const double &i23=0.0)
                      : d11_(i11), d22_(i22), d33_(i33), d12_(i12), d13_(i13), d23_(i23) { }
    /// Assignment operator.
    const SymTensor &operator=(const SymTensor &s);

    /// Equality operator
    bool operator==(const SymTensor &s) const;
    bool operator<(const SymTensor &s) const;

    /// Component access, note that s12 is equivalent to s21 (for instance).
    const double &s11() const { return d11_; }
    const double &s22() const { return d22_; } ///< \copydoc s11
    const double &s33() const { return d33_; } ///< \copydoc s11
    const double &s12() const { return d12_; } ///< \copydoc s11
    const double &s13() const { return d13_; } ///< \copydoc s11
    const double &s23() const { return d23_; } ///< \copydoc s11
    const double &s21() const { return d12_; } ///< \copydoc s11
    const double &s31() const { return d13_; } ///< \copydoc s11
    const double &s32() const { return d23_; } ///< \copydoc s11
    /// Reference access to components, note that s12 is equivalent to s21 (for instance).
    double &rs11() { return d11_; } 
    double &rs22() { return d22_; } ///< \copydoc rs11
    double &rs33() { return d33_; } ///< \copydoc rs11
    double &rs12() { return d12_; } ///< \copydoc rs11
    double &rs13() { return d13_; } ///< \copydoc rs11
    double &rs23() { return d23_; } ///< \copydoc rs11
    double &rs21() { return d12_; } ///< \copydoc rs11
    double &rs31() { return d13_; } ///< \copydoc rs11
    double &rs32() { return d23_; } ///< \copydoc rs11
    /** \brief Allows Index access to tensor components.
    *
    * Order = xx, yy, zz, xy, xz, yz....  base 0
    */
    inline const double &operator[](unsigned int i) const;
    inline double &operator[](unsigned int i);

    /// \brief Allows 2D index access to tensor components - base 0.
    const double &operator()(unsigned int i,unsigned int j) const;
    /// \brief Allows 2D index access to tensor components - base 0.
    double &operator()(unsigned int i,unsigned int j);

    /** \brief Returns the eigenvalues (principal components) and eigenvectors (principal directions).
    *  The information necessary to regenerate the tensor from eigenvalues is placed in the
    *    helper class \a si.
    *  x = minimum (most negative), y = intermediate, z = maximum (most positive)
    */
    DVect3 getEigenInfo(SymTensorInfo *si=0) const;
    /// Returns the trace of the tensor (11+22+33). I1
    double getTrace() const { return d11_+d22_+d33_; }
    /// Returns the deviatoric tensor
    SymTensor getDeviatoric() const { double p=getTrace()/3.0; return SymTensor(d11_-p, d22_-p, d33_-p, d12_, d13_, d23_); }
    /// Same as getTrace() - returns the first invariant
    double getI1() const { return getTrace(); }
    /// Returns the second invariant
    double getI2() const { return d11_*d22_ + d22_*d33_ + d11_*d33_ - d12_*d12_ - d23_*d23_ - d13_*d13_; }
    /// Returns the third invariant, or I3
    double getI3() const { return getDeterminate(); }
    /// Returns the second invariant of the deviatoric -- J2
    double getJ2() const;
    /// Returns the second invariant of the deviatoric -- J2
    double getJ2(SymTensor *dev, double *I1=nullptr);
    //Keep only getJ2(), Von Mises and Octahedral stress/strain are calculated from J2. 
    ///// Returns Von Mises (equivelent) stress -- sqrt(3.0*J2)
    //double getVonMises() const { return sqrt(3.0*getJ2()); }
    ///// Returns Von Mises (equivelent) strain  -- sqrt((4/9)*J2)
    //double getVonMisesStrain() const { return sqrt(4.0*getJ2()/9.0); }
    ///// Returns Octahedral Shear stress -- sqrt((2/3)*J2)
    //double getOctahedralShear() const { return sqrt(2.0*getJ2()/3.0); }
    ///// Returns Octahedral Shear strain -- sqrt((8/3)*J2)
    //double getOctahedralShearStrain() const { return sqrt(8.0*getJ2()/3.0); }
    /// returns the third invariant of the deviatoric -- J3
    double getJ3() const;
    /// returns the Lode's angle in rad, TX = pi/6, TE = -pi/6
    double getLode(double *I1 = nullptr, double *J2 = nullptr, double *J3 = nullptr);
    /// Returns the determinate of the tensor. 
    double getDeterminate() const;
    /// Returns a scalar norm (magnitude) value for the tensor, can be used for tolerance checking, etc. 
    double getNorm2() const { return d11_*d11_ + d22_*d22_ + d33_*d33_ + 2.0*(d12_*d12_ + d13_*d13_ + d23_*d23_); }
    /// Total Measure now is defined as the distance of the stress/strain tensor point at the
    /// principal stress/strain space to the origin of the same space.
    /// Returns a scalar, (I1/sqrt(3), sqrt(2*J2))
    double getTotalMeasure() const;
    /// Transforms tensor from local (\a a) to global system.
    SymTensor toGlobal(const Axes3D &a) const;
    /** \brief Performs the linear mapping represented by the tensor on the vector input.
    * 
    * In stress terms, finds the tractions in the given directions if input is a unit vector.
    */
    inline DVect3 operator*(const DVect3 &input) const;
    inline SymTensor operator*(const double &mul) const;
    inline const SymTensor &operator*=(const double &mul);
    inline SymTensor operator/(const double &mul) const;
    inline const SymTensor &operator/=(const double &mul);
    /// Returns a SymTensor with every component multiplied by a scalar value
    SymTensor mul(const double &d) const { SymTensor ret(d11_*d,d22_*d,d33_*d,d12_*d,d13_*d,d23_*d);  return ret; }
    /// += operator for a SymTensor
    const SymTensor &operator+=(const SymTensor &s) { d11_+=s.d11_; d22_+=s.d22_; d33_+=s.d33_; d12_+=s.d12_; d13_+=s.d13_; d23_+=s.d23_;  return *this; }
    const SymTensor &operator-=(const SymTensor &s) { d11_-=s.d11_; d22_-=s.d22_; d33_-=s.d33_; d12_-=s.d12_; d13_-=s.d13_; d23_-=s.d23_;  return *this; }
    inline SymTensor operator+(const SymTensor &s) const;
    inline SymTensor operator-(const SymTensor &s) const;
    /// Creates a full SymTensor given principal vectors (in a DVect3) and an Axes3D for orientation.
    static SymTensor fromPrincipal(const DVect3 &prin,const Axes3D &axes);
    /// Find a stress that produces the given force when multiplied by the given normal
    /// There is not a unique solution to to this - uses the max normal abs component.
    /// Note does not check normal.
    static SymTensor fromForceNormal(const DVect3 &normal, const DVect3 &force);
        /// Determines whether or not the SymTensor is diagonal.
    bool isDiagonal(double tol = std::numeric_limits<double>::epsilon()*1000.0) const { return (abs(d12_) > tol || abs(d13_) > tol || abs(d23_) > tol) ? false : true; }
    inline void adjustTrace(const double &newTrace);
    inline void incrementDiagonal(const double &increment) { d11_ += increment;  d22_ += increment;  d33_ += increment; }
    inline void rotate(const DVect3 &rot);
    inline double maxAbs() const { return std::max(std::abs(d11_),std::max(std::abs(d22_),std::max(std::abs(d33_),std::max(std::abs(d12_),std::max(std::abs(d13_),std::abs(d23_)))))); }
private:
    double d11_;
    double d22_;
    double d33_;
    double d12_;
    double d13_;
    double d23_;
};

/** \brief SymTensor eigenvalue and direction helper class.
*
* This deliberately opaque structure represents orientation information necessary to regenerate a full
*   symmetric tensor from eigenvalues (principal components). \n
* Note that the eigenvalues may change between a call
*   to SymTensor::getEigenInfo() and SymTensorInfo::resolve().
*/
class SymTensorInfo {
public:
    friend class SymTensor;
    /// Default constructor
    BASE_EXPORT SymTensorInfo(): type_(Type::ThreeDCube) { }
    /// Copy constructor
    BASE_EXPORT SymTensorInfo(const SymTensorInfo &si);
    /// Equality operator
    BASE_EXPORT const SymTensorInfo &operator=(const SymTensorInfo &si);
    /// Returns eigen directions (minimum, intermediate, maximum).
    BASE_EXPORT Axes3D getAxes() const;
    /// Regenerates full tensor from info + principal directions.
    BASE_EXPORT SymTensor resolve(const DVect3 &prin) const;
private:
    enum class Type { ThreeDCube, ThreeDJacobi, ZMax, ZMid, ZMin };
    Type   type_;
    Axes3D axes_;
};

inline const SymTensor &SymTensor::operator=(const SymTensor &s) { 
    d11_ = s.d11_; 
    d22_ = s.d22_; 
    d33_ = s.d33_; 
    d12_ = s.d12_; 
    d13_ = s.d13_; 
    d23_ = s.d23_; 
    return *this; 
}

inline bool SymTensor::operator==(const SymTensor &s) const { 
    return (d11_ == s.d11_ && d22_ == s.d22_ && 
            d33_ == s.d33_ && d12_ == s.d12_ && d13_ == s.d13_ && 
            d23_ == s.d23_);
}

inline const double &SymTensor::operator[](unsigned int i) const { 
    assert(i<6);  
    switch (i) {
    case 0:  return d11_;
    case 1:  return d22_;
    case 2:  return d33_;
    case 3:  return d12_;
    case 4:  return d13_;
    case 5:  return d23_;
    }
    return d11_;
}

inline double &SymTensor::operator[](unsigned int i) { 
    assert(i<6);  
    switch (i) {
    case 0:  return d11_;
    case 1:  return d22_;
    case 2:  return d33_;
    case 3:  return d12_;
    case 4:  return d13_;
    case 5:  return d23_;
    }
    return d11_;
} 

inline double SymTensor::getTotalMeasure() const { 
    double I1 = getTrace(); 
    double J2 = getJ2(); 
    return sqrt(I1*I1/3.0 + 2.0*J2); 
}

inline DVect3 SymTensor::operator*(const DVect3 &normal) const {
    return DVect3(normal.x()*s11() + normal.y()*s12() + normal.z()*s13(),
                  normal.x()*s21() + normal.y()*s22() + normal.z()*s23(),
                  normal.x()*s31() + normal.y()*s32() + normal.z()*s33());
}

inline SymTensor SymTensor::operator*(const double &mul) const {
    SymTensor ret;
    ret.d11_ = d11_ * mul;
    ret.d22_ = d22_ * mul;
    ret.d33_ = d33_ * mul;
    ret.d12_ = d12_ * mul;
    ret.d13_ = d13_ * mul;
    ret.d23_ = d23_ * mul;
    return ret;
}

inline const SymTensor &SymTensor::operator*=(const double &mul) {
    d11_ *= mul;
    d22_ *= mul;
    d33_ *= mul;
    d12_ *= mul;
    d13_ *= mul;
    d23_ *= mul;
    return *this;
}

inline SymTensor SymTensor::operator/(const double &mul) const {
    SymTensor ret;
    ret.d11_ = d11_ / mul;
    ret.d22_ = d22_ / mul;
    ret.d33_ = d33_ / mul;
    ret.d12_ = d12_ / mul;
    ret.d13_ = d13_ / mul;
    ret.d23_ = d23_ / mul;
    return ret;
}

inline const SymTensor &SymTensor::operator/=(const double &mul) {
    d11_ /= mul;
    d22_ /= mul;
    d33_ /= mul;
    d12_ /= mul;
    d13_ /= mul;
    d23_ /= mul;
    return *this;
}

inline SymTensor SymTensor::operator+(const SymTensor &s) const {
    SymTensor ret;
    ret.d11_ = d11_ + s.d11_;
    ret.d22_ = d22_ + s.d22_;
    ret.d33_ = d33_ + s.d33_;
    ret.d12_ = d12_ + s.d12_;
    ret.d13_ = d13_ + s.d13_;
    ret.d23_ = d23_ + s.d23_;
    return ret;
}

inline SymTensor SymTensor::operator-(const SymTensor &s) const {
    SymTensor ret;
    ret.d11_ = d11_ - s.d11_;
    ret.d22_ = d22_ - s.d22_;
    ret.d33_ = d33_ - s.d33_;
    ret.d12_ = d12_ - s.d12_;
    ret.d13_ = d13_ - s.d13_;
    ret.d23_ = d23_ - s.d23_;
    return ret;
}

inline void SymTensor::adjustTrace(const double &newTrace) {
    static const double d1d3 = 0.33333333333333333333333333333333;
    /* --- strain differences --- */
    double dx = s11() - s22();
    double dy = s22() - s33();
    double dz = s33() - s11();
    rs11() = (newTrace + dx - dz) * d1d3;
    rs22() = (newTrace + dy - dx) * d1d3;
    rs33() = (newTrace + dz - dy) * d1d3;
}

inline void SymTensor::rotate(const DVect3 &rot) {
    SymTensor copy(*this);
    d11_ +=  2.0*( copy.s12()*rot.x() + copy.s13()*rot.y());
    d22_ +=  2.0*(-copy.s12()*rot.x()                      + copy.s23()*rot.z());
    d33_ += -2.0*(                      copy.s13()*rot.y() + copy.s23()*rot.z());
    d12_ += ((copy.s22() - copy.s11())*rot.x() +                copy.s23()*rot.x() +                copy.s13()*rot.z());
    d13_ += (               copy.s23()*rot.x() + (copy.s33() - copy.s11())*rot.x() -                copy.s12()*rot.z());
    d23_ += (              -copy.s13()*rot.x() -                copy.s12()*rot.x() + (copy.s33() - copy.s22())*rot.z());
}

/// @}
// EoF
