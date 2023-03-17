#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
    class ModelPower : public ModelElastic {
    public:
        ModelPower();

        virtual String  getName() const { return(L"power"); }
        virtual String  getFullName() const { return(L"Power Law"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelPower *clone() const { return(NEW("ModelPower") ModelPower()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s);
        // Optional
        virtual bool    isCreep() const { return true; }
        virtual bool    supportsHystereticDamping() const { return false; }

    private:
        Double a1_,a2_,n1_,n2_,rs1_,rs2_;
    };
}