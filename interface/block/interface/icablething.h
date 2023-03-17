#pragma once
// icablething.h

#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 *  \brief This is the interface for CableThing, a C++ wrapper for cable elements.
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
  /// \brief This interface provides access to cable thing.
  ///  The fortran offset for the CableThing ID is $KELID in the Fortran cable/beam data array
  /// \ingroup THREEDEC

  class ISELNodeThing;
  class ISelProp;
  class IDowelThing;

  class ICableThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52c57fbf;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Get ID of element in C++ container (not Fortran offset)
    virtual quint64 getCableElementID() const=0;
    /// Return ID of collection of SEL elements
    virtual quint64 getSELCollectionID() const=0;
    /// Returns centroid position
    virtual DVect3  getLocation() const=0;
    /// Returns unit vector along cable length
    virtual DVect3           getNormal() const = 0;
    /// Return element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Return the axial force in bolt element (tension positive)
    virtual double getAxialForce() const=0;
    /// Return the axial stress (in bolt)
    virtual double getAxialStress() const=0;
    /// Return the axial strain (in bolt)
    virtual double getAxialStrain() const=0;
    /// Return thermal force
    virtual double getThermalForce() const = 0;
    /// Return cable element length
    virtual double getLength() const=0;
    /// return cable element state string
    virtual QString getStateString() const=0;
    // get node for input nodeNum (0 - 1)
    virtual const ISELNodeThing * getNode(uint nodeNum) const=0;
    /// Return pointer to structural element property data
    virtual const ISelProp * getSelProp() const=0;
    /// Return list of dowels associated with this cable segment
    virtual FArray<const IDowelThing*> getDowels() const=0;
    // get axial force as a percentage of strength
    virtual double getAxialForcePcStrength() const = 0;
  };

} // end namespace block
// EOF