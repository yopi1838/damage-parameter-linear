#pragma once
// icommandmodule.h

#include "module/interface/dim.h"

namespace itasca {
    class IModule;
}
namespace itascaxd {
    class IDomain;
    class IGlobals;

    class ICommandModule {
    public:
        // Module Functions
        virtual QStringList getAllModules()=0;

        // Generic Contact Functions
        virtual const IDomain *                       getIDomain() const=0;
        virtual IDomain *                             getIDomain()=0;
        virtual const IGlobals *                      getIGlobals() const=0;
        virtual IGlobals *                            getIGlobals()=0;
    };
} // namespace itascaxd
// EoF
