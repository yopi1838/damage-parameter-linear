#pragma once
// imodulefwallthermal.h
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

    class IFWallThermal;
    class IFWallFacetThermal;

    class IModuleFWallThermal {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual TType                    getFWallThermalType() const=0;
        virtual uint                     getFWallThermalPropertyNotice() const=0;
        virtual uint                     getFWallThermalCycleNotice() const=0;
        virtual uint                     getFWallThermalExtraNotice() const=0;
        virtual uint                     getFWallThermalGroupNotice() const=0;
        virtual uint                     getFWallThermalCleanNotice() const=0;
        virtual const IContainer       * getIContainerFWallThermal() const=0;
        virtual IContainer *             getIContainerFWallThermal()=0;
        virtual const IContainer       * getIContainerFWallFacetThermal() const=0;
        virtual IContainer *             getIContainerFWallFacetThermal()=0;
        virtual const IFWallThermal *    findFWallThermalWithID(quint64 id) const=0;
        virtual IFWallThermal *          findFWallThermalWithID(quint64 id) =0;
        virtual const IFWallThermal *    findFWallThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IFWallThermal *          findFWallThermalClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        virtual TType                    getFWallFacetThermalType() const=0;
        virtual const IFWallFacetThermal *   findFWallFacetThermalWithID(quint64 id) const = 0;
        virtual IFWallFacetThermal *         findFWallFacetThermalWithID(quint64 id) = 0;
        virtual const IFWallFacetThermal *   findFWallFacetThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) const = 0;
        virtual IFWallFacetThermal *         findFWallFacetThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) = 0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        /// Populates sets of piece property names based on their type
        virtual void  populatePieceAttributeSets(QStringList *valueAtts,QStringList *labelAtts,QStringList *vectorAtts)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

    };
} // namespace pfcthermal
// EoF
