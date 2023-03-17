#pragma once
// ibodyfluidcontainer.h
#include "utility/interface/icontainer.h"

namespace itasca {
    class IThing;
}

namespace ccfd {
    using namespace itasca;

    class IBodyFluidContainer {
    public:
        inline static const TType type_ = 0x4feca4f7;

        virtual IThing *              getIThing()=0;
        virtual const IThing *        getIThing() const =0;
        virtual IContainer *          getIContainer()          =0;
        virtual const IContainer *    getIContainer() const    =0;
    };

} // namespace ccfd
// EoF
