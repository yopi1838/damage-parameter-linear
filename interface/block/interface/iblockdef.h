#pragma once
/**
  * \file
  * \brief Fortran to C++ type declarations.
  * \defgroup THREEDEC  3DEC interface specification
  */

#include "base/src/base.h"

/// \brief Interface to provide access to 3DEC modules, global data, list of 3DEC entities, etc.
/// \ingroup THREEDEC
#ifdef DOUBLEPRECISION
  /// Fortran  double precision integer
  typedef Long   FInt;
  /// Fortran double precisuon real
  typedef Double Real;
#else
  /// Fortran  single precision integer
  typedef Int    FInt;
  /// Fortran single precision real
  typedef Float  Real;
#endif
/// real vector
typedef Vector3<Real> RVect3;

// EoF