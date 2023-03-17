#pragma once
// iwallzonemodule.h
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd { 
}

namespace wallzone {
    using namespace itasca;
    using namespace itascaxd;

    class IWallZoneModule {
    public:

        typedef Signal2<const QString &,const QVariant &> prop_signal;

    };
} // namespace wallzone
// EoF
