// icreepmodule.h
#pragma once

/**
  * \file
  * \brief Interface to the Creep Module, , see IModuleExtension
  */

#include "base/src/base.h"

namespace zone
{
  /// \brief Interface to the Creep Module, , see IModuleExtension
  /// \ingroup Zone
  class ICreepModule
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a0b;
    /// Returns the accumulated crep time, see setTime()
    virtual double getTotalTime() const =0;
    /// Sets the accumulated creep time, see getTime()
    virtual void setTotalTime(double d)=0;
  };
} // namespace zone
// EoF
