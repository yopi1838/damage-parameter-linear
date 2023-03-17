#pragma once

#include "base/src/baseqt.h"
#include "module/interface/dim.h"
#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Base for a filter that can be represented by a real number filter.
    /** Either as a center number plus a tolerance, or by a range of two numbers (low to high).\n
        * This class serves a as a base for a number of FilterElements that can be represented
        *   by a numeric range.  
        *  \ingroup kernel
        */ 
    class IRangeElementReal { // name = "real"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b9e;

        /// Returns the low value, see setRange(), setTolerance()
        virtual double         getLow() const=0;
        /// Returns the high value, see setRange(), setTolerance() 
        virtual double         getHigh() const=0;
        /// Returns if a tolerance was used to compute the low and high value from a given center value, , see setRange(), setTolerance()
        virtual bool           wasTolerance() const=0;
        /// Sets the low and high value. wasTolerance() will return \b false after this call.
        virtual void           setRange(const double &low,const double &high)=0;
        /// Sets the low and high value. wasTolerance() will return \b true after this call.\n
        /// The low value is set to <b> val minus </b> tol and the high value is set to <b> val plus tol </b>
        virtual void           setTolerance(const double &val,const double &tol)=0;
        /// Returns a pointer to the IRangeElement object
        virtual IRangeElement *getIRangeElement()=0;

        // SIGNALS
        /// Signal emittted when either low or high value have changed, see setLow, setHigh
        virtual void valueChanged(const DVect2 &range)=0;  
        /// Signal emittted when the tolerance status has changed, see setTolerance, setRange
        virtual void toleranceChanged(bool wasTolerance)=0;
    };

    /// \brief Interface to to a Range Element that selects integers between two values.
    /** This interface is similar to IRangeElementReal.
        */
    /// \ingroup kernel
    class IRangeElementUIntPtr { // name = "uintptr"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815ba0;

        /// Returns the first number in the range, see setRange()
        virtual quint64 getFirst() const=0;
        /// Returns the second number in the range, see setRange()
        virtual quint64 getSecond() const=0;
        /// sets the range
        virtual void     setRange(const quint64 &first,const quint64 &second)=0;

        // SIGNALS
        /// Signal emitted when the first number in the range has changed, see setRange()
        virtual void firstChanged(const quint64 &first)=0;
        /// Signal emitted when the second number in the range has changed, see setRange()
        virtual void secondChanged(const quint64 &second)=0;
    };

    /// \brief Interface to to a Range Element that matches an integer to a list of stored integers.
    /** This interface is similar to IFilterElementInt.\n
        * Entries with a false state aren't used by the kernel in any way.\n
        * They exist to make user interface issues easier (a list of regions with some unselected, for instance).
        */
    /// \ingroup kernel
    class IRangeElementUIntPtrList  { // name = "uintptrlist"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815ba2;

        /// Returns the total number of entries. If \b onOnly is true it will return the number of entries whose state is true.
        virtual qint64            getNumberOfEntries(bool onOnly=false) const=0;
        /// Returns a list of all entries. If \b onOnly is true will return a list of all entries whose state is true.
        virtual QVector<quint64> getAllEntries(bool onOnly=false) const=0;
        /// Returns the state of the entry, or false if the entry does not exist.
        virtual bool              getEntryState(const quint64 &entry) const=0;
        /// Adds an entry to the list.\n
        /// If the entry is already in the list, <b> then the state value is ignored </b>.
        /// Use setEntryState() to force a particular entry to a particular state.\n
        /// If the entry is added to the list, then the state value is used for the new entry.\n
        /// Returns \b true if the entry was added, \b false if the entry alread existed.
        virtual bool              addEntry(const quint64 &entry,bool state=true)=0;
        /// Sets the state of entry. Ignored if entry has not already been added.
        virtual void              setEntryState(const quint64 &entry,bool state)=0;

        // SIGNALS
        /// Signal emitted when a new entry was added, see  addEntry()
        virtual void entryAdded(const quint64 &entry,bool state,const qint64 &numberOfEntries)=0;
        /// Signal emitted when a state of an existing entry has changed, see  addEntry(), setEntryState()
        virtual void entryStateChanged(const quint64 &entry,bool state,const qint64 &numberOfEntries)=0;
        /// This SIGNAL is emitted when *all* of the entries in the list have been changed.\n
        /// The list could have been cleared or copied, for instance.
        virtual void allChanged()=0;
    };

    /// \brief Interface to to a Range Element that matches string to a list of stored strings.
    /** This interface is similar to IRangeElementUIntPtrList.\n
        * Entries with a false state aren't used by the kernel in any way.\n
        * They exist to make user interface issues easier (a list of regions with some unselected, for instance).
        */
    /// \ingroup kernel
    class IRangeElementNameList {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x04c815ba4;

        /// Returns the total number of entries. If \b onOnly is true it will return the number of entries whose state is true.
        virtual qint64      getNumberOfEntries(bool onOnly=false) const=0;
        /// Returns a list of all entries. If \b onOnly is true will return a list of all entries whose state is true.
        virtual QStringList getAllEntries(bool onOnly=false) const=0;
        /// Returns the state of the entry, or false if the entry does not exist.
        virtual bool        getEntryState(const QString &entry) const=0;
        /// Adds an entry to the list.\n
        virtual bool        addEntry(const QString &entry,bool state=true)=0;
        virtual void        setEntryState(const QString &entry,bool state)=0;
        /// Sets the state of entry. Ignored if entry has not already been added.
        virtual void        changeEntryState(const QString &entry,bool state)=0;

        // SIGNALS
        /// Signal emitted when a new entry was added, see  addEntry()
        virtual void entryAdded(const QString &entry,bool state,const qint64 &numberOfEntries)=0;
        /// Signal emitted when a state of an existing entry has changed, see  addEntry(), changeEntryState()
        virtual void entryStateChanged(const QString &entry,bool state,const qint64 &numberOfEntries)=0;
        /// This SIGNAL is emitted when *all* of the entries in the list have been changed.\n
        /// The list could have been cleared or copied, for instance.
        virtual void allChanged()=0;
    };

    /// \brief Interface to to a Range Element that matches string to a list of stored strings.
    /** This interface is similar to IRangeElementUIntPtrList.\n
        * Entries with a false state aren't used by the kernel in any way.\n
        * They exist to make user interface issues easier (a list of regions with some unselected, for instance).
        */
    /// \ingroup kernel
    class IRangeElementIdentifierList {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x04c815ba5;

        /// Returns the total number of entries. If \b onOnly is true it will return the number of entries whose state is true.
        virtual qint64      getNumberOfEntries(bool onOnly=false) const=0;
        /// Returns a list of all entries. If \b onOnly is true will return a list of all entries whose state is true.
        virtual QStringList getAllEntries(bool onOnly=false) const=0;
        /// Returns the state of the entry, or false if the entry does not exist.
        virtual bool        getEntryState(const QString &entry) const=0;
        /// Adds an entry to the list.\n
        virtual bool        addEntry(const QString &entry,bool state=true)=0;
        /// Sets the state of entry. Ignored if entry has not already been added.
        virtual void        setEntryState(const QString &entry,bool state)=0;
        /// Removes entry from list if it is present and off.  Returns TRUE if it does so, false if not.
        virtual bool        removeEntryIfOff(const QString &entry)=0;
        /// Removes all OFF entries from the list.
        virtual void        removeAllOffEntries()=0;
        /// Remove entry regardless
        virtual bool        removeEntry(const QString &entry)=0;

        // SIGNALS
        /// Signal emitted when a new entry was added, see  addEntry()
        virtual void entryAdded(const QString &entry,bool state,const qint64 &numberOfEntries)=0;
        /// Signal emitted when an entry is removed
        virtual void entryRemoved(const QString &entry,const qint64 &numberOfEntries)=0;
        /// Signal emitted when a state of an existing entry has changed, see  addEntry(), changeEntryState()
        virtual void entryStateChanged(const QString &entry,bool state,const qint64 &numberOfEntries)=0;
        /// This SIGNAL is emitted when *all* of the entries in the list have been changed.\n
        /// The list could have been cleared or copied, for instance.
        virtual void allChanged()=0;
    };

} // namespace itascaxd
// EoF
