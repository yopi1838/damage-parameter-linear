#pragma once
// ireinforcementarray.h

/**
 * \file
 * \brief Container to store ReinforcementThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IReinforcementThing;

  /// \brief This interface provides access to reinforcement thing array.
  /// \ingroup THREEDEC
  class IReinforcementArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IReinforcementArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IReinforcementArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of reinforcement elements.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of reinforcement elements.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the element, IReinforcementThing, with the ID \b id, or 0 if the reinforcement element does not exist.
    virtual const IReinforcementThing *   findReinforcementWithID(quint64 id) const=0;
    /// Returns a pointer to the element, IReinforcementThing, with the ID \b id, or 0 if the reinforcement element does not exist.
    virtual IReinforcementThing *         findReinforcementWithID(quint64 id)=0;
    /// Returns the reinforcement type identifier
    virtual quint32            getReinforcementType() const=0;
    /// Returns the tag for the reinforcement element extra variable change notice
    virtual uint      getReinforcementExtraNotice() const=0;
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