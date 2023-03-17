#pragma once
// iselnodearray.h

/**
 * \file
 * \brief container to store SELNodeThings.
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

  class ISELNodeThing;


  /// \brief Container to store SELNodeThings.
  ///  ID of SELNodeThing is stored in id offset of Fortran linked list
  /// (42 for beams and cables, 21 for liner nodes)
  /// \ingroup THREEDEC
  class ISELNodeArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the ISELNodeArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the ISELNodeArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of SEL nodes.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of SEL nodes.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the SEL node, ISELNodeThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual const ISELNodeThing *      findSELNodeWithID(quint64 id) const=0;
    /// Returns a pointer to the vertex, ISELNodeThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual ISELNodeThing *            findSELNodeWithID(quint64 id)=0;
    /// Returns the SEL node type identifier
    virtual quint32            getSELNodeType() const=0;

    /// Returns the tag for the SEL node extra variable change notice
    virtual uint               getSELNodeExtraNotice() const=0;
    ///// Returns the tag for the SEL node displacement change notice
    //virtual uint      getDisplacementNotice() const=0;
    ///// Returns the tag for the SEL node velocity change notice
    //virtual uint      getVelocityNotice() const=0;
    /// Returns the tag for the SEL node hidden change notice
    virtual uint                 getHideNotice() const=0;
        /// Returns the tag for the SEL node group change notice
    virtual uint                 getGroupNotice() const=0;
  };

} // end namespace block
// EOF