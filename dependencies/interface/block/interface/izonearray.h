#pragma once
// izoneaarray.h

/**
 *  \file
 *  \brief Container to store ZoneThings.
 *
 */


#include "iblockdef.h"

namespace itasca
{
  class IContainer;
  class IThing;
}
namespace itascaxd 
{
    class IRange;
}

namespace block
{
  using namespace itasca;

  class IZoneThing;
  class IZoneData;

  /// \brief Container to store ZoneThings.
  ///  ID of BlockThing is stored in offset $KZCPP in Fortran zone linked list
  /// \ingroup THREEDEC
  class IZoneArray
  {
  public:
    /// Returns a const pointer to an IThing interface representing the IZoneArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IZoneArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer
    virtual IContainer *       getIContainer() =0;
     /// Returns a const pointer to the zone, IZone, with the ID \b id, or 0 if the zone does not exist.
    virtual const IZoneThing *      findZoneWithID(quint64 id) const=0;
    /// Returns a pointer to the zone, IZone, with the ID \b id, or 0 if the zone does not exist.
    virtual IZoneThing *            findZoneWithID(quint64 id)=0;
    /// Returns the zone type identifier
    virtual quint32            getZoneType() const=0;
    /// Returns the zone face type identifier
    virtual quint32            getZoneFaceType() const=0;
    /// Returns the number of zones
    virtual quint64 getNumZones() const=0;

    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimBit() returns a bit index that can be used with the IZone::getBit() and
    ///   IZone::setBit() functions.
    /// This function is thread-safe (IZone::getBit and IZone::setBit are not).
    /// \sa releaseBit() IZone::setBit() IZone::getBit()
    /// \brief Claims a bit from the zones utility bit field.
    virtual int                claimBit() const=0;
    /// \brief Releases a bit index previously obtained with claimBit().  Thread-safe.
    virtual void               releaseBit(int bit) const=0;
    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimValue() returns a value index that can be used with the IZone::getValue() and
    ///   IZone::setValue() functions.
    /// This function is thread-safe (IZone::getValue and IZone::setValue are not).
    /// \sa releaseBit() IZone::setValue() IZone::getValue()
    /// \brief Claims a bit from the zones utility value array.
    virtual int                claimValue() const=0;
    /// \brief Releases a value index back to the zones utility value array.  Thread-safe.
    virtual void               releaseValue(int bit) const=0;
    /// Returns the pointer to IZoneData. This pointer must be destroyed.
    /// The IZoneData object can be used to retrieve scalar data from a zone.
    virtual IZoneData *        createIZoneData(QObject *parent=0) const=0;
    /// Returns the tag for the zone extra variable change notice
    virtual uint               getZoneExtraNotice() const=0;
    /// Returns the tag for the zone group change notice
    virtual uint               getGroupNotice() const=0;
    /// Returns the tag for the zone geometry change notice
    virtual uint                getGeometryNotice() const=0;
    /// Returns the tag for the zone stress change notice
    virtual uint                getStressNotice() const=0;
    /// Returns the tag for the zone property change notice
    virtual uint                getPropertyNotice() const=0;
    /// Returns the tag for the zone model change notice
    virtual uint                getModelNotice() const=0;
    /// Returns the tag for the zone material change notice
    virtual uint                getMaterialNotice() const=0;
    /// Returns the tag for the zone hide state notice
    virtual uint                 getHideNotice() const=0;
    /// Returns a pointer to the zone, IZonething, containing the location <b> vec </b> , or 0 if the zone does not exist.
    /// Set visible to true to prevent searching hidden or excavated blocks
    virtual IZoneThing*           findZoneContaining(const DVect &v, bool visible=false, double dEpsFac=1.0e-5,const itascaxd::IRange *range=nullptr) const=0;
    /// Return zone closest to input location
    virtual const IZoneThing *        findZone(const DVect &v, bool visible = false) const=0;
  };

} // end namespace block
// EOF