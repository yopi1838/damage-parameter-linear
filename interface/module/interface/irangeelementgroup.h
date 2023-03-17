#pragma once
// irangeelementgroup.h

#include "module/interface/irangeelement.h"

/**
    * \file
    * \brief Interface to a group filter element, used to filter objects by group assignment.
    */
namespace itasca {
    class IGroup;
}

namespace itascaxd {
    /// \brief Interface to a group filter element, used to filter objects by group assignment.
    /** A group filter element filters objects by their associate with group names.
      *  Note that a particular object being filtered may or may not have more than one slot.
      * \ingroup module
      */
    class IRangeElementGroup { // name = "real"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b95;

        typedef std::pair<QString,QString> Entry; // Slot,Group
        typedef std::vector<Entry>         Entries;

        virtual Entries     getAllEntries() const=0; 
        /// Returns the AND state, if true then object must belong to ALL groups.
        virtual bool        getAnd() const=0;
        /// Returns the ONLY state, if true then object must not be a direct member of any groups not on this list
        virtual bool        getOnly() const=0;
        virtual uint        getMatches() const=0;
        virtual QString     getByName() const=0;
        // Looks for "slot=group" string and decomposes into SLOT,GROUP names.
        // If no = is present then SLOT will be null.
        virtual Entry       decompose(const QString &groupName) const=0;
        virtual void        setAllSlots(const QString &slot)=0;

        // SIGNALS
        /// This SIGNAL is emitted when a slot/group pair is added to the list being checked.
        virtual void entryAdded(const QString &slot,const QString &group)=0;
        /// This SIGNAL is emitted when a slot/group pairt is removed from the list of names being checked.
        virtual void entryRemoved(const QString &slot,const QString &group)=0;
        /// This SIGNAL is emitted when the and state is changed.
        virtual void andChanged(bool b)=0;
        /// THis SIGNAL is emitted when the only state is changed.
        virtual void onlyChanged(bool b)=0;
        virtual void matchesChanged(uint i)=0;
        virtual void byNameChanged(const QString &name)=0;

        // SLOTS
        /// This SLOT adds a new slot/group pair to the list of those being checked.
        /// Duplicate names are discarded, but all entries are case sensitive at this point.
        virtual bool addEntry(const QString &slot,const QString &name)=0;
        /// This SLOT removes the group name from the list of those being checked.
        /// This operation is case sensitive.
        virtual bool removeEntry(const QString &slot,const QString &name)=0;
        /// Sets the filter element to require the INTERSECTION of all groups.
        /// Cannot be used with a specific slot (slot must be limits<uint>::max())
        virtual void setAnd(bool b)=0;
        /// Returns the ONLY state, if true then object must not be a direct member of any groups not on this list
        virtual void setOnly(bool b)=0;
        virtual void setMatches(uint i)=0;
        virtual void setByName(const QString &name)=0;
        /// This SLOT removes all group names being checked, but does not change the current
        ///   slot setting.
        virtual void clear()=0;
        /// Set that parsing using the BY keyword is enabled
        virtual void enableByParsing()=0;
    };
} // namespace itasca

// EoF
