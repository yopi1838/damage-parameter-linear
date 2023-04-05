// izonefielddata.h
#pragma once

#include "iblockdef.h"
#include "module/interface/idata.h"

/**
  * \file
  * \brief Interface for accessing the zone field data.
  */

namespace block
{
#ifdef _WIN32
  class IData;
#endif
  class IZoneThing;
  class IVertexThing;
  class IGpData;
  class IZoneData;
  class IFaceData;
  class IFaceThing;
  class IZoneInterpolate;
  class ZoneWt;

  /// \brief Interface for accessing the zone field data class.
  /// This class allows access to both gp and zone scalar field data
  ///   (like density, displacement magnitude, sxx, etc) at any point
  ///   in the model, not necessarily where a gridpoint or a zone centroid fall.\n
  /// Interpolation may be necessary for zone-based variables, and the specifics
  ///    of that process can be set through the IZoneInterpolate interface.
  /// \ingroup THREEDEC
  class IZoneFieldData
  {
  public:
    enum Source { ZoneSource, GpSource, BothSource, FaceSource };
    /// Returns a const pointer to a QObject representing this object.
    virtual const QObject *getQObject() const=0;
    /// Returns a pointer to a QObject representing this object.
    virtual QObject *      getQObject()=0;
    virtual const itascaxd::IData *  getIData() const=0;
    virtual itascaxd::IData *        getIData()=0;
    /// Returns a const pointer to the zone interpolate, IZoneInterpolate.
    virtual const IZoneInterpolate *getIZoneInterpolate() const=0;
    /// Returns a pointer to the zone interpolate, IZoneInterpolate.
    virtual IZoneInterpolate *      getIZoneInterpolate()=0;
    /// Returns a const pointer to the zone data, IZoneData.
    virtual const IZoneData *getIZoneData() const=0;
    /// Returns a pointer to the zone data, IZoneData.
    virtual IZoneData *      getIZoneData()=0;
    /// Returns a const pointer to the gridpoint data, IGpData.
    virtual const IGpData *getIGpData() const=0;
    /// Returns a pointer to the gridpoint data, IGpData.
    virtual IGpData *      getIGpData()=0;

    virtual const IFaceData* getIFaceData() const = 0;
    virtual IFaceData* getIFaceData() = 0;

    virtual QString     getSpecificName() const = 0;
    virtual int         getCurrentSource() const=0;
    virtual int         getSource() const=0;
    virtual bool        getSourceActive() const=0;
    virtual QStringList getSourceNames() const=0;
    virtual QStringList getSourceKeywords() const=0;
    /// Returns the property that would be retrieved if the index is set to zone property data.
    virtual QString     getProperty() const=0;
    /// Returns true if the index is set to zone property data, so that the property name is active.
    virtual bool        getPropertyActive() const=0;
    virtual QStringList getPropertiesAvailable() const=0;
    virtual QList<uint> getInterestedIn() const=0;

    /// Assuming \a pos falls inside any zone of the model, returns the data indicated by the current index interpolated
    ///   to that position. \n
    /// The first value in the return pair is the interpolated value.
    /// The second value is the id (in the ZoneArray) of the zone containing point \a pos.
    virtual QPair<double,quint64> getData(const DVect3 &pos)=0;
    /// Retrieves data directly from IVertexThing \a gp or IZoneThing \a z, depending on if the index type indicates
    /// zone or gp data is desired.  No interpolation is performed.
    virtual double                getData(const IVertexThing *gp,const IZoneThing *z, const IFaceThing *ft = nullptr)=0;
    virtual double                getData(const IZoneThing *z,const ZoneWt &wt)=0;
    /// Initializes the field data object for efficient calculation of many values.\n
    /// After initialization, values are stored in temporary offsets in Gp and Zone data, to minimize recalculation.\n
    /// This is only useful if many values are going to be queried via getData() in sequence.\n
    /// If the model changes after the call to initialize but before a reset(), any further calls to getData() will
    ///   return invalid data.\n
    virtual void               initialize()=0;
    /// Resets the object after a call to initialize(), releasing resources back to the system and losing
    ///   temporarily stored calculations.
    virtual void               reset()=0;
   
    /// Destroys this object, returning the memory to the heap.
    virtual void               destroy()=0;

    // public slots
    virtual void setProperty(const QString &s)=0;
    virtual void setSource(int source)=0;
    virtual void setComponent(int i)=0;
    virtual void setQuantity(int i)=0;

    // signals
    /// This SIGNAL is emitted if the name of the property used for zone property data changes.
    virtual void propertyChanged(const QString &s)=0;
    virtual void propertyActiveChanged(bool b)=0;
    virtual void sourceChanged(int source)=0;
    virtual void sourceActiveChanged(bool b)=0;
    /// This SIGNAL is emitted if the active index changes from zone based  to gp, or vice versa.
    virtual void currentSourceChanged(int source)=0;
  };
} // namespace block
// EoF