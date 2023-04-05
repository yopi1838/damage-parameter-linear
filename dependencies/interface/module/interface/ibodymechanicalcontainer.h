#pragma once
// ibodymechanicalcontainer.h
#include "utility/interface/icontainer.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IBodyMechanicalContainer {
    public:
        inline static const TType type_ = 0x4ec4e346;

        virtual IThing *              getIThing()=0;
        virtual const IThing *        getIThing() const =0;
        virtual IContainer *          getIContainer()          =0;
        virtual const IContainer *    getIContainer() const    =0;
    };

} // namespace itascaxd
// EoF
