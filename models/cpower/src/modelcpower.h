#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
    class ModelCPow : public ModelMohr {
    public:
        ModelCPow();

        virtual String  getName() const { return(L"power-mohr"); }
        virtual String  getFullName() const { return(L"Power-Law with Mohr-Coulumb"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const; 
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelCPow *clone() const { return(NEW("ModelCPow") ModelCPow()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    isCreep() const { return true; }
        virtual bool    supportsHystereticDamping() const { return false; }
        virtual bool    supportsStressStrengthRatio() const { return false; }
        virtual bool    supportsPropertyScaling() const { return false; }

    private:
        Double a1_,a2_,n1_,n2_,rs1_,rs2_;
    };
}
// EOF
