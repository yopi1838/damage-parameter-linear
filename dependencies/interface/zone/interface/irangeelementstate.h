#pragma once
// irangeelementstate.h

/**
  * \file
  * \brief Interface to a filter element that allows user to filter based on a zone state.
  */

#include "module/interface/irangeelement.h"

namespace zone
{
  using namespace itascaxd;

  /// \brief Interface to a filter element that allows user to filter based on a zone state.
  /// \ingroup Zone
  class IRangeElementState {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4f1dd8c7;

    /// Returns the filter state name, see setState().
    virtual QString getState() const=0;

    /// Returns true for any, false for average. See setState().
    virtual bool getStateAny() const=0;

    // slots
    /// Used to set a new state. It issues the signal stateChanged(stateName). See getState().
    virtual void setState(const QString &state)=0;

    /// Used to set the state recovery 'any' or 'average'.
    virtual void setStateAny(bool any)=0;


    // signals
    /// Signal emitted when the mocel name changes
    virtual void stateChanged(const QString &state)=0;
  };
} // namespace zone
// EoF
