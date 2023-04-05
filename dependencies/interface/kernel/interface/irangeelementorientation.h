#pragma once

#include "module/interface/irangeelement.h"

namespace itascaxd {
    class IRangeElementOrientation { // name = "orientation"
    public:
        /// type code
        inline static const TType type_ = 0x4c8f35be;
    
        /// returns orientation object
        virtual Orientation3   getOrientation() const=0;
        /// returns normal tolerance
        virtual Double         getNormalTolerance() const=0;
        /// returns dip tolerance
        virtual Double         getDipTolerance() const=0;
        ///eturns dip direction tolerance
        virtual Double         getDipDirectionTolerance() const=0;
        /// returns pointer to filter element
        virtual IRangeElement *getIRangeElement()=0;
        /// sets orientation filter element
        virtual void           setOrientation(const Orientation3  &o)=0;
        /// sets orientation filter normal tolerance
        virtual void           setNormalTolerance(double d)=0;
        /// sets orientation filter dip tolerance
        virtual void           setDipTolerance(double d)=0;
        /// sets orientation filter dip direction tolerance
        virtual void           setDipDirectionTolerance(double d)=0;
    
        /// signals
        /// signal orientation changed
        /// \param o orientation
        virtual void orientationChanged(const Orientation3 &o)=0;
        /// signal dip tolerance changed
        /// \param d tolerance
        virtual void tolerance1Changed(double d)=0;
        /// signal dd tolerance changed
        /// \param d tolerance
        virtual void tolerance2Changed(double d)=0;
    };
} // namespace itascaxd
// EoF

