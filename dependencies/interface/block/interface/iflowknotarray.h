#pragma once
// iflowknotarray.h

/**
 * \file
 * \brief container to store FlowKnotThings.
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

  class IFlowKnotThing;


  /// \brief Container to store FlowKnotThings.
  /// \ingroup THREEDEC
  class IFlowKnotArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFlowKnotArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFlowKnotArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of flow knots.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of flow knots.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the flow knot, IFlowKnotThing, with the ID \b id, or 0 if the knot does not exist.
    virtual const IFlowKnotThing *      findFlowKnotWithID(quint64 id) const=0;
    /// Returns a pointer to the flow knot, IFlowKnotThing, with the ID \b id, or 0 if the knot does not exist.
    virtual IFlowKnotThing *            findFlowKnotWithID(quint64 id)=0;
    /// Returns the flow knot type identifier
    virtual quint32            getFlowKnotType() const=0;
    /// Return knot closest to input location
    virtual const IFlowKnotThing *     findKnot(const DVect3 & dv) const=0;
    /// Returns the tag for the flow knot extra variable change notice
    virtual uint               getFlowKnotExtraNotice() const=0;
    /// Returns the tag for the flow knot group change notice
    virtual uint               getGroupNotice() const=0;
    /// Returns the tag for the flow knot pore pressure change notice
    virtual uint                 getPorePressureNotice() const=0;
    /// Returns the tag for the flow knot temperature change notice
    virtual uint      getTemperatureNotice() const=0;
    /// Returns the tag for flow knot discharge change notice
    virtual uint      getDischargeNotice() const=0;
  };

} // end namespace block
// EOF