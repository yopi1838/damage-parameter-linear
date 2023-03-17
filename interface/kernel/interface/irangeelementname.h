#pragma once

#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Interface to a RangeElement exposing the IThing getName method. 
    /// \ingroup kernel
    class IRangeElementName { // name = "name"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x553e95e1;
        /// Returns a pointer to itself.
        virtual IRangeElement *getIRangeElement()=0;
  
        /// Returns the list of name that are checked against the getName method.
        virtual QSet<QString> getNames(bool checked=true) const=0;
        /// Sets the name list.
        virtual void setNames(const QSet<QString> &ns)=0;
        /// Adds the set name.
        virtual void addName(const QString &n)=0;
        /// Removes the set name.
        virtual void removeName(const QString &n)=0; 

    signals:
        /// Signal emitted when a name is added.
        virtual void nameAdded(const QString &n)=0;
        /// Signal emitted when a name is removed.
        virtual void nameRemoved(const QString &n)=0;
    };

} // namespace itascaxd
// EoF