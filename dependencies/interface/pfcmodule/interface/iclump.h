#pragma once
/** \file iclump.h
    * \brief Interface for clumps.
    *
    * \addtogroup clumpsrc Clump module interface specification
    */
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IGroup;
    class IContainer;
}
namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IContact;
    class IBody;
    class IBodyMechanical;
    class IRigidBodyMechanical;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;
    class IPebble;
    class ITriPoly;
    class IClumpTemplate;

    /** \brief Interface for a clump.
        * This is the interface for a clump. A clump is a collection of pebbles that define the 
        * clump surface (see IPebble for more information). 
        * This interface provides basic functionality for accessing the Body Attributes and Piece 
        * properties associated with a clump.
        * \ingroup clumpsrc
        */
    class IClump {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x4e665ed6;

        /// Return an IThing interface for this object.
        virtual IThing *                    getIThing() = 0;
        /// Return a const IThing interface for this object.
        virtual const IThing *              getIThing() const = 0;
        /// Return the next IClump object in the global list, or 0 if it is the last set.
        virtual IClump *                    getNext() = 0;
        /// Return the next const IClump object in the global list, or 0 if it is the last set.
        virtual const IClump *              getNext() const = 0;
        /// Return the first IPiece in this clump.
        virtual IPiece *                    getFirstIPiece() = 0;
        /// Return the first const IPiece in this clump.
        virtual const IPiece *              getFirstIPiece() const = 0;
        /// Return the first IPebble in this clump.
        virtual IPebble *                   getFirstPebble() = 0;
        /// Return the first const IPebble in this clump.
        virtual const IPebble *             getFirstPebble() const = 0;
        /// Return the first IPieceMechanical in this clump.
        virtual IPieceMechanical *          getFirstIPieceMechanical() = 0;
        /// Return the first const IPieceMechanical in this clump.
        virtual const IPieceMechanical *    getFirstIPieceMechanical() const = 0;
        /// Return an IBody pointer. 
        virtual IBody *                     getIBody() = 0;
        /// Return a const IBody pointer.
        virtual const IBody *               getIBody() const = 0;
        /// Return an IBodyMechanical pointer. 
        virtual IBodyMechanical *           getIBodyMechanical() = 0;
        /// Return a const IBodyMechanical pointer.
        virtual const IBodyMechanical *     getIBodyMechanical() const = 0;
        /// Return an IRigidBodyMechanical pointer. 
        virtual IRigidBodyMechanical *      getIRigidBodyMechanical() = 0;
        /// Return a const IRigidBodyMechanical pointer.
        virtual const IRigidBodyMechanical *getIRigidBodyMechanical() const = 0;

        /// Return the number of pebbles.
        virtual uint                      getNumberPebbles() const=0;
        /// Return a list of pebbles.
        virtual void                      getPebbleList(FArray<IPebble *> *)=0;
        /// Return a list of pebbles.
        virtual void                      getPebbleList(FArray<const IPebble *> *) const=0;
        /// Return the scale factor going from the clump to the surface representation
        virtual double                    getSurfaceScaleFactor() const = 0;
        /// Return a component of the inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D
        virtual double                    getMoiComp(int) const = 0;
        /// Return a component of the inertial (may be scaled) inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D
        virtual double                    getInertialMoiComp(int) const = 0;
#ifdef THREED
        /// Return the inertia tensor. 
        virtual SymTensor                 getInertialMoiTensor() const = 0;
        virtual SymTensor                 getMoiTensor() const = 0;
#endif
        /// Return the inertia tensor in principal axis form.
        virtual DAVect                    getMoi() const=0;
        /// Return the inertial (may be scaled) inertia tensor in principal axis form.
        virtual DAVect                    getInertialMoi() const=0;

        /// Set a component of the inertia tensor. Admissible values are: 11,12,13,22,23,33 in 3D
        virtual void                      setMoiComp(const double &,int) = 0;
        /// Set the principal values of the inertia tensor directly.
        virtual void                      setMoi(const DAVect &)=0;

        /// Rotate the clump about the axis \a axis through the rotation point \a rotp by the angle \a w (in radians).
        /// If orientation tracking is enabled then it is updated when the \a update boolean is set to true
        virtual bool rotate(const DVect &rotp,const DVect &axis,const double &w,bool update=true,bool noThrow = false) = 0;
        /// Scale the clump to have this area or volume.
        virtual bool scale(const double &)=0;

        /// Return a list of IContacts.
        virtual void getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0)=0;
        /// Return a list of const IContacts.
        virtual void getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const=0;
        /// Return the number of contacts. 
        virtual quint64     getContactCount(bool active,const TType &type=0,bool domainToo = false) const=0;

        /// Return the IContainer pointer of the ChildContainer corresponding to this clump.
        virtual IContainer             *getClumpAsPebbleContainer() =0;
        /// Return the const IContainer pointer of the ChildContainer corresponding to this clump.
        virtual const IContainer       *getClumpAsPebbleContainer() const=0;

        /// Return the ITriPoly surface description of the clump
        virtual const ITriPoly *   getSurfaceDescription() const=0;

#ifdef THREED
        /// Get the quaternion corresponding to the orientation relative to the clump template orientation
        virtual const Quat & getSurfaceOrientation() const=0;
        virtual DVect getSurfaceEuler() const=0;
#else
        /// Get the double corresponding to the orientation relative to the clump template orientation
        virtual double getSurfaceOrientation() const=0;
#endif

        /// Get the rotation matrix with reference to the clump template.
        virtual void getSurfaceRotMatrix(DMatrix<dim,dim> *) const=0;

        /// Remove the pebble from the clump. 
        virtual void removeIPebble(IPebble *,IContainer *)=0;
        /// Add a pebble to this clump. 
        virtual IPebble *createAndAddPebble(const double &rad,const DVect &pos,IContainer *,quint64 id=0)=0;

        /// Return the IClumpTemplate pointer
        virtual IClumpTemplate * getMyClumpTemplate() =0;
        /// Return the const IClumpTemplate pointer
        virtual const IClumpTemplate * getMyClumpTemplate() const =0;
        /// Reset the clump template pointer
        virtual void resetClumpTemplate()=0;
        /// Return true if part of a clump template
        virtual bool getIsClumpTemplate() const=0;

        /// Calculate and set the inertial properties. Change the clump position
        /// but do not translate and no rotation occurs.
        virtual void calcAndSetInertialAttributes(const double &per=0.01)=0;

        /// When setting the MOI from FISH one needs make sure that it is not reset no matter what
        virtual void setMoiFix(bool b)=0;
        virtual bool isMoiFix()=0;
        virtual bool isMoiFix() const=0;

        /// Set the volume without scaling the clump
        virtual void setVolumeNoScale(const double &d)=0;

        /// Calculate the diameter of the sphere enclosing the clump
        virtual double getEnclosingDiameter() const=0;

        virtual int getFragmentIndex() const=0;


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
        virtual void            setFragmentIndex(int ind)=0;
        /// Returns a boolean indicating whether a point in space is inside or outside of the ball 
        virtual bool            getIsIn(const DVect &) const = 0;


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