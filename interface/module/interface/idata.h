#pragma once

#include "shared/interface/itensordata.h"
#include "fish/interface/iparameter.h"

namespace itasca {
    class Archive2;
    class IParse;
}

namespace itascaxd {
    using itasca::Archive2;
    using itasca::IParse;
    using itasca::ITensorData;

    class IData : public ITensorData {
    public:
        typedef fish::IParameter::TreatAs TreatAs;
        typedef fish::IParameter::Component Component;
        typedef itasca::ITensorData::Quantity Quantity;

        virtual QObject *      getQObject()=0;
        virtual const QObject *getQObject() const=0;

        virtual int  getIndex() const=0;
        virtual int  getComponent() const=0;
        virtual int  getTreatAs() const=0;
        virtual int  getQuantity() const=0;
        virtual int  getExtra() const=0; // Base 1!
        virtual bool getStress() const=0;
        virtual bool getLog() const=0;

        virtual QString     getName() const=0;
        virtual QString     getSpecificName() const=0;

        virtual bool getActive() const=0;
        virtual bool getComponentActive() const=0;
        virtual bool getTreatAsActive() const=0;
        virtual bool getQuantityActive() const=0;
        virtual bool getExtraActive() const=0;
        virtual bool getStressActive() const=0;

        virtual QStringList getNames() const=0;
        virtual QStringList getBaseKeywords() const=0;
        virtual QList<TreatAs> getAllTreatAs() const=0;
        virtual QStringList getAllKeywords() const=0;

        virtual void destroy()=0;
        virtual QStringList getComponentNames() const=0;
        virtual QStringList getQuantityNames() const=0;
        virtual QStringList getQuantityKeywords() const=0;
        virtual QStringList getTreatAsNames() const=0;

        //virtual void save(QDataStream &ds) const=0;
        //virtual void restore(QDataStream &ds)=0;
        virtual void save(Archive2 &a) const=0;
        virtual bool restore(Archive2 &a,quint64 label)=0;
        virtual void remap(Archive2 &a)=0;

        virtual bool parseTreatAs(IParse &par)=0;
        virtual bool parse(IParse &par)=0;
        virtual QString getIndexKeyword() const=0;
        virtual QStringList getCommandSwitches() const=0;

        // SLOTS
        virtual void setActive(bool b)=0;
        virtual void setIndex(int i)=0;
        virtual void setIndex(const QString &s)=0;
        virtual void setIndexFromKeyword(int i)=0;
        virtual void setComponent(int i)=0;
        virtual void setTreatAs(int i)=0;
        virtual void setQuantity(int i)=0;
        virtual void setExtra(int i)=0; // Base 1!
        virtual void setStress(bool b)=0;
        virtual void setLog(bool b)=0;

        // SIGNALS
        //virtual void activeChanged(bool active)=0;
        virtual void indexChanged(int index)=0;
        virtual void indexChanged(const QString &index)=0;
        virtual void componentChanged(int component)=0;
        virtual void componentActiveChanged(bool active)=0;
        virtual void treatAsChanged(int type)=0;
        virtual void treatAsActiveChanged(bool active)=0;
        virtual void quantityChanged(int quantity)=0;
        virtual void quantityActiveChanged(bool active)=0;
        virtual void extraChanged(int extra)=0; // Base 1!
        virtual void extraActiveChanged(bool active)=0;
        virtual void stressChanged(bool stress)=0;
        virtual void stressActiveChanged(bool active)=0;
        virtual void logChanged(bool b)=0;
    };
}

// EoF
