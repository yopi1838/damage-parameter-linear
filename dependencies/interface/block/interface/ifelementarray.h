#pragma once
// ifelementarray.h

/**
 * \file
 * \brief Container to store FElementThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IFElementThing;

  /// \brief This interface provides access to beam thing array.
  /// \ingroup THREEDEC
  class IFElementArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFElementArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFElementArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of finite elements.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of finite elements.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the element, IFElementThing, with the ID \b id, or 0 if the finite element does not exist.
    virtual const IFElementThing *   findFElementWithID(quint64 id) const=0;
    /// Returns a pointer to the element, IFElementThing, with the ID \b id, or 0 if the beam finite does not exist.
    virtual IFElementThing *         findFElementWithID(quint64 id)=0;
    /// Returns the finite element type identifier
    virtual quint32            getFElementType() const=0;
    /// Returns the tag for the finite element extra variable change notice
    virtual uint      getFElementExtraNotice() const=0;
    /// Returns tag for group change notice
    virtual uint     getGroupNotice() const=0;
  };

} // end namespace block
// EOF