#pragma once
// ijointthing.h
#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 *  \brief This is the interface for JointFaceThing.
            A JointFaceThing represents a triangle face used only for plotting.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IBlockThing;
  class IContactThing;
  class ISubcontactThing;

  class IJointFaceThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x528d00bc;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return joint face thing fortran offset.
    virtual FInt getFortranOffset() const = 0;
    /// Returns the Thing id of the joint face
    virtual quint64 getJointFaceID() const=0;
    /// Return location of vertex vNum (0 - 2)
    virtual DVect3 getVertexLocation(quint32 vNum) const=0;
    /// Return displacement of vertex vNum (0 - 2)
    virtual DVect3 getVertexDisplacement(quint32 vNum) const = 0;
    /// Return the contact associated with the face
    virtual const IContactThing * getContact() const=0;
    /// Return the id of the contact associated with the face
    virtual quint64 getContactID() const=0;
    /// Return subcontacts and weights associated with vertex vNum (0 - 2)
    /// One or more subcontacts may be null
    virtual void getSubcontacts(FArray< QPair<const ISubcontactThing *, double> > *subcontacts, quint32 vNum) const=0;
    ///// Return true if joint face is on block 1 (??)
    //virtual bool             isOnBlock1() const=0;
    ///// Return true if joint face is on block 2 (??)
    //virtual bool             isOnBlock2() const=0;
    /// Return true if face is degenerate or corrupted
    virtual bool             isBad() const=0;
    /// Return true if face without subcontact
    virtual bool             isContactFace() const = 0;

    virtual uint             getSize() const = 0;
  };

} // end namespace block
// EOF