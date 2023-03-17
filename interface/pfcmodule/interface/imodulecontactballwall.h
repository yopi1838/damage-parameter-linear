#pragma once
// imodulecontactballwall.h
// For BALL-WALL contacts

namespace itasca {
    class IEnergyMap;
}

namespace itascaxd {
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IModuleContactBallWall {
    public:
        virtual const IContainerContact   *getIContainerContactBallWall() const=0;
        virtual IContainerContact *        getIContainerContactBallWall()=0;
        virtual const IEnergyMap         * getIContactBallWallEnergies() const=0;
    };
} // namespace contactballwall
// EoF
