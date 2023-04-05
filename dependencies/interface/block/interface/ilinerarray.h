#pragma once
// ilinerarray.h

/**
 * \file
 * \brief Container to store LinerThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class ILinerThing;

  /// \brief This interface provides access to liner thing array.
  /// \ingroup THREEDEC
  class ILinerArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the ILinerArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the ILinerArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of liner elements.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of liner elements.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the element, IBeamThing, with the ID \b id, or 0 if the liner element does not exist.
    virtual const ILinerThing *   findLinerElementWithID(quint64 id) const=0;
    /// Returns a pointer to the element, IBeamThing, with the ID \b id, or 0 if the liner element does not exist.
    virtual ILinerThing *         findLinerElementWithID(quint64 id)=0;
    /// Returns the liner type identifier
    virtual quint32            getLinerType() const=0;
    /// Returns the tag for the liner element extra variable change notice
    virtual uint      getLinerExtraNotice() const=0;
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