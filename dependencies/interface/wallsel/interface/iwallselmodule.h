#pragma once
// iwallselmodule.h
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd { 
}

namespace wallsel {
    using namespace itasca;
    using namespace itascaxd;

    class IWallSelModule {
    public:

        typedef Signal2<const QString &,const QVariant &> prop_signal;

    };
} // namespace wallsel
// EoF
