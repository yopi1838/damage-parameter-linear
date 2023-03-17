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

    class IModuleContactThermalBallPebble {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual const IContainerContact *getIContainerContactThermalBallPebble() const=0;
        virtual IContainerContact *      getIContainerContactThermalBallPebble()=0;
        virtual const IEnergyMap       * getIContactThermalBallPebbleEnergies() const=0;

    };
} // namespace pfcthermal
// EoF
