#pragma once

#include "models/aniso/src/modelaniso.h"

namespace models {
    class ModelOrthotropic : public ModelAnisotropic {
    public:
        ModelOrthotropic();
        virtual String     getName() const { return(L"orthotropic"); }
        virtual String     getFullName() const { return(L"Orthotropic Elastic"); }
        virtual String     getProperties() const;
        virtual Variant    getProperty(UInt index) const;
        virtual void       setProperty(UInt index, const Variant &p,UInt restoreVersion=0);
        virtual ModelOrthotropic *clone(void) const { return(NEW("ModelOrthotropic")ModelOrthotropic()); }
        virtual Double     getShearModulus() const; 
        virtual Double     getBulkModulus() const; 
        virtual void       initialize(UByte uDim, State *s);
        virtual void       copy(const ConstitutiveModel *mod);
        //
        bool               isModelAdvanced() const { return true; }

    private:
        Double Rot_,E2_,Nu23_,G12_,G23_;
    };
}
// EOF