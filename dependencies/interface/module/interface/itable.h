#pragma once

#include "base/src/baseqt.h"
#include "utility/interface/ithing.h"
/**
    * \file
    * \brief Interface to a particular table of values.
    */

namespace itasca {

    /// \brief Interface to a particular table of values.
    /** An ITable is a list of number pairs (x,y), which may or may not be ordered by increasing x.\n
        * Certain functions will only produce valid results if the table is sorted.
        * \ingroup module
    */
    class ITable {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b99;
        /// Returns a const IThing interface for a table.
        virtual const IThing *getIThing() const =0;
        /// Returns a IThing interface for a table.
        virtual IThing *      getIThing()=0;

        /// Returns the (unique) ID number associated with the table.
        virtual quint64 getID() const=0;

        /// Returns a name associated with the table.
        virtual QString getName() const=0;

        /// Sets the name of the table - only used for identification and not necessarily unique.
        virtual void setName(const QString &s)=0;

        virtual QString getLabel() const=0;
        virtual void    setLabel(const QString &s)=0;

        /// Returns the number of pair entries in the table.
        virtual int getSize() const=0;

        /// Returns the pos pair entry in the table.  Returns (0,0) if pos does not exist.
        virtual DVect2 get(int pos) const=0;

        /// Returns a Y value associated with X value.  Assumes table is sorted.\n
        /// Does linear interpolation between pairs.\n
        /// If x is less than the first entry returns first entry y.  Similar if x greater than the last entry.
        virtual double getValue(const double &x) const=0;

        /// Returns the slope at position x.  Assumes table is sorted.\n
        /// No higher order operations.  Just the slope of the segment bordering x.\n
        /// Returns 0.0 if x falls outside table extent.
        virtual double getSlope(const double &x) const=0;

        /// Inserts value (v.x,v.y) into table based on x value.\n
        /// Replaces entry if v.x matches existing value.  Assumes table is sorted.
        virtual void insert(const DVect2 &v)=0;

        /// Places (v.x,v.y) pair at position pos.\n
        /// Entries are created if pos is larger than current size.
        virtual void put(int pos,const DVect2 &v)=0;

        /// Replaces the x value of entry pos.  Will expand table if pos does not exist.
        virtual void putx(int pos,const double &x)=0;

        /// Replaces the y value of entry pos.  Will expand table if pos does not exist.
        virtual void puty(int pos,const double &y)=0;

        /// Sorts table by x-value.
        virtual void sort()=0;

        /// Removes all table pairs - table is empty.
        virtual void clear()=0;

        /// Adds (v.x,v.y) to the *end* of the table - no sorting.
        virtual void add(const DVect2 &v)=0;

        /// Assumes pv is the beginning of an array of pass_dvects of length len.\n
        /// Fills the array with all of its entries (or len, if smaller).
        virtual QVector<DVect2> getAll() const=0;

        /// Used to iterate on Tables on the global table list. See ITableList::getFirst().
        virtual const ITable *getNext() const=0;

        /// Returns a NoticeManager ID that is unique to this table.\n
        /// This id is returned to the NoticeManager when the table is destroyed.\n
        /// The NoticeManager is tagged with this ID when the contents of the table are changed in any way.
        virtual uint getNotice() const=0;
    };
} // namespace itasca
// EoF
