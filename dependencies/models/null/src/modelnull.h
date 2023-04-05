#pragma once

#include "models/src/conmodel.h"

namespace models {

    class ModelNull : public ConstitutiveModel  {
    public:
        ModelNull();
        virtual String        getName() const { return(L"null"); }
        virtual String        getFullName() const { return(L"Null"); }
        virtual UInt          getMinorVersion() const;
        virtual String        getProperties() const; 
        virtual String        getStates() const; 
        virtual Variant       getProperty(UInt index) const;
        virtual void          setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelNull *clone() const { return(NEW("ModelNull") ModelNull()); }
        virtual Double        getConfinedModulus() const { return(0.0); }
        virtual Double        getShearModulus() const { return(0.0); }
        virtual Double        getBulkModulus() const { return(0.0); }
        virtual void          copy(const ConstitutiveModel *mod);
        virtual void          initialize(UByte dim,State *s);
        virtual void          run(UByte dim,State *s); 
        virtual bool          isNull() const { return true; }
    };
}
// EOF