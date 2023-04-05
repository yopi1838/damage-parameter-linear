#pragma once
/** \file limit.h
  * \brief std::numeric_limits shorthand, with debug overflow checking
  * \addtogroup Base Base interface specification
  * @{
  */
#include "basedef.h"
#include <limits>

#ifdef _WIN32
# pragma warning(disable:4146)
#endif

#ifdef __LINUX 
#include <math.h>
#endif

/** \brief debug checked shorthand for std::numeric_limits<T>:: 
  *
  * This simply allows the use of limits<T> as a convenient shorthand
  *   to the verbose std::numeric_limits<T>.\n
  * It also performs overflow checking in debug mode only.
  */
template <class T>
class limits : public std::numeric_limits<T> {
public:
      /// Returns the "real" minimum value of the type.\n
      /// Intended to evade the weird behavior for std::numeric_limits that min() works (as one would expect)
      ///   for integral types, but not for floating point types.\n
      /// \return min() for integral types, and -max() for floating point types.
      static T real_min() {
          T min_val = limits<T>::is_integer ? (limits<T>::is_signed ? limits<T>::min() : 0) : -limits<T>::max();
          return min_val;
      }
};
template <class T>
constexpr T initVal() { return limits<T>::max(); }
template <>
constexpr double initVal<double>() { return limits<double>::quiet_NaN(); }
template <>
constexpr float initVal<float>() { return limits<float>::quiet_NaN(); }


#ifdef _WIN32
# pragma warning(default:4146)
#endif

/// @}
// EoF
