#pragma once

#include "iapplyitem.h"
#include "izone.h"

namespace zone {
    class IApplyItemZone {
    public:
        virtual const IApplyItem *getApplyItem() const=0;
        virtual IApplyItem *getApplyItem()=0;
        virtual const IZone *getZone()=0;
    };

} // namespace zone
// EoF
