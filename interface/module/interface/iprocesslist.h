#pragma once

namespace itascaxd {
    class IProcess;

    class IProcessList {
    public:
        virtual IProcess *                    findProcess(const QString &name)=0;
        virtual const IProcess *              findProcess(const QString &name) const=0;
        virtual QList<IProcess*>              getAll() const = 0;
        virtual QList<QPair<QString,quint64>> getStepNumbers() const=0;
        virtual QList<QPair<QString,double>>  getModelTimes() const=0;

    };
} // namespace itascaxd
// EoF
