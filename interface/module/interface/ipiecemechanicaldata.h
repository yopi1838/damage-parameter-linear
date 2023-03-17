#pragma once

#include "utility/interface/igroup.h"

namespace fish {
    class IParameter;
}

namespace itascaxd {
    
    using namespace itasca;

    class IPieceMechanicalData {
    public:
        /// The base type of an IThing.
        inline static const TType type_ = 0x50b4d54c;

        /// Return the IThing interface.
        virtual const IThing *getIThing() const=0;
        virtual IThing *getIThing()=0;   ///< \copydoc getIThing

        /// This function returns a character string indicating the type.
        /// It may be compared directly by pointer value (fast), 
        ///   or a string comparison may be given.
        /// Used mostly for save/restore -- to indicate the object type for polymorphic containers.
        virtual TType getType() const=0;

        /// Adds a group entry to this object.  Returns true on success.
        /// If no slot is specified, the default slot is assumed.
        virtual bool addGroup(const IGroupID &id)=0;
        /// Removes all entries matching \a g or \a name from this object, if not slot is specified.
        /// Returns true if one or more entries were removed.
        virtual bool removeGroup(const IGroupID &id)=0;
        /// Returns true if this object is considered to belong to the group.
        /// Applied either to this object directly, or possibly to connected objects.
        /// (For example, gridpoints in FLAC3D are considered a member of a group if any zones
        ///   connected to them are in that group).
        /// If \a slot is specified, then only checks that slot, otherwise checks all.
        virtual uint isInGroup(const FArray<IGroupID> &ids,TType type=0,bool only=false) const=0;
        /// Returns the group entry directly in this object at that slot, or null if it does not exist.
        /// The group_ and groupName_ parts of id are ignored.  
        /// If not specified, the default slot is assumed.
        virtual const IGroup *getGroup(const ISlotID &id=ISlotID((uint)0)) const=0;
        /// Returns the group name directly in this object at that slot, or the null name if it does not exist.
        /// The group_ and groupName_ parts of id are ignored.
        /// If no slot is specified, then returns a contactenation of all groups in all slots.
        virtual QString       getGroupName(const ISlotID &id=ISlotID()) const=0;
        /// Return all groups and all slots assigned to the object in a list.
        virtual uint          getGroupList(FArray<IGroupID> *list) const=0;
        /// Copies group data from one IThing to this, all original group data is lost
        virtual void          copyGroups(const IThing *t)=0;

        /// Returns the number of extra FISH extra variables for this object.
        virtual uint              getExtraSize()  const=0;
        /// Returns an interface to the FISH extra variables for this object.
        /// May be null if no extra variables have ever been specified.
        virtual const fish::IParameter *getExtra(uint index) const=0;
        virtual void                    setExtra(uint index,const fish::IParameter &p)=0;  ///< \copydoc getExtra()
        virtual void              clearExtra()=0;
    };

} // namespace itasca

// EoF
