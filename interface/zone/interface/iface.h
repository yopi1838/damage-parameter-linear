#pragma once
// iface.h

/**
* \file
* \brief Interface to zone faces
*/

#include "izone.h"

namespace itasca {
    class IGroupID;
    class ISlotID;
}

namespace zone {
    using namespace itasca;

    class IGp;

    /// \brief Interface to zone faces
    /// \ingroup Zone
    class IFace {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a0f;

        /// Get an IThing interface
        virtual IThing *      getIThing()=0;
        /// Get an IThing interface
        virtual const IThing *getIThing() const=0;
        /// Returns the number of sides of a face
        virtual quint32       getSize() const=0;
        /// Given an index, returns the gridpoint corresponding to the index (\b gpIdx)
        /// Gridpoint order is clockwise looking down on normal vector, ccw looking along normal vector.
        virtual const IGp *   getGp(quint32 gpIdx) const=0;
        /// Returns the centroid of the face
        virtual DVect         getCentroid() const=0;
        /// Returns a const pointer to the zone of the face
        virtual const IZone * getIZone() const=0;
        /// Sets the zone associated with this face object
        virtual void          setIZone(IZone *zone)=0;
        /// Returns a pointer to the zone of the face
        virtual IZone       * getIZone() =0;
        /// Returns the face number
        virtual quint32       getSide() const=0;
        /// Sets the side associated with this face object
        virtual void          setSide(quint32 side)=0;
        virtual const IZone * getJoin() const=0;
        virtual IZone *       getJoin() =0;
        /// Determines if the face can be considered a surface face.
        /// mech, therm, fluid indicate if zones should be considered absent if they are NULL in that process.
        /// if bit>0, then a 0 return from getBit(bit) indicates that the zone should be considered absent.
        virtual bool          isSurface(bool mech=true,bool therm=false,bool fluid=false,int bit=-1) const=0;
        /// Determines if the face is considered a surface face BASED ON NON-HIDDEN ZONES.
        /// If the face is of surface of the set of all visible and non-null zones then it is true, otherwise false
        virtual bool          isVisibleSurface() const=0;
        /// Returns TRUE if the face is connected to a non-hidden (visible) zone on either side, otherwise false.
        virtual bool          isOnVisibleZone() const=0;
        /// Returns TRUE if the face is on the border between one group and another in the given slot.
        /// If slot is ANY, then returns TRUE if any group changes in any slot from one zone to another.
        /// If the zone is a mechanical surface (isSurface(true,false,false,-1)==true) will always return true.
        /// If provided, group1 and group2 are filled with the group names on either side of the boundary.   
        /// If no group exists on one side group2 will be null.
        /// If ANY slot is used and multiple group surfaces exist, the first one found will be used.
        virtual bool          isOnGroupSurface(const ISlotID &slot,IGroupID *group1=nullptr,IGroupID *group2=nullptr) const=0;
        /// Returns the unique face id for this object.
        virtual FaceID        getFaceID() const=0;
        /// Destroys this object
        virtual void          destroy()=0;
    };
} // namespace zone
// EoF
