#pragma once
// ipebblehermal.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyThermal;
    class IPieceThermal;
    class IPiece;
}

namespace pfc {
    class IPebble;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;
    class IClumpThermal;
    class IPebbleThermal {
    public:
        inline static const TType type_ = 0x4fc90363;

        virtual IThing *                     getIThing()=0;
        virtual const IThing *               getIThing() const=0;
        virtual IPiece *                     getIPiece()=0;
        virtual const IPiece *               getIPiece() const=0;
        virtual IPieceThermal *              getIPieceThermal()=0;
        virtual const IPieceThermal *        getIPieceThermal() const=0;
        virtual IBody *                      getIBody()=0;
        virtual const IBody *                getIBody() const=0;
        virtual IBodyThermal *               getIBodyThermal()=0;
        virtual const IBodyThermal *         getIBodyThermal() const=0;
        virtual IBody *                      getIBodyMechanical()=0;
        virtual const IBody *                getIBodyMechanical() const=0;
        virtual IPebble *                    getIPebble()=0;
        virtual const IPebble *              getIPebble() const=0;
        virtual IClumpThermal *              getIClumpThermal() = 0;
        virtual const IClumpThermal *        getIClumpThermal() const = 0;
        virtual IPebbleThermal *             getNext() = 0;
        virtual const IPebbleThermal *       getNext() const = 0;

    };

} // namespace pfcthermal
// EoF

