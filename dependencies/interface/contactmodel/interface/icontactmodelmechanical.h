#pragma once
// icontactmodelmechanical.h
/** \brief Interface for mechanical contact models.
    * \file icontactmodelmechanical.h
    *
    * \addtogroup contactmodel Mechanical contact model interface specification
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
    class IContactModel;
    class IContact;
    /** * \brief 
      * Interface for mechanical contact models.
      * \ingroup contactmodel
      */
    class IContactModelMechanical
    {
    public:        
        /// Return the IContactModel pointer. Useful in derived classes.
        virtual IContactModel * getContactModel()=0;
        /// Return the const IContactModel pointer. Useful in derived classes.
        virtual const IContactModel * getContactModel() const =0;
        /// Return a Boolean indicating that it is OK to delete the contact. This is useful if
        /// the contact should persist.
        virtual bool isOKToDelete() const=0;

        /// Return the mechanical sliding state.
        virtual bool     isSliding() const=0;
        /// Return the mechanical bonding state.
        virtual bool     isBonded() const=0;
        /// Unbond the contact model.
        virtual void     unbond()=0;
        /// Return whether or not the contact model defined its own normal different from the
        /// contact normal. This is used for plotting.
        virtual bool     hasNormal() const=0;
        /// Return the contact model normal. This is used for plotting.
        virtual DVect3   getNormal() const=0;
        /// Propagate the contact model state information. Used for ball-facet contacts
        /// with reduced or full resolution. 
        virtual void     propagateStateInformation(IContactModelMechanical* oldCm,const CAxes &oldSystem=CAxes(),const CAxes &newSystem=CAxes())=0;
        /// For mechanical contact models the activity is ONLY based on distance! Everything else must be
        /// checked in the FD law when the contact state is full!
        virtual double   getActivityDistance() const =0;
        /// Reset the forces and moments in the contact model - this happens when the contact has become inactive.
        virtual void     resetForcesAndMoments() =0;
        /// Set the force (in the local coordinate system)
        virtual void     setForce(const DVect &,IContact *) = 0;
        /// Set the area used in the contact model. Note that a model clean should be called after this to update the derived quantities. 
        virtual void     setArea(const double &) = 0;
        /// Get the area used in the contact model. 
        virtual double   getArea() const = 0;
    };
} // namespace itascaxd
// EoF
