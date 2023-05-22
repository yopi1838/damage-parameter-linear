#pragma once
// ighostball.h

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IBody;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IBall;
    class IGhostBall {
    public:
        inline static const TType type_ = 0x51c13f5b;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;
        virtual IBody *                 getIBody()=0;
        virtual const IBody *           getIBody() const=0;

        virtual DVect            getPosition() const=0;
        virtual double           getRadius() const=0;
        virtual DVect            getVelocity() const=0;
        virtual DExtent          getCellExtent() const=0;
        virtual DExtent          getToleranceExtent() const=0;
        virtual const IBall *    getMaster() const=0;


    };
} // namespace pfc
// EoF
