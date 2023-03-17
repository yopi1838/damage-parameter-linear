#pragma once

#include "models/elastic/src/modelelastic.h"

namespace models {
    class ModelWipp : public ModelElastic {
    public:
        ModelWipp();
        virtual String  getName() const { return(L"wipp"); }
        virtual String  getFullName() const { return(L"WIPP"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const; 
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelWipp *clone() const { return(NEW("ModelWipp") ModelWipp()); }
        virtual void copy(const ConstitutiveModel *mod);
        virtual void initialize(UByte dim,State *s);
        virtual void run(UByte dim,State *s); 
        // Optional
        virtual bool isPropertyReadOnly(UInt i) const;
        virtual bool supportsHystereticDamping() const { return false; }
        virtual bool isCreep() const { return true; }

    private:
        Double actE_,aWipp_,bWipp_,dWipp_;
        Double eDot_,gasC_,nWipp_,dTemp_,ePrim_,eRate_;
    };
}
// EOF