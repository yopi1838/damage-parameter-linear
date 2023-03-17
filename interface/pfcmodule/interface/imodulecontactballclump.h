#pragma once
// imodulecontactballclump.h
// For BALL-CLUMP contacts

namespace itasca {
    class IEnergyMap;
}

namespace itascaxd {
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IModuleContactBallClump {
    public:
        virtual const IContainerContact   *getIContainerContactBallPebble() const=0;
        virtual IContainerContact *        getIContainerContactBallPebble()=0;
        virtual const IEnergyMap         * getIContactBallPebbleEnergies() const=0;
    };
} // namespace contactballwall
// EoF
