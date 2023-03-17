// ithermzone.h
#pragma once

/**
  * \file
  * \brief Interface to access zone thermal data
*/

namespace zone
{
/// \brief Interface to access zone thermal data
/// \ingroup Zone
  class IThermZone
  {
  public:
    /// Returns the name of the current Thermal model in the zone.
    virtual QString getModelName() const=0;
    /// Sets the zone temperature to \b temp
    virtual void setZoneBasedTemp(double temp) =0;
  };
} // namespace zone
// EoF
