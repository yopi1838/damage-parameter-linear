#pragma once
// iballzonemodule.h
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd { 
}

namespace ballzone {
    using namespace itasca;
    using namespace itascaxd;

    class IBallZoneModule {
    public:

        /// Const access to the Ball container
        virtual const IContainer       * getIContainerBall() const=0; 
        /// Access to the Ball container
        virtual IContainer *             getIContainerBall()=0;
        /// Const access to the GhostBall container
        virtual const IContainer       * getIContainerGp() const=0;
        /// Access to the GhostBall container
        virtual IContainer *             getIContainerGp()=0;

        typedef Signal2<const QString &,const QVariant &> prop_signal;


    };
} // namespace wallzone
// EoF
