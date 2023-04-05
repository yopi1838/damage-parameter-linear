#pragma once

//#ifdef _DEBUG
//#  define _CTRDBG_MAP_ALLOC
//#  include <stdlib.h>
//#  include <crtdbg.h>
//#endif
/** \file export.h
  * \brief Defines automatic generation of DLL exports and imports for each system supported.
  * \addtogroup Base Base interface specification
  * @{
  */
#ifdef _WIN32
#  define EXPORT_TAG __declspec(dllexport)
#  define IMPORT_TAG __declspec(dllimport)
#  ifdef NDEBUG  // Necessary to get around weird warnings given in the STL classes in a release compile.
#    pragma warning(disable:4702)
#    undef _DEBUG
#  endif
#  ifdef __INTEL_COMPILER
#    define INTELCOMP
#  else
#    define VSCOMP
#    pragma warning(disable:4275)
#  endif
#  if _MSC_VER >= 1910
#    define VS2017
#  endif
#  if _MSC_VER >= 1920
#    define VS2019
#  endif
#endif
#ifdef __LINUX
#  define EXPORT_TAG
#  define IMPORT_TAG
#endif

/// @}
// EoF
