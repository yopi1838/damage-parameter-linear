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
    virtual Double         solveQuadratic(Double ,Double, Double);
    virtual void           compCorrection(State* s, UInt *IPlasticity, Double &comp);
    virtual void           shearCorrection(State* s, UInt* IPlasticity, Double &fsm, Double &fsmax);
    virtual void           tensionCorrection(State* s, UInt* IPlasticity, Double& ten);
    // Optional 
    virtual Double         getStressStrengthRatio(const Double &,const DVect3 &) const { return 10.0; }
    virtual void           scaleProperties(const Double &,const std::vector<UInt> &) { throw std::runtime_error("Does not support property scaling"); }
    virtual bool           supportsStressStrengthRatio() const { return false; }
    virtual bool           supportsPropertyScaling() const { return false; }
  private:
    Double kn_;
    Double kn_initial_; //Initial value of the normal stiffness
    Double ks_;
    Double cohesion_;
    Double compression_;
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
    Double G_c; //Compressive fracture energy
    Double dt = 0.0; // tensile damage parameter
    Double dc = 0.0; // Compressive damage parameter
    Double ds = 0.0; // shear damage parameter
    Double d_ts;
    Double cc; //Softening part of shear strength
    String dtTable_, dsTable_; //damage parameter tables
    Double tP_; //plastic tensile displacement
    Double sP_; //plastic shear displacement
    Double Cnn; //Cap user defined parameter in normal direction
    Double Css; //Cap user defined parameter in shear direction
    Double Cn; //Cap user defined parameter for center of ellipsis
    void* iTension_d_ = nullptr;
    void* iShear_d_ = nullptr;
    void* iHard_d_ = nullptr;
    Int    kn_tab_;
    Int    ks_tab_;
    Double  R_yield;
    Double  R_violates;
    Double fc_current;
    Double res_comp_;
    Double friction_current_; //Current friction angle
    Double n_; //Ratio between the elastic displacement to compressive strength
    Double m_; //Ratio between ultimate displacement to displacement at peak compressive strength
    Double uel_;
    Double un_hist_comp;
  };
} // namespace models

// EOF
