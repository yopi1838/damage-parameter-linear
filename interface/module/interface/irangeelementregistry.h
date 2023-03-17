#pragma once

#include "base/src/baseqt.h"
#include "dim.h"
#include "irangeelement.h"

/**
    * \file
    * \brief Interface to a access the list of filter elements types registered with the engine.
    */

namespace itascaxd {

    /// \brief Interface to a access the list of filter elements types registered with the engine.
    /// \ingroup itascaxd
    class IRangeElementRegistry {
    public:
        /// Returns the number of distince RangeElement types registerd with the Range logic.
        virtual int getSize() const=0;

        /// Returns a registered IFilterELement by index (index<getNumberRegistered()).\n
        /// This can be cloned to create new Range elements.
        virtual const IRangeElement *getByIndex(int index) const=0;

        /// Returns a registered IFilterELement matching name name, or 0 if no match.
        virtual const IRangeElement *getByName(const QString &name) const=0;
    };
} // namespace itascaxd
// EoF
