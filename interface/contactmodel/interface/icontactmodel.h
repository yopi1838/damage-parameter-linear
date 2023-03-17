#pragma once
// icontactmodel.h
/** \brief Interface for contact models.
    * \file icontactmodel.h
    *
    * \defgroup contactmodel Contact model interface specification
*/

#include "module/interface/dim.h"
#include "base/src/baseqt.h"

namespace itascaxd
{
    // Local coordinate system: x = normal
    //                   y(z) = component in contact plane.
    // Needs force (DVect), moment (DAVect).
    // Relative surface translation and angular velocities.
    //            (DVect, DAVect).
    // Timestep
    // Contact gap (double).
    // Local Coordinate system (optional).
    class IContact;
    /** * \brief 
        * Interface for contact models.
        * \ingroup contactmodel
        */
    
    class IContactModel
    {
    public:
        /// Return the IContactModel pointer. Useful in derived classes.
        virtual IContactModel * getContactModel()=0;
        /// Return the const IContactModel pointer. Useful in derived classes.
        virtual const IContactModel * getContactModel() const =0;
        /// Return the name of the contact model. The contact model will be identified by this name in commands/FISH.
        virtual QString  getName() const=0;
        /// Return an index corresponding to this contact model. This is defined by the ContactModelList for easy lookup.
        /// This should return a static index.
        virtual int      getIndex() const=0;
        /// Set an index corresponding to this contact model. This is defined by the ContactModelList for easy lookup. A static
        /// index should be kept.
        virtual void     setIndex(int i) =0;
        /// Return the distance for contact activity. 
        virtual double   getActivityDistance() const=0;
        /// Get the comma delimited list of contact model properties.
        virtual QString  getProperties() const=0; // comma delimited
        /// Return the index (>0) of a propery if it is found. Returns 0 if not found. 
        /// All checks should be case insensitive.
        virtual int      isProperty(const QString &c,Qt::CaseSensitivity cs=Qt::CaseInsensitive) const=0; // checks the list for a match, returns the integer (>0) if a match is found
        /// Returns the value of a property with index \a i (from the isProperty method). This is base 1 (i.e., the first property has index 1).
        virtual QVariant getProperty(uint i,const IContact *con=0) const=0;  // Base 1
        /// Returns a boolean indicating if the property with index \a i (from the isProperty method) is in the local or global 
        /// coordinate system. This is base 1 (i.e., the first property has index 1).
        virtual bool     getPropertyGlobal(uint i) const=0;  // Base 1 - returns a boolean indicating if this is in the local or global coordinate system
        /// Set a property with index \a i (from the isProperty method). Returns a Boolean indicating if the timestep 
        /// contribution of this contact model needs to be updated. 
        virtual bool     setProperty(uint i,const QVariant &v,IContact *con=0)=0; // Base 1 - returns true if timestep contributions need to be updated
        /// Returns a Boolean indicating if a property with index \a i (from the isProperty method) is read only.
        virtual bool     getPropertyReadOnly(uint i) const=0; // Base 1
        /// Returns a Boolean indicating if a property with index \a i (from the isProperty method) supports inheritance.
        virtual bool     supportsInheritance(uint i) const=0; // Base 1
        /// Returns a Boolean indicating whether inheritance is on or off for the property with index \a i (from the isProperty method).
        virtual bool     getInheritance(uint i) const=0; 
        /// Sets the inheritance on or off for the property with index \a i (from the isProperty method). 
        virtual void     setInheritance(uint i,bool b)=0;
    
        /// Get the comma delimited list of method names.
        virtual QString  getMethods() const=0; // comma delimited
        /// Get the index of the exact match of a method with name \a c. 
        /// Returns the index (>0) if a match is found. 
        virtual int      isMethod(const QString &c,Qt::CaseSensitivity cs=Qt::CaseInsensitive) const=0; // checks the list for a match, returns the integer (>0) if a match is found
        /// Get the method arguments for method with index \a i (from the isMethod method).
        virtual QString  getMethodArguments(uint i) const=0;  // comma delimited
        /// Set the method arguments for method with index \a i (from the isMethod method).
        virtual bool     setMethod(uint i,const QVector<QVariant> &vl,IContact *con=0)=0; // Base 1 - returns true if timestep contributions need to be updated
    
        /// Get the comma delimited list of energy names.
        virtual QString  getEnergies() const=0; // comma delimited
        /// Get the index of the exact match of an energy with name \a c. 
        /// Returns the index (>0) if a match is found. 
        virtual int      isEnergy(const QString &c,Qt::CaseSensitivity cs=Qt::CaseInsensitive) const=0; // checks the list for a match, returns the integer (>0) if a match is found
        /// Get the energy with index \a i (from the isEnergy method).
        virtual double   getEnergy(uint i) const=0;  // Base 1
        /// Get whether or not the energy with index \a i (from the isEnergy method) is accumulated (like a work term) or instantaneous.
        virtual bool     getEnergyAccumulate(uint i) const=0;  // // Base 1 - returns a boolean indicating if this is an accumulated (work) or instantaneous quantity (energy)
        /// Set the energy with index \a i (from the isEnergy method).
        virtual void     setEnergy(uint i,const double &d)=0; // Base 1
        /// Method to initialize energy calculations in the contact model.
        virtual void     activateEnergy()=0;
    
        /// Get the minor version of the contact model.
        virtual uint     getMinorVersion() const=0;
        /// Set the non-force related contact model properties from \a oldCM. This is used to clone contacts
        /// such as the case of proximity contacts with walls. 
        virtual void     setNonForcePropsFrom(IContactModel *oldCM)=0;
        
        /// Hooked up to \a delete \a this at the Contact level.  
        virtual void destroy()=0;
    
        /// Return a list of spheres for custom plotting with the \a specific contact numeric plot entry. 
        virtual void getSphereList(const IContact *con,std::vector<DVect> *pos,std::vector<double> *rad,std::vector<double> *val)=0;
#ifdef THREED
        /// Return a list of disks for custom plotting with the \a specific contact numeric plot entry. 
        virtual void getDiskList(const IContact *con,std::vector<DVect> *pos,std::vector<DVect> *normal,std::vector<double> *radius,std::vector<double> *val)=0;
#endif
        /// Return a list of cylinders for custom plotting with the \a specific contact numeric plot entry. 
        virtual void getCylinderList(const IContact *con,std::vector<DVect> *bot,std::vector<DVect> *top,std::vector<double> *radlow,std::vector<double> *radhi,std::vector<double> *val)=0;

    };
} // namespace itascaxd
// EoF
