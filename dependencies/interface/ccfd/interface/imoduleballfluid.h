#pragma once
// imoduleballfluid.h
// For BALLTHERMAL Module
#include "utility/interface/isignalbase.h"
#include "base/src/farray.h"

namespace itasca {
    class IContainer;
    class IEnergyMap;
}

namespace itascaxd {
    class ICellSpace;
    class IContainerContact;
}

namespace ccfd {
    using namespace itasca;
    using namespace itascaxd;

    class IBallFluid;

    class IModuleBallFluid {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual TType                    getBallFluidType() const=0;
        virtual uint                     getBallFluidCycleNotice() const=0;
        virtual uint                     getBallFluidExtraNotice() const=0;
        virtual uint                     getBallFluidGroupNotice() const=0;
        virtual uint                     getBallFluidCleanNotice() const=0;
        virtual const IContainer       * getIContainerBallFluid() const=0;
        virtual IContainer *             getIContainerBallFluid()=0;
        virtual const IBallFluid *       findBallFluidWithID(quint64 id) const=0;
        virtual IBallFluid *             findBallFluidWithID(quint64 id) =0;
        virtual const IBallFluid *       findBallFluidClosestTo(const DVect3 &v,const double &radius=limits<double>::max()) const=0;
        virtual IBallFluid *             findBallFluidClosestTo(const DVect3 &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        //virtual const IEnergyMap       * getIBallFluidEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

    };
} // namespace ccfd
// EoF
