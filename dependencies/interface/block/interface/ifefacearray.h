#pragma once
// ifefacearray.h

/**
 * \file
 * \brief Container to store FEFaceThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IFEFaceThing;

  /// \brief This interface provides access to beam thing array.
  /// \ingroup THREEDEC
  class IFEFaceArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFEFaceArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFEFaceArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of finite element faces.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of finite element faces.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the element, IFEFaceThing, with the ID \b id, or 0 if the finite element face does not exist.
    virtual const IFEFaceThing *   findFEFaceWithID(quint64 id) const=0;
    /// Returns a pointer to the element, IFEFaceThing, with the ID \b id, or 0 if the beam finite face does not exist.
    virtual IFEFaceThing *         findFEFaceWithID(quint64 id)=0;
    /// Returns the finite element face type identifier
    virtual quint32            getFEFaceType() const=0;
    /// Returns the tag for the finite element face extra variable change notice
    virtual uint      getFEFaceExtraNotice() const=0;
    /// Returns tag for group change notice
    virtual uint     getGroupNotice() const=0;
  };

} // end namespace block
// EOF