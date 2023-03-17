#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
    class ModelMohr : public ModelElastic {
    public:
        ModelMohr(unsigned short option=0);
        virtual String  getName() const { return L"mohr-coulomb"; }
        virtual String  getFullName() const { return L"Mohr-Coulomb";}
        virtual UInt    getMinorVersion() const; 
        virtual String  getProperties() const;
        virtual String  getStates() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelMohr *clone() const { return NEW("ModelMohr") ModelMohr(); }      
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    run(UByte dim,State *s); 
        virtual void    initialize(UByte dim,State *s);
        // Optional
        virtual bool    isPropertyAdvanced(UInt i) const;
        virtual bool    supportsStressStrengthRatio() const { return true; }
        virtual bool    supportsPropertyScaling() const { return true; }
        virtual Double  getStressStrengthRatio(const SymTensor &st) const;
        virtual void    scaleProperties(const Double &scale,const std::vector<UInt> &props);

    protected:
        virtual bool   updateParameters();
        virtual bool   updateParameters(bool bEUpdated,Double *sf1=nullptr,Double *sf3=nullptr);
        virtual Double moduliReduction(const Double &factor);
        virtual void   apexCorrection(const Double &fric,State *s,DVect3 *prin,UInt *iPlasticity=nullptr,bool bBrittle=false);
        virtual void   tensionCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &ftz,bool bBrittle=false);
        virtual void   shearCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &fs);

        Double cohesion_ = 0.0;
        Double friction_ = 0.0;
        Double dilation_ = 0.0;
        Double tension_ = 0.0;
        bool brittle_ = false;
        Double nph_  = 0.0;
        Double csn_ = 0.0;
        Double nps_ = 0.0;
        Double rc_ = 0.0;
        Double sc1_ = 0.0;
        Double sc2_ = 0.0;
        Double sc3_ = 0.0;
    };
} // namespace models
// EOF
