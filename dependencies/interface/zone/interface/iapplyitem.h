#pragma once

#include "iapply.h"

namespace zone {
    class IApplyItem {
    public:
        virtual IApply *getApply()=0;
        virtual const IApply *getApply() const=0;
    };
} // namespace zone
// EoF

