#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
    class ModelStrainSoftening : public ModelMohr {
    public:
        ModelStrainSoftening();
        virtual String  getName() const { return(L"strain-softening"); }
        virtual String  getFullName() const { return(L"Strain Softening"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const; 
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelStrainSoftening *clone() const { return(NEW("ModelStrainSoftening") ModelStrainSoftening()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    isPropertyReadOnly(UInt i) const;
        virtual bool    isPropertyAdvanced(UInt i) const;
        virtual bool    supportsHystereticDamping() const { return true; }
        virtual bool    supportsStressStrengthRatio() const { return true; }
        virtual bool    supportsPropertyScaling() const { return false;}
        virtual bool    isModelAdvanced() const { return true; }

    protected:
        Double sHP_ = 0.0;
        Double tHP_ = 0.0; 
        Double sf1_ = 0.0; 
        Double sf3_ = 0.0;
        String cTable_,fTable_,dTable_,tTable_;
        void *iCohesion_ = nullptr;
        void *iFriction_ = nullptr; 
        void *iDilation_ = nullptr;
        void *iTension_ = nullptr;
    };
}
// EOF