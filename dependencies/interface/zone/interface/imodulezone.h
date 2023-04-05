#pragma once

namespace zone {
    class IZoneInterpolate;
    class IZoneFieldData;
    class IFluidModule;
    class IThermModule;
    class IDynamicModule;
    class ICreepModule;

    class IModuleZone {
    public:
        /// This reinitializes FLAC3D data structures, as if preparing for cycling.\n
        /// It may be necessary to call this function if geometry or stiffness changes have invalidated
        ///   the current state.
        /// \brief Forces an immediate large-strain update.
        virtual void                     doUpdate()=0;
        
        /// The returned object must be destroyed by the caller.
        /// This object can be used to interpolate zone-based data anywhere in the model.
        /// \brief Creates an IZoneInterpolate object, and returns a pointer to it.
        virtual IZoneInterpolate *       createIZoneInterpolate(QObject *obj) const=0;
        
        /// The returned object must be destroyed by the caller.
        /// This object can be used to interpolate field (gp or zone) data anywhere in the model.
        /// \brief Creates an IZoneFieldData object, and returns a pointer to it.
        virtual IZoneFieldData *         createIZoneFieldData(QObject *obj) const=0;
        
        /// Returns a IFluidModule pointer to the fluid module
        virtual IFluidModule *  getIFluidModule()=0;
        /// Returns a IThermModule pointer to the thermal module
        virtual IThermModule *  getIThermModule()=0;
        /// Returns a IDynamicModule pointer to the dynamic module
        virtual IDynamicModule *getIDynamicModule()=0;
        /// Returns a ICreepModule pointer to the creep module
        virtual ICreepModule *  getICreepModule()=0;
    };
} // namespace zone
// EoF
