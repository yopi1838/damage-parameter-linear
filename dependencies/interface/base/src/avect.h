#pragma once
/**
* \file
* \brief Angular Vectors.
* \addtogroup Base Base interface specification
* @{
*/

#include "vect.h"

/// \brief 2D Angular vector class.

/** This specialized version of a vector class is intended to store "angular" values, 
*  values that are a scalar in 2D (z component only) and a full vector in 3D.

*  The intention is to make it easier to write code that is the same in 2D and 3D without
*    wasting computation time and storage.

*  Also, hopefully they will interact "properly" with standard Vector2<> types.

*  For instance, the cross and dot product operators '&' and '|' should return the correct types,
*    as if they were full 3D vector objects.

*  A Vector2<T> crossed with an AVector2<T> will return a Vector2<T>.

*  A Vector2<T> dotted with an AVector2<T> will return 0.0.
*/

namespace AVector2Util {
    static const double nullValue_ = 0.0;
};

/// \brief 2D Angular vector class.

/** This specialized version of a vector class is intended to store "angular" values,
*  values that are a scalar in 2D (z component only) and a full vector in 3D.

*  The intention is to make it easier to write code that is the same in 2D and 3D without
*    wasting computation time and storage.

*  In the 3D case, and AVector3 is a thin wrapper around a Vector3.
*/
template <class T> class AVector2 {
public:
    // Creators
    /// Default constructor, no data initialization.
#ifdef _DEBUG
    AVector2() { z_ = std::numeric_limits<T>::max(); }
#else
#pragma warning(push)
#pragma warning(disable:26495) // Init warning- not initializing on purpose
    AVector2() { }
#pragma warning(pop)
#endif
    /// Copy constructor
    AVector2(const AVector2 &av) : z_(av.z_) { }
    /// Explicit conversion constructor, the z component is initialized to t.
    explicit AVector2(const T &t) : z_(t) { }
    /// Explicit conversion constructor, the z component is set to 0, the x and y values of \a v are lost.
    explicit AVector2(const Vector2<T> &) : z_(0) { }
    /// Explicit convertion contructor, the z component or \a v is copied but he x and y values are lost.
    explicit AVector2(const Vector3<T> &v) : z_(v.z_) { }

    /// Member access - the x and y values are always uniquely 0 for a 2D angular vector.
    const T &x() const { return AVector2Util::nullValue_; }
    /// Member access - the x and y values are always uniquely 0 for a 2D angular vector.
    const T &y() const { return AVector2Util::nullValue_; }
    /// Member access - returns the z component of the 2D angular vector
    const T &z() const { return z_; }
    /// General degree-of-freedom access.  If u is 2 returns the z component, otherwise returns 0.0.
    const T &dof(UInt u) const {
#ifdef _DEBUG
        if (u>2) throw std::runtime_error("Vector index out of range.");
#endif
        if (u==2) return z_;
        return AVector2Util::nullValue_;
    }

    /// Returns a reference to the z component, cannot reference access x or y.
    T &rz() { return z_; }
    /// Returns a reference to the z component if u=2, any other value of u is an error.
    T &rdof(UInt u) {
#ifdef _DEBUG
        if (u!=2) throw std::runtime_error("Vector index out of range.");
#endif
        u;
        return z_;
    }

    const T &operator[](UInt u) const { return dof(u); } ///< \copydoc dof
    T &operator[](UInt u) { return rdof(u); } ///< \copydoc rdof

    T fsum() const { return std::abs(z_); }   ///< The Manhatten norm.
    T mag2() const { return z_*z_; }          ///< The square of the magnitude, or the vector dotted with itself.
    T mag() const { return std::abs(z_); }    ///< The magnitude of the vector.
    T area() const { return 0; }              ///< Area represented by x*y, always returns 0.0.
    T volume() const { return 0; }            ///< Volume represented by x*y*z, always returns 0.0.
    AVector2<T> unit() const { AVector2<T> v(*this); if (z_) {v.z_ =  z_ < 0 ?  -1 : 1;} return v; } ///< returns a unit vector.
    AVector2<T> abs() const { return std::abs(z_); } ///< Returns a vector of absolute values of components
    void fill(const T &d) { z_=d; } ///<Fill all components with d.
    T maxComp() const { return std::max(0.0,z_); }   ///< Returns the maximum component (assuming x and y are 0.0)
    T minComp() const { return std::min(0.0,z_); }   ///< Returns the minimum component (assuming x and y are 0.0)

    bool operator==(const AVector2<T> &av) const { return z_==av.z_; } ///< comparison operator
    bool operator!=(const AVector2<T> &av) const { return z_!=av.z_; } ///< comparison operator
    bool operator<(const AVector2<T> &av) const { return z_ < av.z_; } ///< comparison operator
    bool operator>(const AVector2<T> &av) const { return z_ > av.z_; } ///< comparison operator

    const AVector2<T> &operator+=(const AVector2<T> &v) { z_ += v.z_; return *this; } ///< in-place math operator
    const AVector2<T> &operator-=(const AVector2<T> &v) { z_ -= v.z_; return *this; } ///< in-place math operator
    const AVector2<T> &operator*=(const AVector2<T> &v) { z_ *= v.z_; return *this; } ///< in-place math operator
    const AVector2<T> &operator*=(const T &t) { z_ *= t; return *this; }              ///< in-place math operator
    const AVector2<T> &operator/=(const AVector2<T> &v) { z_ /= v.z_; return *this; } ///< in-place math operator
    const AVector2<T> &operator/=(const T &t) { z_ /= t; return *this; }              ///< in-place math operator

    AVector2<T> operator+(const AVector2<T> &av) const { AVector2<T> out(z_+av.z_);  return out; } ///< Binary math operator - require temp (till C++0x)
    AVector2<T> operator-(const AVector2<T> &av) const { AVector2<T> out(z_-av.z_);  return out; } ///< Binary math operator - require temp (till C++0x)
    AVector2<T> operator*(const AVector2<T> &av) const { AVector2<T> out(z_*av.z_);  return out; } ///< Binary math operator - require temp (till C++0x)
    AVector2<T> operator*(const T &t) const { AVector2<T> out(z_*t);  return out; }                ///< Binary math operator - require temp (till C++0x)
    AVector2<T> operator/(const AVector2<T> &av) const { AVector2<T> out(z_/av.z_);  return out; } ///< Binary math operator - require temp (till C++0x)
    AVector2<T> operator/(const T &t) const { AVector2<T> out(z_/t);  return out; }                 ///< Binary math operator - require temp (till C++0x)

    T operator&(const AVector2<T> &) const { return 0; }   ///< Cross product with another AVector2 will always return 0.0.
    Vector2<T> operator&(const Vector2<T> &v) const { Vector2<T> out(-z_*v.y(),z_*v.x()); return out; } ///< Cross product with a Vector2 produces a Vector2
    Vector3<T> operator&(const Vector3<T> &v) const { Vector3<T> out(-z_*v.y(),z_*v.x(),0); return out; } ///< Cross product with a Vector3 produces a Vector3 (z=0)

    T operator|(const AVector2<T> &v) const { return z_*v.z(); } ///< Dot product
    T operator|(const Vector2<T> &) const { return 0; }         ///< Dot product with a Vector2 returns 0.0.
    T operator|(const Vector3<T> &v) const { return z_*v.z(); }  ///< Dot product with a Vector3.

    Vector2<T> toVector2() const { return Vector2<T>(0); }      ///< Converts to a Vector2(0,0)
    Vector3<T> toVector3() const { return Vector3<T>(0,0,z_); } ///< Converts to a Vector3(0,0,z)

private:
    T z_;
};

/// \relates AVector2
/// Cross product operator between a Vector2 and an AVector2, returns a Vector2.
template <class T> inline Vector2<T> operator&(const Vector2<T> &v,const AVector2<T> &av) {
    Vector2<T> out(v.y()*av.z(),-v.x()*av.z());
    return out;
}

/// \relates AVector2
/// Cross product operator between a Vector3 and an AVector2, returns a Vector3 (z=0).
template <class T> inline Vector3<T> operator&(const Vector3<T> &v,const AVector2<T> &av) {
    Vector3<T> out(v.y()*av.z(),-v.x()*av.z(),0);
    return out;
}

/// \relates AVector2
/// Cross product operator between a Vector2 and an Vector2, returns an AVector2.
template <class T> inline AVector2<T> operator&(const Vector2<T> &v1,const Vector2<T> &v2) {
    AVector2<T> out((v1.x()*v2.y()) - (v1.y()*v2.x()));
    return out;
}

/// \relates AVector2
/// Dot product operator between a Vector2 and an AVector2, returns 0.
template <class T> inline T operator|(const Vector2<T> &,const AVector2<T> &) { return 0; }




/// \brief 3D Angular vector class.

/** This specialized version of a vector class is intended to store "angular" values,
*  values that are a scalar in 2D (z component only) and a full vector in 3D.

*  The intention is to make it easier to write code that is the same in 2D and 3D without
*    wasting computation time and storage.

*  In the 3D case, and AVector3 is a thin wrapper around a Vector3.
*/
template <class T> class AVector3 : public Vector3<T> {
public:
    using Vector3<T>::Vector3;
    AVector3() : Vector3<T>() { }
    AVector3(const Vector3<T> &v) : Vector3<T>(v) { }
    /// Explicit conversion contructor from an AVector2, x=0, y=0, z=v.z.
    explicit AVector3(const AVector2<T> &v) : Vector3<T>(0,0,v.z()) { }

    /// Converts to a Vector2, the z component is lost.
    Vector2<T> toVector2() const { return Vector2<T>(this->x(),this->y()); }
    /// Converts to a Vector3 directly.
    const Vector3<T> &toVector3() const { return *this; }
};

// Naming convention for most commonly used types.
typedef AVector2<Double>  DAVect2; ///< \related AVector2
typedef AVector2<Float>   FAVect2; ///< \related AVector2
typedef AVector2<Int>     IAVect2; ///< \related AVector2
typedef AVector2<UInt>    UAVect2; ///< \related AVector2

typedef AVector3<Double>  DAVect3; ///< \related AVector3
typedef AVector3<Float>   FAVect3; ///< \related AVector3
typedef AVector3<Int>     IAVect3; ///< \related AVector3
typedef AVector3<UInt>    UAVect3; ///< \related AVector3

// Conversion routines.
/// \relates AVector2
/// Converts an AVector2<T> to a Double version.
template <class T> inline DAVect2 toDAVect2(const AVector2<T> &v) { DAVect2 dv2(to<Double>(v.z()));  return dv2; }
/// \relates AVector2
/// Converts an AVector2<T> to a Float version.
template <class T> inline FAVect2 toFAVect2(const AVector2<T> &v) { FAVect2 fv2(to<Float>(v.z()));  return fv2; }
/// \relates AVector2
/// Converts an AVector2<T> to an Int version
template <class T> inline IAVect2 toIAVect2(const AVector2<T> &v) { IAVect2 iv2(to<Int>(v.z()));  return iv2; }
/// \relates AVector2
/// Converts an AVector2<T> to a UInt version.
template <class T> inline UAVect2 toUAVect2(const AVector2<T> &v) { UAVect2 uv2(to<UInt>(v.z()));  return uv2; }

/// \relates AVector3
/// Converts a Vector3 to a Double AVect3 (no loss of components)
template <class T> inline DAVect3 toDAVect3(const Vector3<T> &v) {
    DAVect3 dv3(to<Double>(v.x()),to<Double>(v.y()),to<Double>(v.z()));
    return dv3;
}

/// \relates AVector3
/// Converts a Vector3 to a Float AVect3 (no loss of components)
template <class T> inline FAVect3 toFAVect3(const Vector3<T> &v) {
    FAVect3 fv3(to<Float>(v.x()),to<Float>(v.y()),to<Float>(v.z()));
    return fv3;
}

/// \relates AVector3
/// Converts a Vector3 to an Int AVect3 (no loss of components)
template <class T> inline IAVect3 toAIVect3(const Vector3<T> &v) {
    IAVect3 iv3(to<Int>(v.x()),to<Int>(v.y()),to<Int>(v.z()));
    return iv3;
}

/// \relates AVector3
/// Converts a Vector3 to a UInt AVect3 (no loss of components)
template <class T> inline UAVect3 toAUVect3(const Vector3<T> &v) {
    UAVect3 uv3(to<UInt>(v.x()),to<UInt>(v.y()),to<UInt>(v.z()));
    return uv3;
}

/// \relates AVector2
/// Converts a AVector2 to an Vector2(0,0)
template <class T> inline Vector2<T> toVect2(const AVector2<T> &) { return Vector2<T>(0); }
/// \relates AVector2
/// Converts a AVector2 to an Vector3(0,0,z)
template <class T> inline Vector3<T> toVect3(const AVector2<T> &v) { return Vector3<T>(0,0,v.z()); }
/// \relates AVector3
/// Converts a AVector3 to an Vector2(x,y) { the z component is lost }
template <class T> inline Vector2<T> toVect2(const AVector3<T> &v) { return Vector2<T>(v.x(),v.y()); }
/// \relates AVector3
/// Converts a AVector3 to an Vector3 (no loss)
template <class T> inline const Vector3<T> &toVect3(const AVector3<T> &v) { return v; }

/// \relates AVector2
/// Converts a AVector2 to an AVector2 ( Identity )
template <class T> inline const AVector2<T> &toAVect2(const AVector2<T> &v) { return v; }
/// \relates AVector3
/// \relates AVector2
/// Converts a AVector3 to an AVector2 (x and y components are lost)
template <class T> inline AVector2<T>        toAVect2(const AVector3<T> &v) { return AVector2<T>(v.z()); }
/// \relates AVector2
/// \relates AVector3
/// Converts a AVector2 to an AVector3 (with optional additional x and y components)
template <class T> inline AVector3<T>        toAVect3(const AVector2<T> &v,const T &x=0,const T &y=0) { return Vector3<T>(x,y,v.z()); }
/// \relates AVector3
/// Converts a AVector3 to an AVector3 ( Identity )
template <class T> inline const AVector3<T> &toAVect3(const AVector3<T> &v) { return v; }

/// Finds the "component" maximums, the return vector is the max of the respective components in v1,v2.
/// \relates AVector3
template <class T> inline AVector3<T> vmax(const AVector3<T> &v1,const AVector3<T> &v2) {
    AVector3<T> out(std::max<T>(v1.x(),v2.x()),std::max<T>(v1.y(),v2.y()),std::max<T>(v1.z(),v2.z()));
    return out;
}

/// Finds the "component" minimums, the return vector is the min of the respective components in v1,v2.
/// \relates AVector3
template <class T> inline AVector3<T> vmin(const AVector3<T> &v1,const AVector3<T> &v2) {
    AVector3<T> out(std::min<T>(v1.x(),v2.x()),std::min<T>(v1.y(),v2.y()),std::min<T>(v1.z(),v2.z()));
    return out;
}

/// This function returns the components of v1 with the same sign as the respective components in v2.
/// \relates AVector3
template <class T> inline AVector3<T> vsign(const AVector3<T> &v1,const AVector3<T> &v2) {
    AVector3<T> out(v2.x() < 0 ? -qAbs(v1.x()) : qAbs(v1.x()), v2.y() < 0 ? -qAbs(v1.y()) : qAbs(v1.y()), v2.z() < 0 ? -qAbs(v1.z()) : qAbs(v1.z()));
    return out;
}

/// Returns the "ceil" of each component of the vector, only valid for double or float types.
/// \relates AVector3
template <class T> inline AVector3<T> vceil(const AVector3<T> &v) {
    AVector3<T> out(ceil(v.x()),ceil(v.y()),ceil(v.z()));
    return out;
}

/// Finds the "component" maximums, the return vector is the max of the respective components in v1,v2.
/// \relates AVector2
template <class T> inline AVector2<T> vmax(const AVector2<T> &v1,const AVector2<T> &v2) {
    AVector2<T> out(std::max<T>(v1.z(),v2.z()));
    return out;
}

/// Finds the "component" minimums, the return vector is the min of the respective components in v1,v2.
/// \relates AVector3
template <class T> inline AVector2<T> vmin(const AVector2<T> &v1,const AVector2<T> &v2) {
    AVector2<T> out(std::min<T>(v1.z(),v2.z()));
    return out;
}

/// This function returns the components of v1 with the same sign as the respective components in v2.
/// \relates AVector2
template <class T> inline AVector2<T> vsign(const AVector2<T> &v1,const AVector2<T> &v2) {
    AVector2<T> out(v2.z() < 0 ? -qAbs(v1.z()) : qAbs(v1.z()));
    return out;
}

/// Returns the "ceil" of each component of the vector, only valid for double or float types.
/// \relates AVector3
template <class T> inline AVector2<T> vceil(const AVector2<T> &v) {
    AVector2<T> out(ceil(v.z()));
    return out;
}

/// @}
// EOF
