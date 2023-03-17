#pragma once
// idowelthing.h

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for DowelThing, a C++ wrapper for cable element dowel segments.
*/

/*! \brief namespace Itasca
 *
 */
namespace itasca
{
    class IThing;
}

namespace block
{
    /// \brief This interface provides access to reinforcement thing.
    ///  The fortran offset for the ReinforcementThing ID is $KELHCPP in the Fortran dowel data array
    /// \ingroup THREEDEC

    class ICableThing;
    class IFaceThing;
    class ISelProp;

    class IDowelThing
    {
    public:

        /// type used in IThing typing system
        inline static const TType type_ = 0x5bf865e8;
        /// Returns a const IThing pointer
        virtual const itasca::IThing *      getIThing() const = 0;
        /// Returns a IThing pointer
        virtual itasca::IThing *            getIThing() = 0;
        /// Get ID of element in C++ container (not Fortran offset)
        virtual quint64 getDowelID() const = 0;
        /// Get host cable
        virtual const ICableThing *getHostCable() const=0;
        /// Return element fortran offset. 
        virtual FInt getFortranOffset() const = 0;
        /// Returns element position
        virtual DVect3              getLocation() const = 0;
        /// Return the shear displacement vector
        virtual DVect3 getShearDisplacement() const = 0;
        /// Return the shear force vector
        virtual DVect3 getShearForce() const = 0;
        /// Return the shear strain
        virtual double getShearStrain() const = 0;
        /// Return the normal vector
        virtual DVect3 getNormal() const = 0;
        /// return reinforcement state string
        virtual QString getStateString() const = 0;
        /// return reinforcement state integer
        // 2 shear yield
        // 5 shear rupture
        virtual int getState() const = 0;
        /// Return the half-length ("active" length) of the element (from property)
        virtual double getHalfLength() const = 0;
        /// return sel property thing
        virtual const ISelProp * getSelProp() const=0;
        /// Get shear factor (0-1)
        virtual double  getSFac() const=0;
        /// Set shear factor (0-1)
        virtual void setSFac(double d)=0;
        
    };

} // end namespace block
// EOF