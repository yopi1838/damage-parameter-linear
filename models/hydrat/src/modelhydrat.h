#pragma once

#include "models/drucker/src/modeldrucker.h"

namespace models {
    class ModelHydrat : public ModelDrucker {
    public:
        // Creators
        ModelHydrat();
        virtual String  getName() const { return L"hydration-drucker-prager"; }
        virtual String  getFullName() const { return L"Hydration Drucker-Prager"; }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelHydrat *clone() const { return NEWC(ModelHydrat()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s);
        // Optional
        virtual bool    isPropertyReadOnly(UInt i) const;
        virtual bool    supportsHystereticDamping() const { return false; }
        virtual bool    supportsStressStrengthRatio() const { return false; }
        virtual bool    supportsPropertyScaling() const { return false; }
        virtual bool    isModelAdvanced() const { return true; }

    private:
        Double bulk_cte_,shear_cte_,young_cte_,tension_cte_;
        Double alpha_0_,c_,a_,dAlpha_min_;
        Double young_,poisson_,compression_;
    };
}
