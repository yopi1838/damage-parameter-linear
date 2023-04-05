#pragma once
// imoduleballthermal.h
#include "utility/interface/isignalbase.h"
#include "base/src/farray.h"

namespace itasca {
    class IContainer;
    class IEnergyMap;
}

namespace itascaxd {
    class ICellSpace;
    class IContainerContact;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    class IModuleContactThermalBallFWallFacet {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual const IContainerContact *getIContainerContactThermalBallFWallFacet() const=0;
        virtual IContainerContact *      getIContainerContactThermalBallFWallFacet()=0;
        virtual const IEnergyMap       * getIContactThermalBallFWallFacetEnergies() const=0;

    };
} // namespace pfcthermal
// EoF
