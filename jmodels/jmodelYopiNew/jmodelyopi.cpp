#include "jmodelyopi.h"
#include "state.h"
#include "version.txt"
#include <algorithm>
#include <limits>


int __stdcall DllMain(void *,unsigned, void *)
{
  return 1;
}

extern "C" __declspec(dllexport) const char *getName() 
{
#ifdef JMODELDEBUG
  return "jmodelyopid";
#else
  return "jmodelyopi";
#endif
}

extern "C" __declspec(dllexport) unsigned getMajorVersion()
{
  return MAJOR_VERSION;
}

extern "C" __declspec(dllexport) unsigned getMinorVersion()
{
  return UPDATE_VERSION;
}

extern "C" __declspec(dllexport) void *createInstance() 
{
  jmodels::JModelYopi*m = new jmodels::JModelYopi();
  return (void *)m;
}

namespace jmodels
{
  static const double dPi  = 3.141592653589793238462643383279502884197169399;
  static const double dDegRad = dPi / 180.0;

  // Plasticity Indicators
  static const uint32 slip_now     = 0x01;  /* state logic */
  static const uint32 tension_now  = 0x02;
  static const uint32 slip_past    = 0x04;
  static const uint32 tension_past = 0x08;
  static const uint32 comp_now = 0x10;
  static const uint32 comp_past = 0x20;

  JModelYopi::JModelYopi() :
    kn_(0),
    kn_initial_(0),
    ks_(0),
    kn_tab_(0),
    ks_tab_(0),
    cohesion_(0),
    compression_(0),
    friction_(0),
    dilation_(0),
    tension_(0),
    zero_dilation_(0),
    res_cohesion_(0),
    res_friction_(0),
    res_tension_(0),
    res_comp_(0),
    tan_friction_(0),
    tan_dilation_(0),
    tan_res_friction_(0),
    G_I(0),
    G_II(0),
    G_c(0),
    dt(0),
    ds(0),
    dc(0),
    d_ts(0),
    cc(0),
    tP_(0),
    sP_(0),
    Cnn(0),
    Css(0),
    Cn(0),
    fc_current(0),
    friction_current_(0),
    m_(0),
    n_(0),
    R_violates(0),
    R_yield(0),
    uel_(0),
    un_hist_comp(0),
    peak_normal(0),
    ds_hist(0),
    un_ro(0),
    fm_ro(0),
    reloadFlag(0),
    un_hist_ten(0),
    dt_hist(0),
    dc_hist(0),
    pertFlag(0),
    plasFlag(0)
  {
  }

  string JModelYopi::getName() const
  { 
#ifdef JMODELDEBUG
    return "yopid";
#else
    return "yopi";
#endif
  }

  string JModelYopi::getFullName() const
  { 
#ifdef JMODELDEBUG
    return "Yopi Debug";
#else
    return "Yopi"; 
#endif
  }

  uint32 JModelYopi::getMinorVersion() const
  {
    return UPDATE_VERSION;
  }

  string JModelYopi::getProperties() const
  {
      return("stiffness-normal       ,stiffness-initial    ,stiffness-shear        ,cohesion   ,compression  ,friction   ,dilation   ,"
          "tension   ,dilation-zero    ,cohesion-residual  ,friction-residual  , comp-residual ,"
          "tension-residual    , G_I   , G_II  ,dt ,ds ,dc ,d_ts   ,cc ,"
          "table-dt    ,table-ds ,"
          "tensile-disp-plastic    ,shear-disp-plastic ,"
          "G_c, Cn, Cnn, Css, fc_current,  fric_current,   peak_ratio, ult_ratio,uel,un_hist_comp,peak_normal,ds_hist,"
          "un_reloading,fm_reloading,reloadFlag,un_hist_ten, utemp, dt_hist");
  }

  string JModelYopi::getStates() const
  {
    return "slip-n,tension-n,slip-p,tension-p,cap-n,cap-p";
  }

  base::Property JModelYopi::getProperty(uint32 index) const
  {
    switch (index) 
    {
    case 1:  return kn_;
    case 2:  return kn_initial_;
    case 3:  return ks_;
    case 4:  return cohesion_;
    case 5:  return compression_;
    case 6:  return friction_;
    case 7:  return dilation_;
    case 8:  return tension_;
    case 9:  return zero_dilation_;
    case 10:  return res_cohesion_;
    case 11:  return res_friction_;
    case 12: return res_comp_;
    case 13: return res_tension_;
    case 14: return G_I;
    case 15: return G_II;
    case 16: return dt;
    case 17: return ds;
    case 18: return dc;
    case 19: return d_ts;
    case 20: return cc;
    case 21: return dtTable_;
    case 22: return dsTable_;
    case 23: return tP_;
    case 24: return sP_;
    case 25: return G_c;
    case 26: return Cn;
    case 27: return Cnn;
    case 28: return Css;
    case 29: return fc_current;
    case 30: return friction_current_;
    case 31: return n_;
    case 32: return m_;
    case 33: return uel_;
    case 34: return un_hist_comp;
    case 35: return peak_normal;    
    case 36: return ds_hist;
    case 37: return un_ro;
    case 38: return fm_ro;    
    case 39: return reloadFlag;
    case 40: return un_hist_ten;
    case 41: return dt_hist;
    }
    return 0.0;
  }

  void JModelYopi::setProperty(uint32 index,const base::Property &prop,uint32)
  {
    JointModel::setProperty(index,prop);
    switch (index) 
    {
    case 1: kn_ = prop.to<double>();  break;
    case 2: kn_initial_ = prop.to<double>(); break;
    case 3: ks_ = prop.to<double>();  break;
    case 4: cohesion_ = prop.to<double>();  break;
    case 5: compression_ = prop.to<double>(); break;
    case 6: friction_ = prop.to<double>();  break;
    case 7: dilation_ = prop.to<double>();  break;
    case 8: tension_ = prop.to<double>();  break;
    case 9: zero_dilation_ = prop.to<double>();  break;
    case 10: res_cohesion_ = prop.to<double>();  break;
    case 11: res_friction_ = prop.to<double>();  break;
    case 12: res_comp_ = prop.to<double>(); break;
    case 13: res_tension_ = prop.to<double>();  break;
    case 14: G_I = prop.to<double>(); break;
    case 15: G_II = prop.to<double>(); break;
    case 16: dt = prop.to<double>(); break;
    case 17: ds = prop.to<double>(); break;
    case 18: dc = prop.to<double>(); break;
    case 19: d_ts = prop.to<double>(); break;
    case 20: cc = prop.to<double>(); break;
    case 21: dtTable_ = prop.to<string>();  break;
    case 22: dsTable_ = prop.to<string>();  break;
    case 23: tP_ = prop.to<double>(); break;
    case 24: sP_ = prop.to<double>(); break;
    case 25: G_c = prop.to<double>(); break;
    case 26: Cn = prop.to<double>(); break;
    case 27: Cnn = prop.to<double>(); break;
    case 28: Css = prop.to<double>(); break;
    case 29: fc_current = prop.to<double>(); break;
    case 30: friction_current_ = prop.to<double>(); break;
    case 31: n_ = prop.to<double>(); break;
    case 32: m_ = prop.to<double>(); break;
    case 33: uel_ = prop.to<double>(); break;
    case 34: un_hist_comp = prop.to<double>(); break;
    case 35: peak_normal = prop.to<double>(); break;    
    case 36: ds_hist = prop.to<double>(); break;
    case 37: un_ro = prop.to<double>(); break;
    case 38: fm_ro = prop.to<double>(); break;
    case 39: reloadFlag = prop.to<double>(); break;
    case 40: un_hist_ten = prop.to<double>(); break;
    case 41: dt_hist = prop.to<double>(); break;
    }
  }

  static const uint32 Dqs = 0;
  static const uint32 Dqt = 1;
  static const uint32 Dqkn = 2;
  static const uint32 Dqc = 3;
  static const uint32 D_un_hist = 4;

  void JModelYopi::copy(const JointModel *m)
  {
    JointModel::copy(m);
    const JModelYopi *mm = dynamic_cast<const JModelYopi*>(m);
    if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
    kn_ = mm->kn_;
    kn_initial_ = mm->kn_initial_;
    ks_ = mm->ks_;
    cohesion_ = mm->cohesion_;
    compression_ = mm->compression_;
    friction_ = mm->friction_;
    dilation_ = mm->dilation_;
    tension_ = mm->tension_;
    zero_dilation_ = mm->zero_dilation_;
    res_cohesion_ = mm->res_cohesion_;
    res_friction_ = mm->res_friction_;
    res_comp_ = mm->res_comp_;
    res_tension_ = mm->res_tension_;
    tan_friction_ = mm->tan_friction_;
    tan_dilation_ = mm->tan_dilation_;
    tan_res_friction_ = mm->tan_res_friction_;
    G_I = mm->G_I;
    G_II = mm->G_II;
    dt = mm->dt;
    ds = mm->ds;
    dc = mm->dc;
    d_ts = mm->d_ts;
    cc = mm->cc;
    dtTable_ = mm->dtTable_;
    dsTable_ = mm->dsTable_;
    tP_ = mm->tP_;
    sP_ = mm->sP_;
    G_c = mm->G_c;
    fc_current = mm->fc_current;
    friction_current_ = mm->friction_current_;
    n_ = mm->n_;
    m_ = mm->m_;
    uel_ = mm->uel_;
    un_hist_comp = mm->un_hist_comp;
    peak_normal = mm->peak_normal;    
    ds_hist = mm->ds_hist;
    un_ro = mm->un_ro;
    fm_ro = mm->fm_ro;
    reloadFlag = mm->reloadFlag;
    un_hist_ten = mm->un_hist_ten;
    dt_hist = mm->dt_hist;
  }

  void JModelYopi::initialize(uint32 dim,State *s)
  {
    JointModel::initialize(dim,s);
    tan_friction_    = tan(friction_ * dDegRad);
    tan_res_friction_ = tan(res_friction_ * dDegRad);
    tan_dilation_    = tan(dilation_ * dDegRad);

    //Initialize parameter for the compressive cap
    R_yield = 0.0;
    R_violates = 0.0;

    //Initialize the null pointer
    iTension_d_ = iShear_d_ = iHard_d_ =  nullptr;
    
    //Get Table index for each material parameters
    if (dtTable_.length()) iTension_d_ = s->getTableIndexFromID(dtTable_);
    if (dsTable_.length()) iShear_d_ = s->getTableIndexFromID(dsTable_);

  
    if (!G_c) 
        throw std::runtime_error("Internal error: Please input compressive fracture energy.");
    if (!n_) n_ = 1.0;
    if (n_ < 1.0)
        throw std::runtime_error("Internal error: peak_ratio (n) must be bigger than 1.0");

    if (G_I && iTension_d_)
        throw std::runtime_error("Internal error: either G_I or dtTable_ can be defined, not both.");

    if (G_II && iShear_d_)
        throw std::runtime_error("Internal error: either G_II or dsTable_ can be defined, not both.");

    if (!compression_) compression_ = 1e20;
    if (!res_comp_) res_comp_ = 0.0;
    if (!Cn) Cn = 0.0;
    if (!Cnn) Cnn = 1.0;
    if (!Css) Css = 9.0;
    
  }

  double JModelYopi::solveQuadratic(double a, double b, double c) {
      double x1;
      double x2;
      x1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
      x2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
      if (x1 > x2) return x1;
      else return x2;
  }

  void JModelYopi::run(uint32 dim,State *s)
  {
    JointModel::run(dim,s);
    /* --- state indicator:                                  */
    /*     store 'now' info. as 'past' and turn 'now' info off ---*/
    if (s->state_ & slip_now) s->state_ |= slip_past;
    s->state_ &= ~slip_now;
    if (s->state_ & tension_now) s->state_ |= tension_past;
    s->state_ &= ~tension_now;
    if (s->state_ & comp_now) s->state_ |= comp_past;
    s->state_ &= ~comp_now;
    uint32 IPlas = 0;       

    double kna  = kn_ * s->area_;
    double ksa  = ks_ * s->area_;
    double kn_comp_ = kn_initial_;

    if (!s->state_) {
        s->working_[Dqs] = 0.0;
        s->working_[Dqt] = 0.0;        
        s->working_[Dqkn] = 0.0;       
        s->working_[Dqc] = 0.0;        
    }
    
    double uel = 0.0;
    double ucel_ = n_ * compression_ / kn_comp_;    
    
    // normal force
    double fn0 = s->normal_force_;
    double uel_limit = compression_ / kn_comp_ / 5.0;
    double sn_ = s->normal_force_ / s->area_;
    double dn_ = s->normal_disp_inc_ * (-1.0);
    double un_current = s->normal_disp_ * (-1.0);
    //Calculate elastic limit
    double fel_limit = compression_ / 5.0;
    double fpeak = compression_;    
    double ftemp = 0.0;        


    //Define the hardening part of the compressive strength here
    if (un_current < 0.0) {
        if (un_current + dn_ <= un_hist_ten) 
        { 
            un_hist_ten = s->normal_disp_ * (-1.0); 
            s->working_[D_un_hist] = un_hist_ten;
        }
        if (dn_ >= 0.0) {
            if (sn_>= 0.0) s->normal_force_ = 0.0;
            else {
                kna = kn_ * s->area_;
                //tension
                s->normal_force_inc_ = kna * dn_;
                s->normal_force_ += s->normal_force_inc_;
            }
        }
        else {
            kna = kn_ * s->area_;
            //tension
            s->normal_force_inc_ = kna * dn_;
            s->normal_force_ += s->normal_force_inc_;
        }              
    }
    else {               
        if (un_current + dn_ >= un_hist_comp && reloadFlag == 0) {
            un_hist_comp = s->normal_disp_ * (-1.0); //Record the current displacement for unloading purposes            
        }        
        if ((sn_ >= peak_normal) && (s->state_ & comp_past) == 0.0) { // Loading   
            kna = kn_comp_ * s->area_;
            reloadFlag = 0;            
            //un_hist_comp = s->normal_disp_ * (-1.0); //Record the current displacement for unloading purposes    
            if (un_current + dn_ <= uel_limit) {                
                //Elastic unloading
                s->normal_force_inc_ = kna * dn_;
                s->normal_force_ += s->normal_force_inc_;
                fc_current = s->normal_force_ / s->area_;
            }
            else if (!s->state_ || sn_ < compression_) {
                ftemp = fel_limit + (fpeak - (fel_limit)) * pow((2 * ((un_current+dn_)- uel_limit) / ucel_) - pow(((un_current+dn_)-uel_limit) / ucel_, 2), 0.5);
                if (ftemp / (un_current+dn_) >= kn_comp_) s->normal_force_ += kna * dn_;
                else s->normal_force_ = ftemp * s->area_;
                fc_current = s->normal_force_ / s->area_;     
                plasFlag = 1;
            }
        }        
        else {                                         
            double un_plastic_rat = 0.25 * pow((un_hist_comp / ucel_), 2) + 0.5 * (un_hist_comp / ucel_);
            double un_plastic = un_plastic_rat * ucel_;
            if (dn_ < 0.0 && (dc > 0.0 || plasFlag == 1)) { //unloading from compression
                //unloading is limitted from the 98% line to differentiate unloading from numerical pertubation.         
                if (un_current + dn_ >= un_hist_comp * .99) pertFlag = 2;
                else pertFlag = 0;
                if (sn_ > 0.0 && (pertFlag == 0 || dc > 0.0)) {
                    double k1 = 1.5 * kn_comp_;
                    double k2 = 0.15 * kn_comp_ / pow(1 + (un_hist_comp / ucel_), 2);
                    double Es = peak_normal / (un_hist_comp - un_plastic);
                    double B1 = k1 / Es;
                    double B3 = 2 - (k2 / Es) * (1 + B1);
                    double B2 = B1 - B3;
                    double Xeta = (un_current - un_hist_comp) / (un_plastic - un_hist_comp);
                    double fm = 0.0;
                    fm = peak_normal + (0.05 - peak_normal) * ((B1 * Xeta + pow(Xeta, 2)) / (1 + B2 * Xeta + B3 * pow(Xeta, 2)));
                    if (fm < 0.0) fm = 0.0;
                    s->normal_force_ = fm * s->area_;                    
                    fc_current = fm;
                    reloadFlag = 1;  
                }            
                else if (sn_ <= 0.0) {
                    fm_ro = 0.0;
                    kna = kn_ * s->area_;
                    ////tension
                    s->normal_force_inc_ = kna * dn_;
                    s->normal_force_ += s->normal_force_inc_;
                    /*s->normal_force_inc_ = 0;
                    s->normal_force_ = 0;*/
                }
                else {
                    //Elastic unloading                    
                    kna = kn_comp_ * s->area_;
                    s->normal_force_inc_ = kna * dn_;
                    s->normal_force_ += s->normal_force_inc_;
                    fc_current = s->normal_force_ / s->area_;
                    reloadFlag = 0;
                }
                             
            }
            else {
                //un_hist_comp = s->normal_disp_ * (-1.0);                 
                if (reloadFlag == 1) {
                    //recalculate un_hist_comp
                    if (un_current >= un_plastic) {
                        double beta = 1.0;                        
                        double k_re = (beta * peak_normal - fm_ro) / ((un_hist_comp) - un_ro);
                        double fm_re = fm_ro + k_re * (un_current - un_ro);
                        s->normal_force_ = fm_re * s->area_;
                        fc_current = fm_re;
                    }
                }
                else {
                    //Elastic unloading                    
                    kna = kn_comp_ * s->area_;
                    s->normal_force_inc_ = kna * dn_;
                    s->normal_force_ += s->normal_force_inc_;
                    fc_current = s->normal_force_ / s->area_;
                    reloadFlag = 0;
                }                
            }         
        } //unloading  
    }

    // correction for time step in which joint opens (or goes into tension)
    // s->dnop_ is part of s->normal_disp_inc_ at which separation or tension takes place
    s->dnop_ = s->normal_disp_inc_;
    if ((fn0 > 0.0)               && 
        (s->normal_force_ <= 0.0) && 
        (s->normal_force_inc_ < 0.0))
    {
        s->dnop_ = -s->normal_disp_inc_ * fn0 / s->normal_force_inc_;
        if (s->dnop_ > s->normal_disp_inc_) s->dnop_ = s->normal_disp_inc_;
    }

    double ten;
    double comp = 0.0;
    uel = tension_ / kn_; //elastic limit on tension    
    double mid_comp = res_comp_ + (compression_ - res_comp_) / 2.0;
    double beta_ = ucel_ * res_comp_; //Coefficient for calculating intermediate ratio
    double kappa_ = ucel_ * compression_;
    double gamma_ = 2.0;
    m_ = (G_c - 0.5 * (pow(compression_, 2) / (9 * kn_comp_)) - 0.5 * (ucel_ - uel_limit) * 1.3 * compression_
        + 0.75 * kappa_ + 0.25 * beta_) / (0.25 * kappa_ * (2 + gamma_) - 0.25 * beta_ * (2 - 3 * gamma_));
    double ucul_ = m_ * ucel_;

    //Define the softening on compressive strength
    if (s->state_) {            
        if ((un_current >= ucel_) && (un_current < ucul_)) {
            dc = (1 - (mid_comp / compression_)) * pow((un_current - ucel_) / (ucul_ - ucel_), 2);
        }
        else if (un_current >= ucul_) {
            double alpha = 2 * (mid_comp - compression_) / (ucul_ - ucel_);
            dc = 1 - (res_comp_ / compression_) - ((mid_comp - res_comp_) / compression_) * exp(alpha * (un_current - ucul_) / (mid_comp - res_comp_));
        }
        else {
            dc = 0.0;
        }        
        if (dc >= dc_hist) dc_hist = dc;
        else dc = dc_hist;
        s->normal_force_inc_ = 0;
        s->shear_force_inc_ = DVect3(0, 0, 0);
        comp = compression_ * (1 - dc) * s->area_;

    }
    else {
        dc = 0.0;
        comp = compression_ * (1 - dc) * s->area_;
    }    
    //fc_current = comp / s->area_;
    uel_ = tension_ / kn_initial_;    
    //Define the softening tensile strength
    if (s->state_)
    {
        bool sign = std::signbit(dn_);
        if (reloadFlag == 1 && sn_ < -tension_ * (1 - d_ts)) { //if 
            //if table_dt is provided.
            tP_ = un_hist_ten*(-1.0) / (tension_ / kn_initial_);                      
            ////Exponential Softening                
            if (sign) {
                if (iTension_d_) dt = s->getYFromX(iTension_d_, tP_); //if table_dt is provided.
                else if (G_I) dt = 1.0 - exp(-tension_ / G_I * (un_hist_ten * (-1.0) - (tension_ / kn_initial_))); //Exponential Softening
                un_hist_ten += dn_;
            }
            d_ts = dt + ds - dt * ds;
        }
        else {
            tP_ = s->normal_disp_ / (tension_ / kn_initial_);
            if (sign) {                    
                if (iTension_d_) dt = s->getYFromX(iTension_d_, tP_); //if table_dt is provided.
                else if (G_I) dt = 1.0 - exp(-tension_ / G_I * (s->normal_disp_ - (tension_ / kn_initial_))); //Exponential Softening
            }           
        }
        if (dt_hist < dt) dt_hist = dt;
        else dt = dt_hist;
        d_ts = dt + ds - dt * ds;
        if (un_current < (-tension_ / kn_))
        {
            s->working_[Dqkn] = (1 - d_ts) * kn_;
            if (sign) {
                kn_ = (1 - d_ts) * kn_;
            }
        }
    }
    ten = -tension_ * ((1 - d_ts) + 1e-14) * s->area_;

    // check tensile failure
    bool tenflag = false;
    double f1;
    f1 = s->normal_force_ - ten;
    // Change the criterion to f1 criterion for tensile instead
    if (f1 <= 0) 
    {
        tensionCorrection(s, &IPlas, ten);
    }
    bool compflag = false;
    // shear force
    if (!tenflag && !compflag)
    {
        s->shear_force_inc_ = s->shear_disp_inc_ * -ksa;
        s->shear_force_ += s->shear_force_inc_;      
        
        //Because the normal force is already in negative anyway, we don't have to change the signs
        double fsmax = (cohesion_ * s->area_ + tan_friction_ * s->normal_force_);
        double fsm = s->shear_force_.mag();
        double f2;
        if (fsmax < 0.0) fsmax = 0.0;
        if (s->state_) {
            //Calculate max shear stress            
            double tmax = cohesion_ + tan_friction_ * s->normal_force_ / s->area_;            
            double usel = tmax / ks_;
            sP_ = s->shear_disp_.mag() / usel;
            ////Exponential Softening                              
            if (iShear_d_) ds = s->getYFromX(iShear_d_, sP_);
            else if (G_II) ds = 1 - exp(-cohesion_/G_II * (s->shear_disp_.mag() - usel));
            if (ds >= ds_hist) ds_hist = ds;
            else ds = ds_hist;
            d_ts = dt + ds - dt * ds;
            double resamueff = tan_res_friction_;
            if (!resamueff) resamueff = tan_friction_;
            cc = res_cohesion_ + (cohesion_ - res_cohesion_) * (1 - d_ts);
            double tan_friction_c = tan_res_friction_ + (tan_friction_ - tan_res_friction_) * (1 - d_ts);
            friction_current_ = atan(tan_friction_c) / dDegRad;
            double tc = cc * s->area_ + s->normal_force_ * tan_friction_c;
            fsmax = tc;
            f2 = fsm - tc;
        }        
        else {
            f2 = fsm - fsmax;
            cc = cohesion_;
            friction_current_ = atan(tan_friction_) / dDegRad;
        }// if (state)

        //Check if slip
        if (f2 >= 0.0) 
        {
            shearCorrection(s, &IPlas, fsm, fsmax);
            //if (s->normal_disp_ < 0.0) {
            //    //Check f3
            //    double f3;
            //    f3 = Cnn * pow(s->normal_force_, 2) + Css * pow(s->shear_force_.mag(), 2) + Cn * s->normal_force_ - pow(comp, 2);
            //    /*if (f3 >= 0.0) {
            //        compCorrection(s, &IPlas, comp);
            //    }*/
            //}                    
        }// if (f2)
        //Check compressive failure (compressive cap)
        if (s->normal_disp_ < 0.0) {
            double f3;
            f3 = Cnn * pow(s->normal_force_, 2) + Css * pow(s->shear_force_.mag(), 2) + Cn * s->normal_force_ - pow(comp, 2);
            //If it violates the yield criterion for compression
            if (f3 >= 0.0)
            {
                compCorrection(s, &IPlas, comp);
                /*if (f2 >= 0.0) {
                    shearCorrection(s, &IPlas, fsm, fsmax);
                }*/
            }
        }//s->normal_disp < 0.0
    } // if (!tenflg)

    // store peak
    if (dn_ >= 0.0 && sn_ >= peak_normal && un_current >= 0.0)
        peak_normal = sn_;
  }//run

  void JModelYopi::tensionCorrection(State* s, uint32* IPlasticity, double& ten) {
      bool tenflag = false;
      if (IPlasticity) *IPlasticity = 1;
      s->normal_force_ = ten;
      if (!s->normal_force_)
      {
          s->shear_force_ = DVect3(0, 0, 0);
          tenflag = true; // complete tensile failure
      }
      s->state_ |= tension_now;
      s->normal_force_inc_ = 0;
      s->shear_force_inc_ = DVect3(0, 0, 0);
  }

  void JModelYopi::shearCorrection(State *s, uint32 *IPlasticity, double& fsm, double& fsmax) {
      if (IPlasticity) *IPlasticity = 2;
      double rat = 0.0;
      if (fsm) rat = fsmax / fsm;
      s->shear_force_ *= rat;
      s->state_ |= slip_now;
      s->shear_force_inc_ = DVect3(0, 0, 0);
      // dilation
      if (dilation_)
      {
          double zdd = zero_dilation_;
          double usm = s->shear_disp_.mag();
          if (!zdd) zdd = 1e20;
          if (usm < zdd)
          {
              double dusm = s->shear_disp_inc_.mag();
              double dil = 0.0;
              if (!s->state_) dil = tan_dilation_;
              else
              {
                  // if residual dilation is zero, take peak value
                  //     Double resdileff = tan_res_dilation_;
                  // Note: In CLJ1 in 3DEC, no residual dilation is defined
                  double resdileff = tan_dilation_;
                  if (!resdileff) resdileff = tan_dilation_;
                  dil = resdileff;
              }
              s->normal_force_ += kn_ * s->area_ * dil * dusm;
          }// if (usm<zdd)
      }// if (dilation_)
      
  }

  void JModelYopi::compCorrection(State *s, uint32 *IPlasticity, double &comp) {
    if (IPlasticity) *IPlasticity = 3;
    double gradient;
    double X_yield;
    double Y_yield;
    double x;
    double ratc;
    double y;
    double a;
    double b;
    double c;
    bool compFlag = false;
    s->state_ |= comp_now;
    //Calculate the radial distance from point to the origin
    x = (s->normal_force_); //normal force would be larger than the position of Cn
    y = s->shear_force_.mag();

    R_violates = sqrt(pow(x, 2) + pow(y, 2));
    gradient = y / x; //Use this gradient to find the intersection point at the ellipsis
    //Find the intercept from the gradient at the yield surface
    a = Cnn + Css * pow(gradient, 2);
    b = Cn;
    c = -pow(comp, 2);
    X_yield = solveQuadratic(a, b, c);
    Y_yield = gradient * X_yield;
    R_yield = sqrt(pow(X_yield, 2) + pow(Y_yield, 2));
    //Correct the normal and shear forces to the yield surface
    ratc = R_yield / R_violates;
    if (!s->normal_force_) {
        s->shear_force_ = DVect3(0, 0, 0);
        compFlag = true;
    }
    if (dc >= 0.99) {
        //Full brittle failure
        s->normal_force_ = res_comp_ * s->area_;;
    }
    else {
        s->normal_force_ = X_yield;
        s->shear_force_ *= ratc;
    }
    s->normal_force_inc_ = 0.0;
    s->shear_force_inc_ = DVect3(0, 0, 0);
  }
} // namespace models


// EOF
