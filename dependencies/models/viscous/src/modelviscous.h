#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
  class ModelViscous : public ModelElastic {
    public:
      ModelViscous();
      virtual String  getName() const { return(L"maxwell"); }
      virtual String  getFullName() const { return(L"Maxwell Substance"); }
      virtual UInt    getMinorVersion() const;
      virtual String  getProperties() const;
      virtual Variant getProperty(UInt index) const;
      virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
      virtual ModelViscous *clone() const { return(NEW("ModelViscous") ModelViscous()); }
      virtual void    copy(const ConstitutiveModel *mod);
      virtual void    initialize(UByte dim,State *s);
      virtual void    run(UByte dim,State *s);
      // Optional
      virtual bool    isCreep() const { return true; }
      virtual bool    supportsHystereticDamping() const { return false; }

    private:
      Double viscosity_;
      Double gD2V_;
  };
}
//EOF