#pragma once
/** \file iballclump.h
    * \brief Interface for the Ball module.
    *
    * \defgroup ballsrc Ball module interface specification
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
    class ICellSpace;
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IBall;
    class IGhostBall;

    /** \brief Interface for the Ball module.
        * This is the interface for the Ball module. 
        * \ingroup ballsrc
        */
    class IModuleBall {
    public:
        /// Signal emitted when a property is created or modified.
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        /// Ball TType for type casting.
        virtual TType                    getBallType() const=0;
        /// Ghost Ball TType for type casting.
        virtual TType                    getGhostBallType() const=0;
        /// Ball geometry notice unsigned integer
        virtual uint                     getBallGeometryNotice() const=0;
        /// Ball property notice unsigned integer
        virtual uint                     getBallPropertyNotice() const=0;
        /// Ball velocity notice unsigned integer
        virtual uint                     getBallVelocityNotice() const=0;
        /// Ball cycle notice unsigned integer
        virtual uint                     getBallCycleNotice() const=0;
        /// Ball fixity condition notice unsigned integer
        virtual uint                     getBallFixNotice() const=0;
        /// Ball extra notice unsigned integer
        virtual uint                     getBallExtraNotice() const=0;
        /// Ball group notice unsigned integer
        virtual uint                     getBallGroupNotice() const=0;
        /// Ball clean notice unsigned integer
        virtual uint                     getBallCleanNotice() const=0;
        /// Ball rotation notice unsigned integer
        virtual uint                     getBallRotationNotice() const=0;

        /// Const access to the Ball container
        virtual const IContainer       * getIContainerBall() const=0; 
        /// Access to the Ball container
        virtual IContainer *             getIContainerBall()=0;
        /// Const access to the GhostBall container
        virtual const IContainer       * getIContainerGhostBall() const=0;
        /// Access to the GhostBall container
        virtual IContainer *             getIContainerGhostBall()=0;
        /// Const access to the BallBallContact container
        virtual const IContainerContact *getIContainerBallBallContact() const=0;
        /// Access to the BallBallContact container
        virtual IContainerContact *      getIContainerBallBallContact()=0;
        /// Const access to the DomainBallContact container
        virtual const IContainerContact *getIContainerDomainBallContact() const=0;
        /// Access to the DomainBallContact container
        virtual IContainerContact *      getIContainerDomainBallContact()=0;
        /// Const access to the Ball cellspace
        virtual const ICellSpace       * getICellSpace() const=0;
        /// Access to the Ball cellspace
        virtual ICellSpace *             getICellSpace()=0;
        /// Find a Ball given its \a id (const access)
        /// \param id Ball ID number
        virtual const IBall *            findBallWithID(quint64 id) const=0;
        /// Find a Ball given its \a id 
        /// \param id Ball ID number
        virtual IBall *                  findBallWithID(quint64 id) =0;
        /// Find a GhostBall given its \a id (const access)
        /// \param id GhostBall ID number
        virtual const IGhostBall *       findGhostBallWithID(quint64 id) const=0;
        /// Find a GhostBall given its \a id
        /// \param id GhostBall ID number
        virtual IGhostBall *             findGhostBallWithID(quint64 id) =0;
        /// Find the Ball closest to the location \a v (const access). If the optional \a radius
        /// is specified, the search is restricted to the sphere (circle in 2D) with center \a v and radius \a radius  
        /// \param v Position
        /// \param radius Optional radius
        virtual const IBall *            findBallClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        /// Find the Ball closest to the location \a v. If the optional \a radius
        /// is specified, the search is restricted to the sphere (circle in 2D) with center \param v and radius \param radius  
        /// \param v Position
        /// \param radius Optional radius
        virtual IBall *                  findBallClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual const IBall *            findBallContaining(const DVect &v,QSet<quint64> *skip=nullptr) const=0;
        virtual IBall *                  findBallContaining(const DVect &v,QSet<quint64> *skip=nullptr)=0;
        /// Validate the Ball module
        virtual void                     validate()=0;
        /// Const access to the Ball energies
        virtual const IEnergyMap       * getIBallEnergies() const=0;
        /// Const access to the BallBallContact energies
        virtual const IEnergyMap       * getIContactBallBallEnergies() const=0;
        /// Fill the FArray \a ret with a list of const IBall pointers to the balls falling in the DExtent \a d.
        /// If \a intersect is true (default), then both balls whose centroids fall inside the extent and balls whose
        /// extents intersect the extent are added to the FArray; otherwise only balls whose centroids fall inside the extent are considered.
        /// \param d DExtent
        /// \param ret Pointer to a FArray of const IBall pointers
        /// \param intersect Boolean intersection flag
        virtual void getBallInBoxList(const DExtent &d,FArray<IBall *> *ret,bool intersect=true)=0;
        /// Fill the FArray \a ret with a list of IBall pointers to the balls falling in the DExtent \a d.
        /// If \a intersect is true (default), then both balls whose centroids fall inside the extent and balls whose
        /// extents intersect the extent are added to the FArray; otherwise only balls whose centroids fall inside the extent are considered.
        /// \param d DExtent
        /// \param ret Pointer to a FArray of IBall pointers
        /// \param intersect Boolean intersection flag
        virtual void getBallInBoxList(const DExtent &d,FArray<const IBall *> *ret,bool intersect=true)=0;
        /// Create a Ball with radius \a rad at position \a pos, and returns an IBall pointer
        /// to the created Ball. If \a id is specified, then this id must not be already used by an 
        /// existing Ball. An exception is thrown if this method cannot complete.
        /// \param rad Radius
        /// \param pos Position
        /// \param id Optional ID number
        virtual IBall *                  createBall(double rad,DVect pos,quint64 id=0)=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

        /// Object for creation of multithreaded loop over balls
        virtual void threadedCallbackObject(Callback4<void,IBall *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) const=0;

        /// Implementation of the multithreaded loop using the threadedCallbackObject  
        template <class S,void (S::*MFP)(IBall *,quint32,quint32,void *)>
            void threadedCallbackMethod(S *s,void *v=nullptr,bool useThreads=true) const {
                Callback4Method<void,IBall *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObject(callback,v,useThreads);
            }

    };
} // namespace pfc
// EoF
