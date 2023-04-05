#pragma once

#include "models/burgers/src/modelburger.h"

namespace models {
  class ModelCVisc : public ModelBurger {
    public:
      ModelCVisc();
      virtual String  getName() const { return(L"burgers-mohr"); }
      virtual String  getFullName() const { return(L"Burgers-Substance with Mohr-Coulomb"); }
      virtual UInt    getMinorVersion() const;
      virtual String  getProperties() const;
      virtual String  getStates() const;
      virtual Variant getProperty(UInt index) const;
      virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
      virtual ModelCVisc *clone() const { return(NEW("ModelCVisc") ModelCVisc()); }
      virtual void    copy(const ConstitutiveModel *mod);
      virtual void    initialize(UByte dim,State *s);
      virtual void    run(UByte dim,State *s);
      // Optional
      virtual bool    isCreep() const { return true; }
      virtual bool    supportsHystereticDamping() const { return false; }
      virtual bool    supportsStressStrengthRatio() const { return false; }
      virtual bool    supportsPropertyScaling() const { return false; }

    private:
      Double cohesion_,friction_,dilation_,tension_;
      Double sHP_,tHP_;
      Double nph_,csn_,rc_,nps_;
  };
}
// EOF
