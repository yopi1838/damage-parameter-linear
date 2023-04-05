#pragma once
// iclumpccfd.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IContainer;
}

namespace itascaxd {
    class IBody;
    class IBodyFluid;
    class IPiece;
    class IContact;
}

namespace pfc {
    class IClump;
}

namespace ccfd {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;
    class IPebbleCCFD;
    class ICcfdElement;
    class IClumpCCFD {
    public:
        inline static const TType type_ = 0x52d00cb0;

        virtual IThing *                    getIThing()=0;
        virtual const IThing *              getIThing() const=0;
        virtual IBody *                     getIBody()=0;
        virtual const IBody *               getIBody() const=0;
        virtual IBodyFluid *                getIBodyFluid()=0;
        virtual const IBodyFluid *          getIBodyFluid() const=0;
        virtual IBody *                     getIBodyMechanical()=0;
        virtual const IBody *               getIBodyMechanical() const=0;
        virtual IClump *                    getIClump()=0;    
        /// Return the first IPebbleCCFD in his CCFD clump.
        virtual IPebbleCCFD *               getFirstPebble() = 0;
        /// Return the first const IPebbleCCFD in this CCFD clump.
        virtual const IPebbleCCFD *         getFirstPebble() const = 0;
        virtual const IClump *              getIClump() const=0;
        /// Return the number of pebbles.
        virtual uint                        getNumberPebbles() const=0;
        /// Return a list of pebbles.
        virtual void                        getPebbleList(FArray<IPebbleCCFD *> *)=0;
        /// Return a list of pebbles.
        virtual void                        getPebbleList(FArray<const IPebbleCCFD *> *) const=0;
        virtual IContainer                 *getClumpCCFDAsPebbleContainer() = 0;
        virtual const IContainer           *getClumpCCFDAsPebbleContainer() const = 0;
        virtual void                        getElements(FArray<ICcfdElement*> *) const=0;
        virtual DVect                       getFluidForce()  const=0;
        virtual void                        setFluidForce(const DVect &d)  =0;
    };

} // namespace ccfd
// EoF

