#pragma once
// isolvelimits.h
#include "iparse.h"

namespace itascaxd {
    using namespace itasca;
    /// \brief Interface for accessing solve command limits.
    /// \ingroup module
    class ISolveData {
    public:
        /// Returns a list of keywords used by the solve coomand, separated by commas. For example, cycle,age,unbalanced,aratio,mratio,lratio.
        virtual const QString  getKeywords() const = 0;

        virtual bool     getIsLimit(const QString &str) =0;
        virtual QVariant getCurrentLimit(const QString &str)=0;
        virtual QVariant getTargetLimit(const QString &str)=0;
        virtual bool     getTargetLimitSet(const QString &str)=0;
        virtual bool     getTargetLimitMet(const QString &str)=0;
    };
} // namespace itascaxd
// EoF
