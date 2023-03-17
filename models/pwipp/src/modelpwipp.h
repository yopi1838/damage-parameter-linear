#pragma once

#include "models/drucker/src/modeldrucker.h"

namespace models {
    class ModelPWipp : public ModelDrucker  {
    public:
        ModelPWipp();
        virtual String  getName() const { return(L"wipp-drucker"); }
        virtual String  getFullName() const { return(L"WIPP with Drucker-Prager"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const; 
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion);
        virtual ModelPWipp *clone() const { return(NEW("ModelPWipp") ModelPWipp()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    isPropertyReadOnly(UInt i) const;
        virtual bool    isCreep() const { return true; }
        virtual bool    supportsHystereticDamping() const { return false; }
        virtual bool    supportsStressStrengthRatio() const { return false; }
        virtual bool    supportsPropertyScaling() const { return false; }

    private:
        Double temp_;
        Double actE_,aWipp_,bWipp_,dWipp_,eDot_;
        Double dGasC,nWipp_,ePrim_,eRate_;
        Double sHP_,tHP_;
    };
}
// EOF