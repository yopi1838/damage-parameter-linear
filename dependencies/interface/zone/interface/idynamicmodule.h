#pragma once
// idynamicmodule.h

/**
  * \file
  * \brief Interface to the Dynamic Module, see IModuleExtension
  */

#include "base/src/base.h"

namespace zone
{
  /// \brief Interface to the Dynamic Module, see IModuleExtension
  /// \ingroup Zone
  class IDynamicModule
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815bf1;
    /// Returns the dynamic multistepping state, true indicates that multistep os on
    virtual bool     getMulStep() const =0;
    /// Returns the accumulated dynamic time, see setTime()
    virtual double   getTotalTime()const=0;
    /// Sets the accumulated dynamic time, getTime()
    virtual void     setTotalTime(double d)=0;
  };
} // namespace zone
// EoF
