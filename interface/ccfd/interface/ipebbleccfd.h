#pragma once
// ipebbleccfd.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyFluid;
    class IPieceFluid;
    class IPiece;
}

namespace pfc {
    class IPebble;
}

namespace ccfd {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;
    class IClumpCCFD;
    class IPebbleCCFD {
    public:
        inline static const TType type_ = 0x52d00974;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;
        virtual IPiece *                getIPiece()=0;
        virtual const IPiece *          getIPiece() const=0;
        virtual IPieceFluid *           getIPieceFluid()=0;
        virtual const IPieceFluid *     getIPieceFluid() const=0;
        virtual IBody *                 getIBody()=0;
        virtual const IBody *           getIBody() const=0;
        virtual IBodyFluid *            getIBodyFluid()=0;
        virtual const IBodyFluid *      getIBodyFluid() const=0;
        virtual IBody *                 getIBodyMechanical()=0;
        virtual const IBody *           getIBodyMechanical() const=0;
        virtual IPebble *               getIPebble()=0;
        virtual const IPebble *         getIPebble() const=0;
        virtual IClumpCCFD *            getIClumpCCFD() = 0;
        virtual const IClumpCCFD *      getIClumpCCFD() const = 0;
        virtual IPebbleCCFD *           getNext() = 0;
        virtual const IPebbleCCFD *     getNext() const = 0;

    };

} // namespace ccfd
// EoF

