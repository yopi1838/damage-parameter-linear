#pragma once

#include "dim.h"
#include "base/src/callback.h"
/**
    * \file
    * \brief Contact container class.
    */


namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;
    class IContact;
    class IContactModel;
    class IPiece;
    class ContactClassDataBase;
    class IContainerActiveContact;
    class MonitorBase;
    /// \brief Contact container class.

    /** Contact container class.
        * \ingroup module
      */
    class IContainerContact {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x04c815ba7;

        /// returns a pointer to the IContainer class representing a list of contacts.
        virtual IContainer *      getIContainer()=0;
        virtual const IContainer *getIContainer() const=0;

        /// returns a pointer to the IContainer class representing a list of all contacts.
        virtual IContainer *      getContainerAllContact()=0;
        virtual const IContainer *getContainerAllContact() const=0;
         
        /// returns a pointer to the IContainer class representing a list of active contacts.
        virtual IContainerActiveContact *      getContainerActiveContact()=0;
        virtual const IContainerActiveContact *getContainerActiveContact() const=0;

        /// returns a pointer to the Piece containers representing each end of the contact
        virtual IContainer *      getEnd1Container()=0;
        virtual IContainer *      getEnd2Container()=0;
        virtual MonitorBase *     getMonitorBase()=0;

        /// Returns the TType of the contact.
        virtual TType             getType() const=0;
        /// Returns the container name.
        virtual QString           getName() const=0;
        /// Returns the contact class name.
        virtual QString           getContactClassName() const=0;
        /// Returns the contact type name.
        virtual QString           getTypeName() const=0;
        /// Returns the TType of the contact.
        virtual TType             getContactType() const=0;
        /// Returns the minimum distance.
        virtual double            getMinDistance() const=0;
        /// Returns true if the contact is mechanical.
        virtual bool              isMechanical() const=0;
        /// Returns true if the contact is mechanical.
        virtual bool              isThermal() const=0;
        /// Register the contact model \a cmodel with this contact container.
        /// Effectively set the container to be interested in the contact model,
        //  meaning that the contact model will be assigned to this type of contact when prompted.
        virtual void              registerContactModel(const QString &cmodel)=0;
        /// Returns true if the contact model can be assigned to this type of contact
        virtual bool              amInterestedIn(const QString &cmodel) const=0;
        /// Updates the state of all contacts in the container.
        virtual void              clean(bool addStiffness)=0;
        /// Set the contact class data pointer.
        virtual void              setClassData(ContactClassDataBase *) =0;
        /// Get the contact class data pointer.
        virtual ContactClassDataBase *getClassData() =0;
        /// Get the contact container energy contribution 
        virtual double            getEnergy(const QString &name) const = 0;
        /// Get the contact between ends with id1 and id2 - The order matters if the end pieces
        /// have different types.
        virtual const IContact *  findContactWithEndIds(quint64 id1,quint64 id2) const = 0;
        /// Specify that the contact is inhibited or not inhibited. This operation puts
        /// inhibited contacts on a separate list
        virtual void              setInhibit(IThing *t,bool b) = 0;
        /// Return the inhibited contacts
        virtual IContainer *      getInhibitedContacts() = 0;
        virtual const IContainer *      getInhibitedContacts() const = 0;


        /// Interface access to the Multicontainer createBlock function
        virtual quint64 createBlocks() = 0;
        /// Interface access to the Multicontainer getNumberOfBlocks function
        virtual quint64 getBlocks() = 0;

        /// Object for creation of multithreaded loop over bodies
        virtual void threadedCallbackObject(Callback4<void,IContact *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) =0;

        /// Implementation of the multithreaded loop using the threadedCallbackObject  
        template <class S,void (S::*MFP)(IContact *,quint32,quint32,void *)>
            void threadedCallbackMethod(S *s,void *v=nullptr,bool useThreads=true) {
                Callback4Method<void,IContact *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObject(callback,v,useThreads);
            }

    };

} // namespace itasca
// EoF
