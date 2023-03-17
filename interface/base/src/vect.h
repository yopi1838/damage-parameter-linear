#pragma once
/** \file vect.h
* \brief 2D and 3D vector utility classes.
* \addtogroup Base Base interface specification 
* @{
*/
#include "basedef.h"
#include "to.h"
#include <cmath>
#include <array>

#ifdef _LINUX
namespace std {
    template <class T> T abs(const T &t) {
        if (t<0) return -t;
        return t;
    }
    template <class T> T max0(const T &t1,const T &t2) {
        if (t2>t1) return t2;
        return t1;
    }
    template <class T> T min0(const T &t1,const T &t2) {
        if (t2<t1) return t1;
        return t1;
    }
} // namespace std
#endif

/** \brief 2D vector utility class.
*/
template <class T> class Vector2 {
public:
    // Creators
    /// Default constructor, no data initialization.
#ifdef _DEBUG
    Vector2() { fill(initVal<T>()); }
#else
#pragma warning(push)
#pragma warning(disable:26495) // Init warning- not initializing on purpose
    Vector2() {  }
#pragma warning(pop)
#endif
    /// Copy constructor.
    Vector2(const Vector2<T> &v) : a_(v.a_) { }
    /// Explicit constructor, from two components.
    Vector2(const T &x,const T &y) : a_({x,y}) {  }
    /// Explicit contructor, each component is given value t.
    explicit Vector2(const T &t) : a_({t,t}) { }
    /// Creates an "empty" vector, useful when Vector2 is used as a 1D extent.
    static Vector2<T> nothing() { Vector2<T> v(limits<T>::max(),-limits<T>::max());  return v; }

    // Accessors
    /// X component access.
    const T &x() const { return c_.x_; }
    /// Y component access.
    const T &y() const { return c_.y_; }
    /// Access to degree of freedom u (0-1).
    const T &dof(unsigned u) const { return a_[u]; }
    /// Reference access to x-component.
    T &rx() { return c_.x_; }
    /// Reference access to y-component.
    T &ry() { return c_.y_; }
    /// Reference accesss to degree-of-freedom u (0-1).
    T &rdof(unsigned u) { return a_[u]; }
    /// Allow array like access by degree of freedom (0-1)
    const T &operator[](unsigned u) const { return dof(u); }
    T &      operator[](unsigned u) { return rdof(u); } ///< \overload
    template <std::size_t N> decltype(auto) get() const {
        static_assert(N<2,"Index out of range");
        return a_[N];
    }

    // Various norms
    ///   Manhattan norm
    T fsum() const { return (std::abs(x())+std::abs(y())); }
    /// Sum of components
    T sum() const { return x() + y(); }
    /// Square of the magnitude, or the dot product with itself.
    T mag2() const { return (x()*x()+y()*y()); }
    /// The magnitude.
    T mag() const { return to<T>(std::sqrt(to<Double>(mag2()))); }
    /// Size of rectangle represented by x*y - can be negative.
    T area() const { return (x()*y()); }
    /// Volume of the rectangle assuming unit depth -- same as area(), provided for 2D/3D compile compatibility with Cube.
    T volume() const { return area(); }
    /// Assumes vector is being used to store a 1D extent--  Returns max-min. (y-x).
    T spread() const { return (y() -x()); }
    /// Unit vector - be sure vector is nonzero
    Vector2<T> unit() const { Vector2<T> v(*this); T m(v.mag()); if (m) v/=m; return v; }
    /// Returns vector of absolute values of components.
    Vector2<T> abs() const { Vector2<T> v(std::abs(x()),std::abs(y())); return v; }
    /// Fills all three components with value d.
    void fill(const T &d) { rx()=d; ry()=d; }
    /// Returns the maximum component.
    T maxComp() const { return std::max(x(),y()); }
    /// Returns the minimum component.
    T minComp() const { return std::min(x(),y()); }
    /// Returns the max component index
    unsigned maxCompIndex() const { return x() > y() ? 0 : 1; }
    /// Returns the min component index
    unsigned minCompIndex() const { return x() < y() ? 0 : 1; }
    /// "Safe" division operation - checks for zero and overflow.
    const Vector2<T> &safeDivE(const Vector2<T> &v) { rx()=::safeDiv(x(),v.x());  ry() = ::safeDiv(y(),v.y());  return *this;  }
    /// "Safe" division operation - checks for zero and overflow.
    const Vector2<T>  safeDiv(const Vector2<T> &v) { Vector2<T> ret(::safeDiv(x(),v.x()),::safeDiv(y(),v.y()));  return ret; }

    /// Comparison operator - no tolerance is used.
    bool operator==(const Vector2<T> &v) const { return ((x()==v.x())&&(y()==v.y())); }
    /// Comparison operator - no tolerance is used.
    bool operator!=(const Vector2<T> &v) const { return ((x()!=v.x())||(y()!=v.y())); }
    /// Comparison operator, based on magnitude.
    bool operator<(const Vector2<T> &v) const { if (x()!=v.x()) return x()<v.x();  return y()<v.y();  }
    /// Comparison operator, based on magnitude.
    bool operator>(const Vector2<T> &v) const { if (x()!=v.x()) return x()>v.x();  return y()>v.y();  }

    // In-place operators
    /// In place mathematical operators using vectors and scalars -- Note that * and / of Vector2 types are done on a component basis.
    const Vector2<T> &operator+=(const Vector2<T> &v) { rx()+=v.x();  ry()+=v.y();  return *this; }
    const Vector2<T> &operator-=(const Vector2<T> &v) { rx()-=v.x();  ry()-=v.y();  return *this; } ///< \copydoc operator+=
    const Vector2<T> &operator*=(const Vector2<T> &v) { rx()*=v.x();  ry()*=v.y();  return *this; } ///< \copydoc operator+=
    const Vector2<T> &operator*=(const T &t)          { rx()*=t;      ry()*=t;      return *this; } ///< \copydoc operator+=
    const Vector2<T> &operator/=(const Vector2<T> &v) { rx()/=v.x();  ry()/=v.y();  return *this; } ///< \copydoc operator+=
    const Vector2<T> &operator/=(const T &t)          { rx()/=t;      ry()/=t;      return *this; } ///< \copydoc operator+=

    // Binary operators - require temp (ugh)
    /// Binary mathematical operators using vectors and scalars -- Note that * and / of Vector2 types are done on a component basis
    Vector2<T> operator+(const Vector2<T> &v) const { Vector2<T> out(x()+v.x(),y()+v.y());  return out; } 
    Vector2<T> operator-(const Vector2<T> &v) const { Vector2<T> out(x()-v.x(),y()-v.y());  return out; } ///< \copydoc operator+
    Vector2<T> operator*(const Vector2<T> &v) const { Vector2<T> out(x()*v.x(),y()*v.y());  return out; } ///< \copydoc operator+
    Vector2<T> operator*(const T &t)          const { Vector2<T> out(x()*t,y()*t);        return out; } ///< \copydoc operator+
    Vector2<T> operator/(const Vector2<T> &v) const { Vector2<T> out(x()/v.x(),y()/v.y());  return out; } ///< \copydoc operator+
    Vector2<T> operator/(const T &t)          const { Vector2<T> out(x()/t,y()/t);        return out; } ///< \copydoc operator+
    //   Cross Product (see DAVect)
    /// Dot Product
    T          operator|(const Vector2<T> &v) const { return (x()*v.x()+y()*v.y()); }

    // Support for use of a Vect2 as a range (min,max)
    /// Expands 1D range to include value \a t.
    const Vector2<T> &expandToInclude(const T &t) { rx() = std::min(x(),t);  ry() = std::max(y(),t);  return *this; }
    /// Expands 1D range to include 1D range \a v.
    const Vector2<T> &expandToInclude(const Vector2<T> &v) { rx() = std::min(x(),v.minComp());  ry() = std::max(y(),v.maxComp());  return *this; }
    /// Returns 1D range expanded to include value \a t;
    Vector2<T>        expandedToInclude(const T &t) const { Vector2<T> ret(*this);  ret.expandToInclude(t);  return ret; }
    /// Returns 1D range expanded to include 1D range \a v.
    Vector2<T>        expandedToInclude(const Vector2<T> &v) const { Vector2<T> ret(*this);  ret.expandToInclude(v);  return ret; }
    /// True if value \a t falls inside this 1D range (inclusive).
    bool              contains(const T &t) const { if (t<x()) return false;  if (t>y()) return false;  return true; }

private:
    union {
        struct {
            T x_;
            T y_;
        } c_;
        std::array<T,2> a_;
    };
};

/** \brief 3D vector utility class.
*/
template <class T> class Vector3 {
public:
    // Creators
    /// Default constructor - no data initialization
#ifdef _DEBUG
    Vector3() { fill(initVal<T>()); }
#else
#pragma warning(push)
#pragma warning(disable:26495) // Init warning- not initializing on purpose
    Vector3() { }
#pragma warning(pop)
#endif
    /// Copy constructor.
    Vector3(const Vector3<T> &v) : a_(v.a_) {  }
    /// Explicit constructor, by the three components of the vector.
    Vector3(const T &x,const T &y,const T &z=0) : a_({x,y,z}) { }
    /// Explicit constructor, all three components are inintialized to t.
    explicit Vector3(const T &t) : a_({t,t,t}) { }

    // Accessors
    /// The x-component of the vector.
    const T &x() const { return c_.x_; }
    /// The y-component of the vector.
    const T &y() const { return c_.y_; }
    /// The z-component of the vector.
    const T &z() const { return c_.z_; }
    /// The degree-of-freedom u (0-2) component.
    const T &dof(unsigned u) const { return a_[u]; }
    /// Reference access to the x-component of the vector.
    T &rx() { return c_.x_; }
    /// Reference access to the y-component of the vector.
    T &ry() { return c_.y_; }
    /// Reference access to the z-component of the vector.
    T &rz() { return c_.z_; }
    /// Reference access to degree-of-freedom u (0-2) of the vector.
    T &rdof(unsigned u) { return a_[u]; }
    /// Allows array like access to degrees-of-freedom.
    const T &operator[](unsigned u) const { return dof(u); }
    /// Allows array like access to degrees-of-freedom.
    T &operator[](unsigned u) { return rdof(u); }
    template <std::size_t N> T get() const {
        static_assert(N<3,"Index out of range");
        return a_[N];
    }

    // Various norms
    /// Manhattan norm
    T fsum() const { return (std::abs(x())+std::abs(y())+std::abs(z())); }
    /// Sum of components
    T sum() const { return x()+y()+z(); }
    /// Square of the magnitude, or the vector dotted with itself.
    T mag2() const { return (x()*x()+y()*y()+z()*z()); }
    /// The vector magnitude.
    T mag() const { return to<T>(std::sqrt(to<Double>(mag2()))); }
    /// Volume of region represented by x*y*z - can be negative.
    T volume() const { return (x()*y()*z()); }
    /// Unit vector - be sure vector is nonzero
    Vector3<T> unit() const { Vector3<T> v(*this); T m(v.mag()); if (m) v/=m; return v; }
    /// Returns vector of absolute values of components.
    Vector3<T> abs() const { Vector3<T> v(std::abs(x()),std::abs(y()),std::abs(z()));  return v; }
    /// Fills all three components with value d.
    void fill(const T &d) { rx()=d; ry()=d; rz()=d; }
    /// Returns the maximum component.
    T maxComp() const { return std::max(x(),std::max(y(),z())); }
    /// Returns the minimum component.
    T minComp() const { return std::min(x(),std::min(y(),z())); }
    /// Returns the max component index
    unsigned maxCompIndex() const { return x() > y() ? (x() > z() ? 0 : 2) : (y() > z() ? 1 : 2); }
    /// Returns the min component index
    unsigned minCompIndex() const { return x() < y() ? (x() < z() ? 0 : 2) : (y() < z() ? 1 : 2); }
    /// "Safe" division operation - checks for zero and overflow.
    const Vector3<T> &safeDivE(const Vector3<T> &v) { rx()=::safeDiv(x(),v.x());  ry() = ::safeDiv(y(),v.y());  rz() = ::safeDiv(z(),v.z()); return *this;  }
    /// "Safe" division operation - checks for zero and overflow.
    const Vector3<T>  safeDiv(const Vector3<T> &v) { Vector3<T> ret(::safeDiv(x(),v.x()),::safeDiv(y(),v.y()),::safeDiv(z(),v.z()));  return ret; }

    /// Comparison operator - no tolerance is used.
    bool operator==(const Vector3<T> &v) const { return ((x()==v.x())&&(y()==v.y())&&(z()==v.z())); }
    /// Comparison operator - no tolerance is used.
    bool operator!=(const Vector3<T> &v) const { return ((x()!=v.x())||(y()!=v.y())||(z()!=v.z())); }
    /// Comparison operator, compare each DOF in order.
    bool operator<(const Vector3<T> &v) const { if (x()!=v.x()) return x()<v.x();  if (y()!=v.y()) return y()<v.y();  return z()<v.z(); }
    /// Comparison operator, compare each DOF in order.
    bool operator>(const Vector3<T> &v) const { if (x()!=v.x()) return x()>v.x();  if (y()!=v.y()) return y()>v.y();  return z()>v.z(); }

    // In-place operators
    /// In-place mathematical operators -- * and / are done on a component basis.
    const Vector3<T> &operator+=(const Vector3<T> &v) { rx()+=v.x();  ry()+=v.y();  rz()+=v.z();  return *this; }
    const Vector3<T> &operator-=(const Vector3<T> &v) { rx()-=v.x();  ry()-=v.y();  rz()-=v.z();  return *this; } ///< \copydoc operator+=
    const Vector3<T> &operator*=(const Vector3<T> &v) { rx()*=v.x();  ry()*=v.y();  rz()*=v.z();  return *this; } ///< \copydoc operator+=
    const Vector3<T> &operator*=(const T &t)          { rx()*=t;      ry()*=t;      rz()*=t;      return *this; } ///< \copydoc operator+=
    const Vector3<T> &operator/=(const Vector3<T> &v) { rx()/=v.x();  ry()/=v.y();  rz()/=v.z();  return *this; } ///< \copydoc operator+=
    const Vector3<T> &operator/=(const T &t)          { rx()/=t;      ry()/=t;      rz()/=t;      return *this; } ///< \copydoc operator+=

    // Binary operators - require temp (ugh)
    /// Binary mathematical operators --  * and / are done on a component basis.
    Vector3<T> operator+(const Vector3<T> &v) const { Vector3<T> out(x()+v.x(),y()+v.y(),z()+v.z());  return out; }
    Vector3<T> operator-(const Vector3<T> &v) const { Vector3<T> out(x()-v.x(),y()-v.y(),z()-v.z());  return out; } ///< \copydoc operator+
    Vector3<T> operator*(const Vector3<T> &v) const { Vector3<T> out(x()*v.x(),y()*v.y(),z()*v.z());  return out; } ///< \copydoc operator+
    Vector3<T> operator*(const T &t)          const { Vector3<T> out(x()*t,y()*t,z()*t);              return out; } ///< \copydoc operator+
    Vector3<T> operator/(const Vector3<T> &v) const { Vector3<T> out(x()/v.x(),y()/v.y(),z()/v.z());  return out; } ///< \copydoc operator+
    Vector3<T> operator/(const T &t)          const { Vector3<T> out(x()/t,y()/t,z()/t);              return out; } ///< \copydoc operator+
    /// Cross Product
    Vector3<T> operator&(const Vector3<T> &v) const { 
        Vector3<T> out((y()*v.z())-(z()*v.y()),(z()*v.x())-(x()*v.z()),(x()*v.y())-(y()*v.x()));  
        return out; 
    }
    /// Dot Product
    T          operator|(const Vector3<T> &v) const { return (x()*v.x()+y()*v.y()+z()*v.z()); }

private:
    union {
        struct {
            T x_;
            T y_;
            T z_;
        } c_;
        std::array<T,3> a_;
    };
};

// Naming convention for most commonly used types.
typedef Vector2<Double>  DVect2; ///< \relates Vector2
typedef Vector2<Float>   FVect2; ///< \relates Vector2
typedef Vector2<Int>     IVect2; ///< \relates Vector2
typedef Vector2<UInt>    UVect2; ///< \relates Vector2
typedef Vector2<Long>    LVect2; ///< \relates Vector2
typedef Vector2<ULong>   ULVect2; ///< \relates Vector2

typedef Vector3<Double>  DVect3; ///< \relates Vector3
typedef Vector3<Float>   FVect3; ///< \relates Vector3
typedef Vector3<Int>     IVect3; ///< \relates Vector3
typedef Vector3<UInt>    UVect3; ///< \relates Vector3
typedef Vector3<Long>    LVect3; ///< \relates Vector3
typedef Vector3<ULong>   ULVect3; ///< \relates Vector3

// Conversion routines.
/// Conversion routines between various predefined types of Vector2.
/// \relates Vector2
template <class T> inline DVect2 toDVect2(const Vector2<T> &v) { DVect2 dv2(to<Double>(v.x()),to<Double>(v.y()));  return dv2; }
/// Conversion routines between various predefined types of Vector2.
/// \relates Vector2
template <class T> inline FVect2 toFVect2(const Vector2<T> &v) { FVect2 fv2(to<Float>(v.x()),to<Float>(v.y()));  return fv2; }
/// Conversion routines between various predefined types of Vector2.
/// \relates Vector2
template <class T> inline IVect2 toIVect2(const Vector2<T> &v) { IVect2 iv2(to<Int>(v.x()),to<Int>(v.y()));  return iv2; }    
/// Conversion routines between various predefined types of Vector2.
/// \relates Vector2
template <class T> inline UVect2 toUVect2(const Vector2<T> &v) { UVect2 uv2(to<UInt>(v.x()),to<UInt>(v.y()));  return uv2; }  

/// Conversion routines between various predefined types of Vector3.
/// \relates Vector3
template <class T> inline DVect3 toDVect3(const Vector3<T> &v) { 
    DVect3 dv3(to<Double>(v.x()),to<Double>(v.y()),to<Double>(v.z()));  
    return dv3; 
}
/// Conversion routines between various predefined types of Vector3.
/// \relates Vector3
template <class T> inline FVect3 toFVect3(const Vector3<T> &v) { 
    FVect3 fv3(to<Float>(v.x()),to<Float>(v.y()),to<Float>(v.z()));  
    return fv3; 
} 
/// Conversion routines between various predefined types of Vector3.
/// \relates Vector3
template <class T> inline IVect3 toIVect3(const Vector3<T> &v) { 
    IVect3 iv3(to<Int>(v.x()),to<Int>(v.y()),to<Int>(v.z()));  
    return iv3; 
}
/// Conversion routines between various predefined types of Vector3.
/// \relates Vector3
template <class T> inline UVect3 toUVect3(const Vector3<T> &v) { 
    UVect3 uv3(to<UInt>(v.x()),to<UInt>(v.y()),to<UInt>(v.z()));  
    return uv3; 
}


/// Conversion between vectors of different dimension.
template <class T> inline const Vector2<T> &toVect2(const Vector2<T> &v) { return v; }
template <class T> inline Vector2<T>        toVect2(const Vector3<T> &v) { return Vector2<T>(v.x(),v.y()); } ///< \copydoc toVect2(const Vector2<T> &v)
template <class T> inline Vector3<T>        toVect3(const Vector2<T> &v,const T &t=0) { return Vector3<T>(v.x(),v.y(),t); }  ///< \copydoc toVect2(const Vector2<T> &v)
template <class T> inline const Vector3<T> &toVect3(const Vector3<T> &v) { return v; } ///< \copydoc toVect2(const Vector2<T> &v)


/// Finds the "component" maximums, the return vector is the max of the respective components in v1,v2.
/// \relates Vector2
template <class T> inline Vector2<T> vmax(const Vector2<T> &v1,const Vector2<T> &v2) { 
    Vector2<T> out(std::max<T>(v1.x(),v2.x()),std::max<T>(v1.y(),v2.y()));  
    return out; 
}
/// Finds the "component" minimums, the return vector is the min of the respective components in v1,v2.
/// \relates Vector2
template <class T> inline Vector2<T> vmin(const Vector2<T> &v1,const Vector2<T> &v2) { 
    Vector2<T> out(std::min<T>(v1.x(),v2.x()),std::min<T>(v1.y(),v2.y()));  
    return out; 
} 

/// This function returns the components of v1 with the same sign as the respective components in v2.
/// \relates Vector2
template <class T> inline Vector2<T> vsign(const Vector2<T> &v1,const Vector2<T> &v2) { 
    Vector2<T> out(v2.x() < 0 ? -qAbs(v1.x()) : qAbs(v1.x()) ,v2.y() < 0 ? -qAbs(v1.y()) : qAbs(v1.y()));  
    return out; 
} 

/// Finds the "component" maximums, the return vector is the max of the respective components in v1,v2.
/// \relates Vector3
template <class T> inline Vector3<T> vmax(const Vector3<T> &v1,const Vector3<T> &v2) {
    Vector3<T> out(std::max<T>(v1.x(),v2.x()),std::max<T>(v1.y(),v2.y()),std::max<T>(v1.z(),v2.z()));  
    return out; 
}
/// Finds the "component" minimums, the return vector is the min of the respective components in v1,v2.
/// \relates Vector3
template <class T> inline Vector3<T> vmin(const Vector3<T> &v1,const Vector3<T> &v2) { 
    Vector3<T> out(std::min<T>(v1.x(),v2.x()),std::min<T>(v1.y(),v2.y()),std::min<T>(v1.z(),v2.z()));  
    return out; 
}

/// This function returns the components of v1 with the same sign as the respective components in v2.
/// \relates Vector3
template <class T> inline Vector3<T> vsign(const Vector3<T> &v1,const Vector3<T> &v2) { 
    Vector3<T> out(v2.x() < 0 ? -qAbs(v1.x()) : qAbs(v1.x()), v2.y() < 0 ? -qAbs(v1.y()) : qAbs(v1.y()), v2.z() < 0 ? -qAbs(v1.z()) : qAbs(v1.z()));  
    return out; 
} 

/// Returns the "ceil" of each component of the vector, only valid for double or float types.
/// \relates Vector2
template <class T> inline Vector2<T> vceil(const Vector2<T> &v) {
    Vector2<T> out(ceil(v.x()),ceil(v.y()));
    return out;
}

/// Returns the "ceil" of each component of the vector, only valid for double or float types.
/// \relates Vector3
template <class T> inline Vector3<T> vceil(const Vector3<T> &v) {
    Vector3<T> out(ceil(v.x()),ceil(v.y()),ceil(v.z()));
    return out;
}

/// Returns the "floor" of each component of the vector, only valid for double or float types.
/// \relates Vector2
template <class T> inline Vector2<T> vfloor(const Vector2<T> &v) {
    Vector2<T> out(floor(v.x()),floor(v.y()));
    return out;
}

/// Returns the "floor" of each component of the vector, only valid for double or float types.
/// \relates Vector3
template <class T> inline Vector3<T> vfloor(const Vector3<T> &v) {
    Vector3<T> out(floor(v.x()),floor(v.y()),floor(v.z()));
    return out;
}

/// Returns the "round" of each component of the vector, only valid for double or float types.
/// \relates Vector2
template <class T> IVect2 vround(const Vector2<T> &v) {
    IVect2 out(qRound(v.x()),qRound(v.y()));
    return out;
}

/// Returns the "round" of each component of the vector, only valid for double or float types.
/// \relates Vector3
template <class T> IVect3 vround(const Vector3<T> &v) {
    IVect3 out(qRound(v.x()),qRound(v.y()),qRound(v.z()));
    return out;
}

namespace std {
    /// Template specialization for max, min
    template <class T> Vector2<T> max(const Vector2<T> &v1,const Vector2<T> &v2) { return vmax(v1,v2); }
    /// Template specialization for max, min
    template <class T> Vector2<T> min(const Vector2<T> &v1,const Vector2<T> &v2) { return vmin(v1,v2); }
    /// Template specialization for max, min
    template <class T> Vector3<T> max(const Vector3<T> &v1,const Vector3<T> &v2) { return vmax(v1,v2); }
    /// Template specialization for max, min
    template <class T> Vector3<T> min(const Vector3<T> &v1,const Vector3<T> &v2) { return vmin(v1,v2); }

    /// We want structured binding support for vector types!
    template <class T> struct tuple_size<Vector2<T>> : std::integral_constant<std::size_t, 2> {};
    template <class T> struct tuple_size<Vector3<T>> : std::integral_constant<std::size_t, 3> {};
    template <std::size_t N,class T> struct tuple_element<N, Vector2<T>> { using type = T; };
    template <std::size_t N,class T> struct tuple_element<N, Vector3<T>> { using type = T; };
}

/// @}
// EoF

