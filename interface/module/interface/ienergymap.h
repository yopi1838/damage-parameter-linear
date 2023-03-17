#pragma once

#include "base/src/baseqt.h"

/// \file
/// \brief Interface to EnergyMap class

namespace itasca {

    /// \brief Interface to EnergyMap class.
    /** This class allow to access names and values of energies stored in the EnergyMap.
      *  \ingroup module
      */

    class IEnergyMap {
    public:
        /// Return a QStringList of all energy names defined in the EnergyMap, comma separated 
        virtual QStringList getKeywords()                const =0;
        /// Return a floating point value corresponding to the current value of energy with name \a key defined in the EnergyMap.
        /// Will cause an error if the energy with name \a key does not exist.
        virtual double      getValue(const QString &key) const =0;
    };
} // namespace itasca
// EoF
