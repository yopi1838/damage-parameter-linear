#pragma once
// ielementcontainer.h
#include "utility/interface/icontainer.h"

namespace itasca {
    class IThing;
}

namespace ccfd {
    using namespace itasca;

    class IElementContainer {
    public:
        inline static const TType type_ = 0x4fece985;

        virtual IThing *              getIThing()=0;
        virtual const IThing *        getIThing() const =0;
        virtual IContainer *          getIContainer()          =0;
        virtual const IContainer *    getIContainer() const    =0;
    };

} // namespace ccfd
// EoF
