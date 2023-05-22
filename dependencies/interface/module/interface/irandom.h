#pragma once

#include "base/src/base.h"

/**
    * \file
    * \brief Interface to the random number generator.
    */

namespace itasca {
    /// \brief Interface to the random number generator.
    /// \ingroup module
    class IRandom {
    public:

        /// Returns a random floating point number generated by a unit gaussian algorithm.\n
        /// Centered at 0.0, standard deviation of 1.0, etc.
        virtual Double gauss()=0;

        /// Returns a value between 0.0 and 1.0 using a uniform distribution.
        virtual Double uniform()=0;

        /// Sets the random number "seed".  If i=0 then uses the default seed.
        virtual void setSeed(Int i)=0;
    };
} // namespace itasca
// EoF