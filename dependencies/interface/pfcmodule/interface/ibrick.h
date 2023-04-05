#pragma once
// ibrick.h


namespace itasca {
    class IThing;
    class IGroup;
}
namespace itascaxd {
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IBrick {
    public:
        inline static const TType type_ = 0x4d6beb3a;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;

        virtual DVect                   getPosition()        const=0;
        virtual void                    doAssemble(const DVect &pos,const UVect &num)=0;
    };

} // namespace pfc
// EoF

