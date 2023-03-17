#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
    class ModelCHSoil : public ModelMohr {
    public:
        // Creators
        ModelCHSoil();
        virtual String       getName() const { return(L"cap-yield-simplified"); }
        virtual String       getFullName() const { return(L"Simplified Cap Yield "); }
        virtual UInt         getMinorVersion() const;
        virtual String       getProperties() const;
        virtual Variant      getProperty(UInt index) const;
        virtual void         setProperty(UInt ul,const Variant &p,UInt restoreVersion) ;
        virtual ModelCHSoil *clone() const { return(NEW("ModelCHSoil") ModelCHSoil()); }
        virtual Double       getConfinedModulus() const;
        virtual Double       getShearModulus() const { return(shear_); }
        virtual Double       getBulkModulus() const { return(bulk_); }
        virtual void         copy(const ConstitutiveModel *m);
        virtual void         initialize(UByte dim,State *s);
        virtual void         run(UByte dim,State *s);
        // Optional
        virtual bool         isPropertyReadOnly(UInt i) const;
        virtual bool         isPropertyAdvanced(UInt i) const;
        virtual bool         supportsHystereticDamping() const { return false; }
        virtual bool         supportsStressStrengthRatio() const { return false; }
        virtual bool         supportsPropertyScaling() const { return false; }

    private:
        Double bulkr_,shearr_,youngr_,fricv_;
        Double pref_,pini_,m_,n_,rf_,fricf_,fric0_,dilaf_;
        String cTable_,dTable_,tTable_;
        void  *iCohesion_,*iDilation_,*iTension_;
        UInt  dilaw_;
        Double sHP_,tHP_;
        //
        Double shearp_,sinr_,sincv_,sinr0_,sf1_,sf3_;
    };
}