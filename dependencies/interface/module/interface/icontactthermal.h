#pragma once
// icontactthermal.h
#include "utility/interface/isignalbase.h"

/**
    * \file
    * \brief ContactThermal class.
    */


namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IContact;
    class IContactModel;
    class IContactModelThermal;
    class IContactMechanical;
    class IPiece;
    class IPieceThermal;

    /// \brief ContactThermal class.

    /** Thermal Contacts may exist between two IPieceThermal objects (e.g. between two balls or between a 
        * ball and a wall). They are formed and broken automatically during the course 
        * of a simulation.

        * The behavior of a material is simulated by associating a constitutive model with each 
        * thermal contact.
        * \ingroup module

      */

    class IContactThermal {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4f63385f;

        /// returns a IThing pointer
        virtual const IThing *                 getIThing() const=0;
        virtual IThing *                       getIThing()=0;
        /// returns a IContact pointer
        virtual const IContact *               getContact() const=0;
        virtual IContact *                     getContact()=0;
        /// returns a IContactThermal pointer
        virtual const IContactThermal *        getContactThermal() const =0;
        virtual IContactThermal *              getContactThermal() =0;
        /// returns a a pointer to the host mechanical contact
        virtual const IContactMechanical *     getContactMechanical() const =0;
        virtual IContactMechanical *           getContactMechanical() =0;
        /// returns a pointer to the mechanical contact first IPieceMechanical object (e.g. a ball)  
        virtual const IPiece *                 getEnd1() const=0;
        virtual IPiece *                       getEnd1()=0;
        /// returns a pointer to the mechanical contact second IPieceMechanical object (e.g. a ball)  
        virtual const IPiece *                 getEnd2() const=0;
        virtual IPiece*                        getEnd2()=0;
        /// returns a pointer to the thermal contact first IPieceThermal object (e.g. a ball)  
        virtual const IPieceThermal *          getEnd1Thermal() const=0;
        virtual IPieceThermal *                getEnd1Thermal()=0;
        /// returns a pointer to the thermal contact second IPieceThermal object (e.g. a ball)  
        virtual const IPieceThermal *          getEnd2Thermal() const=0;
        virtual IPieceThermal *                getEnd2Thermal()=0;
        /// returns a pointer to the thermal contact model present at this mechanical contact  
        virtual const IContactModel *          getModel() const=0;
        virtual IContactModel *                getModel()=0;
        /// assigns a clone of the thermal contact model passed in arguments, and copies its properties  
        virtual IContactModel *                setModel(const IContactModel *toClone,bool validate=true)=0;
        /// returns the name of the thermal contact model present at this mechanical contact    
        virtual QString                        getModelName() const=0;
        /// assigns the thermal contact model based on a name    
        virtual IContactModel *                setModelName(const QString &name)=0;
        /// returns the power (from end1 to end2) at this thermal contact, in the contact local coordinate system     
        virtual double                         getPower() const=0;
        /// sets the power (from end1 to end2) at this thermal contact to \a v, expressed in the contact local coordinate system     
        virtual void                           setPower(const double &d)=0;
        /// returns the power (from end1 to end2) at this thermal contact, in the contact local coordinate system     
        virtual double                         getTempInc() const=0;
        /// sets the power (from end1 to end2) at this thermal contact to \a v, expressed in the contact local coordinate system     
        virtual void                           setTempInc(const double &d)=0;
    };
} // namespace itascaxd
// EoF
