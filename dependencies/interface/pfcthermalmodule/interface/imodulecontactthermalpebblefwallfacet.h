#pragma once
// imodulepebblethermal.h
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

    class IModuleContactThermalPebbleFWallFacet {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual const IContainerContact *getIContainerContactThermalPebbleFWallFacet() const=0;
        virtual IContainerContact *      getIContainerContactThermalPebbleFWallFacet()=0;
        virtual const IEnergyMap       * getIContactThermalPebbleFWallFacetEnergies() const=0;

    };
} // namespace pfcthermal
// EoF
