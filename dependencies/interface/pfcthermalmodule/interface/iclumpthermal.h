#pragma once
// iclumpthermal.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IContainer;
}

namespace itascaxd {
    class IBody;
    class IBodyThermal;
    class IPiece;
    class IContact;
}

namespace pfc {
    class IClump;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;
    class IPebbleThermal;
    class IClumpThermal {
    public:
        inline static const TType type_ = 0x4fc90342;

        virtual IThing *                     getIThing()=0;
        virtual const IThing *               getIThing() const=0;
        virtual IBody *                      getIBody()=0;
        virtual const IBody *                getIBody() const=0;
        virtual IBodyThermal *               getIBodyThermal()=0;
        virtual const IBodyThermal *         getIBodyThermal() const=0;
        virtual IBody *                      getIBodyMechanical()=0;
        virtual const IBody *                getIBodyMechanical() const=0;
        virtual IClump *                     getIClump()=0;    
        /// Return the first IPebbleThermal in this thermal clump.
        virtual IPebbleThermal *             getFirstPebble() = 0;
        /// Return the first const IPebbleThermal in this thermal clump.
        virtual const IPebbleThermal *       getFirstPebble() const = 0;
        virtual const IClump *               getIClump() const=0;
        virtual void                         getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0) = 0;
        virtual void                         getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const = 0;
        /// Return the number of contacts. 
        virtual quint64     getContactCount(bool active,const TType &type=0,bool domainToo = false) const=0;
        /// Return the number of pebbles.
        virtual uint                         getNumberPebbles() const=0;
        /// Return a list of pebbles.
        virtual void                         getPebbleList(FArray<IPebbleThermal *> *)=0;
        /// Return a list of pebbles.
        virtual void                         getPebbleList(FArray<const IPebbleThermal *> *) const=0;
        virtual IContainer                  *getClumpThermalAsPebbleContainer() = 0;
        virtual const IContainer            *getClumpThermalAsPebbleContainer() const = 0;
        virtual bool                         setProperty(const QString &name,const QVariant &v)=0;
    };

} // namespace pfcthermal
// EoF

