#pragma once
// ifwallfacetthermal.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyThermal;
    class IPieceThermal;
    class IPiece;
    class IHalfFacet;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    class IFWallThermal;
    class IFWallFacetThermal {
    public:
        inline static const TType type_ = 0x4fcbfdb6;

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
        virtual IHalfFacet *                 getIHalfFacet()=0;
        virtual const IHalfFacet *           getIHalfFacet() const=0;
        virtual IFWallThermal *              getIFWallThermal() = 0;
        virtual const IFWallThermal *        getIFWallThermal() const = 0;
        virtual IFWallFacetThermal *         getNext() = 0;
        virtual const IFWallFacetThermal *   getNext() const = 0;
        virtual void                         setFacetTemperature(const double &d) = 0;
        virtual double                       getFacetTemperature() const = 0;

    };

} // namespace pfcthermal
// EoF

