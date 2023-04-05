#pragma once
// irangeelementsurface.h

/**
* \file
* \brief Interface to a filter element that allows user to filter based on connection
*        to a "surface" of the zones.
*/

#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Interface to a filter element that allows user to filter based on connection to a "surface" of the zones.
    /// \ingroup Zone
    class IRangeElementSurface {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4ed7e54c;

        /// Returns TRUE if filtering by null mechanical model to determine surface.
        virtual bool getMech() const=0;
        /// Returns TRUE if filtering by null fluid model to determine surface.
        virtual bool getFluid() const=0;
        /// Returns TRUE if filtering by null thermal model to determine surface.
        virtual bool getTherm() const=0;
        /// Returns TRUE if any condition set to true causing the zone to be filtered,
        /// Returns FALSE if *all* conditions set to true cause the zone to be filtered.
        virtual bool getOr() const=0;

        // slots
        /// Slot, sets the mechanical flag filter. See getMech().
        virtual void setMech(bool b)=0;
        /// Slot, sets the fluid flag filter. See getFluid().
        virtual void setFluid(bool b)=0;
        /// Slot, sets the therm flag filter. See getThrem().
        virtual void setTherm(bool b)=0;
        /// Slot, sets the 'OR' condition flag. See getOr()
        virtual void setOr(bool b)=0;

        // signals
        /// Signal emitted when the mechanical flag filter has changed.
        virtual void mechChanged(bool b)=0;
        /// Signal emitted when the fluid flag filter has changed.
        virtual void fluidChanged(bool b)=0;
        /// Signal emitted when the therm flag filter has changed.
        virtual void thermChanged(bool b)=0;
        /// Signal emitted when the or condition has changed.
        virtual void orChanged(bool b)=0;
    };
} // namespace zone
  // EoF
