#pragma once
/**
    * \file
    * \brief Interface for providing an IRangeElement extension for a RangElementFish.
*/

#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Interface for providing an IRangeElement extension for a RangElementFish.
    /** Gives access to the FISH function name.
      *  
      */
    class IRangeElementFish  { // name = "fish"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bc0;

        /// Returns the FISH function name for a Range element
        virtual QString getFunctionName() const=0;
        /// Sets the FISH function name for a Range element
        virtual void    setFunctionName(const QString &s)=0;

        // SIGNALS
        /// Signal emitted when the FISH function for a filter element has changed
        virtual void functionNameChanged(const QString &s)=0;
    };
} // namespace itascaxd
// EoF
