#pragma once

#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Interface to a RangeElement of a general set. A IThing has getIsSet and getSet
    /// methods to return the appropriate information.
    /// \ingroup kernel
    class IRangeElementSet { // name = "set"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x5149e67c;
        /// Returns a pointer to itself.
        virtual IRangeElement *getIRangeElement()=0;
  
        /// Returns the list of set names that are checked against the getName method.
        virtual QSet<QString> getNames(bool checked=true) const=0;
        /// Returns the list of set ids that are checked against the getID method.
        virtual QSet<quint64> getIDs(bool checked=true) const=0;
        /// Returns the list of set type names that are checked against the getTypeName method.
        virtual QSet<QString> getTypeNames(bool checked=true) const=0;
        /// Sets the set name list.
        virtual void setNames(const QSet<QString> &ns)=0;
        /// Sets the set id list.
        virtual void setIDs(const QSet<quint64> &ns)=0;
        /// Sets the set typeName list.
        virtual void setTypeNames(const QSet<QString> &ns)=0;
        /// Adds the set name.
        virtual void addName(const QString &n)=0;
        /// Removes the set name.
        virtual void removeName(const QString &n)=0; 
        /// Adds the set ID.
        virtual void addID(const quint64 &)=0;
        /// Removes the set ID.
        virtual void removeID(const quint64 &)=0; 
        /// Adds the set typeName.
        virtual void addTypeName(const QString &n)=0;
        /// Removes the set name.
        virtual void removeTypeName(const QString &n)=0; 

    signals:
        /// Signal emitted when a set name is added.
        virtual void nameAdded(const QString &n)=0;
        /// Signal emitted when a set name is removed.
        virtual void nameRemoved(const QString &n)=0;
        /// Signal emitted when a set name is added.
        virtual void idAdded(const quint64 &)=0;
        /// Signal emitted when a set name is removed.
        virtual void idRemoved(const quint64&)=0;
        /// Signal emitted when a set name is added.
        virtual void typeNameAdded(const QString &n)=0;
        /// Signal emitted when a set name is removed.
        virtual void typeNameRemoved(const QString &n)=0;
    };

} // namespace itascaxd
// EoF