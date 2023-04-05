#pragma once

namespace zone {
    struct Zone;

    class IFetchFluidProperty {
    public:
        virtual void        initializeMultithreading(quint32 threads)=0;
        virtual bool        getProperty(const Zone *zone,double *val) const=0;
        virtual bool        getProperty(const Zone *zone,double *val,quint32 thread,quint32 block) const=0;
        virtual bool        setProperty(Zone *zone,double val)=0;
        virtual bool        setProperty(Zone *zone,double val,quint32 thread,quint32 block) const=0;
        virtual QString     getName() const=0;
        virtual void        setName(const QString &s)=0;
        virtual void        destroy()=0;
    };
} // namespace itascaxd
  // EoF
