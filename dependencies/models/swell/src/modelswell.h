#pragma once

#include "models/mohr/src/modelmohr.h"
#include "models/base/modelweakplanetype.h"

namespace models {
    class ModelSwell : public ModelMohr, public ModelWeakplaneType {
    public:
        ModelSwell();
        virtual String   getName() const { return(L"swell"); }
        virtual String   getFullName() const { return(L"Swell"); }
        virtual UInt     getMinorVersion() const;
        virtual String   getProperties() const;
        virtual Variant  getProperty(UInt index) const;
        virtual void     setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelSwell *clone() const { return(NEW("ModelSwell") ModelSwell()); }
        virtual void     copy(const ConstitutiveModel *mod);
        virtual void     initialize(UByte d,State *s);
        virtual void     run(UByte dim,State *s);
        // Optional
        virtual bool     isModelAdvanced() const { return true; }
        virtual bool     isPropertyReadOnly(UInt i) const;
        virtual bool     isPropertyAdvanced(UInt i) const;
        virtual bool     supportsHystereticDamping() const { return true; }
        virtual bool     supportsStressStrengthRatio() const { return false; }
        virtual bool     supportsPropertyScaling() const { return false;}

    private:
        Double a1_,c1_,a3_,c3_,m1_,m3_;
        Double pressure_,sv0_,dsxx_,dsyy_,dszz_,dsxy_,dsyz_,dszx_;
        UInt   ninc_,cinc_,modnum_;
        Double normX_,normY_,normZ_;
    };
}
//EOF