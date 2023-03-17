#pragma once
/** \brief Interface for the RBLOCK thermal module.
    * \file irblockthermalmodule.h
    *
    * \defgroup rblockthermalmodule RBlock thermal module interface specification
*/

// irblockthermalmodule.h
#include "base/src/farray.h"
#include "utility/interface/isignalbase.h"

namespace itasca {
    class IContainer;
    class IEnergyMap;
}

namespace itascaxd {
    class ICellSpace;
    class IContainerContact;
}

/** \brief RBLOCKTHERMAL namespace */
namespace rblockthermal {
    using namespace itasca;
    using namespace itascaxd;

    class IRBlockThermal;

    /// \addtogroup rblockthermalmodule
    /// @{

    /// \brief Interface for the RBlock thermal module.
    class IRBlockThermalModule {
    public:

        virtual TType                    getRBlockThermalType() const=0;
        virtual uint                     getRBlockThermalPropertyNotice() const=0;
        virtual uint                     getRBlockThermalCycleNotice() const=0;
        virtual uint                     getRBlockThermalFixNotice() const=0;
        virtual uint                     getRBlockThermalExtraNotice() const=0;
        virtual uint                     getRBlockThermalGroupNotice() const=0;
        virtual uint                     getRBlockThermalCleanNotice() const=0;
        virtual const IContainer       * getIContainerRBlockThermal() const=0;
        virtual IContainer *             getIContainerRBlockThermal()=0;
        virtual const IContainerContact *getIContainerRBlockRBlockContactThermal() const=0;
        virtual IContainerContact *      getIContainerRBlockRBlockContactThermal()=0;
        virtual const IRBlockThermal *     findRBlockThermalWithID(quint64 id) const=0;
        virtual IRBlockThermal *           findRBlockThermalWithID(quint64 id) =0;
        virtual const IRBlockThermal *     findRBlockThermalClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual IRBlockThermal *           findRBlockThermalClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        virtual const IEnergyMap       * getIRBlockThermalEnergies() const=0;
        virtual const IEnergyMap       * getIContactThermalRBlockRBlockEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

        typedef Signal2<const QString &,const QVariant &> prop_signal;

        /// Return the signal associated with the addition of a new property
        virtual prop_signal * getPropertySignal() const = 0;

    };
    /// @}

} // namespace rblockthermal
// EoF
