#pragma once
// imoduleballmechanical.h
// For BALL mechanical extension module
#include "utility/interface/isignalbase.h"
#include "base/src/farray.h"

namespace itasca {
    class IContainer;
    class IEnergyMap;
}

namespace itascaxd {
    class IContact;
    class ICellSpace;
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IModuleBallMechanical {
    public:

        virtual TType                    getBallMechanicalType() const=0;
        virtual uint                     getBallVelocityNotice() const=0;
        virtual uint                     getBallFixNotice() const=0;
        virtual uint                     getBallRotationNotice() const=0;
        //virtual const IContainerContact *getIContainerBallBallContact() const=0;
        //virtual IContainerContact *      getIContainerBallBallContact()=0;
        //virtual const IContainerContact *getIContainerDomainBallContact() const=0;
        //virtual IContainerContact *      getIContainerDomainBallContact()=0;
        virtual void                     validate()=0;
        virtual const IEnergyMap       * getIBallMechanicalEnergies() const=0;
        //virtual const IEnergyMap       * getIContactBallBallEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;

    };
} // namespace pfc
// EoF
