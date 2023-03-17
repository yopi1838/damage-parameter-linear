#pragma once

/**
  * \file
  * \brief Interface to hysteretic damping 
  */

namespace zone
{
  /// \brief Interface to hysteretic damping 
  /// \ingroup Zone
  class IHysDamp
  {
  public:
    virtual double     getHProperty(int n) const=0;
    virtual double     getStateValue(int n) const=0;
    virtual void       setHProperty(int n, const double &v)=0;
    virtual void       setStateValue(int n, const double &v)=0;
  };
} // namespace zone
// EoF
