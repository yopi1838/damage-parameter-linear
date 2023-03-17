#pragma once
// imodulemeasure.h
// For Measure Module

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    class IBodyContainer;
    class IContainerContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IMeasure;

    class IModuleMeasure {
    public:
        virtual TType             getMeasureType() const=0;
        virtual uint              getMeasureGeometryNotice() const=0;
        virtual const IContainer *getIContainerMeasure() const=0;
        virtual IContainer *      getIContainerMeasure()=0;
        virtual const IMeasure *  findMeasureWithID(quint64 id) const=0;
        virtual IMeasure *        findMeasureWithID(quint64 id) =0;
        virtual void              populateAttributeSets(QStringList *scalarAtts,QStringList *stringAtts,
                                                                                                        QStringList *vectorAtts)const=0;
        virtual void              registerMeasureBodyContainer(IBodyContainer *)=0;
        virtual void              registerMeasureContainerContact(IContainerContact *)=0;
        virtual void              addNotificationInterest(uint notice)=0;
        virtual void              computeAttribute(quint32 mask)=0;
    };

} // namespace pfc
// EoF
