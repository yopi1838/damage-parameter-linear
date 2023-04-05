#pragma once
// iflowplanearray.h

/**
 * \file
 * \brief container to store FlowPlaneThings.
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

  class IFlowPlaneThing;


  /// \brief Container to store FlowPlaneThings.
  /// \ingroup THREEDEC
  class IFlowPlaneArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFlowPlaneArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFlowPlaneArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of flow planes.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of flow planes.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the flow plane, IFlowPlaneThing, with the ID \b id, or 0 if the plane does not exist.
    virtual const IFlowPlaneThing *      findFlowPlaneWithID(quint64 id) const=0;
    /// Returns a pointer to the flow plane, IFlowPlaneThing, with the ID \b id, or 0 if the plane does not exist.
    virtual IFlowPlaneThing *            findFlowPlaneWithID(quint64 id)=0;
    /// Returns the flow plane type identifier
    virtual quint32            getFlowPlaneType() const=0;
    /// Returns the tag for the flow plane extra variable change notice
    virtual uint               getFlowPlaneExtraNotice() const=0;
    /// Returns the tag for the flow plane group change notice
    virtual uint               getGroupNotice() const=0;
    /// Returns the tag for the flow plane geometry change notice
    virtual uint      getGeometryNotice() const=0;
    /// Returns the tag for flow plane pore pressure change notice
    virtual uint      getPPNotice() const=0;
    /// Returns the tag for flow plane aperture change notice
    virtual uint      getApertureNotice() const=0;
    /// Returns tag for flow plane proppant concentration change notice
    virtual uint      getProppantNotice() const=0;
    /// Returns tag for flow plane property change notice
    virtual uint      getPropertyNotice() const=0;
  };

} // end namespace block
// EOF