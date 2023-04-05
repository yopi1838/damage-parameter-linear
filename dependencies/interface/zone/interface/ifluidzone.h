//ifluidzone.h
#pragma once

/**
  * \file
  * \brief Interface to access fluid zone data
  */

namespace zone
{
/// \brief Interface to access fluid zone data
/// \ingroup Zone
  class IFluidZone
  {
  public:
    /// Sets the zone pore pressure
    virtual void  setZoneBasedPP(double pp)=0;
    /// Returns the fluid model name
    virtual QString getModelName() const=0;
    virtual double getFluidDensity() const=0;
    virtual void setFluidDensity(double d)=0;
  };
} // namespace zone
// EoF
