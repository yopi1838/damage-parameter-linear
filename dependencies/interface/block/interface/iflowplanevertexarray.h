#pragma once
// iflowplanevertexarray.h

/**
 * \file
 * \brief container to store FlowPlaneVertexThings.
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

  class IFlowPlaneVertexThing;


  /// \brief Container to store IFlowPlaneVertexThings.
  /// \ingroup THREEDEC
  class IFlowPlaneVertexArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFlowPlaneVertexArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFlowPlaneVertexArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of flow plane vertices.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of flow plane vertices.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the flow plane vertex, IFlowPlaneVertexThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual const IFlowPlaneVertexThing *      findFlowPlaneVertexWithID(quint64 id) const=0;
    /// Returns a pointer to the flow plane vertex, IFlowPlaneVertexThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual IFlowPlaneVertexThing *            findFlowPlaneVertexWithID(quint64 id)=0;
    /// Returns the flow plane vertex type identifier
    virtual quint32            getFlowPlaneVertexType() const=0;
    /// Return the tag for the flow plane extra variable change notice
    virtual uint                 getFlowPlaneVertexExtraNotice() const=0;
  };

} // end namespace block
// EOF