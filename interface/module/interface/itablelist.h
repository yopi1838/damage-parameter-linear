#pragma once

#include "base/src/base.h"
#include "itable.h"
#include "utility/interface/icontainer.h"

/**
    * \file
    * \brief Interface to the global list of all tables.
    */

namespace itasca {

    /// \brief Interface to the global list of all tables.
    /// \ingroup module
    class ITableList {
    public:
        /// returns a constant pointer to the IContainer class representing a list of tables.
        virtual const IContainer *getIContainer() const=0;
        /// returns a pointer to the IContainer class representing a list of tables.
        virtual IContainer *getIContainer()=0;

        ///// Returns a const pointer to a table with id given, or 0.
        //virtual const ITable *lookupWithID(quint64 id) const=0;
        ///// Returns a pointer to a table with id given, or 0.
        //virtual ITable *      lookupWithID(quint64 id)=0;

        /// Returns a const pointer to table whose name matches s, or 0.
        virtual const ITable *lookupWithName(const QString &s) const=0;
        /// Returns a pointer to table whose name matches s, or 0.
        virtual ITable *      lookupWithName(const QString &s)=0;

        /// Given a table name, returns a pointer to a table.\n
        /// If a table is not found, one is created and added to list.\n
        /// Always returns valid pointer (barring memory fault). See lookupOrCreateWithID().
        virtual ITable *lookupOrCreateWithName(const QString &s)=0;

        /// Return a pointer to the first table stored in the global list.\n
        /// ITable::getNext() can be used to iterate on the global list.
        virtual const ITable *getFirst() const=0;

        /// Returns the ID assigned by the NoticeManager for changes to the list of tables.\n
        /// Indicates when tables are added or removed from the list.
        virtual uint getNotice() const=0;

        /// Returns the general type of an ITable.
        virtual quint32 getTableGeneralType() const=0;
    }; 
} // namespace itascaxd
// EoF
