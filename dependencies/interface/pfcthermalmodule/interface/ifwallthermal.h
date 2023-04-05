#pragma once
// ifwallthermal.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IContainer;
}

namespace itascaxd {
    class IBody;
    class IPiece;
    class IContact;
}

namespace pfc {
    class IFWall;
}

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    using namespace pfc;
    class IFWallFacetThermal;
    class IFWallThermal {
    public:
        inline static const TType type_ = 0x4fcbfdbf;

        virtual IThing *                     getIThing()=0;
        virtual const IThing *               getIThing() const=0;
        virtual IBody *                      getIBody()=0;
        virtual const IBody *                getIBody() const=0;
        virtual IBody *                      getIBodyMechanical()=0;
        virtual const IBody *                getIBodyMechanical() const=0;
        virtual IFWall *                     getIFWall()=0;    
        virtual IFWallFacetThermal *         getFirstFacet() = 0;
        virtual const IFWallFacetThermal *   getFirstFacet() const = 0;
        virtual const IFWall *               getIFWall() const=0;
        virtual void                         getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0) = 0;
        virtual void                         getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const = 0;
        virtual uint                         getNumberFacets() const=0;
        virtual void                         getFacetList(FArray<IFWallFacetThermal *> *)=0;
        virtual void                         getFacetList(FArray<const IFWallFacetThermal *> *) const=0;
        virtual IContainer                  *getFWallThermalAsFacetContainer() = 0;
        virtual const IContainer            *getFWallThermalAsFacetContainer() const = 0;
        virtual bool                         setProperty(const QString &name,const QVariant &v)=0;

    };

} // namespace pfcthermal
// EoF

