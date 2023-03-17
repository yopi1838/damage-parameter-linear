#pragma once
// iglobaldata.h

/**
* \file
* \brief Interface to access FLAC3D data, see SET and CONFIG commands
*/

#include "module/interface/dim.h"

namespace zone {
    /// \brief Interface to access FLAC3D data, see SET and CONFIG commands
    /// \ingroup Zone
    class IZoneGlobals {
    public:
        /// Returns a pointer to a QObject representing this object.
        virtual const QObject *getQObject() const=0;
        /// If true, then FLAC3D uses a value of pore-pressure that has been stored in the zone, rather than
        ///   calculating an average pore-pressure based on the surrounding gridpoints.
        virtual bool           getUseZoneBasedPP() const=0;
        /// Returns the current unbalanced mechanical-force ratio.
        virtual double         getMechRatio() const=0;
        virtual double         getAverageMechRatio() const=0;
        virtual double         getMaximumMechRatio() const=0;
        virtual double         getLocalMechRatio() const=0;
        /// Returns the maximum unbalanced force in the grid.
        virtual double         getMaximumUnbalancedForce() const=0;
        virtual double         getMaximumConvergence() const=0;
        virtual void           requestUpdate()=0;
        /// Returns whether FLAC type strain mixing is used (relevant for 2D only)
        virtual bool           getFlacStrainMixing() const = 0;
        /// Returns whether FLAC type strain mixing is used by default (relevant for 2D only)
        virtual bool           getFlacStrainMixingDefault() const = 0;

        // SLOTS
        /// Sets whether to use FLAC type strain mixing (relevant for 2D only)
        virtual void setFlacStrainMixing(bool b) = 0;
        /// Sets whether to use FLAC type strain mixing by default (relevant for 2D only)
        virtual void setFlacStrainMixingDefault(bool b) = 0;

        //SIGNALS
        /// Signal emitted when somebody changes type of strain mixing (relevant for 2D only)
        virtual void flacStrainMixingChanged(bool) = 0;
        /// Signal emitted when somebody changes default type of strain mixing (relevant for 2D only)
        virtual void flacStrainMixingDefaultChanged(bool) = 0;
    };
} // namespace zone
// EoF

