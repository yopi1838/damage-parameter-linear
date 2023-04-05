#pragma once
#include "../../src/conmodel.h"

namespace models {
  class ModelCWipp : public ConstitutiveModel {
  public:
    ModelCWipp();

    virtual String  getName() const { return(L"wipp-salt"); }
    virtual String  getFullName() const { return(L"WIPP for Crashed Salts"); }
    virtual UInt    getMinorVersion() const; 
    virtual String  getProperties() const;
    virtual String  getStates() const;
    virtual Variant getProperty(UInt index) const;
    virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
    virtual ModelCWipp *clone() const { return(NEW("ModelCWIpp") ModelCWipp()); }
    virtual Double  getConfinedModulus() const { return(bulk_ + shear_*4.0/3.0); }
    virtual Double  getShearModulus() const { return(shear_); }
    virtual Double  getBulkModulus() const { return(bulk_); }
    virtual void    copy(const ConstitutiveModel *mod);
    virtual void    run(UByte dim,State *s); 
    virtual void    initialize(UByte dim,State *s);
    // Optional
    virtual bool    isPropertyReadOnly(UInt i) const;
    virtual bool    isCreep() const { return true; }
    virtual bool    supportsHystereticDamping() const { return false; }
    virtual bool    supportsStressStrengthRatio() const { return false; }
    virtual bool    supportsPropertyScaling() const { return false; }

  private:
    Double bulk_,shear_,temp_;
    Double act_e_,awipp_,bwipp_,dwipp_,edot_;
    Double gas_c_,nwipp_,kf_,gf_,rhof_,b0_sk_,b1_sk_,b2_sk_,rho_;
    Double e_prim_,e_rate_,k1_,g1_,f_d_;
  };
} // namespace models

// EOF
