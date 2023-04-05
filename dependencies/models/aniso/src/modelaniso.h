#pragma once

#include "models/src/conmodel.h"
#include "models/base/modelweakplanetype.h"

namespace models {
    class ModelAnisotropic : public ConstitutiveModel, public ModelWeakplaneType {
    public:
        ModelAnisotropic();
        virtual String  getName() const { return(L"anisotropic"); }
        virtual String  getFullName() const { return(L"Anisotropic Elastic"); }
        virtual UInt    getMinorVersion() const; 
        virtual String  getProperties() const;
        virtual String  getStates() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelAnisotropic *clone() const { return(NEW("ModelAnisotropic") ModelAnisotropic()); }
        virtual Double  getConfinedModulus() const;
        virtual Double  getShearModulus() const; 
        virtual Double  getBulkModulus() const; 
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    run(UByte dim,State *s); 
        virtual void    initialize(UByte dim,State *s);

        virtual bool    isModelAdvanced() const { return true; }

    protected:
        static bool  xmatinv(Double b[6][6]);
        static void  xmatmul(Double b1[6][6],Double b2[6][6],Double b3[6][6]);
        virtual void buildMatrix(const Double &e1, const Double &e2, const Double &e3,
                                 const Double &v1, const Double &v2, const Double &v3,
                                 const Double &g1, const Double &g2, const Double &g3,
                                 const Double &dd, const Double &dip, const Double &rot = 0.0);

        virtual void AnisotropicElasticTrial(State *s);
        Double E1_,E3_,Nu12_,Nu13_,G13_;

    protected:
        Double A11_,A12_,A13_,A14_,A15_,A16_,A22_,A23_,A24_,A25_,A26_;
        Double A33_,A34_,A35_,A36_,A44_,A45_,A46_,A55_,A56_,A66_;
    };
} // namespace models
// EOF