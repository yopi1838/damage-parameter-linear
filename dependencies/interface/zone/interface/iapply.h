#pragma once

#include "utility/interface/ithing.h"

namespace zone {
    class IApply {
    public:
        virtual itasca::IThing *getIThing()=0;
        virtual const itasca::IThing *getIThing() const=0;
        virtual QString getName() const=0;
        virtual QString getCaptionName() const=0;
    };
} // namespace zone
