#pragma once
// icontact.h

/**
    * \file
    * \brief Contact class.
    */

#include "dim.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IFishParam;
    class IThing;
    class IContainer;
}

namespace itascaxd {
    class IPiece;
    class IContactModel;
    using namespace itasca;

    /// \brief Contact class.

    /** Contacts may exist between two IPiece objects (e.g. between two balls or between a 
        * ball and a wall). They are formed and broken automatically during the course 
        * of a simulation.

        * The behavior of a material is simulated by associating a constitutive model with each 
        * contact.
        * \ingroup module

      */
    class IContact {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b85;

        /// Get the terminal interface TType of the instantiated contact
        virtual TType getContainerType() const=0;

        /// returns a IThing pointer
        virtual IThing *              getIThing()=0;       
        virtual const IThing *        getIThing() const=0; 
        // Get the IContainer of the contact.
        virtual IContainer *          getContainer()=0;
        virtual const IContainer *    getContainer() const=0;

        /// returns a string describing the contact class (eg. "mechanical")
        virtual QString               getClassName() const=0;
        /// returns a string describing the contact class (eg. "mechanical")
        virtual QString               getTypeName() const=0;
        /// returns true if the contact object is active
        virtual bool                  getActive() const=0; 
        /// returns a pointer to the contact first IPiece object (e.g. a ball)  
        virtual IPiece *              getEnd1()=0;         
        virtual const IPiece *        getEnd1() const=0;   
        /// returns a pointer to the contact second IPiece object (e.g. a wall)
        virtual IPiece *              getEnd2()=0;         
        virtual const IPiece *        getEnd2() const=0;   
        /// returns whether or not a contact is with a wall
        virtual bool                  withWall() const=0;
        /// returns the contact local system axes 
        virtual CAxes                 getLocalSystem() const=0;  
        /// returns the contact position
        virtual DVect                 getPosition() const=0; 
        /// returns a normal vector to the contact
        virtual DVect3                getNormal() const=0;   
        /// returns the gap where, for a mechanical contact, negative gap corresponds to physical overlap. 
        virtual double                getGap() const=0; 
        /// returns the area which is 0 for non-mechanical contacts and 0 if no area is used
        virtual double                getArea() const=0; 
        /// Returns the length from getPosition() in the normal direction that is generally representative of the contact (for plotting).
        virtual DVect2                getCharacteristicLength() const=0;
        /// returns true to indicate inhibit condition
        virtual bool                  getInhibit() const=0;  
        /// returns true to indicate always active condition
        virtual bool                  getAlwaysActive() const=0;  
        /// returns true to indicate never delete condition
        virtual bool                  getNeverDelete() const=0;  
        /// Returns TRUE to indicate that it is OK to delete the contact
        /// It is expected that CELL SPACE checking occurs here!
        /// Note:  Criteria should match that used by contact detection, or you can create a situation where contacts are detroyed/created constantly.
        virtual bool                  canDelete() const=0;
        /// returns contact model
        virtual IContactModel *       getModel() =0;  
        virtual const IContactModel * getModel() const=0;
        /// returns the name of the contact model present at this mechanical contact    
        virtual QString               getModelName() const=0;
        /// assigns the contact model based on a name    
        virtual IContactModel        *setModelName(const QString &name)=0;
        virtual IContactModel        *setModel(const IContactModel *toClone,bool validate=true)=0;

        /// Returns TRUE if the contact is a mechanical contact - meaning it is derived from IContactMechanical.
        virtual bool                  isMechanical() const=0;
        /// Returns TRUE if the contact is a thermal contact - meaning it is derived from IContactThermal.
        virtual bool                  isThermal() const=0;
        /// Return a pointer to the parent contact, i.e. the contact that this one has been slaved to upon
        /// creation and destruction.
        virtual IContact *            getParent() =0;
        virtual const IContact *      getParent() const =0;
        /// sets the inhibit condition flag 
        virtual void                  setInhibit(bool b)=0;
        /// sets the alwaysActive condition flag 
        virtual void                  setAlwaysActive(bool b)=0;
        /// sets the never delete condition flag 
        virtual void                  setNeverDelete(bool b)=0;
        /// Gets the FID ID number, an integer tag associated with each contact.
        virtual uint                  getFID() const=0;
        /// Sets the activity flag
        virtual void                  setActive(bool b)=0; 
        /// Sets the FID ID number, an integer tag associated with each contact.
        virtual void                  setFID(uint i)=0;
        /// Sets the contact offset (which will be added to position of end2)
        virtual void                  setOffSet(const DVect &v)=0;
        /// Gets the contact offset (which is added to position of end2)
        virtual DVect                 getOffSet() const =0;
        /// Gets whether or not the contact is with a ghost
        virtual bool                  getIsWithGhost() const=0;
        // Extra Variable Section
        /// Returns true if the contact model can be assigned to this contact
        virtual bool                  amInterestedIn(const QString &cmodel) const=0;
        /// Clean the contact, updating activity state
        virtual void                  clean(bool addStiffness)=0;
        /// returns the value of the property with name \a name     
        virtual QVariant              getProperty(const QString &name) const=0;
        /// sets the value of the property with name \a name     
        virtual bool                  setProperty(const QVariant &v,const QString &name)=0;
        /// returns the index of the property with name \a name     
        virtual uint                  getPropertyIndex(const QString &name) const=0;
        /// returns the name of the property with index \a index     
        virtual QString               getPropertyName(uint index) const=0;
        /// Tag the contact property notice
        virtual void                  tagPropertyNotice()=0;
        /// Return self
        virtual IContact *            getIContact()=0;

    };
} // namespace itasca

// EoF
