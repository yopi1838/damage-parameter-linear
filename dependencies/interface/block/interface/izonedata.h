#pragma once
// izonedata.h

#include "module/interface/idata.h"

/**
  * \file
  * \brief This interface provides access to zone data.
  */

namespace block
{
#ifdef _WIN32
  class IData;
#endif
  class IZoneThing;

  /// \brief Access to zone scalar data in a generic way.
  /// This class maintains a list of scalar data (sxx, density, porosity, etc).\n
  /// getNames() returns the names of the data available through this interface.\n
  /// Which data is being accessed is specified by setIndex().\n
  /// This class is used in contour plotting and profiling, as a generic way to get the same
  ///   list of zone data to use for various purposes.
  /// \ingroup THREEDEC
  class IZoneData
  {
  public:
   
    /// Returns a pointer const to a QObject representing this object.
    virtual const QObject *getQObject() const=0;
    /// Returns a pointer to a QObject representing this object.
    virtual QObject *      getQObject()=0;
    virtual const itascaxd::IData *  getIData() const=0;
    virtual itascaxd::IData *        getIData()=0;
    
    /// Returns the name of the property value being retrieved if the current data is Zone Property.
    virtual QString        getProperty() const=0;
    /// Returns TRUE if the current data index is a Zone Property.
    virtual bool           getPropertyActive() const=0;
    /// Returns list of available properties
    virtual QStringList    getPropertiesAvailable() const=0;
    /// Retrieves scalar data from the \a zone, depending on the current index and property data.
    virtual double         getData(const IZoneThing *z) const=0;
    /// Returns a list of notification codes that indicate the current data type has changed.\n
    /// The geometry code is implies, and so never sent here.
    virtual QList<uint>    getInterestedIn() const=0;
    /// Destroys this object safely across interface boundaries.
    virtual void           destroy()=0;

    /// This SLOT set the name of the property (keyword matched) used if the data type is Zone Property.
    /// It will cause a propertyChanged() signal.
    virtual void setProperty(const QString &s)=0;

    // signals
    /// This SIGNAL is sent when the property name changes, used when retrieving zone properties.
    virtual void propertyChanged(const QString &s)=0;
    virtual void propertyActiveChanged(bool active)=0;
    virtual void propertiesAvailableChanged(const QStringList &s)=0;
  };
} // namespace block
// EoF
