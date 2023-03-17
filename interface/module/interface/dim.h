#pragma once

#include "base/src/base.h"
namespace itasca {
    class Accumulator3;
    class Accumulator2;
    class Accumulator;
}

/**
    * \file
    * \brief DIM - Provides code portability between 2D and 3D codes.
    *
    * This file provides code portability between 2 and 3 dimension codes.\n
    * Depending on how DIM is defined, this file declares TYPEDEF and template 
    * functions for vectors, extents, axes, cubes, etc. \n
    * Declaration for vectors, axes, extents, etc for 2D and 3D.\n
    *  The intention is to make it easier to write portable code that is the same in 
    *  2D and 3D.\n
    * \defgroup module Module interface specification.
    * \addtogroup module
    * @{
    */

#ifndef DIM
#  error DIM must be defined as 2 or 3 to compile this module.
#endif

#if DIM==2
    /// \brief A Vector2 in 2D, a Vector3 in 3D.
    template <class T> 
    class Vector : public Vector2<T> {
    public:
        /// Default constructor, no data initialization.
        Vector() { }
        /// Copy constructor.
        Vector(const Vector<T> &v) : Vector2<T>(v) { }
        /// Copy constructor.
        Vector(const Vector2<T> &v) : Vector2<T>(v) { }
        /// Single component constructor
        explicit Vector(const T &t) : Vector2<T>(t) { }
    };
    /// \brief An AVector2 in 2D, an AVector3 in 3D.
    template <class T> 
    class AVector : public AVector2<T> {
    public:
        /// Default constructor, no data initialization.
        AVector() { }
        /// Copy constructor
        AVector(const AVector &av) : AVector2<T>(av) { }
        /// Copy constructor
        AVector(const AVector2<T> &av) : AVector2<T>(av) { }
    };
    /// \brief A Extent2<T> in 2D, and a Extent3<T> in 3D.
    template <class T> 
    class Extent : public Extent2<T> {
    public:
        /// Default constructor, not data initialization.
        Extent() { }
        /// Explicit constructor, given the lower and upper bound as two Vector.
        Extent(const Vector<T> &v11,const Vector<T> &v22) : Extent2<T>(v11,v22) { }
        /// Copy constructor
        Extent(const Extent &r) : Extent2<T>(r) { }
        Extent(const Extent2<T> &r) : Extent2<T>(r) { }
    };
    /// Creates an "empty" vector, useful when Vector2 is used as a 1D extent.
    using DVect       = DVect2;       ///< Vector of doubles, either 2D or 3D
    using FVect       = FVect2;       ///< Vector of floats, either 2D or 3D
    using IVect       = IVect2;       ///< Vector of ints, either 2D or 3D
    using UVect       = UVect2;       ///< Vector of uints, either 2D or 3D
    using DExtent     = DExtent2;     ///< A DExtent2 in 2D, a DExtent3 in 3D
    using IExtent     = IExtent2;     ///< An IExtent2 in 2D, an IExtent3 in 3D
    using DAVect      = DAVect2;      ///< Angular vector of doubles, either 2D or 3D
    using FAVect      = FAVect2;      ///< Angular vector of floats, either 2D or 3D
    using Axes        = Axes2D;       ///< Axes system, either 2D or 3D
    using CAxes       = CAxes2D;      ///< Compact axis system, either 2D or 3D
    using Orientation = Orientation2; ///< Class for storing an "orientation", or a direction in space, either 2D or 3D
    using Quat        = Quat2;        ///< A complex number in 2D and quaternion in 3D.
    using VAccum      = itasca::Accumulator2; ///< 2D vector accumulator
    using AVAccum     = itasca::Accumulator; ///< 2D angular accumulator

#  ifndef TWOD     
#      define TWOD        ///< Only defined if a 2D compile, TWOD is defined for a 2D compile.
#  endif
#  define DIMSTR "2d"     ///< Expands to "2d" or "3d" depending on the compile
#  define CDIMSTR "2D"    ///< Expands to "2D" or "3D" depending on the compile
#  undef itascaxd
#  define itascaxd itasca2d  ///< Expands to itasca2d or itasca3d, for use as a namespace name....
#  define DIMRET(x,y) x
    template <class T> inline const Vector2<T> & toVect(const Vector2<T> &in) { return toVect2(in); }  ///< given a 2D vector template class, returns a 2D vector 
    template <class T> inline Vector<T>          toVect(const Vector3<T> &in) { return toVect2(in); }  ///< given a 3D vector template class, returns a 2D vector 
    template <class T> inline Vector<T>          toVect(const AVector2<T> &) { return Vector2<T>(0); } ///< given a 2D angular vector template class, returns a 2D vector
    template <class T> inline AVector<T>         toAVect(const AVector2<T> &in) { return in; } ///< given a 2D angular vector template class, returns a 2D angular vector
    template <class T> inline AVector<T>         toAVect(const AVector3<T> &in) { return AVector2<T>(in.z()); } ///< given a 3D angular vector template class, returns a 2D angular vector 
    template <class T> inline AVector<T>         toAVect(const Vector2<T> &) { return AVector2<T>(0.0); } ///< given a 2D vector template class, returns a 2D angular vector
    template <class T> inline AVector<T>         toAVect(const Vector3<T> &in) { return AVector2<T>(in.z()); } ///< given a 3D vector template class, returns a 2D angular vector 
    template <class T> inline const Extent<T> &  toExtent(const Extent2<T> &t) { return t; } ///< given a Extent2, returns the Extent2
    template <class T> inline Extent<T>          toExtent(const Extent3<T> &t) { return Extent2<T>(t.x1(),t.x2(),t.y1(),t.y2()); } ///< given a Extent3, returns a Extent2
    template <class T> inline IVect toIVect(const Vector2<T> &in) { IVect iv(to<Int>(in.x()),to<Int>(in.y()));  return iv; } ///< given a 2D vector of any type, returns a 2D vector class of int values  
    template <class T> inline FVect toFVect(const Vector2<T> &in) { FVect fv(to<Float>(in.x()),to<Float>(in.y()));  return fv; } ///< given a 2D vector of any type, returns a 2D vector class of float values  
    template <class T> inline DVect toDVect(const Vector2<T> &in) { DVect dv(to<Double>(in.x()),to<Double>(in.y()));  return dv; } ///< given a 2D vector of any type, returns a 2D vector class of double values  
    template <class T> inline DVect toDVect(const Vector3<T> &in) { DVect dv(to<Double>(in.x()),to<Double>(in.y()));  return dv; } ///< given a 2D vector of any type, returns a 2D vector class of double values  
    template <class T,class U> inline constexpr const T &dimval(const T &val2,const U &) { return val2; }  ///< returns a 2D value
    static const unsigned int totdof = 3;
#else // DIM==3 
    /// \brief A Vector2 in 2D, a Vector3 in 3D.
    template <class T> 
    class Vector : public Vector3<T> {
    public:
        /// Default constructor, no data initialization.
        Vector() { }
        /// Copy constructor.
        Vector(const Vector<T> &v) : Vector3<T>(v) { }
        /// Copy constructor.
        Vector(const Vector3<T> &v) : Vector3<T>(v) { }
        /// From three
        Vector(const T &x,const T &y,const T &z) : Vector3<T>(x,y,z) { }
        /// Single component constructor
        explicit Vector(const T &t) : Vector3<T>(t) { }
    };
    /// \brief An AVector2 in 2D, an AVector3 in 3D.
    template <class T> 
    class AVector : public AVector3<T> {
    public:
        /// Default constructor, no data initialization.
        AVector() { }
        /// Copy constructor
        AVector(const AVector &av) : AVector3<T>(av) { }
        /// Copy constructor
        AVector(const AVector3<T> &av) : AVector3<T>(av) { }
    };
    /// \brief A Extent2<T> in 2D, and a Extent3<T> in 2D.
    template <class T> 
    class Extent : public Extent3<T> {
    public:
        /// Default constructor, not data initialization.
        Extent() { }
        /// Explicit constructor, given the lower and upper bound as two Vector.
        Extent(const Vector<T> &v11,const Vector<T> &v22) : Extent3<T>(v11,v22) { }
        /// Copy constructor
        Extent(const Extent &r) : Extent3<T>(r) { }
        /// Copy constructor
        Extent(const Extent3<T> &r) : Extent3<T>(r) { }
    };
    using DVect       = DVect3;       ///< Vector of doubles, either 2D or 3D
    using FVect       = FVect3;       ///< Vector of floats, either 2D or 3D
    using IVect       = IVect3;       ///< Vector of ints, either 2D or 3D
    using UVect       = UVect3;       ///< Vector of uints, either 2D or 3D
    using DExtent     = DExtent3;     ///< A DExtent2 in 2D, a DExtent3 in 3D
    using IExtent     = IExtent3;     ///< An IExtent2 in 2D, an IExtent3 in 3D
    using DAVect      = DAVect3;      ///< Angular vector of doubles, either 2D or 3D
    using FAVect      = FAVect3;      ///< Angular vector of floats, either 2D or 3D
    using Axes        = Axes3D;       ///< Axes system, either 2D or 3D
    using CAxes       = CAxes3D;      ///< Compact axis system, either 2D or 3D
    using Orientation = Orientation3; ///< Class for storing an "orientation", or a direction in space, either 2D or 3D
    using Quat        = Quat3;        ///< A complex number in 2D and quaternion in 3D.
    using VAccum      = itasca::Accumulator3; ///< 3D vector accumulator
    using AVAccum     = itasca::Accumulator3; ///< 3D angular accumulator
#  ifndef THREED     
#      define THREED ///< Only defined if a 3D compile, TWOD is defined for a 2D compile.
#  endif
#  define DIMSTR "3d"  ///< Expands to "2d" in 2D, "3d" in 3D.
#  define CDIMSTR "3D"  ///< Expands to "2D" in 2D, "3D" in 3D.
#  undef itascaxd
#  define itascaxd itasca3d ///< Expands to itasca2d in 2D, itasca3d in 3D, for use as a namespace name....
#  define DIMRET(x,y) y     ///< Macro will disregard parameter x
    template <class T> inline Vector3<T>         toVect(const Vector2<T> &in) { return toVect3(in); }   ///< Converts a Vector2 to a Vector.
    template <class T> inline const Vector3<T> & toVect(const Vector3<T> &in) { return in; }   ///< Converts a Vector3 to a Vector.
    template <class T> inline Vector3<T>         toVect(const AVector2<T> &in) { return Vector3<T>(0,0,in.z()); } ///< Converts an AVector2 to Angular Vector2 to a Vector.
    template <class T> inline AVector3<T>        toAVect(const AVector2<T> &in) { return AVector3<T>(in); } ///< Converts an AVector2 to an AVector.
    template <class T> inline const AVector3<T> &toAVect(const AVector3<T> &in) { return in; } ///< Converts an AVector3 to an AVector.
    template <class T> inline AVector3<T>        toAVect(const Vector2<T> &in) { return AVector3<T>(in.x(),in.y(),0.0); } ///< Converts an Vector2 to an AVector3.
    template <class T> inline AVector3<T>        toAVect(const Vector3<T> &in) { return AVector3<T>(in.x(),in.y(),in.z()); } ///< Converts an AVector3 to an AVector.
    template <class T> inline Extent<T>          toExtent(const Extent2<T> &t) { return Extent3<T>(t.x1(),t.x2(),t.y1(),t.y2(),0,1); } ///< Converts Extent2 to an Extent.
    template <class T> inline Extent<T>          toExtent(const Extent3<T> &t) { return t; } ///< Converts a Extent3 to an Extent.
    template <class T> inline IVect              toIVect(const Vector3<T> &in) { IVect iv(to<Int>(in.x()),to<Int>(in.y()),to<Int>(in.z()));  return iv; } ///< Converts a Vector3<T> to an IVect.
    template <class T> inline FVect              toFVect(const Vector3<T> &in) { FVect fv(to<Float>(in.x()),to<Float>(in.y()),to<Float>(in.z()));  return fv; } ///< Converts a Vector3<T> to a FVect.
    template <class T> inline DVect              toDVect(const Vector3<T> &in) { DVect dv(to<Double>(in.x()),to<Double>(in.y()),to<Double>(in.z()));  return dv; } ///< Converts a Vector3<T> to a DVect.
    template <class T,class U> inline constexpr const U &dimval(const T &,const U &val3) { return val3; } ///< Returns the first argument in a 2D compile, and the second in a 3D compile.
    static const unsigned int totdof = 6;
#endif  // DIM==2

static constexpr unsigned int dim = DIM;              ///< 2 if 2D, 3 if 3D.
static constexpr bool         is2D = dimval(true,false);  ///< true if code is 2D.
static constexpr bool         is3D = dimval(false,true);  ///< true if code is 3D.

/// @}
// EoF
