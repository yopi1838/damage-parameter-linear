#pragma once
// irangeelementmodel.h

/**
  * \file
  * \brief Interface to a filter element that allows user to filter based on a zone model.
  */

#include "module/interface/irangeelement.h"

namespace zone
{
  using namespace itascaxd;

  /// \brief Interface to a filter element that allows user to filter based on a zone model.
  /// \ingroup Zone
  class IRangeElementModel {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815bea;

    /// Returns the filter model name, see setModel().
    virtual QString getModel() const=0;

    // slots
    /// Used to set a new model name. It issues the signal modelChanged(modelName). See getModel().
    virtual void setModel(const QString &model)=0;

    // signals
    /// Signal emitted when the mocel name changes
    virtual void modelChanged(const QString &model)=0;
  };
} // namespace zone
// EoF
