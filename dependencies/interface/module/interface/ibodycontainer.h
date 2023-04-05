#pragma once
// ibodycontainer.h
#include "utility/interface/icontainer.h"
#include "base/src/callback.h"

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IBody;
}

namespace itascaxd {
    using namespace itasca;

    class IBodyContainer {
    public:
        inline static const TType type_ = 0x4e99d617;

        virtual IThing *              getIThing()=0;
        virtual const IThing *        getIThing() const =0;
        virtual IContainer *          pieces()                 =0;
        virtual const IContainer *    pieces() const           =0;
        virtual IContainer *          getIContainer()          =0;
        virtual const IContainer *    getIContainer() const    =0;
        virtual IThing *              create()                 =0;
        virtual void                  addPieceToBody(IThing *,IThing *) =0;

        /// Interface access to the Multicontainer createBlock function
        virtual quint64 createBlocks() = 0;
        /// Interface access to the Multicontainer getNumberOfBlocks function
        virtual quint64 getBlocks() = 0;

        /// Object for creation of multithreaded loop over bodies
        virtual void threadedCallbackObject(Callback4<void,IBody *,quint32,quint32,void *> &callback,void *v=nullptr,bool useThreads=true) =0;

        /// Implementation of the multithreaded loop using the threadedCallbackObject  
        template <class S,void (S::*MFP)(IBody *,quint32,quint32,void *)>
            void threadedCallbackMethod(S *s,void *v=nullptr,bool useThreads=true) {
                Callback4Method<void,IBody *,quint32,quint32,void *,S,MFP> callback(s);
                threadedCallbackObject(callback,v,useThreads);
            }

    };

} // namespace itascaxd
// EoF
