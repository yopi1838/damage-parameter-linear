#pragma once

#include "models/ubiquit/src/modelubiquit.h"

namespace models {
  class ModelCUPow : public ModelUbiquitous {
    public:
      ModelCUPow();

      virtual String  getName() const { return(L"power-ubiquitous"); }
      virtual String  getFullName() const { return(L"Power Law with Ubiquitous Joint"); }
      virtual UInt    getMinorVersion() const;
      virtual String  getProperties() const;
      virtual Variant getProperty(UInt index) const;
      virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
      virtual ModelCUPow *clone() const { return(NEW("ModelCUPow") ModelCUPow()); }
      virtual void    copy(const ConstitutiveModel *mod);
      virtual void    initialize(UByte dim,State *s);
      virtual void    run(UByte dim,State *s);
      // Optional
      virtual bool    isCreep() const { return true; }
      virtual bool    supportsHystereticDamping() const { return false; }
      virtual bool    supportsStressStrengthRatio() const { return false; }
      virtual bool    supportsPropertyScaling() const { return false; }

    private:
      Double a1_,a2_,n1_,n2_,rs1_,rs2_;
  };
}
// EOF