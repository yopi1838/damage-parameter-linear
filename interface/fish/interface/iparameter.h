#pragma once

#include "base/src/vect.h"
#include "base/src/symtensor.h"
#include "shared/interface/itensordata.h"

namespace itasca {
    class IThing;
}

namespace fish {
    using itasca::IThing;
    class List;
    class Map;
    
    class IParameter {
    public:
        enum class TreatAs { Scalar,Vector,Any,Tensor };
        enum class Component { X,Y,Z,Mag };
        typedef itasca::ITensorData::Quantity Quantity;

        virtual ~IParameter() { }

        virtual QString getName() const=0;

        virtual bool getIsBool() const=0;
        virtual bool getIsInt() const=0;
        virtual bool getIsDouble() const=0;
        virtual bool getIsNumber() const=0;
        virtual bool getIsString() const=0;
        virtual bool getIsVect2() const=0;
        virtual bool getIsVect3() const=0;
        virtual bool getIsPoint() const=0;
        virtual bool getIsList() const = 0;
        virtual bool getIsMap() const = 0;

        virtual bool      getBool() const=0;
        virtual qint64    getInt(qint64 min=limits<qint64>::min(),qint64 max=limits<qint64>::max()) const=0;
        virtual double    getDouble(double min=-limits<double>::max(),double max=limits<double>::max()) const=0;
        virtual QString   getString() const=0;
        virtual DVect2    getVect2() const=0;
        virtual DVect3    getVect3() const=0;
        virtual IThing *  getPoint() const=0;
        virtual SymTensor getTensor() const=0;
        virtual QVariant  getQVariant() const=0;
        virtual List      getList() const = 0;
        virtual Map       getMap() const = 0;

        virtual double    safeToDouble(double min=-limits<double>::max(),double max=limits<double>::max()) const=0;
        virtual DVect2    safeToVect2() const=0;
        virtual DVect3    safeToVect3() const=0;
        virtual SymTensor safeToTensor() const=0;

        virtual QString list() const=0;

        virtual double getScalar(TreatAs type,Component comp,Quantity quant,bool stress) const = 0;

        virtual void copy(const IParameter &p)=0;
        // Sets to the value in the string, converting to the best type it can
        virtual void fromString(const QString &s)=0;
    };
} // namespace fish
// EoF

