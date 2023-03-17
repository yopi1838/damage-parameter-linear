#pragma once

#include "models/ssoft/src/modelssoft.h"

namespace models {
    class ModelDoubleYield : public ModelStrainSoftening {
    public:
        ModelDoubleYield();
        virtual String  getName() const { return(L"double-yield"); }
        virtual String  getFullName() const { return(L"Double-Yield"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const;
        virtual String  getStates() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelDoubleYield *clone() const { return(NEW("ModelDoubleYield") ModelDoubleYield()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    isModelAdvanced() const { return true; }
        virtual bool    isPropertyReadOnly(UInt i) const;
        virtual bool    isPropertyAdvanced(UInt i) const;
        virtual bool    supportsHystereticDamping() const { return true; }
        virtual bool    supportsStressStrengthRatio() const { return true; }
        virtual bool    supportsPropertyScaling() const { return false;}

    private:
        Double capPressure_ = 0.0;
        Double multiplier_ = 0.0;
        Double vHP_ = 0.0;
        String pTable_;
        void *iCapPressure_ = nullptr;
        Double bulk_max = 0.0; 
        Double shear_max = 0.0;
    };
}
// EOF