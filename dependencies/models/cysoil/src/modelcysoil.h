#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
    class ModelCYSoil : public ModelMohr {
    public:
        // Creators
        ModelCYSoil();
        virtual String   getName() const { return(L"cap-yield"); }
        virtual String   getFullName() const { return(L"Cap-Yield"); }
        virtual UInt     getMinorVersion() const;
        virtual String   getProperties() const;
        virtual String   getStates() const;
        virtual Variant  getProperty(UInt index) const;
        virtual void     setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelCYSoil *clone() const { return(NEW("ModelCYSoil") ModelCYSoil()); }
        virtual Double   getConfinedModulus() const { return(bulk_ + shear_*d4d3); }
        virtual Double   getShearModulus() const { return(shear_); }
        virtual Double   getBulkModulus() const { return(bulk_); }
        virtual void     copy(const ConstitutiveModel *mod);
        virtual void     initialize(UByte dim,State *s);
        virtual void     run(UByte dim,State *s);
        // Optional
        virtual bool     isModelAdvanced() const { return true; }
        virtual bool     isPropertyReadOnly(UInt i) const;
        virtual bool     isPropertyAdvanced(UInt i) const;
        virtual bool     supportsHystereticDamping() const { return false; }
        virtual bool     supportsStressStrengthRatio() const { return false; }
        virtual bool     supportsPropertyScaling() const { return false;}

    private:
        void             updMobVars(State *s);

        double   bulk_max = 0.0;
        double   shear_max = 0.0;
        double   poisson_ = 0.2;
        double   frictUlt_ = 0.0;
        double   dilatUlt_ = 0.0;
        double   capPressure_ = 0.0;
        double   multiplier_ = 0.0;
        double   vHP_ = 0.0;
        double   sHP_ = 0.0;
        double   tHP_ = 0.0;
        String   cTab_,dTab_,fTab_,pTab_,tTab_;
        void *   icohesion_ = nullptr;
        void *   idilation_ = nullptr;
        void *   ifriction_ = nullptr;
        void *   icapPressure_ = nullptr;
        void *   itension_ = nullptr;
        double   alpha_ = 1.0;
        double   frictCV_ = 0.0;
        double   dSfrictUlt_ = 0.0; 
        double   dSfriction_ = 0.0;
        double   dSfrictCV_ = 0.0;
        double   dSFrict0_ = 0.0;
        double   dSdilation_ = 0.0;
        double   dSFrictdiff_ = 0.0;
        double   pnew_ = 0.0;
        double   qnew_ = 0.0;
        double   Gref_ = 0.0;
        double   pref_ = 100.0;
        double   m_ = 0.5;
        double   rf_ = 0.9;
        double   dcoe_ = 0.0;
        double   frict0_ = 0.0;
        double   beta_ = 1.0;
        double   pini_ = 0.0;
        double   ocr_ = 1.0;
        unsigned cFlag_ = 0;
        unsigned sFlag_ = 0;
        unsigned dFlag_ = 0;
        double   eini_ = 1.0;
        double   emax_ = 999.0;
        double   enow_ = 0.0;
        double   ev_ = 0.0;
        double   shear_min = 0.0;
    };
}
