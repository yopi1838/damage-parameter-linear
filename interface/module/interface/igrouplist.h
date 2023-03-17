#pragma once
/**
    * \file
    * \brief Group container class.
    */

#include "utility/interface/signal2.h"
#include "utility/interface/icontainer.h"
#include "utility/interface/igroup.h"

#include <unordered_map>

namespace itasca {
    /// \brief Group container class.
    class IGroupList {
    public:
        /// A unique type number that identifies this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4ecd61c1;
        /// Signal type used to notify system of changes.
        using name_signal = Signal2<QString,QString>;
        using slot_signal = Signal2<QString,uint>;
        using groupcount_signal = Signal2<TType, uint>;
        enum class Resolve { Write, ReadQuiet, ReadException };

        /// Returns a const pointer to the container class representing a list of groups.
        virtual const IContainer *getIContainer() const=0;
        /// Returns a pointer to the container class representing a list of groups.
        virtual IContainer *      getIContainer()=0;
         
        /// Resolve (if necessary) a groupid into actual pointers.
        virtual IGroupID resolve(const IGroupID &input,Resolve resolve)=0;

        /// Returns a list of all group names found in the container
        virtual QStringList getAllGroupNames() const=0;

        /// Returns a list of all category names found in the container
        virtual QStringList getAllSlotNames() const=0;

        /// Returns slots that have been assigned to objects with the general type type.
        /// Returns the same as getAllSlotNames() if type==0.
        /// See getGroupCountSignal to be notified of changes.
        virtual QStringList slotsFromType(TType type) const=0;

        // Returns the groups that have been assigned to objects with the general tyep type.
        // If type==0 returns all groups assigned to slot for all types.
        // If slot==ISlotID::anyName() "Any" then returns all groups assigned to that type in any slot.
        // if both than returns the same as getAllGroupNames()
        // See getGroupCountSignal to be notified of changes.
        virtual QStringList groupsFromTypeSlot(TType type, const QString& slot) const=0;

        virtual const ISlot * getSlot(const ISlotID &id,Resolve r) = 0;
        virtual const ISlot * getSlot(const ISlotID &id) const = 0;
        virtual const ISlot * getDefaultSlot() const=0;
        virtual const ISlot * getAnySlot() const = 0;
        virtual QString       getSlotName(quint32 slot,quint32 thread) const = 0;
        virtual const IGroup *getGroup(const IGroupID &id,Resolve r) = 0;
        virtual const IGroup *getGroup(const IGroupID &id) const = 0;
        virtual const IGroup *getNullGroup() const=0;

        /// Emitted when a group is renamed from arg1 to arg2
        virtual name_signal *getGroupRenameSignal() const=0; 
        virtual name_signal *getSlotRenameSignal() const=0;
        // Emitted when slots are added or removed.
        // Note on restore Added signal has QString(), limits<uint>::max() as arguments.
        virtual slot_signal *getSlotAddedSignal() const=0;
        virtual slot_signal *getSlotRemovedSignal() const=0;
        // This is emitted when a new slot is added to the list of those available to a type.
        // It is also emitted when a new group is added to a group slot combination.
        // In the former case the second argument is ISlotID::any_.
        // It is NOT sent when the group count changes, or when a group or slot is *removed*.
        virtual groupcount_signal *getGroupCountSignal() const=0;
    };
} // namespace itasca
// EoF
