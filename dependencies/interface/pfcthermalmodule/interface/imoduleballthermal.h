#pragma once
// imoduleballthermal.h
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

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    class IBallThermal;

    class IModuleBallThermal {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual TType                    getBallThermalType() const=0;
        virtual uint                     getBallThermalPropertyNotice() const=0;
        virtual uint                     getBallThermalCycleNotice() const=0;
        virtual uint                     getBallThermalFixNotice() const=0;
        virtual uint                     getBallThermalExtraNotice() const=0;
        virtual uint                     getBallThermalGroupNotice() const=0;
        virtual uint                     getBallThermalCleanNotice() const=0;
        virtual const IContainer       * getIContainerBallThermal() const=0;
        virtual IContainer *             getIContainerBallThermal()=0;
        virtual const IContainerContact *getIContainerBallBallContactThermal() const=0;
        virtual IContainerContact *      getIContainerBallBallContactThermal()=0;
        virtual const IBallThermal *     findBallThermalWithID(quint64 id) const=0;
        virtual IBallThermal *           findBallThermalWithID(quint64 id) =0;
        virtual const IBallThermal *     findBallThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IBallThermal *           findBallThermalClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        virtual const IEnergyMap       * getIBallThermalEnergies() const=0;
        virtual const IEnergyMap       * getIContactThermalBallBallEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

    };
} // namespace pfcthermal
// EoF
