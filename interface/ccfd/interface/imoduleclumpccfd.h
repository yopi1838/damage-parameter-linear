#pragma once
// imoduleclumpccfd.h
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

    class IClumpCCFD;
    class IPebbleCCFD;

    class IModuleClumpCCFD {
    public:
        typedef Signal2<const QString &,const QVariant &> prop_signal;

        virtual TType                    getClumpCCFDType() const=0;
        virtual TType                    getPebbleCCFDType() const=0;
        virtual uint                     getClumpCCFDCycleNotice() const=0;
        virtual uint                     getClumpCCFDExtraNotice() const=0;
        virtual uint                     getClumpCCFDGroupNotice() const=0;
        virtual uint                     getClumpCCFDCleanNotice() const=0;
        virtual const IContainer       * getIContainerClumpCCFD() const=0;
        virtual IContainer *             getIContainerClumpCCFD()=0;
        virtual const IClumpCCFD *       findClumpCCFDWithID(quint64 id) const=0;
        virtual IClumpCCFD *             findClumpCCFDWithID(quint64 id) =0;
        virtual const IContainer       * getIContainerPebbleCCFD() const=0;
        virtual IContainer *             getIContainerPebbleCCFD()=0;
        virtual const IPebbleCCFD *      findPebbleCCFDWithID(quint64 id) const=0;
        virtual IPebbleCCFD *            findPebbleCCFDWithID(quint64 id) =0;
        virtual const IClumpCCFD *       findClumpCCFDClosestTo(const DVect3 &v,const double &radius=limits<double>::max()) const=0;
        virtual IClumpCCFD *             findClumpCCFDClosestTo(const DVect3 &v,const double &radius=limits<double>::max())=0;
        virtual void                     validate()=0;
        //virtual const IEnergyMap       * getIClumpCCFDEnergies() const=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;
        // Populates sets of property names based on their type
        virtual void  populatePropertySets(QStringList *scalarProps,QStringList *stringProps,QStringList *vectorProps)const=0;

    };
} // namespace ccfd
// EoF
