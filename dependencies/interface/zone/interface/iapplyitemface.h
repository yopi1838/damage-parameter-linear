#pragma once

#include "iapplyitem.h"
#include "iface.h"

namespace zone {
    class IApplyItemFace {
    public:
        virtual const IApplyItem *getApplyItem() const=0;
        virtual IApplyItem *getApplyItem()=0;
        virtual const IFace *getFace()=0;
    };
} // namespace zone
// EoF