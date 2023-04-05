#pragma once
// irblockthermal.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyThermal;
    class IPiece;
}

namespace rblock {
    class IRBlock;
}

namespace rblockthermal {
    using namespace itasca;
    using namespace itascaxd;

    using namespace rblock;

    class IRBlockThermal {
    public:
        inline static const TType type_ = 0x5bb538c5;

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
        virtual IRBlock *                    getIRBlock()=0;
        virtual const IRBlock *              getIRBlock() const=0;
    };

} // namespace rblockthermal
// EoF

