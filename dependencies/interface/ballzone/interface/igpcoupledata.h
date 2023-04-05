#pragma once
// igpcoupledata.h

namespace itasca {
    class IThing;
}

namespace zone {
    class IGp;
}

namespace ballzone {
    using namespace itasca;

    using namespace zone;

    class IGpCoupleData {
    public:
        inline static const TType type_ = 0x58f60f39;

        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0;
        virtual IGp *         getIGp()=0;
        virtual const IGp *   getIGp() const=0;
        virtual double        getMF() =0;
        virtual void          setMF(const double &)=0;

    };

} // namespace ballzone
// EoF

