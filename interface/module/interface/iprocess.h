#pragma once
// iprocess.h
#include "iparse.h"

namespace itasca {
    class IEnergyMap;
}

namespace itascaxd {
    using namespace itasca;
    class ISolveData;

    class IProcess {
    public:
        /// Return process name used for save/restore
        virtual QString     getName() const=0;
        /// Return process keyword used for commands and output
        virtual QString     getKeyword() const=0;
        virtual QObject *   getQObject()=0;
        virtual bool        getEnergyOn() const=0;
        virtual IEnergyMap *getEnergies() const=0;
        virtual bool        getConfigured() const=0;
        virtual bool        getSlave() const=0;
        virtual bool        getActive() const=0;
        virtual bool        getOn() const=0;
        virtual double      getMaxTimestep() const=0;
        virtual double      getGivenTimestep() const=0;
        virtual double      getDTCalcMode() const=0;
        virtual uint        getDTCalcInt() const=0;
        virtual double      getDTIncrement() const=0;
        virtual DVect2      getTimestepLimits() const=0;
        /// Returns the safety factor, by which the computed stable timestep is multiplied.
        virtual double      getSafetyFactor() const=0;
        virtual uint        getSubstep() const=0;

        // SLOTS
        virtual void setSlave(bool b)=0;
        virtual void setActive(bool b)=0;
        virtual void setOn(bool b)=0;
        virtual void setGivenTimestep(const double &d)=0;
        virtual void setMaxTimestep(const double &d)=0;
        virtual void setDTCalcMode(uint)=0;
        virtual void setDTCalcInt(uint)=0;
        virtual void setSafetyFactor(const double &d)=0;
        virtual void setEnergyOn(bool b)=0;
        virtual void setSubstep(uint)=0;

        // SIGNALS
        virtual void targetSolveLimitsChanged(const QStringList &)=0;
        virtual void slaveChanged(bool b)=0;
        virtual void activeChanged(bool b)=0;
        virtual void onChanged(bool b)=0;
        virtual void maxTimestepChanged(const double &d)=0;
        virtual void givenTimestepChanged(const double &d)=0;
        virtual void safetyFactorChanged(const double &d)=0;
        virtual void energyOnChanged(bool b)=0;
        virtual void energyMapChanged()=0;
        virtual void substepChanged(uint)=0;
    };
} // namespace itascaxd
// EoF
