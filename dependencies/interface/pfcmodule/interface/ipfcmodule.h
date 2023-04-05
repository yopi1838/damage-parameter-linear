#pragma once
/** \brief Interface for the PFC module.
    * \file ipfcmodule.h
    *
    * \defgroup pfcmodulesrc PFC interface specification
*/

#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
}

namespace itascaxd {
}

/** \brief PFC namespace */
namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /// \addtogroup pfcmodulesrc
    /// @{

    /// \brief Interface for the PFC module.
    class IPFCModule {
    public:

        typedef Signal2<const QString &,const QVariant &> prop_signal;

    };
    /// @}

} // namespace pfc
// EoF
