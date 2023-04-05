#pragma once
// igpdata.h

/**
* \file
* \brief Allows access to gridpoint scalar data in a generic way.
*/

#include "module/interface/idata.h"

namespace itasca {
    class IParse;
}
namespace zone {
    using itascaxd::IData;
    class IGp;

    /** \brief Interface that allows access to gridpoint scalar data in a generic way.
    *
    * This class maintains a list of scalar data (x-position, density, velocity magnitude, etc).\n
    * getNames() returns the names of the data available through this interface.\n
    * Which data is being accessed is specified by setIndex().\n
    * This class is used in contour plotting and profiling, as a generic way to get the same
    * list of gp data to use for various purposes.
    * \ingroup Zone
    */
    class IGpData {
    public:
        /// Returns a const pointer to a QObject representing this object.
        virtual const QObject *getQObject() const=0;
        /// Returns a pointer to a QObject representing this object.
        virtual QObject *      getQObject()=0;
        virtual IData *        getIData()=0;
        virtual const IData *  getIData() const=0;
        /// Returns data given a Gridpoint.  Which data is determined by the current setIndex().
        virtual double         getData(const IGp *gp) const=0;
        /// Returns a list of notification codes that indicate the current data type has changed.\n
        /// The geometry code is implies, and so never sent here.
        virtual QList<uint>    getInterestedIn() const=0;
        /// Destroys this object safely across interface boundaries.
        virtual void           destroy()=0;
        virtual bool           getVelocityLimitActive() const=0;
        virtual double         getVelocityLimit() const=0;
        /// SLOTS
        virtual void setVelocityLimit(double d)=0;
        /// SIGNALS
        virtual void velocityLimitActiveChanged(bool b)=0;
        virtual void velocityLimitChanged(double d)=0;
    };
} // namespace zone
// EoF

