#pragma once
// icontactthing.h

#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 *  \brief This is the interface for ContactThing, a class that holds extra data not in the fortran arrays.
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
  /// \brief This interface provides linked list offsets to access to contact thing.
  ///  The offset for the ContactThing ID is cpp below and also $KCCPP in the Fortran contact data array
  /// \ingroup THREEDEC

  class IBlockThing;
  class ISubcontactThing;
  class IFlowPlaneThing;

  class IContactThing
  {
  public:

    /// enumeration of possible contact types
    enum ContactType {Null=0, FaceFace, FaceEdge, FaceVertex, EdgeEdge, EdgeVertex, VertexVertex, Joined};
    /// enumeration of possible energy types
    enum StrainEnergyType {ESTotal=1, ESTensile, ESCompressive, ESShear};

    /// offset for ContactThing ID
    static const FInt cpp   = 38; // C++ extension from Fortran array

    /// type used in IThing typing system
    inline static const TType type_ = 0x4f54d324;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Get ID of contact in C++ container (not Fortran offset)
    virtual quint64 getContactID() const=0;
    /// Return contact fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Returns contact position
    virtual DVect3              getLocation() const=0;
    /// Returns contents of contact first spare offset
    virtual double              getSpare1() const=0;
    /// Returns ID of DFN used to create contact. 0 if DFN was not used
    virtual quint64              getDFN_ID() const=0;
    /// Returns name of DFN used to create contact.  Empty string if DFN was not used
    virtual QString    getDFN_name() const = 0;
    /// Returns true if contact or either host block is hidden
    virtual bool                getHidden() const=0;
    /// Return contact type.
    virtual ContactType getContactType() const=0;
    /// Return original contact type.
    virtual ContactType getOriginalContactType() const = 0;
    /// Return block 1 of the contact
    virtual const IBlockThing * getBlock1() const=0;
    /// Return block 2 of the contact
    virtual const IBlockThing * getBlock2() const=0;
    /// Return the id of block 1 of the contact
    virtual quint64 getBlock1_ID() const=0;
    /// Return the id of block 2 of the contact
    virtual quint64 getBlock2_ID() const=0;
    /// Get array of SubcontactThings that make up this contact.
    virtual FArray<const ISubcontactThing *> getSubcontacts() const=0;
    /// Return joint set or dfn number used to create contact
    virtual int getJointSet() const=0;
    /// Return ID of DFN fracture associated with contact
    /// Return 0 if contact is not part of a dfn
    virtual quint64 getFractureID() const=0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Return constitutive model number
    virtual int getConstitutive() const=0;
    /// Return the normal vector
    virtual DVect3 getNormal() const=0;
    /// Return area
    virtual double getArea() const=0;
    /// Returns true if either host block is marked as excavated
    virtual bool      isExcavated() const=0;
    /// Returns true if either host block is removed
    virtual bool      isRemoved() const=0;
    /// Get Flow plane associated with this contact
    virtual const IFlowPlaneThing * getFlowPlane() const=0;
    /// Return type of contact as a string
    virtual QString getTypeString() const = 0;
    /// Return strain energy for input type
    virtual double getStrainEnergy(StrainEnergyType t) const = 0;
    /// Return the frictional energy
    virtual double getFrictionEnergy() const = 0;
    /// Return first fblockthing (nullptr if not a faceblock)
    virtual IBlockThing*         getFaceBlock1() const = 0;
    /// Return second fblockthing (nullptr if not a faceblock)
    virtual IBlockThing*         getFaceBlock2() const = 0;
    /// Triangulate subcontacts for plotting
    virtual void triangulate()=0;
    /// Triangulate subcontacts for plotting
    virtual void triangulateJoined() = 0;
    /// Find contact face for plotting
    virtual void findContactFace() = 0;
    /// Return true if this contact has subcontacts
    virtual bool hasSubcontacts() const = 0;
  };

} // end namespace block
// EOF