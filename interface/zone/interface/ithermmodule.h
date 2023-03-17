// ithermmodule.h
#pragma once

/**
  * \file
  * \brief Interface to the thermal module
*/

namespace zone
{
  /// \brief Interface to the thermal module
  /// \ingroup Zone
  class IThermModule
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x3c815bf6;
    /// Returns the accumulated thermal time.
    virtual double getTotalTime() const =0;
    /// Sets the accumulated thermal time to value  \b d.
    virtual void   setTotalTime(double d)=0;
    /// Returns the current Thermal Ratio
    virtual double getRatio() const =0;
  };
} // namespace zone
// EoF
