#pragma once
// imodulecontactballwall.h
// For CLUMP-WALL contacts

namespace itasca {
    class IEnergyMap;
}

namespace itascaxd {
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IModuleContactClumpWall {
    public:
        virtual const IContainerContact   *getIContainerContactClumpWall() const=0;
        virtual IContainerContact *        getIContainerContactClumpWall()=0;
        virtual const IEnergyMap         * getIContactClumpWallEnergies() const=0;
    };
} // namespace contactballwall
// EoF
