#pragma once
// imoduleclumpthermal.h
// For CLUMPTHERMAL Module
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

    class IClumpThermal;
    class IPebbleThermal;

    class IModuleClumpThermal {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual TType                    getClumpThermalType() const=0;
        virtual uint                     getClumpThermalPropertyNotice() const=0;
        virtual uint                     getClumpThermalCycleNotice() const=0;
        virtual uint                     getClumpThermalFixNotice() const=0;
        virtual uint                     getClumpThermalExtraNotice() const=0;
        virtual uint                     getClumpThermalGroupNotice() const=0;
        virtual uint                     getClumpThermalCleanNotice() const=0;
        virtual const IContainer       * getIContainerClumpThermal() const=0;
        virtual IContainer *             getIContainerClumpThermal()=0;
        virtual const IContainer       * getIContainerPebbleThermal() const=0;
        virtual IContainer *             getIContainerPebbleThermal()=0;
        virtual const IContainerContact *getIContainerPebblePebbleContactThermal() const=0;
        virtual IContainerContact *      getIContainerPebblePebbleContactThermal()=0;
        virtual const IClumpThermal *    findClumpThermalWithID(quint64 id) const=0;
        virtual IClumpThermal *          findClumpThermalWithID(quint64 id) =0;
        virtual const IClumpThermal *    findClumpThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IClumpThermal *          findClumpThermalClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        virtual TType                    getPebbleThermalType() const=0;
        virtual const IPebbleThermal *   findPebbleThermalWithID(quint64 id) const = 0;
        virtual IPebbleThermal *         findPebbleThermalWithID(quint64 id) = 0;
        virtual const IEnergyMap       * getIClumpThermalEnergies() const=0;
        virtual const IEnergyMap       * getIContactThermalPebblePebbleEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

    };
} // namespace pfcthermal
// EoF
