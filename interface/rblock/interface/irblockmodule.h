#pragma once
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"
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

namespace rblock {
    using namespace itasca;
    using namespace itascaxd;

    class IRBlock;
    class IGhostRBlock;
    class IRBlockTemplate;

    class IRBlockModule {
    public:
        /// Typedef for the small granularity signal used to indicate to the gui that a propery has been added.
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        /// Return the RBlock TType.
        virtual TType                   getRBlockType() const=0;
        /// Get the RBlock template TType.
        virtual TType                   getRBlockTemplateType() const=0;
        /// Get the GhostRBlock TType.
        virtual TType                   getGhostRBlockType() const=0;
        /// Return the RBlock geometry notice.
        virtual uint                    getRBlockGeometryNotice() const=0;
        /// Return the RBlock hide notice.
        virtual uint                    getRBlockHideNotice() const=0;
        /// Return the RBlock select notice.
        virtual uint                    getRBlockSelectNotice() const=0;
        /// Return the RBlock cycle notice.
        virtual uint                    getRBlockCycleNotice() const=0;
        /// Return the RBlock fixity notice.
        virtual uint                     getRBlockFixNotice() const=0;
        /// Return the RBlock property notice.
        virtual uint                     getRBlockPropertyNotice() const=0;
        /// Return the RBlock extra notice.
        virtual uint                     getRBlockExtraNotice() const=0;
        /// Return the RBlock group notice.
        virtual uint                     getRBlockGroupNotice() const=0;
        /// Return the const IContainer of the RBlocks.
        virtual const IContainer       * getIContainerRBlock() const=0;
        /// Return the IContainer of the RBlocks.
        virtual IContainer *             getIContainerRBlock()=0; 
        /// Return the const IContainer of the RBlock templates.
        virtual const IContainer       * getIContainerRBlockTemplate() const=0;
        /// Return the IContainer of the RBlock templates.
        virtual IContainer *             getIContainerRBlockTemplate()=0; 
        /// Return the const IContainer of the Ghost RBlocks.
        virtual const IContainer *      getIContainerGhostRBlock() const=0;
        /// Return the IContainer of the Ghost RBlocks.
        virtual IContainer *            getIContainerGhostRBlock()=0;
        /// Return the IContainerContact of RBlock-RBlock contacts.
        virtual const IContainerContact * getIContainerRBlockRBlock() const=0;
        /// Return the IContainerContact of RBlock-RBlock contacts.
        virtual IContainerContact *       getIContainerRBlockRBlock()=0;

        /// Return a const ICellSpace pointer to the RBlock cell space
        virtual const ICellSpace       * getRBlockCellSpace() const=0;
        /// Return an ICellSpace pointer to the RBlock cell space
        virtual ICellSpace *             getRBlockCellSpace()=0;
        /// Find the const IRBlock with ID \a id;
        virtual const IRBlock *            findIRBlockWithID(quint64 id) const=0;
        /// Find the IRBlock with ID \a id.
        virtual IRBlock *                  findIRBlockWithID(quint64 id) =0;
        /// Find the const IRBlock with ID \a id that is a template;
        virtual const IRBlockTemplate *    findIRBlockTemplateWithID(quint64 id) const=0;
        /// Find the IRBlock with ID \a id that is a template.
        virtual IRBlockTemplate *          findIRBlockTemplateWithID(quint64 id) =0;
        /// Find the const IRBlock with name \a s that is a template;
        virtual const IRBlockTemplate *    findIRBlockTemplateWithName(const QString &s) const=0;
        /// Find the IRBlock with name \a s that is a template.
        virtual IRBlockTemplate *          findIRBlockTemplateWithName(const QString &s) =0;
        /// Find the const IGhostRBlock with ID \a id;
        virtual const IGhostRBlock *            findIGhostRBlockWithID(quint64 id) const=0;
        /// Find the IGhostRBlock with ID \a id.
        virtual IGhostRBlock *                  findIGhostRBlockWithID(quint64 id) =0;
        /// Find the const RBlock closest to \a v.
        virtual const IRBlock *           findIRBlockClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        /// Find the RBlock closest to \a v. 
        virtual IRBlock *                 findIRBlockClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        /// Find the const RBlock containing to \a v.
        virtual const IRBlock *           findIRBlockContaining(const DVect &v,QSet<quint64> *skip=nullptr) const=0;
        /// Find the RBlock containing to \a v. 
        virtual IRBlock *                 findIRBlockContaining(const DVect &v,QSet<quint64> *skip=nullptr)=0;
        /// Returns a boolean indicating whether a two rigid blocks intersect.
        virtual bool                      collide(IRBlock *rb1, IRBlock *rb2) const = 0;

        /// Return the EnergyMap attached to the rblocks
        virtual const IEnergyMap       * getIRBlockEnergies() const=0;

        /// Return the RBlocks in a box
        virtual void getIRBlocksInBoxList(const DExtent &d,FArray<IRBlock *> *ret,bool intersect=true)=0;
        virtual void getIRBlocksInBoxList(const DExtent &d,FArray<const IRBlock *> *ret,bool intersect=true)=0;

        /// Populates sets of property names based on their type
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices) const=0;
        /// Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *valueProps,QStringList *labelProps,QStringList *vectorProps)const=0;
        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

        ///// Object for creation of multithreaded loop over rblocks
        virtual void threadedCallbackObject(Callback4<void,IRBlock *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectRBlock  
        template <class S,void (S::*MFP)(IRBlock *,quint32,quint32,void *)>
            void threadedCallbackMethod(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IRBlock *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObject(callback,v,useThreads);
            }
    };
} // namespace rblock
// EoF
