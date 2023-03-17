#pragma once
/** \file
* \brief A overflow checked shorthand for static_cast<T>().
* \addtogroup Base Base interface specification 
* @{
*/

#include "limit.h"
#include <cassert>
#include <stdexcept>

#ifdef _WIN32
#  ifdef __INTEL_COMPILER
#   pragma warning(disable:2557)
#  else
#   pragma warning(disable:4018)
#  endif
#endif

/** \brief This template function serves as an alternative to static_cast<T>().
*
* It is not as verbose, and it checks for out-of-range casting in debug mode.
*/
template <class D,class T> D to(const T &t) {
#ifdef _WIN32
#  ifdef _DEBUG
#    ifndef __INTEL_COMPILER  // NEED TO FIND SOMETHING THAT WORKS FOR INTEL!
    if (t<0 && t<limits<D>::lowest())
        throw std::runtime_error("Underflow error in numeric conversion.");
    if (t>limits<D>::max()) 
        throw std::runtime_error("Overflow error in numeric conversion.");
#    endif
#  endif
#endif
    return static_cast<D>(t);
}
#ifdef _WIN32
# pragma warning(default:4018)
#endif

/** \brief This template function serves as a fast alternative to dynamic_cast, when you know the base offset is the same.
*
* In debug mode, it checks for a base offset change by comparing the result against an actual dynamic_cast.
*/
template <class D,class T> D *check_cast(T *t) {
    D *d = (D *)t;
#ifdef _DEBUG
    D *d2 = dynamic_cast<D *>(t);
    assert(d2==d);
#endif
    return d;
}

/** \brief This template function serves as a fast alternative to dynamic_cast, when you know the base offset is the same.
*
* In debug mode, it checks for a base offset change by comparing the result against an actual dynamic_cast.
*/
template <class D,class T> const D *check_cast(const T *t) {
    const D *d = (const D *)t;
    assert(dynamic_cast<const D *>(t)==d);
    return d;
}

/** \brief This function provids "safe" division operation, checks explicitly for zero
*
* Don't use when performance is important!
* (Could add overflow underflow checking here, but would have to find a way to do that without undue calculation burden)
*/
template <class T> const T safeDiv(const T num,const T denom) {
    if (!denom) return num;
    return num/denom;
}

/// @}
// EoF
