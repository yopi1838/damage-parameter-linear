//igplist.h
#pragma once

/**
* \file
* \brief Interface to the global list of all grid points.
*/

#include "utility/interface/igroup.h"

namespace itasca {
    class IContainer;
    class IThing;
}

namespace zone {
    using namespace itasca;

    class IGp;
    class IGpData;

    /// \brief Interface to the global list of all grid points.
    /// \ingroup Zone
    class IGpList {
    public:
        /// Returns a const pointer to an IThing interface representing the IGpList.
        virtual const IThing *     getIThing() const=0;
        /// Returns a pointer to an IThing interface representing the IGpList.
        virtual IThing *           getIThing()=0;
        /// Returns a const pointer to the IContainer interface representing the global list of grid points.
        virtual const IContainer * getIContainer() const=0;
        /// Returns a pointer to the IContainer interface representing the global list of grid points.
        virtual IContainer *       getIContainer() =0;
        /// Return a const pointer to the first gridpoint stored.\n
        /// IGp::getNext() can be used to iterate on the list.
        virtual const IGp *        getFirstGp() const=0;
        /// Return a const pointer to the first gridpoint stored.\n
        /// IGp::getNext() can be used to iterate on the list.
        virtual IGp *              getFirstGp()=0;
        /// Returns the gridpoint general type
        virtual quint32            getGridPointType() const=0;
        /// Given a gridpoint \b id, returns a const pointer to the gridpoint.
        virtual const IGp *        findGridPointWithID(quint64 id) const=0;
        /// Returns the a const pointer to grip point closest to location \b vec.\n
        /// Parameter \b excludeMask is used to mask grid points that will be excluded from the 
        ///  search. (value 0 include all grid points in the search)
        virtual const IGp *        findGridPoint(const DVect &vec,quint32 excludeMask=0) const=0;
        /// Returns the a pointer to grip point closest to location \b vec.\n
        /// Parameter \b excludeMask is used to mask grid points that will be excluded from the 
        ///  search. (value 0 include all grid points in the search)
        virtual IGp *              findGridPoint(const DVect &vec,quint32 excludeMask=0) =0;
        /// Returns the a pointer to grip point closest to location \b vec.\n
        /// Parameter \b group, if non-null, is used to restrict the search only to gridpoints that belong to that group.
        virtual IGp *              findGridPoint(const DVect &vec,const IGroupID &group) =0;
        /// Returns the number of grid points in the list
        virtual quint64           getNGp() const=0;
        /// Returns a pointer to the gridpoint closest to location \b dv3. \n
        /// Parameters \b bMech, \b bFluid and \b bTherm are used to returns a gridpoint connected to one or 
        ///  more zones that have a live mechanical, fluid and/or thermal model.\n
        /// Parameter \b bOr affects the interaction of parameters \b bMech, \b bFluid and \b bTherm:
        ///  true = Union, false = Intersection \n
        /// If the criteria is not met, the function returns 0.
        virtual IGp *              findGpLive(const DVect &dv3, bool bMech,bool bFluid,bool bTherm,bool bOr)=0;
        /// Returns the tag for the gridpoint applied force change notice
        virtual uint               getFapNotice() const=0;
        /// Returns the tag for the gridpoint out of balance change force notice
        virtual uint               getFobNotice() const=0;
        /// Returns the tag for the gridpoint extra variable change notice
        virtual uint               getGpExtraNotice() const=0;
        /// Returns the tag for the gridpoint pore-pressure change notice
        virtual uint               getPorePressureNotice() const=0;
        /// Returns the tag for the gridpoint temperature change notice
        virtual uint               getTemperatureNotice() const=0;
        /// Returns the tag for the gridpoint geometry change notice
        virtual uint               getGeometryNotice() const=0;
        /// Returns the tag for the gridpoint fix condition change notice
        virtual uint               getFixityNotice() const =0;
        /// Returns the tag for the gridpoint velocity change notice
        virtual uint               getVelocityNotice() const=0;
        /// Returns the tag for the gridpoint displacement change notice
        virtual uint               getDisplacementNotice() const=0;
        /// Returns the tag for the gridpoint saturation change notice
        virtual uint               getSaturationNotice() const=0;
        /// Returns the tag for the zone null change notice
        virtual uint               getNullChangeNotice() const=0;
        virtual uint               getMultiplierNotice() const=0;
        /// It is sometimes useful to be able to store flags and data in a gridpoint
        ///   during calculations unrelated to the actual model cycling logic.\n
        /// Two utility arrays are available for this purpose, they are primarily 
        ///   intended to be briefly claimed, used, and then released for back
        ///   for use in other algorithms.\n
        /// claimBit() returns a bit index that can be used with the IGp::getBit() and
        ///   IGp::setBit() functions. 
        /// This function is thread-safe (IGp::getBit and IGp::setBit are not).
        /// \sa releaseBit() IGp::setBit() IGp::getBit()
        /// \brief Claims a bit from the gridpoints utility bit field.
        virtual int                claimBit() const=0;
        /// \brief Releases a bit index previously obtained with claimBit().  Thread-safe.
        virtual void               releaseBit(int bit) const=0;
        /// It is sometimes useful to be able to store flags and data in a gridpoint
        ///   during calculations unrelated to the actual model cycling logic.\n
        /// Two utility arrays are available for this purpose, they are primarily 
        ///   intended to be briefly claimed, used, and then released for back
        ///   for use in other algorithms.\n
        /// claimValue() returns an array index can be used with the IGp::getValue() and
        ///   IGp::setValue() functions. 
        /// This function is thread-safe (IGp::getValue and IGp::setValue are not).
        /// \sa releaseValue() IGp::setValue() IGp::getValue()
        /// \brief Claims a value index from the gridpoints utility value array.
        virtual int                claimValue() const=0;
        /// \brief Releases a value index back to the gridpoints utility value array.  Thread-safe.
        virtual void               releaseValue(int bit) const=0;
        /// Returns a pointer to a new IGpData object. This pointer must be destroyed by the caller.
        virtual IGpData *          createIGpData(QObject *parent=0) const=0; 
    };
} // namespace zone
// EoF
