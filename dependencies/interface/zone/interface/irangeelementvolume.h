#pragma once
// irangeelementvolume.h

/**
  * \file
  * \brief Interface to a filter element that allows user to filter based on a volume.
  */

#include "module/interface/irangeelement.h"

namespace zone
{
  using namespace itascaxd;
  /// \brief Interface to a filter element that allows user to filter based on a volume.
  /// \ingroup Zone
  class IRangeElementVolume {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815be4;
    /// Returns a pointer to itsel.
    virtual IRangeElement *getIRangeElement()=0;
    /// Returns the ID of the volume, see setID()
    virtual quint64       getID() const=0;
    // slots

    /// Sets the ID of the volume, see getID(). This function emits the signal idChanged()
    virtual void setID(quint64 id)=0;
    // signals
    /// Signal emitted when the volume ID changes, see setID()
    virtual void idChanged(quint64 id)=0;
  };
} // namespace zone
// EoF

