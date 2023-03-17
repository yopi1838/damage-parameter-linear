//iwatertable.h
#pragma once

/**
  * \file
  * \brief Interface for accessing water table data 
*/

#include "module/interface/dim.h"

namespace zone
{
  /// \brief Interface for accessing water table data
  class IWaterTable
  {
  public:
    /// Returns the name of the geometry set used for water table data
    virtual QString getWaterTableGeometrySetName() const=0;
    /// Returns the tag associated with the water table change notice
    virtual uint    getWaterNotice() const=0;
    /// Returns \b true if the water table is infinite, otherwise returns \b false.
    virtual bool    isInfinite() const=0;
    /// Returns the unit normal to the water plane and pointing in the direction of 
    ///  increasing pore-pressure
    virtual DVect   getNormal() const=0;
    /// Returns one point on the water plane.
    virtual DVect   getOrigin() const=0;
    /// Returns zone extent used in water table calculations
    virtual DExtent getExtent() const = 0;
  };
} // namespace zone
// EoF
