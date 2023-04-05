#pragma once

#include "models/src/conmodel.h"

namespace models {
    class ModelElastic : public ConstitutiveModel {
    public:
        ModelElastic(unsigned short option=0);
        virtual String  getName() const { return(L"elastic"); }
        virtual String  getFullName() const { return(L"Isotropic Elastic"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const { return(L"bulk,shear,young,poisson"); }
        virtual String  getStates() const { return(L""); }
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion);
        virtual ModelElastic *clone() const { return(NEW("ModelElastic") ModelElastic()); }
        virtual Double  getConfinedModulus() const { return bulk_ + shear_*d4d3; }
        virtual Double  getShearModulus() const { return shear_; }
        virtual Double  getBulkModulus() const { return bulk_; }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    supportsHystereticDamping() const { return true; }

    protected:
        virtual void   elasticTrial(State *s);
        virtual bool   updateParameters();
        virtual Double moduliReduction(const Double &factor);

        double bulk_ = 0.0;
        double shear_ = 0.0;
        double e1_ = 0.0;
        double e2_ = 0.0;
        double g2_ = 0.0;
    };
} // namespace models
// EOF