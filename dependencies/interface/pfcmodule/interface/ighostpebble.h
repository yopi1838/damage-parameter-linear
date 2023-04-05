#pragma once
// ighostpebble.h

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IBody;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IPebble;
    class IGhostPebble {
    public:
        inline static const TType type_ = 0x4d360d54;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;
        virtual IBody *                 getIBody()=0;
        virtual const IBody *           getIBody() const=0;

        virtual DVect            getPosition() const=0;
        virtual double           getRadius() const=0;
        virtual DVect            getVelocity() const=0;
        virtual DExtent          getCellExtent() const=0;
        virtual DExtent          getToleranceExtent() const=0;
        virtual const IPebble *  getMaster() const=0;


    };
} // namespace pfc
// EoF

