#pragma once

// This contains low-level common things.
// It is expected you will include it in anything making use of utility and above.
// It mostly contains handy math utilities, with some debug tools as well.

#include "base/src/baseqt.h"
#include <cmath>

#ifdef UTILITY_LIB
#  define UTILITY_EXPORT Q_DECL_EXPORT
#else
#  define UTILITY_EXPORT Q_DECL_IMPORT
#endif

// Check MS compiler version number, to be certain it is compatible
#ifdef _WIN32
#if _MSC_VER<1700
#  if _MSC_FULL_VER != 160040219
#    error Microsoft C++ Compiler is of a different version than the source.
#  endif
#endif
#endif
//#ifdef __INTEL_COMPILER
//#  if __INTEL_COMPILER_BUILD_DATE != 20150121 // For Intel Comp 15 Update 2.
//#    error Intel C++ Compiler is of a different version than the source.
//#  endif
//#endif

//#define QFile ERROR YOU SHOULD NOT USE QFILE DIRECTLY ANYMORE

namespace itasca {
    // absolute value function
    template <class T> inline T abs(const T &t) { return  t < 0 ? -t : t;  }
    template <> inline double abs(const double &d) { return std::fabs(d); }
    template <> inline int abs(const int &i) { return ::abs(i); }

    // This function returns t1 with the same sign as t2.
    template <class T1,class T2> 
    inline T1 sign(const T1 &t1,const T2 &t2) { return t2 < 0 ? -qAbs(t1) : qAbs(t1); }

    template <class D> inline D round(const D &d) {
        D f = std::floor(d);
        D f2 = d - f;
        return (f2>=0.5 ? (f+1.0) : (f2<=-0.5 ? f - 1.0 : f));
    }

    template <class T,class D> inline T lround(const D &d) {
        T i = to<T>(std::floor(d));
        D f2 = d - to<D>(i);
        return (f2>=0.5 ? (i+1) : (f2<=-0.5 ? i-1 : i));
    }

    namespace debug {
        // Throws an exception if b is false.  Can be called directly even in a release compile.
        void UTILITY_EXPORT check(bool b,uint line,const char *file);
        // In debug mode outputs to a console (I think), in release mode the first call creates 
        //   a file called debug.txt and subsequent calls continue to output to it.
        void UTILITY_EXPORT output(const QString &sOutput);
    } // namespace debug
} // namespace itasca

#ifdef _WIN32
#  define NOOP __noop()
#else
#  define NOOP do {} while(0)
#endif

// Handy debug utilities that evaluate to nothing if _DEBUG isn't defined.
#ifdef _WIN32

#ifdef _DEBUG
#  define ASSERT(x) itasca::debug::check(x ? true : false,__LINE__,__FILE__)
#  define DEBUG(x)  itasca::debug::output(x)
#  define DEBUG_ONLY(x) x
#else
#  define ASSERT(x) NOOP
#  define DEBUG(x) NOOP
#  define DEBUG_ONLY(x)
#endif

#else

#ifdef _DEBUG
#  define ASSERT(x) assert(x)
#  define DEBUG(x)  NOOP
#  define DEBUG_ONLY(x) x
#else
#  define ASSERT(x) NOOP
#  define DEBUG(x) NOOP
#  define DEBUG_ONLY(x)
#endif

#endif

// EoF
