#pragma once
// iudtensor.h

/**
 * \file
 * \brief IUDTensor is a IUDType<SymTensor>, for user-defined tensor data
 */
#include "iudtype.h"

namespace itascaxd {
    /// \addtogroup kernel
    /// @{

    /// \brief Interface to user-defined tensor data
    /** Implemented as an IUDType<SymTensor>
        */
    class IUDTensor : public IUDType<SymTensor> {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bd5;
        /// Returns the next IUDTensor in whatever container it is stored in.
        virtual IUDTensor *getNext()=0;
        /// Returns the next IUDTensor in whatever container it is stored in.
        virtual const IUDTensor *getNext() const=0;
    };
    /// @}
} // namespace itascaxd
// EoF
