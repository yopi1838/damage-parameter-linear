#pragma once
// ibodymechanical.h

// Interface for functionality of all mechanical bodies. There can be a mixture of methods for both
// rigid and deformable bodies. The distinction must be made between rigid and deformable bodies as deformable
// bodies may require quite different storage when compared with rigid bodies.

namespace itascaxd {
    using namespace itasca;
    class IBody;
    class IPieceMechanical;

    class IBodyMechanical {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4e67b814;
        /// Return the IBody pointer.
        virtual const IBody    *getIBody() const=0;
        /// Return the IBody pointer.
        virtual IBody          *getIBody()=0;
        /// Return a boolean indicating the fixity condition.
        virtual bool            getFix(quint32 dof) const=0;
        /// Return a fixity label.
        virtual QString         getFixLabel() const=0;
        /// Return the property of the piece 
        virtual QVariant        getPieceProperty(const QString &name,const IPiece *p=0) const=0;
        /// Return the density.    
        virtual double          getDensity() const=0;
        /// Return the volume in 3D, or the volume per unit thickness in 2D.
        virtual double          getVolume() const=0;
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
        /// Set the volume in 3D, or the volume per unit thickness in 2D.
        virtual void            setVolume(const double &d)=0;
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


    };
} // namespace itascaxd

// EoF
