#pragma once
// iinlet.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
}
namespace itascaxd {
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;
    class IBrick;

    class IInlet {
    public:
        inline static const TType type_ = 0x5fd09f1b;

        virtual IThing* getIThing() = 0;
        virtual const IThing* getIThing() const = 0;

        virtual QVariant      getAttribute(const QString& name) const = 0;
        virtual QStringList   getAllAttributes() const = 0;
        virtual void          getFacetGeometry(FArray<DVect>* vert) const = 0;

        virtual DExtent3 getExtent()        const = 0;
        virtual DVect    getPosition()      const = 0;
        virtual DVect    getFlowVector()    const = 0;
        virtual DVect    getTransVelocity() const = 0;
        virtual DVect    getRotCenter()     const = 0;
        virtual DAVect   getOrientation()   const = 0;
        virtual DAVect   getAngVelocity()   const = 0;
        virtual bool     getActive()        const = 0;
        virtual IBrick*  getBrick()         const = 0;

        virtual void  setPosition(const DVect &)     = 0;
        virtual void  setFlowVector(const DVect&)    = 0;
        virtual void  setTransVelocity(const DVect&) = 0;
        virtual void  setRotCenter(const DVect&)     = 0;
        virtual void  setOrientation(const DAVect&)  = 0;
        virtual void  setAngVelocity(const DAVect&)  = 0;
        virtual void  setActive(bool)                = 0;
        virtual void  setBrick(uint)                 = 0;
    };

} // namespace pfc
// EoF

