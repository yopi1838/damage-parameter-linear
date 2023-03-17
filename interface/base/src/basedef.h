#pragma once
/** \file basedef.h
 *  \brief Base type definitions - if QT is not in use.
 *
 *  These are base type definitions, used to avoid system-specific changes in size
 *  and capacity.  Use then only if QT is not being used. Qt defines its
 *  own types.
 * \addtogroup Base Base interface specification
 * @{
 */

#include "export.h"
#include <wchar.h>

/// Define BASE_LIB in project settings to export
///   If not defined assumes using as import.
///   This is a standard for headers in DLLs.
#if defined(BASE_LIB) | defined(BASE2017_LIB)
#  define BASE_EXPORT EXPORT_TAG
#else
#  define BASE_EXPORT IMPORT_TAG
#endif
//#define itascaxd "Error: Need to define DIM and include dim.h"

typedef wchar_t                 Char;   ///< string character type, Unicode supporting.
typedef char                    Byte;   ///< signed 8 bit
typedef unsigned char           UByte;  ///< unsigned 8 bit
typedef short                   Short;  ///< signed 16 bit
typedef unsigned short          UShort; ///< unsigned 16 bit
typedef int                     Int;    ///< signed 32 bit
typedef unsigned int            UInt;   ///< unsigned 32 bit
#ifdef __LINUX
typedef long long               Long;   ///< signed 64 bit
typedef unsigned long long      ULong;  ///< unsigned 64 bit
#else
typedef __int64                 Long;   ///< signed 64 bit
typedef unsigned __int64        ULong;  ///< unsigned 64 bit
#endif
typedef float                   Float;  ///< 32 bit floating point
typedef double                  Double; ///< 64 bit floating point
typedef unsigned int            TType;  ///< class type indicator

// Better typedefs to use in the future
using char16 = char16_t; // Assume UTF-16 encoding
using char8 = char; // Assuming UTF-8 encoding
using int8 = signed char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = Long;
using uint64 = ULong;

#ifdef __LINUX
#include <fenv.h>
#define _EM_OVERFLOW FE_OVERFLOW
#define _EM_INVALID FE_INVALID
#define _EM_UNDERFLOW FE_UNDERFLOW
#define _EM_ZERODIVIDE FE_DIVBYZERO
inline unsigned doClearFP() {
    int ret = fetestexcept(FE_ALL_EXCEPT);
    feclearexcept(FE_ALL_EXCEPT);
    return (unsigned)ret;
}
#define _clearfp doClearFP
#define __noop() { }
#define OS_CURRENT 2
#else
#define OS_CURRENT 1
#endif

#define OS_LINUX 2
#define OS_WINDOWS 1 

#ifndef DOXYGEN
#  ifdef _DEBUG
#    define CHECK_FP_STATUS checkFPStatus(__FILE__,__LINE__)
#    define CLEAR_FP_STATUS _clearfp()
#  else
#    define CHECK_FP_STATUS __noop()
#    define CLEAR_FP_STATUS __noop()
#  endif
// A concise version of CHECK_FP_STATUS.
#  define FP_S CHECK_FP_STATUS
// A concise version of CLEAR_FP_STATUS.
#  define FP_C CLEAR_FP_STATUS

// To check overflow error or invalid floating point result from the \a line of the \a file
BASE_EXPORT const char16 *checkFPStatus(const char *file=0,UInt line=0);
enum FloatingPointCheck { FloatingPointCheckOff, FloatingPointCheckWarning, FloatingPointCheckException };
BASE_EXPORT FloatingPointCheck floatingPointCheckStatus();
BASE_EXPORT void floatingPointCheckStatus(FloatingPointCheck fpc);
BASE_EXPORT bool isCurrentOS(int i);
#endif

#define FOR(x,y) for (auto x=(y).begin();x!=(y).end();++x)

/// isDebugCompile useful for templates and default arguments.
#ifdef NDEBUG
static constexpr bool isDebugCompile = false;
#else
static constexpr bool isDebugCompile = true;
#endif
/// @}
// EoF
