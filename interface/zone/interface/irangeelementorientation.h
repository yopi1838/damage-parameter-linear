#pragma once
// irangeelementorientation.h

/**
  * \file
  * \brief Interface to a filter element that allows user to filter based on the orientation of zone faces.
  */

#include "module/interface/irangeelement.h"

namespace zone
{
  using namespace itascaxd;

  /// \brief Interface to a filter element that allows user to filter based on the orientation of zone faces.
  /// \ingroup Zone
  class IRangeElementOrientation {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815be8;
    /// Returns the orientation, see setOrientation()
    virtual Orientation getOrientation() const=0;
    /// Returns the tolerance (radians) that the face normal will be checked against the orientation normal 
    virtual double      getTolerance() const=0;

    // slots
    /// Sets a new orientation, emits the sigmal getOrientation(). See getOrientation()
    virtual void setOrientation(const Orientation &o)=0;
    /// Sets the tolerance, emits signal toleranceChanged(). See getTolerance().
    virtual void setTolerance(const double &d)=0;

    // signals
    /// Signal emitted when a new orientation is set. See setOrientation().
    virtual void orientationChanged(const Orientation &o)=0;
    /// Signal emitted when a new tolerance is set. See setTolerance().
    virtual void toleranceChanged(const double &d)=0;
  };
} // namespace zone
// EoF

