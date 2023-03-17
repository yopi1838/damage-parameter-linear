#pragma once

#include "base/src/baseqt.h"
#include <functional>

namespace itasca {
    class IThing;

    class IMultiContainer {
    public:
        inline static const TType type_ = 0x51cdf4ee;

        struct IMultiCallInfo {
            IThing *thing_;
            uint    thread_;
            quint64 index_;
            quint64 block_;
        };
        typedef std::function<void(void *,IMultiCallInfo &)> base_func;

        // Returns the IThing pointer of the container. 
        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0;
        virtual IContainer *      getIContainer()=0;
        virtual const IContainer *getIContainer() const=0;
        virtual quint64 getBlockSize() const=0;
        virtual void imultiLoopBase(base_func &func,void *v)=0;
        template <class Data>
            void imultiLoop(std::function<void(Data *,IMultiCallInfo &)> &func,Data *data);
    };

    template <class Data> 
    void IMultiContainer::imultiLoop(std::function<void(Data *,IMultiCallInfo &)> &func,Data *data) {
        base_func out = [&](void *v,IMultiCallInfo &info)->void{ func((Data *)v,info); };
        imultiLoopBase(out,(void *)data);
    }
} // namespace itasca

// EoF