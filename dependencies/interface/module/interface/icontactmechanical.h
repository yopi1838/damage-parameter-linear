#pragma once
// icontactmechanical.h
#include "utility/interface/isignalbase.h"
#include "base/src/mat.h"

/**
    * \file
    * \brief ContactMechanical class.
    */


namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IContact;
    class IContactModel;
    class IContactModelMechanical;
    class IPiece;
    class IPieceMechanical;

    /// \brief ContactMechanical class.

    /** Mechanical Contacts may exist between two IPiece objects (e.g. between two balls or between a 
        * ball and a wall). They are formed and broken automatically during the course 
        * of a simulation.

        * The behavior of a material is simulated by associating a constitutive model with each 
        * mechanical contact.
        * \ingroup module

      */

    class IContactMechanical {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x04c815ba9;

        /// Fracture ID signal
        typedef Signal2<IContactModel *,const IContact *> fid_signal;

        /// returns a IThing pointer
        virtual const IThing *                 getIThing() const=0;
        virtual IThing *                       getIThing()=0;
        /// returns a IContact pointer
        virtual const IContact *               getContact() const=0;
        virtual IContact *                     getContact()=0;
        /// returns a pointer to the mechanical contact first IPiece object (e.g. a ball)  
        virtual const IPiece *                 getEnd1() const=0;
        virtual IPiece *                       getEnd1()=0;
        /// returns a pointer to the mechanical contact first IPieceMechanical object (e.g. a ball)  
        virtual const IPieceMechanical *       getEnd1Mechanical() const=0;
        virtual IPieceMechanical *             getEnd1Mechanical()=0;
        /// returns a pointer to the mechanical contact second IPiece object (e.g. a ball)  
        virtual const IPiece *                 getEnd2() const=0;
        virtual IPiece *                       getEnd2()=0;
        /// returns a pointer to the mechanical contact second IPieceMechanical object (e.g. a ball)  
        virtual const IPieceMechanical *       getEnd2Mechanical() const=0;
        virtual IPieceMechanical *             getEnd2Mechanical()=0;
        // Return the curvature of end1
        virtual DVect2                         getEnd1Curvature() const=0;
        // Return the curvature of end2
        virtual DVect2                         getEnd2Curvature() const=0;
        // Return the curvature of end1
        virtual DVect2                         getEnd1Extent() const=0;
        // Return the curvature of end2
        virtual DVect2                         getEnd2Extent() const=0;
        /// returns a pointer to the contact model present at this mechanical contact  
        virtual const IContactModel *          getModel() const=0;
        virtual IContactModel *                getModel()=0;
        /// returns a pointer to the mechanical contact model present at this mechanical contact  
        virtual const IContactModelMechanical *getModelMechanical() const=0;
        virtual IContactModelMechanical *      getModelMechanical()=0;
        /// assigns a clone of the contact model passed in arguments, and copies its properties  
        virtual IContactModel *                setModel(const IContactModel *toClone,bool validate=true)=0;
        /// Replace the contact model with a new one. The old one is deleted.
        virtual void                           replaceContactModel(IContactModel*)=0;
        /// returns the name of the contact model present at this mechanical contact    
        virtual QString                        getModelName() const=0;
        /// assigns the contact model based on a name    
        virtual IContactModel *                setModelName(const QString &name)=0;
        /// returns the value of the property with name \a name     
        virtual QVariant                       getProperty(const QString &name) const=0;
        /// sets the value of the property with name \a name     
        virtual bool                           setProperty(const QVariant &v,const QString &name)=0;
        /// returns the index of the property with name \a name     
        virtual uint                           getPropertyIndex(const QString &name) const=0;
        /// returns the name of the property with index \a index     
        virtual QString                        getPropertyName(uint index) const=0;
        /// returns the force at this mechanical contact, in the contact local coordinate system     
        virtual DVect                          getLocalForce() const=0;
        /// returns the moment applied to end1 in the contact local coordinate system     
        virtual DAVect                         getLocalMomentOn1() const=0;
        /// returns the moment applied to end2 in the contact local coordinate system     
        virtual DAVect                         getLocalMomentOn2() const=0;
        /// sets the force at this mechanical contact to \a v, expressed in the global coordinate system     
        virtual DVect                          getGlobalForce() const=0;
        /// Gets the normal force of this mechanical contact
        virtual double                         getNormalForce() const=0;
        /// Gets the shear force of this mechanical contact
        virtual double                         getShearForce() const=0;
        /// returns the moment applied to end1 in the global coordinate system     
        virtual DAVect                         getGlobalMomentOn1() const=0;
        /// returns the moment applied to end2 in the global coordinate system     
        virtual DAVect                         getGlobalMomentOn2() const=0;
        /// Gets the FID ID number, an integer tag associated with each contact.
        virtual uint                           getFID() const=0;
        /// Sets the FID ID number, an integer tag associated with each contact.
        virtual void                           setFID(uint i)=0;
        /// Get the Signal2 for induction when a fid tag is active. This is used to set the contact model properties.
        virtual fid_signal *                   getFIDSignal() =0;
        /// Execute the fid_signal with this IContactModel.
        virtual void                           executeFIDSignal(IContactModel *)=0;
        /// Execute the fid_signal with this IContactModel.
        virtual void                           executeFIDSignal(IContactModel *) const=0;
        /// Return the gap of this mechanical contact. A negative gap corresponds to physical overlap. 
        virtual double                         getGap() const=0;
        /// Return the actual volume of the overlaping region between the contacting pieces - null if the overlap is negative. 
        virtual double                         calculateOverlapVolume() const = 0;
        /// Return the length of this mechanical contact. 
        /// Length is defined as the vector going from the centroid of the body comprising end1 to the centroid of the body
        /// comprising end2
        virtual DVect                          getLength() const = 0;
        /// Given a force on end2 and the contact position calculate the torque on end1 - in local coordinates
        virtual void                           updateResultingTorqueOn1Local(const DVect &forceOn2,DAVect *ret) const=0;
        /// Given a force on end2 and the contact force and position calculate the torque on end2 - in local coordinates
        virtual void                           updateResultingTorqueOn2Local(const DVect &forceOn2,DAVect *ret) const=0;
        /// Convert a local quantity to the global system
        virtual DVect                          inGlobal(const DVect &vin) const=0;
        /// Convert a global quantity to the local system
        virtual DVect                          inLocal(const DVect &vin) const=0;
        /// Get the contact model effective stiffness in translation (normal,shear) and rotation.
        virtual DVect2                         getEffectiveTranslationalStiffness() const =0;
        virtual DAVect                         getEffectiveRotationalStiffness()    const =0;

        /// Return the bonding state
        virtual bool                           getIsBonded() const = 0;
        /// Unbond the contact
        virtual void                           unbondMe()=0;

        virtual void                           setFullUpdate(bool)=0;
        virtual bool                           getFullUpdate() const=0;
        virtual DVect                          localForceFromStress(const Mat &) const=0;
        virtual void                           setForce(const DVect &vin) = 0;
        /// Return contact specific joint geometry. Returns true if triangles 
        virtual bool                           getJointGeometry(std::vector<DVect> *points,int whichProc=0) const=0;
        /// Return the relative velocity at a point in local coordinate system.
        virtual DVect                          getRelativeLocalVelocity(const DVect &pos) const = 0;
    };
} // namespace itascaxd
// EoF
