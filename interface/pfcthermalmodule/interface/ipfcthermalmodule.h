#pragma once
/** \brief Interface for the PFC thermal module.
    * \file ipfcthermalmodule.h
    *
    * \defgroup pfcthermalmodule PFC thermal module interface specification
*/

// ipfcthermalmodule.h
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd {
}

/** \brief PFCTHERMAL namespace */
namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;
    /// \addtogroup pfcthermalmodule
    /// @{

    /// \brief Interface for the PFC thermal module.
    class IPFCThermalModule {
    public:

        typedef Signal2<const QString &,const QVariant &> prop_signal;

    };
    /// @}

} // namespace pfcthermal
// EoF
