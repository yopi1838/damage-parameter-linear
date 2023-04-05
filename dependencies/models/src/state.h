#pragma once

#include "conmodelbase.h"
#include "igenerictet.h"
#include "igenericzone.h"
#include <array>

namespace itasca {
    class IProgram;
}

/**
* \file
* \brief The base class for constitutive model plug-ins.
*/

namespace models {
    using namespace itasca;

    /// The most important link between FLAC3D and a user-written model is the member-function
    ///   run(unsigned dim, State *ps), which computes the mechanical response of the model
    ///   during cycling.\n
    /// A structure, State (defined in state.h?, is used to transfer information to and
    ///   from the model.\n
    /// State is also sent to the initialize() method, although not all fields are guaranteed
    ///   to be valid in that case.\n
    /// The members of State (all public) are as follows.\n
    /// Not all the information may be used by a particular code; the structure is intended 
    ///   to serve all Itasca codes.  
    /// \brief The structure used to pass information to the constitutive model.
    /// \ingroup models
    struct State {
    public:
        static const UInt max_working_ = 50; ///< \brief The size of the working_ array.
        static const UInt max_iworking_ = 10; ///< \brief The size of the iworking_ array.

        /// Volume of the current sub zone.  
        /// If the implementation does not define sub-zones (total_sub_zones_ = 1), 
        ///   then this should return the same as getZoneVolume().
        virtual Double    getSubZoneVolume() const=0;
        /// Volume of the current zone as a whole.
        virtual Double    getZoneVolume(bool deformed = false) const=0; 
        /// Three components of zone rotation (spin velocity multiplied by timestep), 
        ///   input to the model in large-strain mode.\n
        /// This information may be used by models that have directional properties that must
        ///   be updated in large-strain mode.
        virtual DVect3    getRotation() const=0; ///< \brief Three components of overlay unit rotation in large strain.
        /// Density of full zone. Note that density is automatically
        ///   updated by FLAC3D in large-strain mode if the volume changes.    
        virtual Double    getDensity() const=0; ///< \brief Density of the zone.
        virtual void      setDensity(const Double&)=0; ///< \brief set Density of the zone.
        virtual Double    getTemperature() const=0; ///< \brief Temperature of the zone.
        virtual Double    getTimeStep() const=0;  ///< \brief The current time step.
        virtual Double    getPorosity() const=0; ///< \brief The current zone fluid porosity.
        virtual bool      isLarge() const=0; ///< \brief true if in large strain mode.
        virtual bool      isThermal() const=0; ///< \brief true if thermal is configured.
        virtual bool      isCreep() const=0; ///< \brief true if creep is configured.
        virtual bool      isFluid() const=0; ///< \brief true if fluid/flow is configured.
        virtual bool      isThermalInUse() const = 0; ///< \brief true if thermal is in use.
        virtual bool      isCreepInUse() const = 0; ///< \brief true if creep is in use.
        virtual bool      isFluidInUse() const = 0; ///< \brief true if fluid/flow is in use.
        /// For some stress-dependent constitutive models to access the previous zone-level stress,
        /// It should be called only once before any stress update in any subzone.
        virtual SymTensor getEffectiveZoneStress() const=0; ///< \brief The average effective stress in the zone.
        virtual Double    getAveragePP() const=0; ///< \brief The average pore pressure in the zone.
        virtual double    getTMUtility(unsigned index=0) const=0; ///< \brief Returns Thermal to Mechanical communication utility values.
        virtual double    getFMUtility(unsigned index=0) const=0; ///< \brief Returns Fluid to Mechanical communication utility values.
        /// Finding a table given an \a id number can be an expensive operation.\n
        /// In general, it is better to store a reference pointer using this function during initialize(),
        ///   and use it to access the table during run().
        /// Index pointers should *not* be saved and restored, and should be found during every initialize() call.
        ///   (Table data may have changed in the interim).
        virtual void *    getTableIndexFromID(const String &id) const=0; ///< \brief Return a table index given an ID.
        /// The index pointer should be previously obtained from a getTableIndexFromID() call.
        virtual Double    getYFromX(void *index,const Double &x) const=0; ///< \brief Lookup a value in a table.
        /// The index pointer should be previously obtained from a getTableIndexFromID() call.
        virtual Double    getSlopeFromX(void *index,const Double &x) const=0; ///< \brief Lookup a rate of change at a point in the table.
        virtual IProgram *getIProgram()=0; ///< \brief A pointer to an IProgram interface for this code.  May be null.
        virtual IGenericZone *getIZone()=0; ///< \brief A pointer to the IZone being processed.  May be null.
        virtual IGenericTet * getITet()=0; ///< \brief A pointer to the ITet in the IZone being processed.  May be null.
        virtual ULong     getZoneID() const=0;  ///< \brief The ID or index of the Zone being processed.  May be 0.
        virtual ULong     getTetID() const=0;  ///< \brief The ID or index of the Tetra being processed.  May be 0.
        CONMODEL_EXPORT State();
        /// This information may be used to scale accumulated sub-zone data correctly. 
        /// For example, if ten sub-zones are present (see total_sub_zones_), accumulated
        /// values will need to be divided by ten, in order to obtain the average for the whole zone.    
        unsigned int  sub_zone_;           ///< \brief # of sub zone being operated on (starting at 0).
        unsigned int  total_sub_zones_;    ///< Total # of sub zones involved, from all overlays.  Minimum value of 1.
        unsigned int  overlay_;            ///< # of times sub zones represent volume (1 or 2 usually).
        /// Model state indicator bit flag.\n
        /// Specific bits in this integer correspond to names in the getStates() member function.\n
        /// For example, a flag value of 1 (bit 0) represents the first state, a value of 2 (bit 1) 
        ///   represents the second, a value of 4 (bit 2) represents the third, a value of 8 (bit 3) 
        ///   represents the fourth, etc.\n
        /// Any number of bits may be selected simultaneously (for example, both shear and tensile yield
        ///   may occur together).
        unsigned long state_;              ///< \brief Sub zone state bit flag.
        SymTensor     stnE_;               ///< \brief 6 components of strain increment, input to run().
        /// The current stress tensor is input to run(), and the model must return the updated tensor.
        SymTensor     stnS_;               ///< \brief 6 components of stress.
        Double        modulus_reduction_factor_; ///< \brief modulus reduction factor from hysteretic damping or small strain effect.
        /// true if viscous strains are to be computed for the stiffnessproportional component of Rayleigh damping.\n
        /// This flag defaults to false if not set.\n
        /// Typically, it should be set true for an elastic increment, and false for an increment in which yielding occurs.   
        bool          viscous_;            ///< \brief True if viscous strains are to be computed.
        /// A change in the elastic modulus of a zone (particulary if it gets stiffer) may have stability implications.\n
        /// Setting this flag to true during run() notifies the code of this fact, so that
        ///   stability requirements may be recalculated.
        bool          modulus_changed_;    /// \brief Set to true in run() if modulus has changed (confined, shear, bulk).
        /// Mean plastic stress change value, needed for nodal mixed discretization calculations.
        /// If plastic flow changes the pressure, it needs to be indicates here so that pressure averaging will proceed correctly.
        Double        pore_pressure_increment_; ///< \brief Increment in pore pressure supplied by model.
        std::array<Double,max_working_> working_; ///< \brief This is a working area for values that must be stored between run() calls.
        std::array<Int,max_iworking_> iworking_; ///< \brief This is a working area for values that must be stored between run() calls.
    };
} // namespace models
// EoF
