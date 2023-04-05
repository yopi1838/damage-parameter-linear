#pragma once
/** \file imoduleclump.h
    * \brief Interface for the Clump module.
    *
    * \defgroup clumpsrc Clump module interface specification
    * \ingroup pfcmodulesrc 
    */
#include "utility/interface/isignalbase.h"
#include "base/src/farray.h"
#include "base/src/callback.h"

namespace itasca {
    class IContainer;
    class IEnergyMap;
}

namespace itascaxd {
    class IContact;
    class ICellSpace;
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IClump;
    class IPebble;
    class IGhostPebble;
    class IClumpTemplate;

    /** \brief Interface for the Clump module.
        * This is the interface for the Clump module. 
        * \ingroup clumpsrc
        */
    class IModuleClump {
    public:
        /// Signal emitted when a property is created or modified.
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        /// Clump TType for type casting.
        virtual TType                    getClumpType() const=0;
        /// Pebble TType for type casting.
        virtual TType                    getPebbleType() const=0;
        /// ClumpTemplate TType for type casting.
        virtual TType                    getClumpTemplateType() const=0;
        /// GhostPebble TType for type casting.
        virtual TType                    getGhostPebbleType() const=0;
        /// Clump geometry notice unsigned integer
        virtual uint                     getClumpGeometryNotice() const=0;
        /// Clump property notice unsigned integer
        virtual uint                     getClumpPropertyNotice() const=0;
        /// Clump velocity notice unsigned integer
        virtual uint                     getClumpVelocityNotice() const=0;
        /// Clump cycle notice unsigned integer
        virtual uint                     getClumpCycleNotice() const=0;
        /// Clump extra notice unsigned integer
        virtual uint                     getClumpExtraNotice() const=0;
        /// Clump group notice unsigned integer
        virtual uint                     getClumpGroupNotice() const=0;
        /// Clump clean notice unsigned integer
        virtual uint                     getClumpCleanNotice() const=0;
        /// Clump rotation notice unsigned integer
        virtual uint                     getClumpRotationNotice() const=0;
        /// Pebble extra velocity notice unsigned integer
        virtual uint                     getPebbleExtraNotice() const=0;
        /// Pebble group notice unsigned integer
        virtual uint                     getPebbleGroupNotice() const=0;

        /// const access to the Clump container
        virtual const IContainer       * getIContainerClump() const=0;
        /// access to the Clump container
        virtual IContainer *             getIContainerClump()=0;
        /// const access to the ClumpTemplate container
        virtual const IContainer       * getIContainerClumpTemplate() const=0;
        /// access to the ClumpTemplate container
        virtual IContainer *             getIContainerClumpTemplate()=0;
        virtual const IContainer       * getIContainerClumpTemplateClumps() const=0;
        virtual IContainer *             getIContainerClumpTemplateClumps()=0;
        virtual const IContainer       * getIContainerClumpTemplatePebbles() const=0;
        virtual IContainer *             getIContainerClumpTemplatePebbles()=0;
        virtual const IContainer       * getIContainerPebble() const=0;
        virtual IContainer *             getIContainerPebble()=0;
        virtual const IContainer       * getIContainerBasePebble() const=0;
        virtual IContainer *             getIContainerBasePebble()=0;
        virtual const IContainer       * getIContainerGhostPebble() const=0;
        virtual IContainer *             getIContainerGhostPebble()=0;
        virtual const IContainerContact *getIContainerPebblePebbleContact() const=0;
        virtual IContainerContact *      getIContainerPebblePebbleContact()=0;
        virtual const IContainerContact *getIContainerDomainPebbleContact() const=0;
        virtual IContainerContact *      getIContainerDomainPebbleContact()=0;
        virtual const ICellSpace       * getICellSpacePebble() const=0;
        virtual ICellSpace *             getICellSpacePebble()=0;
        virtual const IClump *           findClumpWithID(quint64 id) const=0;
        virtual IClump *                 findClumpWithID(quint64 id) =0;
        virtual const IClump *           findClumpClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IClump *                 findClumpClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual const IClump *           findClumpContaining(const DVect &v,QSet<quint64> *skip=nullptr) const=0;
        virtual IClump *                 findClumpContaining(const DVect &v,QSet<quint64> *skip=nullptr)=0;
        virtual const IPebble *          findPebbleWithID(quint64 id) const=0;
        virtual IPebble *                findPebbleWithID(quint64 id) =0;
        virtual const IPebble *          findPebbleClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IPebble *                findPebbleClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual const IPebble *          findPebbleContaining(const DVect &v,QSet<quint64> *skip=nullptr) const=0;
        virtual IPebble *                findPebbleContaining(const DVect &v,QSet<quint64> *skip=nullptr)=0;
        virtual const IClumpTemplate *   findClumpTemplateWithID(quint64 id) const=0;
        virtual IClumpTemplate *         findClumpTemplateWithID(quint64 id) =0;
        virtual const IClumpTemplate *   findClumpTemplateWithName(const QString &) const=0;
        virtual IClumpTemplate *         findClumpTemplateWithName(const QString &) =0;
        virtual const IPebble *          findClumpTemplatePebbleWithID(quint64 id) const=0;
        virtual IPebble *                findClumpTemplatePebbleWithID(quint64 id) =0;
        virtual const IGhostPebble *     findGhostPebbleWithID(quint64 id) const=0;
        virtual IGhostPebble *           findGhostPebbleWithID(quint64 id) =0;
        virtual void                     validate()=0;
        virtual const IEnergyMap       * getIClumpEnergies() const=0;
        virtual const IEnergyMap       * getIContactPebblePebbleEnergies() const=0;

        /// Return the clumps in a box
        virtual void getClumpInBoxList(const DExtent &d,FArray<IClump *> *ret,bool intersect=true)=0;
        virtual void getClumpInBoxList(const DExtent &d,FArray<const IClump *> *ret,bool intersect=true)=0;

        /// Return the pebbles in a box
        virtual void getPebbleInBoxList(const DExtent &d,FArray<IPebble *> *ret,bool intersect=true)=0;
        virtual void getPebbleInBoxList(const DExtent &d,FArray<const IPebble *> *ret,bool intersect=true)=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        /// Populates sets of piece property names based on their type
        virtual void  populatePieceAttributeSets(QStringList *valueAtts,QStringList *labelAtts,QStringList *vectorAtts)const=0;
        /// Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

        /// Create a clump template from a clump, add it to the clump template container, and 
        /// return a pointer
        virtual IClumpTemplate * createFromClump(const IClump &c,const QString &name)=0;

        /// Object for creation of multithreaded loop over clumps
        virtual void threadedCallbackObjectClump(Callback4<void,IClump *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectClump  
        template <class S,void (S::*MFP)(IClump *,quint32,quint32,void *)>
            void threadedCallbackMethodClump(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IClump *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObjectClump(callback,v,useThreads);
            }

        /// Object for creation of multithreaded loop over pebbles
        virtual void threadedCallbackObjectPebble(Callback4<void,IPebble *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectPebble  
        template <class S,void (S::*MFP)(IPebble *,quint32,quint32,void *)>
            void threadedCallbackMethodPebble(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IPebble *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObjectPebble(callback,v,useThreads);
            }

    };
} // namespace pfc
// EoF
