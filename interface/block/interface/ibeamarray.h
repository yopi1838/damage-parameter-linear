#pragma once
// ibeamarray.h

/**
 * \file
 * \brief Container to store BeamThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IBeamThing;

  /// \brief This interface provides access to beam thing array.
  /// \ingroup THREEDEC
  class IBeamArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IBeamArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IBeamArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of beam elements.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of beam elements.
    virtual IContainer *            getIContainer() =0;
    /// Returns the beam type identifier
    virtual quint32            getBeamType() const=0;
    /// Returns a const pointer to the element, IBeamThing, with the ID \b id, or 0 if the beam element does not exist.
    virtual const IBeamThing *   findBeamElementWithID(quint64 id) const=0;
    /// Returns a pointer to the element, IBeamThing, with the ID \b id, or 0 if the beam element does not exist.
    virtual IBeamThing *         findBeamElementWithID(quint64 id)=0;
    /// Returns the tag for the beam element extra variable change notice
    virtual uint      getBeamExtraNotice() const=0;
    /// Returns the tag for beam element geometry change notice
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