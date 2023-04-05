#pragma once
// imodulemeasure.h
// For Measure Module

namespace itasca {
    class IContainer;
}

namespace itascaxd {
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IBrick;
    class IInlet;

    class IModuleBrick {
    public:
        virtual TType                 getBrickType() const=0;
        virtual TType                 getInletType() const=0;
        virtual uint                  getBrickGeometryNotice() const=0;
        virtual uint                  getInletGeometryNotice() const=0;
        virtual const IContainer     *getIContainerBrick() const=0;
        virtual IContainer *          getIContainerBrick()=0;
        virtual const IBrick *        findBrickWithID(quint64 id) const=0;
        virtual IBrick *              findBrickWithID(quint64 id) =0;
        virtual const IContainer*     getIContainerInlet() const = 0;
        virtual IContainer*           getIContainerInlet() = 0;
        virtual const IInlet*         findInletWithID(quint64 id) const = 0;
        virtual IInlet*               findInletWithID(quint64 id) = 0;
        virtual void                  populateInletAttributeSets(QStringList* scalarAtts, QStringList* stringAtts,
                                                                 QStringList* vectorAtts)const = 0;

    };

} // namespace pfc
// EoF
