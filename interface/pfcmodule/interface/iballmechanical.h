#pragma once
// iballmechanical.h

#include "base/src/matrix.h"

namespace itasca {
    class IThing;
    class IGroup;
}
namespace itascaxd {
    class IRigidBodyMechanicalExt;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IBallMechanical {
    public:
        virtual IRigidBodyMechanicalExt *       getIRigidBodyMechanicalExt()=0;
        virtual const IRigidBodyMechanicalExt * getIRigidBodyMechanicalExt() const=0;
        virtual DMatrix<DIM,DIM>                getStress() const=0;
        virtual double                          getInertialMoi() const=0;
        virtual double                          getMoi() const=0;
        virtual double                          getRadius() const=0;

        virtual void setRadius(const double &d)=0;
    };
} // namespace pfc
// EoF

