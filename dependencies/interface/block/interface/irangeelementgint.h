#pragma once
// irangeelementgroup.h

#include "module/interface/irangeelement.h"

/**
    * \file
    * \brief Interface to a group intersection filter element, used to filter objects by group intersection.
    */

namespace block {

  using namespace itascaxd;

    /// \brief Interface to a group intersection filter element, used to filter objects by group intersection.
    /** A group intersection filter element filters objects by the intersection of pairs of groups.
        For example, a contact may lie on the intersection of two block groups.
        Currently, this only works for input BLOCK groups.
      */
    class IRangeElementGInt   { // name = "real"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x52ffc5ee;

        /// Returns the two group names associated with this filter element
        virtual QStringList getGroupNames() const=0;
        /// Get the two slot names associated with the filter element
        virtual QStringList getSlotNames() const=0;
        /// Returns the slot name of the object being checked.
        /// null indicates that *all* slots are to be checked.
        virtual QString     getDefaultSlot() const=0;
        // Looks for "slot=group" string and decomposes into SLOT,GROUP names.
        // If no = is present then SLOT will be null.
        virtual std::pair<QString, QString> decompose(const QString& groupKeyword)=0;

        // SIGNALS
        /// This SIGNAL is emitted when the ith group name is changed  (i = 0,1)
        virtual void groupNameChanged(uint i, const QString &name)=0;
        /// This SIGNAL is emitted when the slot being checked is changed.
        virtual void defaultSlotChanged(const QString &slot)=0;

        // SLOTS
        /// Sets the filter element to require the INTERSECTION of all groups.
        /// Cannot be used with a specific slot (slot must be limits<uint>::max())
        /// This SLOT changes the ith group name (i = 0,1)
        virtual void changeGroupName(uint i, const QString &name)=0;
        virtual void removeGroupName(const QString& name)=0;
        /// This SLOT sets the slot number of the object being checked, may be ignored if
        ///   the object does not support group slots.
        virtual void setDefaultSlot(const QString &slot)=0;
        /// This SLOT clears all group names being checked, but does not change the current
        ///   slot setting.
        virtual void clear()=0;
    };
} // namespace itasca

// EoF
