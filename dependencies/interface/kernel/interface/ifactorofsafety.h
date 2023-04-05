#pragma once

#include "base/src/baseqstring.h"

namespace itascaxd {
    class IFactorOfSafety {
    public:
        using FOSData    = std::vector<double>; // Velocity mag at given model object
        using FOSDataMap = std::map<quint64,FOSData>; // Map model object ID to velocity mags
        using FOSTypeMap = QStringMap<FOSDataMap>; // Map model object name to data map
        using FOSValues  = std::vector<double>; // Stores FOS values at each index, which will
                                                //    correspond to data in each FOSData.
        
        virtual const FOSDataMap &getDataMap(const QString &s) const=0;
        virtual const FOSValues &getFOSValues() const=0;
        virtual double getFOSData(const QString &s,quint64 id,double velocityLimit) const=0;
        virtual uint   getFOSNotice() const=0;
    };
} // itascaxd
// EoF
