#pragma once
// ireinforcementthing.h

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for ReinforcementThing, a C++ wrapper for reinforcement elements.
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
  ///  The fortran offset for the ReinforcementThing ID is $KRECPP in the Fortran reinforcement data array
  /// \ingroup THREEDEC

  class IBlockThing;
  class IContactThing;
  class IFaceThing;

  class IReinforcementThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52b3460a;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Get ID of element in C++ container (not Fortran offset)
    virtual quint64 getReinforcementID() const=0;
    /// Return ID of collection of reinforcement elements
    virtual quint64 getSELCollectionID() const=0;
    /// Return element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Returns element position
    virtual DVect3              getLocation() const=0;
    /// Return first block connected
    virtual const IBlockThing * getBlock1() const=0;
    /// Return second block connected
    virtual const IBlockThing * getBlock2() const=0;
    /// Return first face connected
    virtual const IFaceThing * getFace1() const=0;
    /// Return second face connected
    virtual const IFaceThing * getFace2() const=0;
    /// Return contact associated with reinforcement element
    virtual const IContactThing *getContact() const=0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Return the shear displacement vector
    virtual DVect3 getShearDisplacement() const=0;
    /// Return the shear force vector
    virtual DVect3 getShearForce() const=0;
    /// Return the shear strain
    virtual double getShearStrain() const = 0;
    /// Return the normal displacement
    virtual double getAxialDisplacement() const=0;
    /// Return the normal force
    virtual double getAxialForce() const=0;
    /// Return the axial strain
    virtual double getAxialStrain() const = 0;
    /// Return the normal vector
    virtual DVect3 getNormal() const=0;
    /// return reinforcement state string
    virtual QString getStateString() const=0;
    /// return reinforcement state integer
    // 1 yield in past
    // 2 shear yield
    // 3 axial yield
    // 4 axial rupture
    // 5 shear rupture
    virtual int getState() const = 0;
    /// Return the half-length ("active" length) of the element (from property)
    virtual double getHalfLength() const=0;
    // other properties
    virtual double getAxialStiffness() const=0;
    virtual double getShearStiffness() const=0;
    virtual double getAxialYield() const=0;
    virtual double getShearYield() const=0;
    virtual double getAxialRuptureStrain() const=0;
    virtual double getShearRuptureStrain() const=0;
    // get axial and shear force as a percentage of strength
    virtual double getShearForcePcStrength() const=0;
    virtual double getAxialForcePcStrength() const=0;
  };

} // end namespace block
// EOF