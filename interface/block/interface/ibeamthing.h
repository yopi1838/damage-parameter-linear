#pragma once
// ibeamthing.h

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for BeamThing, a C++ wrapper for beam elements.
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
  /// \brief This interface provides access to beam thing.
  ///  The fortran offset for the BeamThing ID is $KELID in the Fortran cable/beam data array
  /// \ingroup THREEDEC

  class ISELNodeThing;
  class ISelProp;

  class IBeamThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52c5c58f;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Get ID of element in C++ container (not Fortran offset)
    virtual quint64 getBeamElementID() const=0;
    /// Return ID of collection of SEL elements
    virtual quint64 getSELCollectionID() const=0;
    /// Return element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Returns centroid position
    virtual DVect3  getLocation() const=0;
    /// Returns directions
    virtual DVect3  getNormal() const = 0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Return the axial force in beam element (tension positive)
    virtual double getAxialForce() const=0;
    /// Return the axial stress
    virtual double getAxialStress() const=0;
    /// Return the axial strain
    virtual double getAxialStrain() const = 0;
    /// Return thermal force
    virtual double getThermalForce() const = 0;
    /// Return beam element length
    virtual double getLength() const=0;
    /// return beam element state string
    virtual QString getStateString() const=0;
    // get node for input nodeNum (0 - 1)
    virtual const ISELNodeThing * getNode(uint nodeNum) const=0;
    /// return pointer to SEL property object
    virtual ISelProp * getSelProp() const = 0;

  };

} // end namespace block
// EOF