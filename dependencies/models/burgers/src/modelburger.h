#pragma once

#include "models/src/conmodel.h"

namespace models {
    class ModelBurger : public ConstitutiveModel {
    public:
        ModelBurger();
        virtual String  getName() const { return(L"burgers"); }
        virtual String  getFullName() const { return(L"Burgers Substance"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const;
        virtual String  getStates() const { return(L""); }
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelBurger *clone() const { return(NEW("ModelBurger") ModelBurger()); }
        virtual Double  getConfinedModulus() const { return(bulk_ + d4d3 * (Kshear_ > Mshear_ ? Kshear_:Mshear_)); }
        virtual Double  getShearModulus() const { return Mshear_; }
        virtual Double  getBulkModulus() const { return bulk_; }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s);
        //
        virtual bool    isCreep() const { return true; }

    protected:
        Double bulk_,Kshear_,Mshear_,Kviscosity_,Mviscosity_;
        Double Mekd_[6];     
    };
}