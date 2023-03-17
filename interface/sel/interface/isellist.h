//isellist.h
#pragma once

/**
  * \file
  * \brief Interface to the global list of all structural elements.
  */

#include "utility/interface/igroup.h"
#include "geometry/interface/igeomimportexport.h"

namespace itasca {
    class IContainer;
    class IThing;
}
namespace itascaxd {
    class IRange;
}

namespace sel {
    using namespace itasca;
    using namespace itascaxd;

    class ISEL;
    class ISELNode;

    /// \brief Interface to the global list of all structural elements.
    /// \ingroup SEL
    class ISELList {
    public:
        struct SELInfo {
            TType    type_;
            uint     segments_;
            double   maxLen_;
            quint64  cid_;
            IGroupID group_;
            TType    femType_;
            bool     embedded_; 
            bool     crossDiagonal_;
            bool     separated_;  // All nodes on every element distince and new
            bool     snap_;       // Snap to existing node position
            IGroupID beginGroup_; // Group name to add to begin/end nodes (1D only)
            IGroupID endGroup_;
            bool     groupEnds_ = true; // Whether to add group names to end nodes (1D only).
            bool     dowel_; // 2 links
        };

        /// returns a const IThing pointer
        virtual const IThing *getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *getIThing()=0;
        /// Returns a const pointer to the IContainer interface representing the global list of structural elements.
        virtual const IContainer * getIContainer() const=0;
        /// Returns a pointer to the IContainer interface representing the global list of structural elements.
        virtual IContainer *       getIContainer() =0;
        /// Return a const pointer to the first structural element stored.\n
        /// ISEL::getNext can be used to iterate on the list.
        virtual const ISEL *       getFirstSEL() const=0;
        /// Return a pointer to the first structural element stored.\n
        /// ISEL::getNext can be used to iterate on the list.
        virtual ISEL *             getFirstSEL()=0;
        /// Given a strucutal elemnent \b id, returns a const pointer to the structural element.
        virtual const ISEL *       findSELWithID(quint64 id) const=0;
        /// Returns a const pointer to the closest structural element to \b position of type \b sel_type. 
        ///   Judges by element centroid.   Will check all types of elements if sel_type=0.
        ///   Returns nullptr if the element is not found.
        virtual const ISEL *       findSEL(const DVect &pos,TType type=0,double radius=limits<double>::max(),bool update=true) const=0;
        /// Returns a pointer to the structural element of type \b sel_type closest to \b position. 
        ///   Judges by element centroid.   Will check all types of elements if sel_type=0.
        ///   Returns 0 if the element is not found.
        virtual ISEL *             findSEL(const DVect &pos,TType type=0,double radius=limits<double>::max(),bool update=true) =0;

        /// Returns the general SEL type
        virtual QString             getKeywordFromType(TType type) const=0;
        virtual QList<const ISEL *> getAllTypes() const=0;
        virtual const ISEL *        getRegisteredType(TType type) const=0;
        virtual quint32             getNumberRegistered() const=0;
        virtual quint64             getNextCollectionID() const=0;

        /// Returns the tag for the structural element group change notice
        virtual uint               getGroupNotice() const=0;
        /// Returns the tag for the structural element property change notice
        virtual uint               getPropertyNotice() const=0;
        /// Returns the tag for the structural element state change notice
        virtual uint               getStateNotice() const=0;
        /// Returns the tag for the structural element applied force change notice
        virtual uint               getForceNotice() const=0;
        /// Returns the tag for the structural element extra variable change notice
        virtual uint               getFISHNotice() const=0;
        /// Returns the tag for the structural element pressure change notice
        virtual uint               getPressureNotice() const=0;
        /// Returns the tag for the structural element hide change notice
        virtual uint               getHideNotice() const=0;
        /// Returns the tag for the structural element select change notice
        virtual uint               getSelectNotice() const=0;

        /// Stress resultant calculation logic
        /// Claims an index for the storage of stress resultant data.
        virtual uint claimResultantStorage()=0;
        /// Releases the storage of stress resultant data index, so it can be reused later.
        virtual void releaseResultantStorage(uint index)=0;
        /// Marks the SEL element and nodes that will be used in subsequent callls that check the IRange::isIn function
        virtual void markItemsInRange(uint index,const IRange *ran,TType type)=0;
        /// Establishes the local coordinate system used to resolve the stress quantities. Should be called after
        /// markItemsInFilter. Parameter \b X is the value used to match to local X. Retunrs false on error, probably 
        /// an element parallel to X.
        virtual bool assignSurfaceSystem(uint index,const DVect3 &x,bool exceptionsAllowed,
                                         QString *error,TType type)=0;
        /// This function calculates stress resultants quantities (mx,my,mxy,nx,ny,nxy,qx,qy). 
        /// This function must be called after calling functions markItemsInFilter and assignSurfaceSystem.
        virtual bool recoverStressResultants(uint index,bool exceptionsAllowed,
                                             QString *error,TType type)=0;
        /// This function calculates sxx,syy,szz,sxz,sxy,sxz,smin,smax and sint.
        /// Called after recoverStressResultants if a stress quantity is required.
        virtual bool recoverStress(uint index,const double &depth,bool exceptionsAllowed,
                                   QString *error,TType type)=0;

        virtual void setForceUpdate(bool b)=0;
        virtual bool getWasUpdated() const=0;
    };
} // namespace sel
// EoF

