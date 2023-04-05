#pragma once
// iselnodething.h

#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 * \brief This is the interface for SELNodeThing, a wrapper class for all structural element nodes
*/

namespace itasca
{
  class IThing;
}

namespace block
{
    class ICableThing;
    class IBeamThing;
    class ILinerThing;
    class ISelProp;

  /// \brief This is the interface for SELNodeThing, a class that holds extra data not in the fortran arrays.
  ///           This will be stored in a container and the ID will be stored in
  ///           the corresponding offset $xxCPP in the appropriate fortran array
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class ISELNodeThing
  {
  public:

    /// Enum of possible types
    enum SELType {TypeBeam=1, TypeCable, TypeLiner};

    /// type used in IThing typing system
    inline static const TType type_ = 0x52c31f5a;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return Thing ID
    virtual quint64 getSELNodeID() const=0;
    /// Return ID of collection of SEL elements
    virtual quint64 getSELCollectionID() const=0;
    /// Return node fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Returns SEL Node position
    virtual DVect3              getLocation() const=0;
    /// Return the mass associated with this node
    virtual double       getMass() const=0;
    /// Returns SEL Node displacement
    virtual DVect3              getDisplacement() const=0;
    /// Returns SEL Node axial displacement
    virtual double              getAxialDisplacement() const=0;
    /// Returns SEL Node velocity
    virtual DVect3              getVelocity() const=0;
    /// Set SEL Node velocity
    virtual void              setVelocity(const DVect3 &vel)=0;
    /// Returns SEL Node force sum
    virtual DVect3              getForce() const=0;
    /// Return the shear force magnitude
    virtual double getShearForce() const=0;
    /// Returns the load (force) applied to the SEL Node
    virtual DVect3       getAppliedLoad() const=0;
    /// Set the load (force) applied to the SEL Node
    virtual void       setAppliedLoad(const DVect &force) const=0;
    /// Return the normal force at the contact (beams and liners only)
    virtual double getContactNormalForce() const=0;
    /// Return the shear force at the contact (beams and liners only)
    virtual DVect3 getContactShearForce() const=0;
    /// Return the normal stress at the contact (beams and liners only)
    virtual double getContactNormalStress() const = 0;
    /// Return the shear stress at the contact (beams and liners only)
    virtual DVect3 getContactShearStress() const = 0;
    /// Return the normal displacement at the contact (beams and liners only)
    virtual double getContactNormalDisplacement() const=0;
    /// Return the shear displacement at the contact (beams and liners only)
    virtual DVect3 getContactShearDisplacement() const=0;
    /// return x,y,z components of moment (beams only).  Local coords?
    virtual DVect3       getMoment() const=0;
    /// Return the maximum moment (beams only - for liners this is element based)
    virtual double       getMaximumMoment() const=0;
    /// Return the minimum moment (beams only - for liners this is element based)
    virtual double       getMinimumMoment() const=0;
    /// Return the confining stress on the grout (cables only)
    virtual double getConfiningStress() const=0;
    /// return true if fixed, false otherwise
    virtual bool getIsFixed() const=0;
    /// Return true if attached, false otherwise
    virtual bool         getIsAttached() const=0;
    /// Set fix condition (true for fixed, false for free)
    virtual void         setFixed(bool b)=0;
    /// Set attach condition (true for attached, false otherwise)
    virtual void         setAttached(bool b)=0;
    /// Get node state: 1 = Intact, 2 = Broken, 3 = no bond
    virtual int          getState() const = 0;
    /// Return the state as a string (bond intact or broken)
    virtual QString     getStateString() const=0;
    /// Return the bond state as a number (1 intact, 2 broken, 3 absent)
    virtual int          getBond() const=0;
    /// Set the bond state (1 intact, 2 broken, 3 absent)
    virtual void         setBond(int i)=0;
    /// Return the type of SEL Node (beam, cable or liner)
    virtual SELType       getSELType() const=0;
    /// get effective length associated with this node (beam or cable)
    virtual double getEffectiveLength() const=0;
    /// Return pointer to structural element property data
    virtual const ISelProp * getSelProp() const=0;

    // need to come up with a better way to do these!

    /// Return associated cable elements.  Empty array if none
    virtual FArray<ICableThing *> getCableElements() const=0;
    /// Return associated beam elements.  Empty array if none
    virtual FArray<IBeamThing *> getBeamElements() const=0;
    /// Return associated liner elements.  Empty array if none
    virtual FArray<ILinerThing *> getLinerElements() const=0;
    /// Get ID of next node in list
    virtual quint64 getNextID() const=0;
    /// get material ID
    virtual int getMaterial() const=0;
    /// Set material ID
    virtual void setMaterial(int i)=0;
  };

} // end namespace block
// EOF