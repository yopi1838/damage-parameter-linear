#pragma once

#include "models/src/conmodel.h"

namespace models {
    class ModelCamClay : public ConstitutiveModel  {
    public:
        ModelCamClay();
        virtual String        getName() const { return L"modified-cam-clay"; }
        virtual String        getFullName() const { return L"Modified Cam-Clay"; }
        virtual UInt          getMinorVersion() const;
        virtual String        getProperties() const;
        virtual String        getStates() const;
        virtual Variant       getProperty(UInt index) const;
        virtual void          setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelCamClay *clone() const { return(NEW("ModelCamClay") ModelCamClay()); }
        virtual Double        getConfinedModulus() const { return(std::max(bulk_+shear_*d4d3, bulkB_ + shear_*d4d3)); }
        virtual Double        getShearModulus() const { return(shear_); }
        virtual Double        getBulkModulus() const { return(bulk_); }
        virtual void          copy(const ConstitutiveModel *mod);
        virtual void          initialize(UByte dim,State *s);
        virtual void          run(UByte dim,State *s); 
        // Optional
        virtual bool          isPropertyReadOnly(UInt i) const;
        virtual bool          isPropertyAdvanced(UInt i) const;

    private:
        Double bulk_,bulkB_,shear_,poisson_,Kappa_,Lambda_,MM_,MPC_,MP1_;
        Double MV_L_,MV_,EV_,EV_P_,MP_,MQ_;
    };
}
//EOF