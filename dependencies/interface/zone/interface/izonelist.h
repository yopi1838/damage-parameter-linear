//izonelist.h
#pragma once

/**
  * \file
  * \brief Interface for accessing the global list of zones 
  */

#include "base/src/farray.h"
#include "base/src/callback.h"

#include "modelsutil/interface/imodellist.h"
#include "utility/interface/igroup.h"

#include <set>

namespace itasca {
    class IContainer;
    class IThing;
}
namespace itascaxd {
    class IFetchProperty;
}

namespace zone {
    using namespace itasca;

    class IFace;
    class IFetchFluidProperty;
    class IFetchThermalProperty;
    class IZone;
    class IZoneData;

    /// \brief Interface for accessing the global list of zones, IZoneList
    /// \ingroup Zone
    class IZoneList {
    public:
        typedef IModelList::ModelQueryInfo ModelQueryInfo;
        /// Returns a const pointer to an IThing interface representing the IZoneList.
        virtual const IThing *     getIThing() const=0;
        /// Returns a pointer to an IThing interface representing the IZoneList.
        virtual IThing *           getIThing()=0;
        /// Returns a const pointer to the IContainer
        virtual const IContainer * getIContainer() const=0;
        /// Returns a pointer to the IContainer
        virtual IContainer *       getIContainer() =0;
        /// Returns a const pointer to the first zone, IZone, in the list, or 0 if list is empty.
        /** Use in conjunction with IZone::getNext() */
        virtual const IZone *      getFirstZone() const=0;
        /// Returns a pointer to the first zone, IZone, in the list, or 0 if list is empty.
        /** Use in conjunction with IZone::getNext() */
        virtual IZone *            getFirstZone()=0;
        /// Returns const pointer to the first character of the string retaining the type of zone.
        virtual quint32            getZoneType() const=0;
        /// Returns const pointer to the first character of the string retaining the type of face.
        virtual quint32            getFaceType() const=0;
        /// Returns a const pointer to the zone, IZone, with the ID \b id, or 0 if the zone does not exist.
        virtual const IZone *      findZoneWithID(quint64 id) const=0;
        /// Returns a pointer to the zone, IZone, with the ID \b id, or 0 if the zone does not exist.
        virtual IZone *            findZoneWithID(quint64 id)=0;
        /// Returns a const pointer to the zone, IZone, 
        ///   whose centroid is the closest to the location <b> vec </b>, 
        ///   or 0 if the zone does not exist.
        virtual const IZone *      findZoneNearest(const DVect &vec,const double &radius=limits<double>::max()) const=0;
        /// Returns a pointer to the zone, IZone, 
        ///   whose centroid is the closest to the location in <b> vec </b>, 
        ///   or 0 if the zone does not exist.
        virtual IZone *            findZoneNearest(const DVect &vec,const double &radius=limits<double>::max()) =0;
        /// Returns the total number of zone in the model.
        virtual quint64           getNZone() const=0;
        /// Returns a pointer to the zone, IZone, containing the location <b> vec </b> , or 0 if the zone does not exist.
        /// \a bHideMechNull indicates that zones with a null mechanical model should be ignored.
        /// \a bHideFluidNull indicates that zones with a null fluid model should be ignored.
        /// \a bHideThermNull indicates that zones with a null thermal model should be ignored.
        virtual IZone *            findZoneContaining(const DVect &v,bool bHideMechNull=false, bool bHideFluidNull=false, bool bHideThermNull=false) const =0;
        /// Returns a pointer to the zone closest to location \b dv3.
        /** Parameters \b bMech, \b bFluid and \b bTherm are used to returns a zone that has a live mechanical, fluid and\\or thermal model.\n
        Parameter \b bOr affects the interaction of parameters \b bMech, \b bFluid and \b bTherm:
        true = Union, false = Intersection \n
        If the criteria is not met, the function returns 0.
        */
        virtual IZone *            findZoneNearestLive(const DVect &dv3, bool bMech,bool bFluid,bool bTherm,bool bOr) =0;
        /// Returns the tag for the zone extra variable change notice
        virtual uint               getZoneExtraNotice() const=0; 
        /// Returns the tag for the zone model change notice
        virtual uint               getModelNotice() const=0;  
        /// Returns the tag for the zone property change notice
        virtual uint               getPropertyNotice() const=0;
        /// Returns the tag for the zone state change notice
        virtual uint               getStateNotice() const=0;
        /// Returns the tag for the zone density change notice
        virtual uint               getDensityNotice() const=0;
        /// Returns the tag for the zone group change notice
        virtual uint               getGroupNotice() const=0;
        /// Returns the tag for the zone stress state change notice
        virtual uint               getStressNotice() const=0;
        /// Returns the tag for the zone specific discharge change notice
        virtual uint               getSpecificDischargeNotice() const =0;
        /// Returns the tag for the zone thermal flux change notice
        virtual uint               getThermalFluxNotice() const =0;
        /// Returns the tag for the zone group change notice
        virtual uint               getFaceGroupNotice() const=0;
        /// Returns the tag for the zone extra FISH variable change notice
        virtual uint               getFaceExtraNotice() const=0;
        /// Returns the tag for zone hide status
        virtual uint               getHideNotice() const=0;
        /// REturns the tag for zone select status
        virtual uint               getSelectNotice() const=0;
        /// Returns the tag for face hide status
        virtual uint               getFaceHideNotice() const=0;
        /// Returns the tag for face select status
        virtual uint               getFaceSelectNotice() const=0;
        virtual uint               getMultiplierNotice() const=0;
        virtual uint               getFluidDensityNotice() const=0;
        /// Returns the total number of faces (surface or otherwise) that are hidden.
        virtual quint64           getNumberFacesHidden() const=0; 
        /// Returns the total number of faces (surface or otherwise) that are selected.
        virtual quint64           getNumberFacesSelected() const=0;
        /// Returns the total number of faces considered VISIBLE, either by mechanical surface or on visible mechanical zones
        //virtual quint64           getNumberFacesVisible(bool onVisibleZones=false) const=0;
        /// Return all the groups assigned to faces in the specified slot
        virtual std::set<const IGroup *> getFaceGroupsForSlot(const ISlotID &slot) const=0;
        /// It is sometimes useful to be able to store flags and data in a zone
        ///   during calculations unrelated to the actual model cycling logic.\n
        /// Two utility arrays are available for this purpose, they are primarily 
        ///   intended to be briefly claimed, used, and then released for back
        ///   for use in other algorithms.\n
        /// claimBit() returns a bit index that can be used with the IZone::getBit() and
        ///   IZone::setBit() functions. 
        /// This function is thread-safe (IZone::getBit and IZone::setBit are not).
        /// \sa releaseBit() IZone::setBit() IZone::getBit()
        /// \brief Claims a bit from the zones utility bit field.
        virtual int                claimBit() const=0;
        /// \brief Releases a bit index previously obtained with claimBit().  Thread-safe.
        virtual void               releaseBit(int bit) const=0;
        /// It is sometimes useful to be able to store flags and data in a zone
        ///   during calculations unrelated to the actual model cycling logic.\n
        /// Two utility arrays are available for this purpose, they are primarily 
        ///   intended to be briefly claimed, used, and then released for back
        ///   for use in other algorithms.\n
        /// claimValue() returns a value index that can be used with the IZone::getValue() and
        ///   IZone::setValue() functions. 
        /// This function is thread-safe (IZone::getValue and IZone::setValue are not).
        /// \sa releaseBit() IZone::setValue() IZone::getValue()
        /// \brief Claims a bit from the zones utility value array.
        virtual int                claimValue() const=0;
        /// \brief Releases a value index back to the zones utility value array.  Thread-safe.
        virtual void               releaseValue(int bit) const=0;
        /// Returns the pointer to IZoneData. This pointer must be destroyed.
        /// The IZoneData object can be used to retrieve scalar data from a zone.
        virtual IZoneData *        createIZoneData(QObject *parent=0) const=0;
        /// Returns a QStringList containing all unique mechanical constitutive models in all zones (null excluded).
        virtual QStringList        getAllMechanicalModels() const=0;
        /// Returns a QStringList containing all unique property names of all mechanical constitutive models in all zones in the model.
        virtual QStringList        getAllMechanicalProperties() const=0;
        
        // Returns a ModelQueryInfo filled with information based on currently selected zones.  Multithreaded!
        virtual ModelQueryInfo     getModelQueryOnSelected() const=0;

        /// Returns an IFace object representing face \a side of zone \a z.
        /// Created from the heap, must be deleted by the user with the destroy() method.
        virtual const IFace *      makeFace(const IZone *z,quint32 side) const=0;
        /// Returns an IFace object representing face \a side of zone \a z.
        /// Created from the heap, must be deleted by the user with the destroy() method.
        virtual IFace *            makeFace(IZone *z,quint32 side)=0;
        virtual itascaxd::IFetchProperty *makeFetchProperty(const QString &prop) const=0;
        virtual IFetchThermalProperty *makeFetchThermalProperty(const QString &prop) const=0;
        virtual IFetchFluidProperty *makeFetchFluidProperty(const QString &prop) const=0;
        virtual QPair<IZone *,quint32> findIFace(const DVect &base,const DVect &dir,bool useHiddenZones=false,
                                                 bool groupedFacesOnly=false,const ISlotID &slot=ISlotID(),
                                                 bool useHiddenFaces=false)=0;
        virtual QString getZoneGroupsOnSides(std::set<IGroupID> *zf1GroupList,
                                             std::set<IGroupID> *zf2GroupList) const=0;
        
        virtual void threadedCallbackObject(Callback4<void,const IZone *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;
        virtual void threadedCallbackFunction(void (*function)(const IZone *,quint32,quint32,void *),void *v=nullptr,bool useThreads=true) const=0;
        template <class S,void (S::*MFP)(const IZone *,quint32,quint32,void *)>
            void threadedCallbackMethod(S *s,void *v=nullptr,bool useThreads=true) const;

        struct FaceGroupCallbackData {
            const IZone * zone_;
            quint32       side_;
            const IThing *entry_;
        };
        virtual void threadedCallbackObjectFaceGroups(Callback3<void,const FaceGroupCallbackData *,quint32,quint32> &callback,bool useThreads=true) const=0;
        template <class S,void (S::*MFP)(const FaceGroupCallbackData *,quint32,quint32)>
            void threadedCallbackMethodFaceGroups(S *s,bool useThreads=true) const;

        struct FaceSelectCallbackData {
            const IZone * zone_;
            quint32       side_;
        };
        virtual void threadedCallbackObjectFaceSelect(Callback3<void,const FaceSelectCallbackData *,quint32,quint32> &callback,bool useThreads=true) const=0;
        template <class S,void (S::*MFP)(const FaceSelectCallbackData *,quint32,quint32)>
            void threadedCallbackMethodFaceSelect(S *s,bool useThreads=true) const;
    };

    template <class S,void (S::*MFP)(const IZone *,quint32,quint32,void *)>
    void IZoneList::threadedCallbackMethod(S *s,void *v,bool useThreads) const {
        Callback4Method<void,const IZone *,quint32,quint32,void *,S,MFP> callback(s);
        threadedCallbackObject(callback,v,useThreads);
    }

    template <class S,void (S::*MFP)(const IZoneList::FaceGroupCallbackData *,quint32,quint32)>
    void IZoneList::threadedCallbackMethodFaceGroups(S *s,bool useThreads) const {
        Callback3Method<void,const IZoneList::FaceGroupCallbackData *,quint32,quint32,S,MFP> callback(s);
        threadedCallbackObjectFaceGroups(callback,useThreads);
    }

    template <class S,void (S::*MFP)(const IZoneList::FaceSelectCallbackData *,quint32,quint32)>
    void IZoneList::threadedCallbackMethodFaceSelect(S *s,bool useThreads) const {
        Callback3Method<void,const IZoneList::FaceSelectCallbackData *,quint32,quint32,S,MFP> callback(s);
        threadedCallbackObjectFaceSelect(callback,useThreads);
    }
} // namespace zone
// EoF
