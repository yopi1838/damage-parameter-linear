#pragma once
// iballthermal.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyThermal;
    class IPiece;
}

namespace pfc {
    class IBall;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;

    class IBallThermal {
    public:
        inline static const TType type_ = 0x51c0f9ad;

        virtual IThing *                     getIThing()=0;
        virtual const IThing *               getIThing() const=0;
        virtual IPiece *                     getIPiece()=0;
        virtual const IPiece *               getIPiece() const=0;
        virtual IBody *                      getIBody()=0;
        virtual const IBody *                getIBody() const=0;
        virtual IBodyThermal *               getIBodyThermal()=0;
        virtual const IBodyThermal *         getIBodyThermal() const=0;
        virtual IBody *                      getIBodyMechanical()=0;
        virtual const IBody *                getIBodyMechanical() const=0;
        virtual IBall *                      getIBall()=0;
        virtual const IBall *                getIBall() const=0;
    };

} // namespace pfcthermal
// EoF

