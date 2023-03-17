#pragma once
// ilinerthing.h

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for LinerThing, a C++ wrapper for liner elements.
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
  /// \brief This interface provides access to liner thing.
  ///  The fortran offset for the LinerThing ID is $KLNID in the Fortran liner data array
  /// \ingroup THREEDEC

  class ISELNodeThing;
  class IFaceThing;
  class ISelProp;

  class ILinerThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52c6d009;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Get ID of element in C++ container (not Fortran offset)
    virtual quint64 getLinerElementID() const=0;
    /// Return ID of collection of liner elements
    virtual quint64 getSELCollectionID() const=0;
    /// Return element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Returns centroid position
    virtual DVect3  getLocation() const = 0;
    /// Return the normal of the liner element
    virtual DVect3 getNormal() const=0;
    /// get node for input nodeNum (0 - 2)
    virtual const ISELNodeThing * getNode(uint nodeNum) const=0;
    /// Return stress in LOCAL coordinates.  Components are xx, yy and xy
    /// This does not return a tensor because we do not want to consider zz stress at all.
    virtual DVect3 getStress() const=0;
    /// Return moment in LOCAL coordinates.  Components are xx, yy and xy
    /// This does not return a tensor because we do not want to consider zz component at all.
    virtual DVect3 getMoment() const=0;
    /// Return minimum fiber stress (most compressive)
    virtual double getMinimumFiberStress() const=0;
    /// Return maximum fiber stress (most tensile)
    virtual double getMaximumFiberStress() const=0;
    /// Return minimum membrane stress (most compressive)
    virtual double getMinimumMembraneStress() const=0;
    /// Return maximum membrane stress (most tensile)
    virtual double getMaximumMembraneStress() const=0;
    /// Return the maximum moment
    virtual double       getMaximumMoment() const=0;
    /// Return the minimum moment 
    virtual double       getMinimumMoment() const=0;
    /// Return the coincident face
    virtual const IFaceThing * getFace() const=0;
    /// Return area
    virtual double getArea() const = 0;
    /// Return property thing
    virtual const ISelProp * getSelProp() const = 0;
  };

} // end namespace block
// EOF