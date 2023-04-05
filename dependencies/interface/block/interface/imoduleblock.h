#pragma once

#include "iblockdef.h"

namespace block {
  class IZoneInterpolate;
  class IZoneFieldData;

  class IModuleBlock {
  public:

    /// returns REAL pointer to main data array
    virtual const Real *getA() const=0;
    /// returns FInt pointer to main data array
    virtual const FInt *getIA() const=0;

    /// returns size of main data array
    virtual FInt getMemoryTop() const=0;
    /// returns index of first unused location in main data array
    virtual FInt getMemoryFree() const=0;
    /// returns true if subcontacts have been created
    virtual FInt getSubcontactFlag() const=0;

    /// returns index of first block
    virtual FInt getBlockHead() const=0;

    /// returns pointer to list of loaded zone constitutive models
    virtual QStringList loadModelPlugins()=0;
    /// returns pointer to list of loaded joint constitutive models
    virtual QStringList loadJModelPlugins(const QString &location=QString())=0;

    /// The returned object must be destroyed by the caller.
    /// This object can be used to interpolate zone-based data anywhere in the model.
    /// \brief Creates an IZoneInterpolate object, and returns a pointer to it.
    virtual IZoneInterpolate *       createIZoneInterpolate(QObject *obj) const=0;

    /// The returned object must be destroyed by the caller.
    /// This object can be used to interpolate field (gp or zone) data anywhere in the model.
    /// \brief Creates an IZoneFieldData object, and returns a pointer to it.
    virtual IZoneFieldData *         createIZoneFieldData(QObject *obj) const=0;

  };
} // namespace block
// EoF
