#pragma once

namespace zonesel {
    class IModuleZoneSEL {
    public:
        /// This reinitializes SEL data structures, as if preparing for cycling.\n
        /// It may be necessary to call this function if geometry or stiffness changes have invalidated
        ///   the current state.
        /// \brief Forces an immediate large-strain update.
        virtual void doUpdate()=0;
    };
} // namespace zone
// EoF
