#pragma once
// ifacedata.h

#include "module/interface/idata.h"

/**
  * \file
  * \brief Allows access to face scalar data in a generic way.
  */
namespace block
{
    class IFaceThing;

    /// \brief This class maintains a list of scalar data .\n
    /// getNames() returns the names of the data available through this interface.\n
    /// Which data is being accessed is specified by setIndex().\n
    /// This class is used in contour plotting and profiling, as a generic way to get the same
    ///   list of gp data to use for various purposes.
    /// \ingroup THREEDEC
    class IFaceData
    {
    public:
        /// Returns a const pointer to a QObject representing this object.
        virtual const QObject* getQObject() const = 0;
        /// Returns a pointer to a QObject representing this object.
        virtual QObject* getQObject() = 0;
        virtual itascaxd::IData* getIData() = 0;
        virtual const itascaxd::IData* getIData() const = 0;
        /// Returns data given a face.  Which data is determined by the current setIndex().
        virtual double         getData(const IFaceThing* v) const = 0;
        /// Returns a list of notification codes that indicate the current data type has changed.\n
        /// The geometry code is implies, and so never sent here.
        virtual QList<uint>    getInterestedIn() const = 0;
        /// Destroys this object safely across interface boundaries.
        virtual void           destroy() = 0;

    };
} // namespace block
// EoF
