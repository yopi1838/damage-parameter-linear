#pragma once
// ibeamarray.h

/**
 * \file
 * \brief Container to store SelProps.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  /// This interface provides access to SelProp array.
  class ISelPropArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the ISelPropArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the ISelPropArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of selprops.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of selprops.
    virtual IContainer *            getIContainer() =0;
  };

} // end namespace block
// EOF