#include "jmodelYopi.h"
#include "jmodels/src/state.h"
#include "version.txt"
#include <algorithm>
#include <limits>

#ifdef EXAMPLE_EXPORTS
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
#endif

namespace jmodels
{
  static const Double dPi  = 3.141592653589793238462643383279502884197169399;
  static const Double dDegRad = dPi / 180.0;

  // Plasticity Indicators
  static const UInt slip_now     = 0x01;  /* state logic */
  static const UInt tension_now  = 0x02;
  static const UInt slip_past    = 0x04;
  static const UInt tension_past = 0x08;
  static const UInt comp_now = 0x10;
  static const UInt comp_past = 0x20;

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
    uel_(0),
    un_hist_comp(0),
    R_violates(0),
    R_yield(0)
  {
  }

  String JModelYopi::getName() const
  { 
#ifdef JMODELDEBUG
    return L"yopid";
#else
    return L"yopi";
#endif
  }

  String JModelYopi::getFullName() const
  { 
#ifdef JMODELDEBUG
    return L"Yopi Debug";
#else
    return L"Yopi"; 
#endif
  }

  UInt JModelYopi::getMinorVersion() const
  {
    return UPDATE_VERSION;
  }

  String JModelYopi::getProperties() const
  {
      return(L"stiffness-normal       ,stiffness-initial    ,stiffness-shear        ,cohesion   ,compression  ,friction   ,dilation   ,"
          L"tension   ,dilation-zero    ,cohesion-residual  ,friction-residual  , comp-residual ,"
          L"tension-residual    , G_I   , G_II  ,dt ,ds ,dc ,d_ts   ,cc ,"
          L"table-dt    ,table-ds ,"
          L"tensile-disp-plastic    ,shear-disp-plastic ,"
          L"G_c, Cn, Cnn, Css, fc_current,  fric_current,   ult_ratio, peak_ratio,uel,un_hist_comp");
  }

  String JModelYopi::getStates() const
  {
    return L"slip-n,tension-n,slip-p,tension-p,cap-n,cap-p";
  }

  Variant JModelYopi::getProperty(UInt index) const
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
    case 31: return m_;
    case 32: return n_;
    case 33: return uel_;
    case 34: return un_hist_comp;
    }
    return 0.0;
  }

  void JModelYopi::setProperty(UInt index,const Variant &prop,UInt)
  {
    JointModel::setProperty(index,prop);
    switch (index) 
    {
    case 1: kn_ = prop.toDouble();  break;
    case 2: kn_initial_ = prop.toDouble(); break;
    case 3: ks_ = prop.toDouble();  break;
    case 4: cohesion_ = prop.toDouble();  break;
    case 5: compression_ = prop.toDouble(); break;
    case 6: friction_ = prop.toDouble();  break;
    case 7: dilation_ = prop.toDouble();  break;
    case 8: tension_ = prop.toDouble();  break;
    case 9: zero_dilation_ = prop.toDouble();  break;
    case 10: res_cohesion_ = prop.toDouble();  break;
    case 11: res_friction_ = prop.toDouble();  break;
    case 12: res_comp_ = prop.toDouble(); break;
    case 13: res_tension_ = prop.toDouble();  break;
    case 14: G_I = prop.toDouble(); break;
    case 15: G_II = prop.toDouble(); break;
    case 16: dt = prop.toDouble(); break;
    case 17: ds = prop.toDouble(); break;
    case 18: dc = prop.toDouble(); break;
    case 19: d_ts = prop.toDouble(); break;
    case 20: cc = prop.toDouble(); break;
    case 21: dtTable_ = prop.toString();  break;
    case 22: dsTable_ = prop.toString();  break;
    case 23: tP_ = prop.toDouble(); break;
    case 24: sP_ = prop.toDouble(); break;
    case 25: G_c = prop.toDouble(); break;
    case 26: Cn = prop.toDouble(); break;
    case 27: Cnn = prop.toDouble(); break;
    case 28: Css = prop.toDouble(); break;
    case 29: fc_current = prop.toDouble(); break;
    case 30: friction_current_ = prop.toDouble(); break;
    case 31: n_ = prop.toDouble(); break;
    case 32: uel_ = prop.toDouble(); break;
    case 33: un_hist_comp = prop.toDouble(); break;
    }
  }

  static const UInt Dqs = 0;
  static const UInt Dqt = 1;
  static const UInt Dqkn = 2;
  static const UInt Dqc = 3;

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
    m_ = mm->m_;
    n_ = mm->n_;
  }

  void JModelYopi::initialize(UByte dim,State *s)
  {
    JointModel::initialize(dim,s);
    tan_friction_    = tan(friction_ * dDegRad);
    tan_res_friction_ = tan(res_friction_ * dDegRad);
    tan_dilation_    = tan(dilation_ * dDegRad);

    //Initialize parameter for the compressive cap
    fc_current = 0.0;
    R_yield = 0.0;
    R_violates = 0.0;

    //Initialize the null pointer
    iTension_d_ = iShear_d_ = iHard_d_ =  nullptr;
    
    //Get Table index for each material parameters
    if (dtTable_.length()) iTension_d_ = s->getTableIndexFromID(dtTable_);
    if (dsTable_.length()) iShear_d_ = s->getTableIndexFromID(dsTable_);

    tP_ = 1.0;
    sP_ = 1.0;

    if (!G_c) 
        throw std::runtime_error("Internal error: Please input compressive fracture energy.");
    if (!n_) n_ = 1.0;

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

  

  Double JModelYopi::solveQuadratic(Double a, Double b, Double c) {
      Double x1;
      Double x2;
      x1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
      x2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
      if (x1 > x2) return x1;
      else return x2;
  }

  void JModelYopi::run(UByte dim,State *s)
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
    UInt IPlas = 0;
    Double kna  = kn_ * s->area_;
    Double ksa  = ks_ * s->area_;
    Double kn_comp_ = kn_initial_;

    if (!s->state_) {
        s->working_[Dqs] = 0.0;
        s->working_[Dqt] = 0.0;
        s->working_[Dqkn] = 0.0;        
    }
    Double ucel_ = n_ * compression_ / kn_comp_; //peak compressive displacement in positive
    
    // normal force
    Double fn0 = s->normal_force_;
    Double uel_limit = compression_ / kn_comp_ / 5.0; //elastic limit for hardening in positive

    //Define the hardening part of the compressive strength here
    if (s->normal_disp_ > 0.0) {
        //tension
        s->normal_force_inc_ = -kna * s->normal_disp_inc_;
        s->normal_force_ += s->normal_force_inc_;
    }
    else {                
        Double force_hist_comp = 0.0;
        
        kna = kn_comp_ * s->area_;
        
        //Get the current compressive displacement with inverted sign
        Double un_current = s->normal_disp_ * (-1.0);
        
        //Calculate elastic limit
        Double fel_limit = compression_ / 5.0 * s->area_;
        Double fpeak = compression_ * s->area_;
        bool sign = std::signbit(s->normal_disp_inc_); //Check whether unloading occurs      
        ////For now the stiffness is made the same.
        if (un_current < uel_limit) {
            s->normal_force_inc_ = -kna * s->normal_disp_inc_;
            s->normal_force_ += s->normal_force_inc_;
            fc_current = s->normal_force_ / s->area_;
            if (!sign) {
                un_hist_comp = s->working_[Dqc];
                force_hist_comp = s->working_[4];
                s->normal_force_inc_ = -kna * s->normal_disp_inc_;
                s->normal_force_ += s->normal_force_inc_;
            }
        }
        else if (!s->state_){ //If tensile/shear undamaged but compression on hardening side.                             
            if (!sign) {
                un_hist_comp = s->working_[Dqc];
                force_hist_comp = s->working_[4];
                s->normal_force_inc_ = -kna * s->normal_disp_inc_;
                s->normal_force_ += s->normal_force_inc_;
            }
            else {
                un_hist_comp = s->normal_disp_ * (-1.0); //Record the current displacement for unloading purposes    
                force_hist_comp = s->normal_force_;
                s->working_[Dqc] = un_hist_comp;
                s->working_[4] = force_hist_comp;
                s->normal_force_ = fel_limit + (fpeak - fel_limit) * pow((2 * (un_current - uel_limit) / ucel_) - pow((un_current - uel_limit) / ucel_, 2), 0.5);
                fc_current = s->normal_force_ / s->area_;
            }       
        }
        else {
            if (un_current > uel_limit) {
                s->normal_force_ = fel_limit + (fpeak - fel_limit) * pow((2 * (un_current - uel_limit) / ucel_) - pow((un_current - uel_limit) / ucel_, 2), 0.5);
                fc_current = s->normal_force_ / s->area_;
            }
            else {
                s->normal_force_inc_ = -kna * s->normal_disp_inc_;
                s->normal_force_ += s->normal_force_inc_;
            }
        }
        
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

    Double ten;
    Double comp = 0.0;
     //elastic limit on tension
    Double mid_comp = res_comp_ + (compression_ - res_comp_) / 2.0;
    Double beta_ = ucel_ * res_comp_; //Coefficient for calculating intermediate ratio
    Double kappa_ = ucel_ * compression_;
    Double gamma_ = 2.0;
    m_ = (G_c - 0.5 * (pow(compression_, 2) / (9 * kn_)) - 0.5 * (ucel_ - uel_limit) * 1.3 * compression_ 
            + 0.75 * kappa_ + 0.25 * beta_) / (0.25 * kappa_ * (2+ gamma_) - 0.25 * beta_ * (2-3* gamma_));
    Double ucul_ = m_ * ucel_;
    //Define the softening on compressive strength
    if (s->state_) {
        Double un_current = 0.0;
        if (s->normal_disp_ < 0.0) un_current = s->normal_disp_;
        if ((un_current < ucel_ * (-1.0)) && (un_current > ucul_ * (-1.0))) {
            dc = (1 - (mid_comp/compression_)) * pow((un_current - ucel_ * (-1.0)) / (ucul_ * (-1.0) - ucel_ * (-1.0)),2);
        }
        else if (un_current <= ucul_ * (-1.0)) {
            Double alpha = 2 * (mid_comp - compression_) / (ucul_ - ucel_);
            dc = 1 - (res_comp_ / compression_) - ((mid_comp - res_comp_) / compression_) * exp(alpha * (un_current*(-1.0) - ucul_)/(mid_comp- res_comp_));
            s->normal_force_inc_ = 0;
            s->shear_force_inc_ = DVect3(0, 0, 0);
        }
        else {
            dc = 0.0;
        }
        comp = compression_ * ((1 - dc)) * s->area_;
        fc_current = comp / s->area_;
    }
    else {
        comp = compression_ * s->area_;
    }
    uel_ = tension_ / kn_initial_;
    //Define the softening tensile strength
    if (s->state_)
    {
        if (G_I) {
            //if G_I is provided
            //Calculate ultimate(bound) displacement
            Double u_ul = 2 * G_I / tension_;
            Double un_current = 0.0;
            if (s->normal_disp_ > 0.0) un_current = s->normal_disp_;
            if (un_current < u_ul && un_current > (tension_/ kn_))
            {
                dt = (s->normal_disp_ - (tension_ / kn_)) / (u_ul - (tension_ / kn_));
            }
            else if (s->normal_disp_ >= u_ul)
            {
                dt = 1.0;
            }
            else {
                dt = 0.0;
            }
            ////Exponential Softening
            d_ts = dt + ds - dt * ds;
            ten = -tension_ * (1 - d_ts + 1e-14) * s->area_;
        }
        else
        {
            if (s->normal_disp_ > 0.0)
            {
                //if table_dt is provided.
                tP_ = s->normal_disp_ / (tension_ / kn_initial_);
                ////Exponential Softening
                if (iTension_d_) dt = s->getYFromX(iTension_d_, tP_);
                if (dt >= s->working_[Dqt]) {
                    s->working_[Dqt] = dt;
                }
                else {
                    dt = s->working_[Dqt];
                }
                d_ts = dt + ds - dt * ds;
            }                       
            Double un_hist_ten = tension_ / kn_;
            if (s->normal_disp_> 0.0 && s->normal_disp_ > un_hist_ten)
            {
                s->working_[Dqkn] = (1 - d_ts) * kn_;
                bool sign = std::signbit(s->normal_disp_inc_);
                if (sign) {                    
                    kn_ = s->working_[Dqkn];
                }
            }
            ten = -tension_ * ((1-d_ts) + 1e-14) * s->area_;
        }
    }
    else 
    {
        ten = -tension_ * s->area_;
    }

    // check tensile failure
    bool tenflag = false;
    Double f1;
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
        Double fsmax = (cohesion_ * s->area_ + tan_friction_ * s->normal_force_);
        Double fsm = s->shear_force_.mag();
        Double f2;
        if (fsmax < 0.0) fsmax = 0.0;
        if (s->state_) {
            //Calculate max shear stress            
            Double tmax = cohesion_ + tan_friction_ * s->normal_force_ / s->area_;
            Double tres = res_cohesion_ + tan_res_friction_ * s->normal_force_ / s->area_;
            Double usel = tmax / ks_;
            if (G_II) {
                Double u_uls = 2 * G_II / (tmax - tres) + (tres / ks_);
                if (s->shear_disp_.mag() < u_uls && s->shear_disp_.mag() >= (tmax / ks_))
                {
                    s->working_[Dqs] = std::max((s->shear_disp_.mag() - (tmax / ks_)) / (u_uls - (tmax / ks_)), s->working_[Dqs]);
                    ds = s->working_[Dqs];
                }
                else if (s->shear_disp_.mag() >= u_uls)
                {
                    ds = 1.0;
                }
                else {
                    ds = 0.0;
                }
                d_ts = dt + ds - dt * ds;
                Double resamueff = tan_res_friction_;
                if (!resamueff) resamueff = tan_friction_;
                cc = res_cohesion_ + (cohesion_ - res_cohesion_) * (1 - d_ts);
                Double tan_friction_c = tan_res_friction_ + (tan_friction_ - tan_res_friction_) * (1 - d_ts);
                friction_current_ = atan(tan_friction_c) / dDegRad;
                Double tc = cc * s->area_ + s->normal_force_ * tan_friction_c;
                fsmax = tc;
                f2 = fsm - tc;
            }
            else 
            {
               sP_ = s->shear_disp_.mag() / usel;
               ////Exponential Softening
               if (iShear_d_) ds = s->getYFromX(iShear_d_, sP_);
               if (ds > s->working_[Dqs]) {
                   s->working_[Dqs] = ds;
               }
               else {
                   ds = s->working_[Dqs];
               }
               d_ts = dt + ds - dt * ds;
               Double resamueff = tan_res_friction_;
               if (!resamueff) resamueff = tan_friction_;
               cc = res_cohesion_ + (cohesion_ - res_cohesion_) * (1 - d_ts);
               Double tan_friction_c = tan_res_friction_ + (tan_friction_ - tan_res_friction_) * (1 - d_ts);
               friction_current_ = atan(tan_friction_c) / dDegRad;
               Double tc = cc * s->area_ + s->normal_force_ * tan_friction_c;
               fsmax = tc;
               f2 = fsm - tc;
            }
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
            if (s->normal_disp_ < 0.0) {
               //Check f3
               Double f3;
               f3 = Cnn * pow(s->normal_force_, 2) + Css * pow(s->shear_force_.mag(), 2) + Cn * s->normal_force_ - pow(comp, 2);
               if (f3 >= 0.0) {
                   compCorrection(s, &IPlas, comp);
               }
            }
        }// if (f2)
        //Check compressive failure (compressive cap)
        if (s->normal_disp_ < 0.0) {
            Double f3;
            f3 = Cnn * pow(s->normal_force_, 2) + Css * pow(s->shear_force_.mag(), 2) + Cn * s->normal_force_ - pow(comp, 2);
            //If it violates the yield criterion for compression
            if (f3 >= 0.0)
            {
                compCorrection(s, &IPlas, comp);
                if (f2 >= 0.0) {
                    shearCorrection(s, &IPlas, fsm, fsmax);
                }
            }
        }//s->normal_disp < 0.0
    } // if (!tenflg)
  }//run

  void JModelYopi::tensionCorrection(State* s, UInt* IPlasticity, Double& ten) {
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

  void JModelYopi::shearCorrection(State *s, UInt *IPlasticity, Double& fsm, Double& fsmax) {
      if (IPlasticity) *IPlasticity = 2;
      Double rat = 0.0;
      if (fsm) rat = fsmax / fsm;
      s->shear_force_ *= rat;
      s->state_ |= slip_now;
      s->shear_force_inc_ = DVect3(0, 0, 0);
      // dilation
      if (dilation_)
      {
          Double zdd = zero_dilation_;
          Double usm = s->shear_disp_.mag();
          if (!zdd) zdd = 1e20;
          if (usm < zdd)
          {
              Double dusm = s->shear_disp_inc_.mag();
              Double dil = 0.0;
              if (!s->state_) dil = tan_dilation_;
              else
              {
                  // if residual dilation is zero, take peak value
                  //     Double resdileff = tan_res_dilation_;
                  // Note: In CLJ1 in 3DEC, no residual dilation is defined
                  Double resdileff = tan_dilation_;
                  if (!resdileff) resdileff = tan_dilation_;
                  dil = resdileff;
              }
              s->normal_force_ += kn_ * s->area_ * dil * dusm;
          }// if (usm<zdd)
      }// if (dilation_)
  }

  void JModelYopi::compCorrection(State *s, UInt *IPlasticity, Double &comp) {
    if (IPlasticity) *IPlasticity = 3;
    Double gradient;
    Double X_yield;
    Double Y_yield;
    Double x;
    Double ratc;
    Double y;
    Double a;
    Double b;
    Double c;
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
    if (dc == 1.0) {
        //Full brittle failure
        s->normal_force_ = 1e-14;
        s->shear_force_ = DVect3(0.0, 0.0, 0.0);
    }
    else {
        s->normal_force_ = X_yield;
        s->shear_force_ *= Y_yield / y;
    }
    s->normal_force_inc_ = 0.0;
    s->shear_force_inc_ = DVect3(0, 0, 0);
  }
} // namespace models


// EOF
