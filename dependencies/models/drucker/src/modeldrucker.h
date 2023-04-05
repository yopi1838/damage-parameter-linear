#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
    class ModelDrucker : public ModelElastic {
    public:
        ModelDrucker();

        virtual String  getName() const { return(L"drucker-prager"); }
        virtual String  getFullName() const { return(L"Drucker Prager"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const;
        virtual String  getStates() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelDrucker *clone() const { return(NEW("ModelDrucker") ModelDrucker()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    supportsStressStrengthRatio() const { return true; }
        virtual bool    supportsPropertyScaling() const { return false; }
        //virtual void    scaleProperties(const Double &scale,const std::vector<UInt> &props);

    protected:
        virtual bool updateParameters();
        virtual Double moduliReduction(const Double &factor);
        virtual void apexCorrection(State *s,UInt *iPlasticity);
        virtual void tensionCorrection(State *s,UInt *iPlasticity,const Double &ft);
        virtual void shearCorrection(State *s,UInt *iPlasticity,const Double &fs, const SymTensor &dev);
        virtual Double getStressStrengthRatio(const SymTensor &st) const;
        //
    protected:
        Double qPhi_,kPhi_,qPsi_,tension_;
        Double gKQ_,kQ_,rc_,tau_,sig_;
    };
}
// EOF