#pragma once

#include "jmodels/src/jointmodel.h"

namespace jmodels
{
  class JModelYopi : public JointModel {
  public:
    JModelYopi();
    virtual String         getName() const;
    virtual String         getPluginName() const { return getName(); }
    virtual String         getFullName() const;
    virtual UInt           getMinorVersion() const; 
    virtual String         getProperties() const;
    virtual String         getStates() const;
    virtual Variant        getProperty(UInt index) const;
    virtual void           setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
    virtual JModelYopi *clone() const { return new JModelYopi(); }
    virtual Double         getMaxNormalStiffness() const { return kn_; }
    virtual Double         getMaxShearStiffness() const { return ks_; }
    virtual void           copy(const JointModel *mod);
    virtual void           run(UByte dim,State *s); // If !isValid(dim) calls initialize(dim,s)
    virtual void           initialize(UByte dim,State *s); // calls setValid(dim)
    // Optional 
    virtual Double         getStressStrengthRatio(const Double &,const DVect3 &) const { return 10.0; }
    virtual void           scaleProperties(const Double &,const std::vector<UInt> &) { throw std::runtime_error("Does not support property scaling"); }
    virtual bool           supportsStressStrengthRatio() const { return false; }
    virtual bool           supportsPropertyScaling() const { return false; }
  private:
    Double kn_;
    Double ks_;
    Double cohesion_;
    Double friction_;
    Double dilation_;
    Double tension_;
    Double zero_dilation_;
    Double res_cohesion_;
    Double res_friction_;
    Double res_tension_;
    Double tan_friction_;
    Double tan_dilation_;
    Double tan_res_friction_;
    Double G_I; //first mode fracture energy
    Double G_II; //Second mode fracture energy
    Double dt; // tensile damage parameter
    Double ds; // shear damage parameter
    Double d_ts;
    Double cc; //Softening part of shear strength
    String dtTable_, dsTable_;
    Double tP_ = 0.0; //plastic tensile displacement
    Double sP_ = 0.0; //plastic shear displacement
    void  *iTension_d_ = nullptr;
    void  *iShear_d_ = nullptr;
    Int    kn_tab_;
    Int    ks_tab_;
  };
} // namespace models

// EOF
