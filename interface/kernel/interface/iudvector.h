#pragma once
// iudvector.h

/// \file
/// \brief IUDVector is a IUDType<DVect>, for user-defined vectors

#include "iudtype.h"

namespace itascaxd {
    /// \brief Interface to user-defined vector data
    /// Implemented as an IUDType<DVect>
    /// \ingroup kernel
    class IUDVector : public IUDType<DVect> {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bd3;
        /// Returns the next IUDVector in whatever container it is stored in.
        virtual IUDVector *getNext()=0;
        /// Returns the next IUDVector in whatever container it is stored in.
        virtual const IUDVector *getNext() const=0;
    };
} // namespace itascaxd
// EoF
