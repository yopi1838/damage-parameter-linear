#pragma once
// ighostrblock.h

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IBody;
}

namespace rblock {
    using namespace itasca;
    using namespace itascaxd;

    class IRBlock;
    class IGhostRBlock {
    public:
        inline static const TType type_ = 0x5ab63d90;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;
        virtual IBody *                 getIBody()=0;
        virtual const IBody *           getIBody() const=0;

        virtual DVect            getPosition() const=0;
        virtual DVect            getFacetPosition(int i) const=0;
        virtual DVect            getBallPosition() const=0;
        virtual double           getBallRadius() const=0;
        virtual DVect            getVelocity() const=0;
        virtual DExtent          getCellExtent() const=0;
        virtual DExtent          getToleranceExtent() const=0;
        virtual const IRBlock *  getMaster() const=0;



    };
} // namespace rblock
// EoF

