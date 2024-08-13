#pragma once

#include "jointmodel.h"

namespace jmodels
{
  class JModelYopi : public JointModel {
  public:
    JModelYopi();
    virtual string         getName() const;
    virtual string         getPluginName() const { return getName(); }
    virtual string         getFullName() const;
    virtual uint32         getMinorVersion() const;
    virtual string         getProperties() const;
    virtual string         getStates() const;
    virtual base::Property getProperty(uint32 index) const;
    virtual void           setProperty(uint32 index, const base::Property& p, uint32 restoreVersion = 0);
    virtual JModelYopi *clone() const { return new JModelYopi(); }
    virtual double         getMaxNormalStiffness() const { return kn_; }
    virtual double         getMaxShearStiffness() const { return ks_; }
    virtual void           copy(const JointModel *mod);
    virtual void           run(uint32 dim,State *s); // If !isValid(dim) calls initialize(dim,s)
    virtual void           initialize(uint32 dim,State *s); // calls setValid(dim)    
    virtual double         solveQuadratic(double, double, double);
    virtual void           compCorrection(State* s, uint32*IPlasticity, double&comp);
    virtual void           shearCorrection(State* s, uint32* IPlasticity, double&fsm, double&fsmax);
    virtual void           tensionCorrection(State* s, uint32* IPlasticity, double& ten);
    // Optional 
    virtual double         getStrengthStressRatio(const double&, const DVect3&) const { return 10.0; }
    virtual void           scaleProperties(const double&, const std::vector<uint32>&) {
        throw std::runtime_error("Does not support property scaling");
    }
    virtual bool           supportsStrengthStressRatio() const { return false; }
    virtual bool           supportsPropertyScaling() const { return false; }
  private:
      double kn_;
      double kn_initial_; //Initial value of the normal stiffness
      double ks_;
      double cohesion_;
      double compression_;
      double friction_;
      double dilation_;
      double tension_;
      double zero_dilation_;
      double res_cohesion_;
      double res_friction_;
      double res_tension_;
      double tan_friction_;
      double tan_dilation_;
      double tan_res_friction_;
      double G_I; //first mode fracture energy
      double G_II; //Second mode fracture energy
      double G_c; //Compressive fracture energy
      double dt = 0.0; // tensile damage parameter
      double dc = 0.0; // Compressive damage parameter
      double ds = 0.0; // shear damage parameter
      double d_ts;
      double cc; //Softening part of shear strength
     string dtTable_, dsTable_; //damage parameter tables
     double tP_; //plastic tensile displacement
     double sP_; //plastic shear displacement
     double Cnn; //Cap user defined parameter in normal direction
     double Css; //Cap user defined parameter in shear direction
     double Cn; //Cap user defined parameter for center of ellipsis
    void* iTension_d_ = nullptr;
    void* iShear_d_ = nullptr;
    void* iHard_d_ = nullptr;
    uint32    kn_tab_;
    uint32    ks_tab_;
    double  R_yield;
    double  R_violates;
    double fc_current;
    double res_comp_;
    double friction_current_; //Current friction angle
    double n_; //Ratio between the elastic displacement to compressive strength
    double m_; //Ratio between ultimate displacement to displacement at peak compressive strength
    double uel_; //The elastic limit in tension
    double un_hist_comp; // The maximum current displacement
    double peak_normal; //The current peaks in compression   
    double ds_hist; 
    double un_ro;//reloading displacement
    double fm_ro; //reloading stress
    double un_hist_ten;
    double reloadFlag; //reloading flag
    uint32 plasFlag;    
    /*double un_el_temp;
    double f_el_temp;*/
    double dc_hist;
    uint32 pertFlag;
    double utemp; //temporary normal displacement to store the damaged compressive displacement.
    double ftemp_comp;    
    double dt_hist;
    double un_hist_inc;
  };
} // namespace models

// EOF
