#pragma once

#include "security/interface/isecurity.h"

namespace sel {
    class IModuleSEL {
    public:
        /// This reinitializes SEL data structures, as if preparing for cycling.\n
        /// It may be necessary to call this function if geometry or stiffness changes have invalidated
        ///   the current state.
        /// \brief Forces an immediate large-strain update.
        virtual void doUpdate()=0;
        virtual double getRatioAverage() const=0;
        virtual double getRatioLocal() const=0;
        virtual double getRatioMax() const=0;
        virtual double getConvergence() const=0;
        virtual double getUnbalanced() const=0;
        virtual void addRequirement(const security::Requirement &r)=0;
    };
} // namespace zone
// EoF
