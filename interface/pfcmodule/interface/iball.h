#pragma once
/** \file iball.h
    * \brief Interface for a ball.
    *
    * \addtogroup ballsrc Ball module interface specification
    */

#include "base/src/matrix.h"

namespace itasca {
    class IThing;
    class IGroup;
}

namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IBody;
    class IBodyMechanical;
    class IRigidBodyMechanical;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /** * \brief 
        * Interface for a ball.
        * \details 
        * A ball is a rigid mechanical body composed of one unique mechanical piece. 
        * This interface provides basic functionality for accessing the body attributes and the piece 
        * properties associated with a ball.
        * \ingroup ballsrc
        */
    class IBall {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x51c13f16;

        /// Return an IThing interface for this object.
        virtual IThing *                     getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *               getIThing() const=0;
        /// Return an IPiece interface for this object.
        virtual IPiece *                     getIPiece()=0;
        /// Return a const IPiece interface for this object.
        virtual const IPiece *               getIPiece() const=0;
        /// Return an IPieceMechanical interface for this object.
        virtual IPieceMechanical *           getIPieceMechanical()=0;
        /// Return a const IPieceMechanical interface for this object.
        virtual const IPieceMechanical *     getIPieceMechanical() const=0;
        /// Return an IBody interface for this object.
        virtual IBody *                      getIBody()=0;
        /// Return a const IBody interface for this object.
        virtual const IBody *                getIBody() const=0;
        /// Return an IBodyMechanical interface for this object.
        virtual IBodyMechanical *            getIBodyMechanical()=0;
        /// Return a const IBodyMechanical interface for this object.
        virtual const IBodyMechanical *      getIBodyMechanical() const=0;
        /// Return an IRigidBodyMechanical interface for this object.
        virtual IRigidBodyMechanical *       getIRigidBodyMechanical()=0;
        /// Return a const IRigidBodyMechanical interface for this object.
        virtual const IRigidBodyMechanical * getIRigidBodyMechanical() const=0;
        /// Return the ball's cell extent
        virtual DExtent                      getCellExtent() const=0;
        /// Return the ball's tolerance extent
        virtual DExtent                      getToleranceExtent() const=0;
        /// Return the current stress computed for this ball
        virtual DMatrix<DIM,DIM>             getStress() const=0;
        /// Return the ball scaled moment of inertia
        virtual double                       getInertialMoi() const=0;
        /// Return the ball real moment of inertia
        virtual double                       getMoi() const=0;
        /// Return the ball radius
        virtual double                       getRadius() const=0;
        /// Return the ball fragment index
        virtual int                          getFragmentIndex() const=0;
        /// Set the ball fragment index
        virtual void                         setFragmentIndex(int ind)=0;
        /// Returns a boolean indicating whether a point in space is inside or outside of the ball 
        virtual bool                         getIsIn(const DVect &) const = 0;

        /// Set the ball radius
        /// \param d New radius
        /// \param lock Optional boolean to lock the ball for multithreading purpose (default to false)
        virtual void setRadius(const double &d,bool lock=false)=0;
        virtual void setRadiusNoLock(const double &d)=0;

        /// Methods from the IBodyMechanical interface for convenience
        /// Return a boolean indicating the fixity condition.
        virtual bool            getFix(quint32 dof) const=0;
        /// Return a fixity label.
        virtual QString         getFixLabel() const=0;
        /// Return the property of the piece 
        virtual QVariant        getPieceProperty(const QString &name,const IPiece *p=0) const=0;
        /// Return the density.    
        virtual double          getDensity() const=0;
        /// Return the inertial mass.
        virtual double          getInertialMass() const=0;
        /// Return the mass.
        virtual double          getMass() const=0;
        /// Return the resultant of all contact forces.
        virtual DVect           getContactForce() const=0;
        /// Return the resultant of all contact moments.
        virtual DAVect          getContactMoment() const=0;
        /// Return the applied force.
        virtual DVect           getAppliedForce() const=0;
        /// Return the applied moment.
        virtual DAVect          getAppliedMoment() const=0;
        /// Return the unbalanced force.
        virtual DVect           getUnbalancedForce() const=0;
        /// Return the unbalanced moment.
        virtual DAVect          getUnbalancedMoment() const=0;
        /// Return the local damping coefficient.
        virtual double          getLocalDamping() const=0;
        /// Set the fixity condition.
        virtual void            setFix(quint32 dof,bool b)=0;
        /// Set the property name.
        virtual bool            setPieceProperty(const QString &name,const QVariant &v,IPiece *p=0) =0;
        /// Set the density.
        virtual void            setDensity(const double &d)=0;
        /// Set the contact force.
        virtual void            setContactForce(const DVect &v)=0;
        /// Set the contact moment.
        virtual void            setContactMoment(const DAVect &v)=0;
        /// Set the applied force.
        virtual void            setAppliedForce(const DVect &v)=0;
        /// Set the applied moment.
        virtual void            setAppliedMoment(const DAVect &v)=0;
        /// Set the local damping coefficient.
        virtual void            setLocalDamping(const double &d)=0;
        /// Scale the volume to the specified value while preserving the mass
        virtual void            scaleVolumePreserveMass(const double &d,bool checkState=true,bool lock=false)=0;


        /// IRigidBodyMechanical interface
#ifdef TWOD
        virtual double            getAngDisplacement() const=0;
#endif
        virtual DVect             getCentroid() const=0;
        virtual DVect             getVelocity() const=0;
        virtual DAVect            getAngVelocity() const=0;
        virtual DVect             getDisplacement() const=0;
        virtual double            getVolume() const=0;
        virtual Quat              getOrient() const=0;

#ifdef TWOD
        virtual void              setAngDisplacement(const double &v)=0;
#endif
        virtual void              setCentroid(const DVect &p)=0;
        virtual void              setVelocity(const DVect &p)=0;
        virtual void              setAngVelocity(const DAVect &p)=0;
        virtual void              setDisplacement(const DVect &v)=0;
        virtual void              setOrient(const Quat &q)=0;
#ifdef THREED
        virtual DVect             getEulerFromOrient() const=0;
        virtual void              setOrientFromEuler(const DVect3 &d)=0;
#endif

    };
} // namespace pfc
// EoF

