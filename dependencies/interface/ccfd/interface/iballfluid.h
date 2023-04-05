#pragma once
// iballfluid.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
    class IBodyFluid;
    class IPiece;
}

namespace pfc {
    class IBall;
}

namespace ccfd {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;

    class ICcfdElement;

    class IBallFluid {
    public:
        inline static const TType type_ = 0x4fe8e795;

        virtual IThing *                     getIThing()=0;
        virtual const IThing *               getIThing() const=0;
        virtual IPiece *                     getIPiece()=0;
        virtual const IPiece *               getIPiece() const=0;
        virtual IBody *                      getIBody()=0;
        virtual const IBody *                getIBody() const=0;
        virtual IBodyFluid *                 getIBodyFluid()=0;
        virtual const IBodyFluid *           getIBodyFluid() const=0;
        virtual IBody *                      getIBodyMechanical()=0;
        virtual const IBody *                getIBodyMechanical() const=0;
        virtual IBall *                      getIBall()=0;
        virtual const IBall *                getIBall() const=0;
        //virtual ICcfdElement *               getElement()=0;
        //virtual const ICcfdElement *         getElement() const=0;
        virtual void getElements(FArray<ICcfdElement*> *) const=0;
        virtual void getOverlaps(FArray<double> *) const=0;
        virtual DVect                       getFluidForce()  const=0;
        virtual void                        setFluidForce(const DVect &d)  =0;

    };

} // namespace ccfd
// EoF

