#pragma once

#include "iface.h"
#include "iapplyitemface.h"
#include "iapplyitemzone.h"

namespace zone {
    class IApplyList {
    public:
        virtual void findAllIFace(const FaceID &f,FArray<const IApply *> *ret) const=0;
        virtual void findAllIZone(quint64 id,FArray<const IApply *> *ret) const=0;
        virtual quint32 getNotice() const=0;
    };
} // namespace zone
// EoF

