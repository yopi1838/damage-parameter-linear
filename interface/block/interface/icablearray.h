#pragma once
// icablearray.h

/**
 * \file
 * \brief Container to store CableThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class ICableThing;

  /// \brief This interface provides access to cable thing array.
  /// \ingroup THREEDEC
  class ICableArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the ICableArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the ICableArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of cable elements.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of cable elements.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the element, ICableThing, with the ID \b id, or 0 if the cable element does not exist.
    virtual const ICableThing *   findCableElementWithID(quint64 id) const=0;
    /// Returns a pointer to the element, ICableThing, with the ID \b id, or 0 if the cable element does not exist.
    virtual ICableThing *         findCableElementWithID(quint64 id)=0;
    /// Returns the cable type identifier
    virtual quint32            getCableType() const=0;
    /// Returns the tag for the cable element extra variable change notice
    virtual uint      getCableExtraNotice() const=0;
    /// Returns the tag for reinforcement element geometry change notice
    virtual uint      getGeometryNotice() const=0;
    /// Returns tag for group change notice
    virtual uint     getGroupNotice() const=0;
    /// Returns the tag for model change notice
    virtual uint     getModelNotice() const=0;
    /// Returns the tag for material change notice
    virtual uint     getMaterialNotice() const=0;
  };

} // end namespace block
// EOF