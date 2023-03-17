#pragma once
// imeasure.h


namespace itasca {
    class IThing;
    class IGroup;
}

namespace itascaxd {
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /* Measure Mask Defines */
    static const quint32 MEAS_POROS                = 0x000001;  /* Porosity             */
    static const quint32 MEAS_COORD                = 0x000002;  /* Coordination number  */
    static const quint32 MEAS_STRESS               = 0x000004;  /* Stress Tensor        */
    static const quint32 MEAS_SRATE                = 0x000008;  /* Strain Rate Tensor   */
    static const quint32 MEAS_SIZE                 = 0x000010;  /* Size distribution    */

    class IMeasure {
    public:
        inline static const TType type_ = 0x4c8f0e17;

        virtual IThing *                getIThing()=0;
        virtual const IThing *          getIThing() const=0;

        virtual DVect             getPosition()        const=0;
        virtual double            getRadius()          const=0;
        virtual double            getVolume()          const=0;
        virtual double            getPorosity()        const=0;
        virtual double            getCoordination()    const=0;
        virtual DMatrix<DIM,DIM>  getStress()          const=0;
        virtual DMatrix<DIM,DIM>  getSRate()           const=0;
        virtual QVector<QPair<double,double>> getSizeDist()        const=0;

        virtual void setPosition(const DVect &p)=0;
        virtual void setRadius(const double &d)=0;

        virtual bool          setAttribute(const QString &name,const QVariant &v)=0;
        virtual QVariant      getAttribute(const QString &name) const=0;
        virtual bool          getAttributeSupportsValid(const QString &name) const=0;
        virtual bool          getAttributeValid(const QString &name) const=0;
        virtual QStringList   getAllAttributes() const=0;
        virtual bool          isAttribute(const QString &name) const=0;

    };

} // namespace pfc
// EoF

