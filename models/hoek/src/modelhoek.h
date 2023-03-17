#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
  class ModelHoek : public ModelMohr {
    public:
        ModelHoek();
        virtual String     getName() const { return L"hoek-brown"; }
        virtual String     getFullName(void) const { return L"Hoek-Brown"; }
        virtual UInt       getMinorVersion() const;
        virtual String     getProperties() const;
        virtual Variant    getProperty(UInt index) const;
        virtual void       setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelHoek *clone() const { return new ModelHoek(); }
        virtual void       copy(const ConstitutiveModel *mod);
        virtual void       initialize(UByte dim,State *s);
        virtual void       run(UByte dim,State *s);
        // Optional
        virtual bool       isPropertyReadOnly(UInt i) const;
        virtual bool       isPropertyAdvanced(UInt i) const;
        virtual bool       supportsStressStrengthRatio() const { return true; }
        virtual bool       supportsPropertyScaling() const { return true; }
        virtual void       scaleProperties(const Double &scale,const std::vector<UInt> &props);

    private:
        Double scale_;
        Double hb_mmi_, hb_mmc_;
        Double hb_ssi_, hb_ssc_;
        Double hb_aai_, hb_aac_;
        Double hb_sci_, hb_scc_;
        Double hb_cd_,  hoek_psi_;
        Double hb_len;
        UInt   hb_sop;
        String mTab_,sTab_,aTab_,cTab_,mULTab_,tTab_;
        void *iMTab_,*iSTab_,*iATab_,*iCTab_,*iMULTab_,*iTTab_;
        Double hb_evolution_;
        UInt   bEplas_s_;
    };
}
//EOF
