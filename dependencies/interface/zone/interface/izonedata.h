#pragma once
// izonedata.h

/**
* \file
* \brief Allows access to zone scalar data in a generic way.
*/

namespace itascaxd {
    class IData;
}

namespace zone {
    class IZone;
    using itascaxd::IData;

    /// This class maintains a list of scalar data (sxx, density, porosity, etc).\n
    /// getNames() returns the names of the data available through this interface.\n
    /// Which data is being accessed is specified by setIndex().\n
    /// This class is used in contour plotting and profiling, as a generic way to get the same
    ///   list of zone data to use for various purposes.
    /// \brief Access to zone scalar data.
    /// \ingroup Zone
    class IZoneData {
    public:
        /// Returns a pointer const to a QObject representing this object.
        virtual const QObject *getQObject() const=0;
        /// Returns a pointer to a QObject representing this object.
        virtual QObject *      getQObject()=0;
        virtual const IData *  getIData() const=0;
        virtual IData *        getIData()=0;
        virtual void           initializeMultithreading(quint32 threads)=0;

        /// Retrieves scalar data from the \a zone, depending on the current index and property data.
        virtual double         getData(const IZone *zone,quint32 thread=0,quint32 block=0) const=0;
        /// Returns the name of the property value being retrieved if the current data is Zone Property.
        virtual QString        getProperty() const=0;
        virtual bool           getPropertyActive() const=0;
        virtual QStringList    getPropertiesAvailable() const=0;
        /// Returns a list of notification codes that indicate the current data type has changed.\n
        /// The geometry code is implies, and so never sent here.
        virtual QList<uint>    getInterestedIn() const=0;
        /// Destroys this object safely across interface boundaries.
        virtual void           destroy()=0;
        // public slots
        virtual void setProperty(const QString &s)=0;
        /// This SLOT sets the name of the constitutive model used for stress/strength ratio values.\n
        /// If the string is null or is not recognized, then the current constitutive model of the zone is used.

        // signals
        /// This SIGNAL is sent when the property name changes, used when retrieving zone properties.
        virtual void propertyChanged(const QString &s)=0;
        virtual void propertyActiveChanged(bool active)=0;
        virtual void propertiesAvailableChanged(const QStringList &s)=0;
    };
} // namespace zone
// EoF

