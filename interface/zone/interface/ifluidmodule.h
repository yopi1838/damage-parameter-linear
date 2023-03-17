// ifluidmodule.h
#pragma once

/**
  * \file
  * \brief Interface to the Fluid Module
  */

#include "base/src/base.h"

namespace zone
{
  /// \brief Interface to the Fluid Module
  /// \ingroup Zone
  class IFluidModule
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815bf3;
    /// Returns true if to use BIO modulus
    virtual bool   getBiot() const=0;
    /// Returns the accumulated fluid-flow time, see setTime()
    virtual double getTotalTime()const=0;
    /// Sets the the accumulated fluid-flow time, see getTime()
    virtual void   setTotalTime(double d)=0;
    /// Returns the current fluid-flow force ratio limit.
    virtual double getRatio()const =0;

    // Fast flow items
    /// Returns the current average unbalanced fluid volume for fastflow 
    virtual double getAvgUnbCur()const =0;
    /// Returns the current maximum unbalanced fluid volume for fastflow
    virtual double getMaxUnbCur()const =0;
    /// Returns the representative stress magnitude for fastflow
    virtual double getStrNorm()const =0;

  };
} // namespace zone
// EoF
