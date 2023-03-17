#pragma once
//ibeamcontact.h

/**
  * \file
  * \brief This interface provides access to beam contact data.
  */

#include "iblockdef.h"

namespace block
{
  /// \brief This interface provides access to beam contact data.
  /// \ingroup THREEDEC
  class IBeamContact
  {
  public:
    /// linked list offset for next beam contact
    static const FInt next         =  0;
  };
} // namespace block
// EoF