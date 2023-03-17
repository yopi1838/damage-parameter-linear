#pragma once
// iballcoupledata.h

namespace itasca {
    class IThing;
}

namespace pfc {
    class IBall;
}

namespace ballzone {
    using namespace itasca;

    using namespace pfc;

    class IBallCoupleData {
    public:
        inline static const TType type_ = 0x58f60f39;

        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0;
        virtual IBall *       getIBall()=0;
        virtual const IBall * getIBall() const=0;
        virtual double        getMF() =0;
        virtual void          setMF(const double &)=0;

    };

} // namespace ballzone
// EoF

