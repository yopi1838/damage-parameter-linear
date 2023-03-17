#pragma once
// igp.h

/**
* \file
* \brief Interface to provide access to a gridpoint
*/

// Gp Masks 
//static const uint    N_XFIX       = 0x00000001;      /* X Fix Flag */
//static const uint    N_YFIX       = 0x00000002;      /* Y Fix Flag */
//static const uint    N_ZFIX       = 0x00000004;      /* Z Fix Flag */
static const quint32 GF_RELAX     = 0x00000008;   // Relaxation condition flag
//static const quint32 GF_NULL     = 0x00000010;   // Defined as PRIVATE
static const quint32 GF_AVAIL     = 0x00000020;   // Available to user  
static const quint32 GF_AVAIL2    = 0x00000040;   // Available to user
static const quint32 GF_GEN1      = 0x00000080;   // Gen Iterator Mask
static const quint32 GF_GEN2      = 0x00000100;   // Gen Iterator Mask
static const quint32 GF_XQUIET    = 0x00000200;   // Quiet bound. flag
static const quint32 GF_YQUIET    = 0x00000400;   // Quiet bound. flag
static const quint32 GF_ZQUIET    = 0x00000800;   // Quiet bound. flag
//static const quint32 GF_TFIX      = 0x00001000;   // Thermal FIX flag 
//static const quint32 GF_THNULL    = 0x00002000;   // Thermal NULL flag PRIVATE
//static const quint32 GF_FFIX      = 0x00004000;   // Fluid   FIX flag 
//static const quint32 GF_FLNULL    = 0x00008000;   // Fluid   NULL flag PRIVATE
static const quint32 GF_NOEOM     = 0x00010000;   // Indiciates equations-of-motion are to be skipped
static const quint32 GF_TEMP      = 0x00020000;   // TEMPORARY FLAG  - used internally
static const quint32 GF_FF1       = 0x00040000;   // Gp in corner FF
static const quint32 GF_FF2       = 0x00080000;   // Gp in side FF 
static const quint32 GF_INTER     = 0x00100000;   // Indicates GP is a coincidentGp on a host interface node
static const quint32 GF_FDSCRG    = 0x00200000;   // Fluid discharge flag
static const quint32 GF_FLKG      = 0x00400000;   // Fluid leakage flag
static const quint32 GF_FPWELL    = 0x00800000;   // Fluid pwell flag
static const quint32 GF_LOCSMALL  = 0x01000000;   // Local small-strain Motion update
//static const quint32 GF_GHOST     = 0x02000000;   // DEFINED AS PRIVATE Used with MPI to deisgnate a slave gridpoint if in force/velocity mode
//static const quint32 GF_MECHLIVE  = 0x04000000;   // set if gp is connected to a live mech zone
//static const quint32 GF_FLUIDLIVE = 0x10000000;   // set if gp is connected to a live fluid zone
//static const quint32 GF_THERMLIVE = 0x20000000;   // set if gp is connected to a live therm zone
static const quint32 GF_WG        = 0x40000000;   // Westergaard condition flag


#include "module/interface/dim.h"

namespace fish {
    class File;
}

namespace itasca {
    class IThing;
    class IFishParam;
    class IFish;
    class IGroup;
}

namespace zone {
    class INode;
    class IFluidGp;
    class IThermGp;
    class IZone;
    using itasca::IThing;

    /// \brief Interface to provide access to a gridpoint
    /// \ingroup Zone
    class IGp {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bed;
        enum class Change { MechNull, FlNull, ThNull, AddToList, RemoveFromList };
        /// Returns a const IThing pointer
        virtual const IThing *      getIThing() const=0;
        /// Returns a IThing pointer
        virtual IThing *            getIThing()=0;
        /// Returns a pointer to itself
        virtual const INode *       getINode() const=0;
        virtual INode *             getINode()=0;
        /// Returns a const pointer to the next gridpoint object, IGp, in the global gridpoint list, or 0 to indicate the end of the list.
        /// See IGpList::getFirst().
        virtual const IGp *         getNext() const=0;
        /// Returns true to indicate that gridpoint is "nulled".
        virtual bool                isMechanicalNull() const=0;
        /// Returns true to indicate that gridpoint thermal flag is set to NULL
        virtual bool                isThermalNull() const=0;
        /// Returns true to indicate that gridpoint fluid flag is set to NULL
        virtual bool                isFluidNull() const=0;
        /// Returns the gridpoint velocity, see setVelocity();
        virtual DVect               getVelocity(bool local=false) const=0;
        /// Sets the gridpoint velocity, see getVelocity();
        virtual void                setVelocity(const DVect &v,bool local=false)=0;
        /// Returns the gridpoint displacement, seet setDisplacement();
        virtual DVect               getDisplacement(bool local=false) const=0;
        /// Sets the gridpoint displacement, seet getDisplacement();
        virtual void                setDisplacement(DVect v,bool local=false) =0;
        /// Returns the gridpoint displacement accumulated for small strain, see setSmallDisplacement()
        virtual DVect               getSmallDisplacement(bool local=false) const=0;
        /// Sets the gridpoint displacement for small strain, see getSmallDisplacement()
        virtual void                setSmallDisplacement(DVect v,bool local=false) =0;
        /// Returns the gridpoint out of balance force
        virtual DVect               getFobLast(bool local=false) const=0;
        /// Returns the sum of the gridpoint static applied force and the dynamic applied force
        virtual DVect               getFap(bool local=false) const=0;
        /// Returns gridpoint out of balance force (FOB) with static and dynamic applied force vectors included and
        /// also take into consideration the gravitational acceleration \b gravity.
        /// If gridpoint is attached, returns values interpolated (if necessary) from master.
        /// If gridpoint is FIXED, removes force associated with that DOF (Possibly in a local system).
        virtual DVect               getTotalForceLast(bool reaction=false,bool local=false) const=0;
        virtual DVect               getTotalForceCurrent(const DVect &gravity,bool reaction=false,bool local=false) const=0;
        /// Returns the gridpoint position, see setPosition()
        virtual DVect               getPosition() const=0;
        /// Sets the gridpoint position, see getPosition()
        virtual void                setPosition(const DVect &v) =0;
#ifdef THREED        
        /// Returns a const pointer to the gridpoint fluid-flow data.
        virtual const IFluidGp *    getIFluidGp() const=0;
        /// Returns a pointer to the gridpoint fluid-flow data.
        virtual IFluidGp *          getIFluidGp() =0;
        /// Copies the state of this gridpoint to \b gp
#endif
        virtual void                copyState(const IGp *gp)=0;
        /// Returns 1 if gridpoint behaves as in small-strain mode. See setLocSmall();
        virtual bool                getLocSmall() const =0;
        /// If parameter \b b  is true, the gridpoint will behave as in small-strain mode. See getLocSmall();
        virtual void                setLocSmall(bool b) =0;
        /// Returns the gridpoint inertial mass, see setInertialMass()
        virtual double              getInertialMass() const=0;
        /// Sets the gridpoint inertial mass, see getInertialMass()
        virtual void                setInertialMass(const double &d) =0;
        /// Returns the gridpoint gravitational mass, see setGravitationalMass()
        virtual double              getGravitationalMass() const=0;
        /// Sets the gridpoint gravitational mass, see getGravitationalMass()
        virtual void                setGravitationalMass(const double &d)=0;
        /// Returns the gridpoint pore-pressure, see setPP()
        virtual double              getPP() const =0;
        /// Sets the gridpoint pore-pressure, see getPP()
        virtual void                setPP(double d) =0;
        /// Returns 1 if the gridpoint has fixed pore-pressure, otherwise value 0 indicates non-fixed pore-pressure
        virtual bool                getPPFix() const = 0;
        /// If parameter b is treu, sets the gridpoint pore-pressure to be kept fixed during calculation, see getPPFix().
        virtual void                setPPFix(bool b) = 0;
#ifdef THREED
        /// Returns the gridpoint fluid saturation, see setSat()
        virtual double              getSat() const=0;
        /// Sets the gridpoint fluid saturation, see getSat()
        virtual void                setSat(const double &d) =0;
        /// Returns a const pointer to IThermGp
        virtual const IThermGp  *   getIThermGp() const =0;
        /// Returns a pointer to IThermGp
        virtual IThermGp  *         getIThermGp()  =0;
#endif
        /// Returns 1 if the gridpoint thermal FIX flag is set, otherwise value 0 indicates that the thermal fix flag is not set
        virtual bool                getTFix() const =0;
        /// If parameter \b is true, sets the thermal FIX flag on.
        virtual void                setTFix(bool b) =0;
        /// Returns a vector indicating the fix condition for the gridpoint. 
        /// The component x, y or z of the vector are set to 1 (fixed) or 0(not fixed). See setFix()
        virtual IVect               getFix() const =0;
        /// Sets the fix condition for the gridpoint. 
        /// The component x, y or z of the vector are set to 1 (fixed) or 0(not fixed). See getFix()
        virtual void                setFix(IVect v)  =0;
        /// Returns the static applied force. This load is in addition to forces applyed to the gridpoint 
        /// using the APPLY command. See setFApplyUser()
        virtual DVect               getFApplyUser(bool local=false) const=0;
        /// Returns the additional applied mass set through FISH function gp_massadd.
        /// This mass is added to the gridpoint inertial mass used in the EOMs in static or dynamic mode.
        virtual DVect               getMassAdd(bool local=false) const=0;
        /// Sets the additional applied mass at a gridpoint.
        virtual void                setMassAdd(const DVect &v,bool local=false) =0;    /// Sets the static applied force. This load is in addition to forces applyed to the gridpoint 
        /// using the APPLY command. See getFApplyUser()
        virtual void                setFApplyUser(const DVect &d,bool local=false) =0;
        virtual bool                getLocalSystem() const=0;
        /// Returns the user-defined local axes
        virtual Axes                getSystem() const=0;
        /// Sets the user-defined local axes
        virtual void                setSystem(const Axes &a) = 0;
        /// Returns the boolean stored in utility bit \a bit in the gridpoint. 
        /// \arg bit must be a bit index previouly returned from IGpList::claimBit().
        virtual bool                getBit(int bit) const=0;
        /// Sets the boolean stored in utility bit \a bit in the gridpoint.
        /// This function is const, to indicate that setting bit values does not impact the calculation
        ///   logic in any way.
        /// \arg bit must be a bit index previouly returned from IGpList::claimBit().
        virtual void                setBit(int bit,bool b) const=0; 
        /// Retrieves the value stored in the utility array at \a index.
        /// \arg index must be a value previously returned from IGpList::claimValue().
        virtual QVariant            getValue(int index) const=0;
        /// Sets the value stored in the utility array at \a index.
        /// This is const to indicate that these bit values to not modify the model in any way.
        /// \arg index must be a value previously returned from IGpList::claimValue().
        virtual void                setValue(int index,const QVariant &v) const=0; 
        /// Returns a pointer to a zone that is connected to this Gp.
        /// getLinkZone() and getLinkIndex() can be used to iterator though all zones connected to a Gp.
        /// See IZone::getLinkZone() and IZone::getLinkIndex().
        /// \code quint32 index = gp->getLinkIndex();
        ///       const IZone *z = gp->getLinkZone();
        ///       while (z) {
        ///         /* Do something with the zone */
        ///         quint32 next = z->getLinkIndex(index);
        ///         z = z->getLinkZone(index);
        ///         index = next;
        ///       }
        /// \endcode
        virtual IZone *             getLinkZone()=0; 
        virtual const IZone *       getLinkZone() const=0;///< \copydoc getLinkZone()
        /// Returns the index in the zone getLinkZone() that should be used to follow the list of
        ///   all zones connected to this gp.
        virtual quint32             getLinkIndex() const=0;
        /// If \a save is true, save the grip point state information to IFish.\n
        /// If \a save is false, restore the grip point state information to IFish.\n
        /// Returns 0 if function is successful, otherwise an error code correspondig to a IFishIO error.\n
        /// See 'Saving State Information' in  FISH in FLAC manual.
        virtual bool                fishIO(bool save,fish::File *f)=0;
        /// Returns the gridpoint stiffness
        virtual double              getStiff() const=0;
        /// Sets the gridpoint stiffness
        //virtual void                setStiff(const double &d) =0;
#ifdef THREED
        /// Gets dDTemp
        virtual double           getTempChange() const=0;
        /// Sets dDTemp
        virtual void             setTempChange(const double &d)=0;
#endif
        virtual double getConvergence() const=0;
        virtual double getRatioTarget() const=0;
        virtual void   setRatioTarget(const double &d)=0;
        virtual bool   getAttached() const=0;
    };
} // namespace zone
// EoF
