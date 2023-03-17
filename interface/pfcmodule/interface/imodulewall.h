#pragma once
/** \file imodulewall.h
    * \brief Interface for the wall module.
    *
    * \addtogroup wallsrc
    */
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
    class IHalfFacet;
    class IHalfVertex;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IWall;
    class IFWall;

    /** \brief Interface for the wall module.
        *
        * This is the interface for the wall module. The outside world accesses information about
        * walls through this interface.
        * Right now only faceted walls have been implemented and this interface reflects that fact.
        * It is anticipated that there will be parallel methods to access the information for 
        * other wall types that are added to this module (point walls, line walls, etc.). The intention
        * is that all walls of any type will have unique ids so that the findWallWithID method will
        * find any IWall with the appropriate id.
        * 
        * \ingroup wallsrc
        */

    class IModuleWall {
    public:
        /// Typedef for the small granularity signal used to indicate to the gui that a propery has been added.
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        /// Return the FWall TType.
        virtual TType                    getFWallType() const=0;
        /// Return the HalfFacetBase TType.
        virtual TType                    getFacetType() const=0;
        /// Return the HalfVertexBase TType.
        virtual TType                    getVertexType() const=0;
        /// Return the FWall geometry notice.
        virtual uint                     getFWallGeometryNotice() const=0;
        /// Return the FWall cycle notice.
        virtual uint                     getFWallCycleNotice() const=0;
        /// Return the FWall velocity notice.
        virtual uint                     getFWallVelocityNotice() const=0;
        /// Return the FWall displacement notice.
        virtual uint                     getFWallDisplacementNotice() const=0;
        /// Return the FWall property notice.
        virtual uint                     getFWallPropertyNotice() const=0;
        /// Return the FWall extra notice.
        virtual uint                     getFWallExtraNotice() const=0;
        /// Return the FWall group notice.
        virtual uint                     getFWallGroupNotice() const=0;
        /// Return the FWallFacet extra notice.
        virtual uint                     getFWallFacetExtraNotice() const=0;
        /// Return the FWallFacet group notice.
        virtual uint                     getFWallFacetGroupNotice() const=0;
        /// Return the FWall clean notice.
        virtual uint                     getFWallCleanNotice() const=0;
        /// Return the const IContainer of the FWalls.
        virtual const IContainer       * getIContainerFWall() const=0;
        /// Return the IContainer of the FWalls.
        virtual IContainer *             getIContainerFWall()=0; 
        /// Return the IContainer of the Facets.
        virtual IContainer *             getIContainerFacet()=0;
        /// Return the const IContainer of the Facets.
        virtual const IContainer       * getIContainerFacet() const=0;
        /// Return the IContainer of the Vertices.
        virtual IContainer *             getIContainerVertex()=0;
        /// Return the const IContainer of the Vertices.
        virtual const IContainer       * getIContainerVertex() const=0;

        /// Return a const ICellSpace pointer to the FWall cell space
        virtual const ICellSpace       * getFWallCellSpace() const=0;
        /// Return an ICellSpace pointer to the FWall cell space
        virtual ICellSpace *             getFWallCellSpace()=0;
        /// Find the const IWall with ID \a id;
        virtual const IWall *            findWallWithID(quint64 id) const=0;
        /// Find the IWall with ID \a id.
        virtual IWall *                  findWallWithID(quint64 id) =0;
        /// Find the const IFWall with ID \a id;
        virtual const IFWall *           findFWallWithID(quint64 id) const=0;
        /// Find the IFWall with ID \a id.
        virtual IFWall *                 findFWallWithID(quint64 id) =0;
        /// Find the const IWall with name \a name.
        virtual const IWall *            findWallWithName(const QString &name) const=0;
        /// Find the IWall with name \a name.
        virtual IWall *                  findWallWithName(const QString &name) =0;
        /// Find the const IWall with name \a name.
        virtual const IFWall *           findFWallWithName(const QString &name) const=0;
        /// Find the IWall with name \a name.
        virtual IFWall *                 findFWallWithName(const QString &name) =0;
        /// Find the const FWall closest to \a v.
        virtual const IFWall *           findFWallClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        /// Find the FWall closest to \a v. 
        virtual IFWall *                 findFWallClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        /// Create a faceted wall with name \a name and ID \a id
        virtual IFWall * createIFWall(QString name,quint64 id=0)=0;
        /// Find the const IHalfFacet with ID \a id;
        virtual const IHalfFacet *       findFacetWithID(quint64 id,IFWall *w=0) const=0;
        /// Find the IHalfFacet with ID \a id.
        virtual IHalfFacet *             findFacetWithID(quint64 id,IFWall *w=0) =0;
        /// Find the IHalfFacet with ID \a id.
        virtual IHalfFacet *             findFacetAloneWithID(quint64 id) =0;
        /// Find the const IHalfFacet closest to \a v.
        virtual const IHalfFacet *       findFacetClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        /// Find the IHalfFacet closest to \a v. 
        virtual IHalfFacet *             findFacetClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        /// Remove the facet \a fac from its faceted wall.
        virtual IFWall *                 removeIHalfFacet(IHalfFacet *fac) = 0;

        /// Find the const IHalfVertex with ID \a id;
        virtual const IHalfVertex *      findVertexWithID(quint64 id,IFWall *w=0) const=0;
        /// Find the IHalfVertex with ID \a id.
        virtual IHalfVertex *            findVertexWithID(quint64 id,IFWall *w=0) =0;
        /// Find the IHalfVertex with ID \a id.
        virtual IHalfVertex *            findVertexAloneWithID(quint64 id) =0;
        /// Find the const IHalfVertex closest to \a v.
        virtual const IHalfVertex *      findVertexClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        /// Find the IHalfVertex closest to \a v. 
        virtual IHalfVertex *            findVertexClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;

        /// Return a list of IWalls.
        virtual void getWallList(FArray<IWall *> *ret)=0;
        /// Erase the IWall - could be of any type.
        virtual void eraseIWall(IWall *w) = 0;

        /// Return the EnergyMap attached to the walls
        virtual const IEnergyMap       * getIWallEnergies() const=0;

        /// Return the Walls in a box
        virtual void getWallInBoxList(const DExtent &d,FArray<IFWall *> *ret,bool intersect=true)=0;
        virtual void getWallInBoxList(const DExtent &d,FArray<const IFWall *> *ret,bool intersect=true)=0;

        /// Return the Facets in a box
        virtual void getFacetInBoxList(const DExtent &d,FArray<IHalfFacet *> *ret,bool intersect=true)=0;
        virtual void getFacetInBoxList(const DExtent &d,FArray<const IHalfFacet *> *ret,bool intersect=true)=0;

        /// Return the Vertices in a box
        virtual void getVertexInBoxList(const DExtent &d,FArray<IHalfVertex *> *ret,bool intersect=true)=0;
        virtual void getVertexInBoxList(const DExtent &d,FArray<const IHalfVertex *> *ret,bool intersect=true)=0;

        /// Populates sets of property names based on their type
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices) const=0;
        /// Populates sets of piece property names based on their type
        virtual void  populatePieceAttributeSets(QStringList *valueAtts,QStringList *labelAtts,QStringList *vectorAtts)const=0;
        /// Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *valueProps,QStringList *labelProps,QStringList *vectorProps)const=0;
        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

        /// Object for creation of multithreaded loop over walls
        virtual void threadedCallbackObjectWall(Callback4<void,IFWall *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectFWall  
        template <class S,void (S::*MFP)(IFWall *,quint32,quint32,void *)>
            void threadedCallbackMethodWall(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IFWall *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObjectWall(callback,v,useThreads);
            }

        /// Object for creation of multithreaded loop over facets
        virtual void threadedCallbackObjectFacet(Callback4<void,IHalfFacet *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectFacet  
        template <class S,void (S::*MFP)(IHalfFacet *,quint32,quint32,void *)>
            void threadedCallbackMethodFacet(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IHalfFacet *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObjectFacet(callback,v,useThreads);
            }

        /// Object for creation of multithreaded loop over vertices
        virtual void threadedCallbackObjectVertex(Callback4<void,IHalfVertex *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObjectVertex  
        template <class S,void (S::*MFP)(IHalfFacet *,quint32,quint32,void *)>
            void threadedCallbackMethodVertex(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IHalfVertex *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObjectVertex(callback,v,useThreads);
            }

    };
} // namespace pfc
// EoF
