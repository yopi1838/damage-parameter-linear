#pragma once
// iflowzonearray.h

/**
 * \file
 * \brief container to store FlowZoneThings.
 *
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IFlowZoneThing;


  /// \brief Container to store IFlowZoneThings.
  /// \ingroup THREEDEC
  class IFlowZoneArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFlowZoneArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFlowZoneArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of flow zones.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of flow zones.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the flow zone, IFlowZoneThing, with the ID \b id, or 0 if the zone does not exist.
    virtual const IFlowZoneThing *      findFlowZoneWithID(quint64 id) const=0;
    /// Returns a pointer to the flow zone, IFlowZoneThing, with the ID \b id, or 0 if the zone does not exist.
    virtual IFlowZoneThing *            findFlowZoneWithID(quint64 id)=0;
    /// Returns the flow zone type identifier
    virtual quint32            getFlowZoneType() const=0;
    /// Returns the tag for the flow zone extra variable change notice
    virtual uint               getFlowZoneExtraNotice() const=0;
    /// Returns the tag for the flow zone group change notice
    virtual uint               getGroupNotice() const=0;
  };

} // end namespace block
// EOF