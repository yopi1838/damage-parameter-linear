#pragma once
// ifishcalllist.h

#include <vector>

namespace fish {
    class Parameter;
}

namespace itascaxd {
    class IContact;
    class IFishCallList {
    public:
        virtual void setCMFishCallArguments(const IContact *,const std::vector<fish::Parameter> &arg,int)=0;
    };
} // namespace itascaxd

// EOF
