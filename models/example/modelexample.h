#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
    class ModelExample : public ModelElastic {
    public:
        ModelExample(unsigned short option=0);
        virtual String        getName() const override;
        virtual String        getFullName() const override;
        virtual UInt          getMinorVersion() const override; 
        virtual String        getProperties() const override;
        virtual String        getStates() const override;
        virtual Variant       getProperty(UInt index) const override;
        virtual void          setProperty(UInt index,const Variant &p,UInt restoreVersion=0) override;
        virtual ModelExample *clone() const override { return new ModelExample(); }      
        virtual void          copy(const ConstitutiveModel *mod) override;
        virtual void          run(UByte dim,State *s) override; 
        virtual void          initialize(UByte dim,State *s) override;
        // Optional
        virtual bool          isPropertyAdvanced(UInt i) const override;
        virtual bool          supportsStressStrengthRatio() const override { return true; }
        virtual bool          supportsPropertyScaling() const override { return true; }
        virtual Double        getStressStrengthRatio(const SymTensor &st) const override;
        virtual void          scaleProperties(const Double &scale,const std::vector<UInt> &props) override;

    private:
        virtual bool          updateParameters() override;
        virtual bool          updateParameters(bool bEUpdated,Double *sf1=nullptr,Double *sf3=nullptr);
        virtual Double        moduliReduction(const Double &factor) override;
        virtual void          apexCorrection(const Double &fric,State *s,DVect3 *prin,UInt *iPlasticity=nullptr,bool bBrittle=false);
        virtual void          tensionCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &ftz,bool bBrittle=false);
        virtual void          shearCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &fs);

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
