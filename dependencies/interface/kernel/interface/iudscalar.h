#pragma once
// iudscalar.h

/**
 * \file
 * \brief IUDScalar is a IUDType<double>, for user-defined scalar
 */
#include "iudtype.h"

namespace itascaxd {
    /// \addtogroup kernel
    /// @{

    /// \brief Interface to user-defined scalar data
    /** Implemented as an IUDType<double>
        */
    class IUDScalar : public IUDType<double> {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bd7;
        /// Returns the next IUDScalar in whatever container it is stored in.
        virtual IUDScalar *getNext()=0;
        /// Returns the next IUDScalar in whatever container it is stored in.
        virtual const IUDScalar *getNext() const=0;
    };
    /// @}
} // namespace itascaxd
// EoF
