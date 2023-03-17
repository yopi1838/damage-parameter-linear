// izone.h
#pragma once

/**
* \file
* \brief Interface to provide access to a zone 
*/

#include "module/interface/dim.h"
#include "utility/interface/igroup.h"
#include "models/src/igenericzone.h"
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IFish;
}
namespace itascaxd {
    class Parse;
    class IRange;
}
namespace models {
    class ConstitutiveModel;
}
namespace fish {
    class File;
    class IParameter;
}
namespace zone {
    using namespace itasca;
    using namespace itascaxd;

    static const quint32 ulNumZoneType  = dimval(2,5);  // number of zone types: 2 in 2D, 5 in 3D
    static const quint32 numTperOv_     = dimval(2,5);  // number of tets/triangles per zone: 2 in 2D, 5 in 3D
    static const quint32 numGPperZ_     = dimval(4,8);  // max number of gridpoints per zone in 2D/3D
    static const quint32 numGPperT_     = dimval(3,4);  // number of gridpoint per triangle (2D) / tetra (3D)
    static const quint32 numAdjZones_   = dimval(4,6);  // max number of adjoint zones to a zone in 2D/3D
    static const quint32 numFacesZ_     = dimval(4,6);  // max number of faces per zones in 2D/3D; in 2D a face is the edge of a quad or triangle
    static const quint32 numGPperFace_  = dimval(2,4);  // max number of gripoints per face in 2D/3D; in 2D a face is the edge of a quad or triangle
    static const quint32 minGPperFace_  = dimval(2,3);  // min number of GP that define face in 2D/3D
    static const quint32 numEdgeZ_      = dimval(4,12); // max number of edges per zone in 2D/3D

    class IGp;
    class IFace;
    class ITet;
    class IHysDamp;
    class IThermZone;
    class IFluidZone;
    class IDynZone;

    /// Struction that can *uniquely* identify this face.
    /// Made up of the ID numbers of the four gridpoints around it, in descending order.
    /// If the face is three sided, then id4_=0.
#ifdef THREED
    struct FaceID {
        quint64 id1_ = 0;
        quint64 id2_ = 0;
        quint64 id3_ = 0;
        quint64 id4_ = 0;
        inline bool operator==(const FaceID &id) const {  return id1_==id.id1_ && id2_==id.id2_ && id3_==id.id3_ && id4_==id.id4_; }
        inline bool operator!=(const FaceID &id) const { return !operator==(id); }
        inline bool operator<(const FaceID &id) const { 
            if (id1_<id.id1_) return true;  if (id1_>id.id1_) return false; 
            if (id2_<id.id2_) return true;  if (id2_>id.id2_) return false;
            if (id3_<id.id3_) return true;  if (id3_>id.id3_) return false;
            if (id4_<id.id4_) return true;  return false;
        }
        inline quint64 &operator[](quint32 index) {
            switch (index) {
            case 0:  return id1_;
            case 1:  return id2_;
            case 2:  return id3_;
            case 3:  return id4_;
            }
            return id1_;
        }
        inline quint64 operator[](quint32 index) const {
            switch (index) {
            case 0:  return id1_;
            case 1:  return id2_;
            case 2:  return id3_;
            case 3:  return id4_;
            }
            return id1_;
        }
    };
#else
    struct FaceID {
        quint64 id1_ = 0;
        quint64 id2_ = 0;
        inline bool operator==(const FaceID &id) const {  return id1_==id.id1_ && id2_==id.id2_; }
        inline bool operator!=(const FaceID &id) const { return !operator==(id); }
        inline bool operator<(const FaceID &id) const { 
            if (id1_<id.id1_) return true;  if (id1_>id.id1_) return false; 
            if (id2_<id.id2_) return true;  return false;
        }
        inline quint64 &operator[](quint32 index) {
            switch (index) {
            case 0:  return id1_;
            case 1:  return id2_;
            }
            return id1_;
        }
        inline quint64 operator[](quint32 index) const {
            switch (index) {
            case 0:  return id1_;
            case 1:  return id2_;
            }
            return id1_;
}
    };
#endif

    /// \brief Interface to provide access to a zone
    /// \ingroup Zone
    class IZone : public models::IGenericZone {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bf8;
        enum class Change { MechNull, AddToList, RemoveFromList, FlNull, ThNull };
        /// Possible Zone types code
        enum class Code { 
#ifdef THREED
            Brick=0,  /*!< Brick shaped zone. */
            Wedge,    /*!< Wedge shaped zone. */
            Pyramid,  /*!< Pyramidal shaped zone. */    
            DBrick,   /*!< Degenerated Brick shaped zone. */
            Tetra     /*!< Tetrahedron shaped zone. */
#else
            Quad = 0, /*!< Quadrilateral shaped zone. */
            Tria     /*!< Triangular zone. */
#endif
        };

        /// returns a const IThing pointer
        virtual const IThing    *   getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing          *   getIThing()=0;
        /// returns a const pointer to the next zone object, IZone, in the global zone list, or 0 to indicate the end of the list.
        /// See IZoneList::getFirstZone().
        virtual const IZone     *   getNext() const=0;
        /// returns a pointer to the next zone object, IZone, in the global zone list, or 0 to indicate the end of the list.
        /// See IZoneList::getFirstZone().
        virtual IZone *             getNext()=0;
        /// Returns the number of gridpoints used by the zone.
        virtual quint32             getNumGp() const=0;
        /// Returns a const pointer to the gridpoint with index \b index.
        /// \param index starts at 0, and must be lower than the number of gridpoint used by the zone, see getNumGp()
        virtual const IGp       *   getGp(quint32 index) const=0;
        /// Returns the number of gridpoints used by the face of index \b face.
        /// \param face starts at 0, and must be lower than 6.
        virtual quint32             getFaceSize(quint32 face) const=0;
        /// Returns a const pointer to the gridpoint on face \b face and vertex \b index.
        /// \param face starts at 0, and must be lower than 6.
        /// \param index starts at 0, and must be lower than 4.
        virtual const IGp       *   getFaceGp(quint32 face,quint32 index) const=0;
        /// Returns the index of the gridpoint on face \a face and vertex \a index.
        /// \param face starts at 0, and must be lower than 6.
        /// \param index starts at 0, and must be lower than 4.
        virtual quint32             getFaceGpIndex(quint32 face,quint32 index) const=0;
        /// Returns the index of the gridpoint on edge \a edge and vertex \a index.
        /// \param edge starts at 0, and must be lower than 12.
        /// \param index starts at 0, and must be lower than 2.
        virtual quint32             getEdgeGpIndex(quint32 edge, quint32 index) const=0;

        virtual FaceID              getFaceID(quint32 side) const=0;

        ///// Returns the index of the face on edge \a edge and index \a index.
        ///// \param edge starts at 0, and must be lower than 12.
        ///// \param index starts at 0, and must be lower than 2.
        //virtual quint32             getFaceOnEdge(quint32 edge, quint32 index) const=0;

        /// Returns the unit normal vector to face \b face.
        /// \param face starts at 0, and must be lower than 6.
        virtual DVect               getFaceNormal(quint32 face) const=0;
        /// Returns the location of the centroid of face \b face in the global coordinate system.
        /// \param face starts at 0, and must be lower than 6.
        virtual DVect               getFaceCentroid(quint32 face) const=0;
        /// Returns a const pointer to the zone, IZone, across face \b face.
        /// \param face starts at 0, and must be lower than 6.
        virtual const IZone     *   getJoin(quint32 face) const=0;
        // Face Group support
        /// Given a face \b side and a group \b slot, returns a const pointer to an IGroup.
        virtual const IGroup *      getFaceGroup(quint32 side,const ISlotID &slot=ISlotID((uint)0)) const=0;
        /// Given a face \b side and a group \b slot, returns the group name. If slot is not given  
        /// (or set to limits<uint>::max()), the group name will contain the name of all groups that
        /// this face is associated with, separated by character ':'.
        virtual QString             getFaceGroupName(quint32 side,const ISlotID &slot=ISlotID()) const=0;
        /// Associates an IGroup to face \b side in a given \b slot. If \b slot is set to limits<uint>::max(), 
        /// program will use the first available slot.
        virtual bool                addFaceGroup(quint32 side,const IGroupID &group)=0;
        /// Removes the association of a given \b group with face \b side
        virtual bool                removeFaceGroup(quint32 side,const IGroupID &group)=0;
        /// Returns true is face \b side us associated with a given \b group in a given \b slot. If the slot is set to 
        /// limits<uint>::max(), returns true the association occurs in any slot for that group.
        virtual uint                isFaceInGroup(quint32 side,const FArray<IGroupID> &ids,TType type=0,bool only=false) const=0;
        /// Return all groups and all slots assigned to the face in a list.
        virtual uint                getFaceGroupList(quint32 side,FArray<IGroupID> *list) const=0;
        /// Copy the groups in IThing object to face /side
        virtual void                copyFaceGroups(quint32 side,const IThing *t)=0;
        /// Face Extra Variable support
        /// Returns the number of extra FISH variables associated with this face \b side
        virtual uint                    getFaceExtraSize(quint32 side) const=0;
        /// Given an \b index, returns a const pointer to IFishParam associated with this face \b side.
        virtual const fish::IParameter *getFaceExtra(quint32 side,uint index) const=0;
        /// Given an \b index, returns a pointer to IFishParam associated with this face \b side. Throws an exception
        /// if \b index is greater than 128. Function will automaticlly resize the array of FISH varaiables if necessary.
        virtual void                    setFaceExtra(quint32 side,uint index,const fish::IParameter &p)=0;
        /// Returns TRUE if that face of the zone is marked as hidden.
        virtual bool                getFaceHidden(quint32 side) const=0;
        /// Sets the hidden condition of that face of the zone, returns true if the condition changed.
        virtual bool                setFaceHidden(quint32 side,bool b)=0;
        /// Returns TRUE if that face of the zone is marked as selected.
        virtual bool                getFaceSelected(quint32 side) const=0;
        /// sets the selected condition of that face of the zone, returns true if the condition chagned.
        virtual bool                setFaceSelected(quint32 side,bool b)=0;
        /// Returns TRUE if the FACE is considered in the range.  Creates a temp Face class.
        virtual bool                isFaceInRange(quint32 side,const IRange *range) const=0;
        /// Returns the zone density.
        virtual double              getDensity() const=0;
        /// Sets the zone density.
        virtual void                setDensity(const double &d)=0;
        /// Returns \b true if the Mechanical model in the zone is null, returns \ b false otherwise.
        virtual bool                isMechanicalNull() const=0;
        /// Returns \b true if the Thermal model in the zone is null, returns \ b false otherwise.
        virtual bool                isThermalNull() const = 0;
        /// Returns \b true if the Fluid model in the zone is null, returns \ b false otherwise.
        virtual bool                isFluidNull() const = 0;
        /// Returns the Code corresponding the the Zone type, see the Code enumeration
        virtual Code                getCode() const=0;
        /// Test the aspect ratio of the Zone and returns a double between 0 (if the zone has a bad geometry) and 1 (if the geometry is good).\n
        /// A zone is considered to have a bad geometry if it contains negative or zero volume tetrahedra in its overlays.
        virtual double              getAspectTest() const=0;
        /// Tests the aspect ratio of the Zones internal tetrahedra, and returns the minimum value obtained for all.
        /// Returns a double between 0 (bad geometry) and 1 (perfect possible).
        virtual double              getAspectTetTest() const=0;
        /// Copies the state information from zone \b zone to the current zone.
        virtual void                copyState(const IZone *zone)=0;
#ifdef THREED
        /// Returns the name of the fluid flow model used by the Zone, or "undefined" if the zone has not been configured for fluid flow analysis.
        virtual QString             getFlowModel() const=0;
        /// Returns the thermal flux vector in the Zone, or a null vector if the zone has not been configured for thermal analysis.
        virtual DVect               getThermalFlux() const=0; // jkf: should be const
#endif
        /// Returns the face index corresponding to any three (in 3D) or 2 (in 2D) gridpoint ids on a zone face, return the face index. \n
        /// Returns -1 if the face is not found.
        virtual int                 getFace(const std::array<quint64,minGPperFace_> &uid) const=0;
        /// Caculates the outward facing face normal and the face area, and stores the values in \b dvNorm and \b dArea.
        /// Returns false if this face has less than 3 gridpoints on it or if it does not exist.
        virtual bool                getFaceNormalArea(int iFace,DVect *dvNorm,double *dArea) const=0;
        /// This functions computes and returns zone deformation quantities.
        /// \arg imode is an integer ranging between 1 and 8. \n
        /// If \a imode is lower or equal to 4, then the returned value corresponds to: \n
        ///    \arg imode=1: shear strain rate, 
        ///    \arg imode=2: shear strain increment,
        ///    \arg imode=3: volumetric strain rate,
        ///    \arg imode=4: volumetric strain increment. \n
        /// If \a imode is greater than 4, then the returned value is null, and the result of the compuatation is stored in the tensor \a dpFSR, with: \n
        ///    \arg imode=5: full strain-rate tensor,
        ///    \arg imode=6: full strain-increment tensor,
        ///    \arg imode=7: full rotation-rate tensor,
        ///    \arg imode=8: full rotation-increment tensor.
        enum class StrainMode { ShearRate=1, ShearInc=2, VolRate=3, VolInc=4, FullRate=5, FullInc=6, FullRotRate=7, FullRotInc=8 };
        virtual double              getSSR(StrainMode mode,SymTensor *fsr) const=0;
        virtual SymTensor           getTetStrainRate(int overlay,int number) const=0;
        virtual SymTensor           getTetStrainIncrement(int overlay,int number) const=0;
        virtual SymTensor           getStrainRate() const=0;
        virtual SymTensor           getStrainIncrement() const=0;
        /// Returns \b true if the zone is degenerate. Additionnal information are stored in \b sErr.
        virtual bool                getIsDegenerate(QString &sErr) const =0;
        /// Returns \b true if the Zone supports two overlays.
        virtual bool                getSupports2Overlays(QString &sErr) =0;
        /// Returns a const pointer to the head of the list of tetrahedron, Itet, comprising the first overlay.
        virtual const ITet       *  getOverlay(quint32 ov) const=0;
        /// Returns a pointer to the head of the list of tetrahedron, Itet, comprising the first overlay.
        virtual ITet             *  getOverlay(quint32 ov)=0;
#ifdef THREED
        /// Returns a const pointer to the Hysteretic Damping model, IHysDamp, or \b null if the zone does not support hysteretic damping.
        virtual const IHysDamp   *  getHysDamp() const=0;
        /// Returns a const pointer to the Hysteretic Damping model, IHysDamp, or \b null if the zone does not support hysteretic damping.
        virtual IHysDamp         *  getHysDamp() =0;
        virtual IHysDamp         *  setHysDamp() =0;

#endif
        /// Initialize all active models (Fluid, Thermal and mechanical). Returns \b 0 upon completion.  
        virtual int                 initializeModels()=0; 
        /// Returns true if the zone contains a non-null \b mechanical constitutive model. 
        virtual bool                getHasALiveMechModel() const=0;
        /// Returns true if the zone contains a non-null \b fluid model. 
        virtual bool                getHasALiveFluidModel() const=0;
        /// Returns true if the zone contains a non-null \b thermal model. 
        virtual bool                getHasALiveThermModel() const=0;
        /// Returns the name of the mechanical constitutive model present in the zone. 
        virtual QString             getModelName() const=0;
        virtual const models::ConstitutiveModel *getModel() const=0;
        /// Load the constitutive model corresponding to name \b qs.
        /// \param qs is the name of the model to be loaded. An exception is thrown if this name is not recognized. 
        virtual void                setModelName(const QString &qs) =0;
        /// Return the volume of the smallest tetrahedron in the overlays, normalized to the zone volume.
        virtual double              getSmallestNormalizedTetVolume() const=0;
#ifdef THREED
        /// Performs a geometrical quality measurement on the zone, and returns a value between 0 (poor quality) and 1 (good quality)
        /// \param iTestNum must be either 1 or 2.
        ///   \arg iTestNum=1: Returns the minimal tet quality, defined as 72*sqrt(3)*volume_of_tet / (sum of squares of edges)^1.5 \n
        ///   \arg iTestNum=2: Returns the minimal normalized measure of skewness quality.
        virtual double              getQualityTest(int iTestNum) const=0;
        /// Performs a test on zone faces planarity
        virtual double              getPlanarity() const=0;
#endif
        /// Returns the number of tetrahedrons in the zone.
        virtual int                 getNumTets() const =0;
        /// Performs an orthogonality test on the zone.
        virtual double              getOrthoTest() const=0;
        /// Performs an volume test on the zone.
        virtual double              getVolumeTest() const=0;
        /// Returns the condition of zone geometry.
        virtual double              getZoneCondition(int i=0) const=0;
#ifdef TWOD
        /// Checks for bad zone as is done in the extruder. True means the zone is bad
        virtual bool                getBadZoneExtruder() const=0;
#endif
        /// Retrieves the average of the zone gridpoints pore pressures 
        virtual double              getAvePP() const=0;
        /// Returns the value of the zone MECHANICAL property corresponding to \b sc. \n
        /// An exception is thrown if \b sc is not recognized.
        virtual QVariant            getProperty(const QString &sc) const =0;
        virtual QVariant            getProperty(uint index) const=0;
        /// Sets the value \b v to zone MECHANICAL property \b sc. \n
        /// Returns \b true if the value was assigned, or \b false if \b sc is not recognized
        virtual bool                setProperty(const QString &sc,const QVariant &v) =0;
        virtual bool                setProperty(int index,const QVariant &v) =0;
        /// Calculates and returns the current volume of the zone.\n
        /// If \a deformed is true, then includes the current displacement field in the calculation.
        virtual double              getVolumeD(bool deformed) const=0;
        /// Returns the location of the centroid of the zone. 
        virtual DVect               getCentroid() const=0;
        /// Computes the average of the overlays tetrahedron stress tensors. 
        virtual SymTensor           getAveStress(bool effective=false) const=0;
#ifdef THREED
        /// Retrieves the average of the zone gridpoints temperatures. 
        virtual double              getAvgTemp() const=0;
        /// Returns the flow vector.
        virtual DVect               getFlowVector() const=0; 
#endif
        /// If \a bAvgKeyword is true, then sets a bit if the more than 50% of the volume of the
        ///   internal tetrahedra have the bit set.\n
        /// If false, then sets the bit if *any* of the internal tetrahdra have the bit set.
        /// \arg piNum, if non-null, is filled with the total number of states available.
        /// \brief Returns a mask filled with a state indicator for the zone.
        virtual uint                getAvgState(int *piNum, bool bAvgKeyword) const=0;
        /// Returns the name of the state associated with bit \a bit in the current constitutive model,
        ///  or QString() if none.
        virtual QString             getStateName(quint32 bit) const=0;
        /// assign the value \a val to the component \a pn of every overlay tetrahedron.\n
        /// \arg pn indicates the stress tensor component as follows.\n
        /// \arg pn=0: XX-component.
        /// \arg pn=1: YY-component.
        /// \arg pn=2: ZZ-component.
        /// \arg pn=3: XY-component.
        /// \arg pn=4: YZ-component.
        /// \arg pn=5: XZ-component.
        virtual void                setStress(const double &val,quint32 pn)=0;
        /// Assign the stress state in sym to the zone, overwriting every overlay tetrahedron.
        virtual void                setStress(const SymTensor &sym)=0;
        /// Assigns the value \a state to the state fields of ALL internal tetraha.
        /// Note that this assigns all bit values at once.
        virtual void                setState(quint32 state)=0;
        /// Retrieves the gridpoints associated with internal tetra \a iTet of overlay \a iOverlay (0 to 1).
        /// The gridpoint pointers are stored in the array \a gplist.
        /// \note No error checking is done on the values of \a iOverlay or \a iTet.  
        ///       Passing in illegal values will result in undefined behavior.
        virtual void                getTetGridPoints(int iOverlay, int iTet, IGp *gplist[]) =0;
#ifdef THREED
        /// returns a constant pointer to the zone thermal extension data
        virtual const IThermZone  * getIThermZone() const=0;
        /// returns a pointer to the zone thermal extension data
        virtual IThermZone  *       getIThermZone() =0;
#endif
        /// returns a list of all the valid properties of the current constitutive model assigned to this zone
        virtual QStringList         getPropertyNames() const=0;
        /// Given a property name, returns the index of the property of the constitutive model assigned to this zone.
        /// \note Base 1, returns 0 if name is not found.
        virtual quint32             getPropertyIndex(const QString &sc) const=0; 
        /// Given an index, returns the name of the property of the constitutive model assigned to this zone.
        /// \note Base 1, returns QString() if \a index \a is less then 1 or greater than the number of properties.
        virtual QString             getPropertyName(quint32 index) const=0;
        /// Returns the boolean stored in utility bit \a bit in the zone. 
        /// \arg bit must be a bit index previouly returned from IZoneList::claimBit().
        virtual bool                getBit(int bit) const=0;
        /// Sets the boolean stored in utility bit \a bit in the zone.
        /// This function is const, to indicate that setting bit values does not impact the calculation
        ///   logic in any way.
        /// \arg bit must be a bit index previouly returned from IZoneList::claimBit().
        virtual void                setBit(int bit,bool b) const=0; 
        /// Retrieves the value stored in the utility array at \a index.
        /// \arg index must be a value previously returned from IZoneList::claimValue().
        virtual QVariant            getValue(int index) const=0;
        /// Sets the value stored in the utility array at \a index.
        /// This is const to indicate that these bit values to not modify the model in any way.
        /// \arg index must be a value previously returned from IZoneList::claimValue().
        virtual void                setValue(int index,const QVariant &v) const=0;
#ifdef THREED
        /// Returns a const pointer to the interface to access fluid zone data
        virtual const IFluidZone *  getIFluidZone() const=0;
        /// Returns a pointer to the interface to access fluid zone data
        virtual IFluidZone *        getIFluidZone() =0;
        /// Returns a const pointer to the interface to access dynamic zone data
        virtual const IDynZone  *   getIDynZone() const=0;
        /// Returns a pointer to the interface to access dynamic zone data
        virtual IDynZone  *         getIDynZone() =0;
#endif
        /// Returns a pointer to the next zone connected to the gridpoint at index \a index, or 0 if at the end of the list.
        /// \sa IGp::getLinkZone().
        virtual IZone *             getLinkZone(quint32 index)=0;
        virtual const IZone *       getLinkZone(quint32 index) const=0; ///< \copydoc getLinkZone()
        /// Returns an the index of the gridpoint at index \a index, in the zone returned from getLinkZone().
        /// Will return 0 if getLinkZone() returns null.
        /// \sa IGp::getLinkZone().
        virtual quint32             getLinkIndex(quint32 index) const=0;

        /// If \a save is true, save the zone information to IFish.\n
        /// If \a save is false, restore the zoneinformation to IFish.\n
        /// Returns 0 if function is successful, otherwise an error code correspondig to a IFishIO error.\n
        /// See 'Saving State Information' in  FISH in FLAC manual.
        virtual bool                fishIO(bool save,fish::File *pnt)=0;
        /// Returns the Stress to Strength ratio based on the current constitutive model properties and stress state \b s.\n
        /// The Stress to Strength ratio is greater than or equal to 1.0. A value of 1.0 means that the zone is failing 
        /// If the zone does not currently have any assigned constitutive model, or if the loaded constitutive model does not support 
        ///   this operation, then a default value of 10 is returned.
        virtual double              getStressStrengthRatio(const SymTensor &s) const=0; 
        /// Applies a boundary condition to face \a face of this zone.
        /// \arg item must be "sxx", "syy", "szz", "sxy", "syz", "sxz", "dstress", "sstress", or "nstress".
        /// \arg options can be "add", "mul", or "remove", otherwise the value is assigned.
        /// \arg val is the value of that item applies.  It is ignored if options is "remove".
        //virtual QString             applyBoundaryCondition(quint32 face,QString item,QString options,const double &val)=0;

        /// Returns the total volumetric plastic work dissipated, see SET ENERGY command
        virtual double            getWPVol() const =0;
        /// Sets the total volumetric plastic work dissipated, see SET ENERGY command
        virtual void              setWPVol(const double &dwpv)=0;
        /// Returns the total shear plastic work dissipated, see SET ENERGY command
        virtual double            getWPShear() const=0;
        /// Sets the total shear plastic work dissipated, see SET ENERGY command
        virtual void              setWPShear(const double &dwps)=0;
        /// Returns the total shear plastic work dissipated (sum of getWPVol() and getWPShear()), see SET ENERGY command
        virtual double            getWPTotal() const=0;
        /// Returns the total volumetric elastic work, see SET ENERGY command
        virtual double            getWEVol() const =0;
        /// Sets the total volumetric elastic work, see SET ENERGY command
        virtual void              setWEVol(const double &dwpv)=0;
        /// Returns the total shear elastic work, see SET ENERGY command
        virtual double            getWEShear() const=0;
        /// Sets the total shear elastic work, see SET ENERGY command
        virtual void              setWEShear(const double &dwps)=0;
        /// Returns the total elastic work (sum of getWEVol() and getWEShear()), see SET ENERGY command
        virtual double            getWETotal() const=0;
#ifdef THREED
        /// Given a valid fluid model name (fl_null, fl_isotropic or fl_anisotropic) applies the
        /// model to the zone. Throws Exception if name is invalid.
        virtual void              setFluidModel(const QString &name)=0;
        /// Returns the name of the zone fluid model
        virtual QString           getFluidModel(bool exception=true)const=0; 
        /// Returns the list of properties for the fluid model
        virtual QStringList       getFluidProperties()const=0; 
        /// Given a fluid property name, returns its value;
        virtual double            getFluidProp(const QString &name,bool exception=true) const =0;
        virtual int               getFluidPropIndex(const QString &name) const=0;
        virtual double            getFluidProp(uint index) const=0;
        /// Sets the values of a given a fluid property name
        virtual void              setFluidProp(const QString &name, const double &val)=0;
        virtual void              setFluidProp(int index, const double &val)=0;
        /// Given a valid thermal model name (th_null, th_isotropic, th_anisotropic, th_ac or th_hydration) applies the
        /// model to the zone. Throws Exception if namei s invalid.
        virtual void              setThermModel(const QString &name)=0;
        /// Returns the list of properties for the thermal model
        virtual QStringList       getThermProperties() const=0; 
        /// Returns the name of the zone thermal model
        virtual QString           getThermModel(bool exception=true)const=0; 
        /// Given a thermal property name, returns its value;
        virtual double            getThermProp(const QString &name,bool exception=true) const =0;
        virtual int               getThermPropIndex(const QString &name) const=0;
        virtual double            getThermProp(uint index) const=0;
        /// Sets the values of a given a thermal property name
        virtual void              setThermProp(const QString &name, const double &val)=0;
        virtual void              setThermProp(int index, const double &val)=0;
#endif
        /// Function returns TRUE if the zone is considered "invalid" by being null.
        /// By default, function only checks for mechanical null, but it can check for fluid and thermal as well.
        /// If \b or is false, then all conditions need to be true for the zone to be considered invalid.
        ///   (in other words invalidZone(true,true,false,false) will return true only if
        ///    the zone is both mechanically null *and* has a null fluid model).
        /// If \b or is true then it is considered invalid if any of the conditions are true.
        virtual bool              invalidZone(bool mech=true,bool therm=false,bool fluid=false,bool orVal=false) const=0;
        /// Returns TRUE if the point \a dv falls inside this zone, within a relative tolerance of \a relTol.
        /// Implemented by checking the volume owned by the internal tetra overlays.
        /// If \a use2 is set to true, then BOTH internal overlays are checked, otherwise only the first overlay is checked.
        /// \a relTol is a tolerance factor used in the "inside" check.  
        /// Multiplied by maximum zone extent edge size to get absolute tolerance.
        /// Uses internal tetra, and by default uses one of the two tetra overlays.  
        /// If \a useTwo is true, checks both overlays.
        virtual bool              isInZn(const DVect &dv,const double &relTol=1.0e-5, bool use2=false) const=0;  // use same default epsilon factor as ZoneList::zoneIn function
        virtual bool              checkZoneForPlot(bool removeHidden,bool showSelected,bool hideMech,bool hideTherm,bool hideFluid) const=0;

        struct FaceCheckInput { 
            quint32 side_ = 0; 
            bool    internalFaces_ = false; // Show ALL faces, not just surface faces.
            int     bit_ = 0; // Used to check if a zone is in or out of the range of zones being considered (only if internalFace_ is false).
            bool    zoneFace_ = false; // If true, then being used for plot of faces (as opposed to a plot of zones).
            bool    onlySelectedFaces_ = false; // Only show selected faces (ignored if zoneFace_ is false).
        };
        struct FaceCheckOutput { // Filled out only if return value is true.
            bool          selected_ = false; // True if zone or face is selected (based on zoneFace_ flag).
            quint64       id_ = 0; // ID number of zones
            FArray<DVect> vertices_; // Vertices of zone face.

        };
        virtual bool   checkFaceForPlot(const FaceCheckInput &inp,FaceCheckOutput *outp) const=0;
        virtual bool   checkFaceForPlotFace(quint32 side,quint64 *id,FArray<DVect> *vertices,bool excludeSelected=true) const=0;
        virtual bool   checkFaceForPlotFaceSelected(quint32 side,quint64 *id,FArray<DVect> *vertices) const=0;
        virtual void   fillZoneVertices(FArray<DVect> *vertices) const=0;
        virtual void   fillAllZoneFaces(FArray<FArray<DVect>> *allVertices,bool *selected,quint64 *id) const=0;
        virtual int    findFaceNormal(const DVect &v) const=0;
        virtual double getMultiplier() const=0;
        virtual bool   CMSupportsHysDamp()const {return false;} ;
    };
} // namespace zone
// EoF
