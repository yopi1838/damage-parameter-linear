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

  JModelYopi::JModelYopi() :
    kn_(0),
    ks_(0),
    cohesion_(0),
    friction_(0),
    dilation_(0),
    tension_(0),
    zero_dilation_(0),
    res_cohesion_(0),
    res_friction_(0),
    res_tension_(0),
    tan_friction_(0),
    tan_dilation_(0),
    tan_res_friction_(0),
    G_I(0),
    G_II(0),
    dt(0),
    ds(0),
    d_ts(0),
    cc(0)
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
      return(L"stiffness-normal       ,stiffness-shear        ,cohesion   ,friction   ,dilation   ,"
          L"tension   ,dilation-zero,cohesion-residual,friction-residual,"
          L"tension-residual, G_I, G_II,dt,ds,d_ts,cc");
  }

  String JModelYopi::getStates() const
  {
    return L"slip-n,tension-n,slip-p,tension-p";
  }

  Variant JModelYopi::getProperty(UInt index) const
  {
    switch (index) 
    {
    case 1:  return kn_;
    case 2:  return ks_;
    case 3:  return cohesion_;
    case 4:  return friction_;
    case 5:  return dilation_;
    case 6:  return tension_;
    case 7:  return zero_dilation_;
    case 8:  return res_cohesion_;
    case 9:  return res_friction_;
    case 10: return res_tension_;
    case 11: return G_I;
    case 12: return G_II;
    case 13: return dt;
    case 14: return ds;
    case 15: return d_ts;
    case 16: return cc;
    }
    return 0.0;
  }

  void JModelYopi::setProperty(UInt index,const Variant &prop,UInt)
  {
    JointModel::setProperty(index,prop);
    switch (index) 
    {
    case 1: kn_ = prop.toDouble();  break;
    case 2: ks_ = prop.toDouble();  break;
    case 3: cohesion_ = prop.toDouble();  break;
    case 4: friction_ = prop.toDouble();  break;
    case 5: dilation_ = prop.toDouble();  break;
    case 6: tension_ = prop.toDouble();  break;
    case 7: zero_dilation_ = prop.toDouble();  break;
    case 8: res_cohesion_ = prop.toDouble();  break;
    case 9: res_friction_ = prop.toDouble();  break;
    case 10: res_tension_ = prop.toDouble();  break;
    case 11: G_I = prop.toDouble(); break;
    case 12: G_II = prop.toDouble(); break;
    case 13: dt = prop.toDouble(); break;
    case 14: ds = prop.toDouble(); break;
    case 15: d_ts = prop.toDouble(); break;
    case 16: cc = prop.toDouble(); break;
    }
  }


  void JModelYopi::copy(const JointModel *m)
  {
    JointModel::copy(m);
    const JModelYopi *mm = dynamic_cast<const JModelYopi*>(m);
    if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
    kn_ = mm->kn_;
    ks_ = mm->ks_;
    cohesion_ = mm->cohesion_;
    friction_ = mm->friction_;
    dilation_ = mm->dilation_;
    tension_ = mm->tension_;
    zero_dilation_ = mm->zero_dilation_;
    res_cohesion_ = mm->res_cohesion_;
    res_friction_ = mm->res_friction_;
    res_tension_ = mm->res_tension_;
    tan_friction_ = mm->tan_friction_;
    tan_dilation_ = mm->tan_dilation_;
    tan_res_friction_ = mm->tan_res_friction_;
    G_I = mm->G_I;
    G_II = mm->G_II;
    dt = mm->dt;
    ds = mm->ds;
    d_ts = mm->d_ts;
    cc = mm->cc;
  }

  void JModelYopi::initialize(UByte dim,State *s)
  {
    JointModel::initialize(dim,s);
    tan_friction_    = tan(friction_ * dDegRad);
    tan_res_friction_ = tan(res_friction_ * dDegRad);
    tan_dilation_    = tan(dilation_ * dDegRad);
  }

  static const UInt Dqs = 0;
  static const UInt Dqt = 1;

  void JModelYopi::run(UByte dim,State *s)
  {
    JointModel::run(dim,s);
    /* --- state indicator:                                  */
    /*     store 'now' info. as 'past' and turn 'now' info off ---*/
    if (s->state_ & slip_now) s->state_ |= slip_past;
    s->state_ &= ~slip_now;
    if (s->state_ & tension_now) s->state_ |= tension_past;
    s->state_ &= ~tension_now;

    Double kna  = kn_ * s->area_;
    Double ksa  = ks_ * s->area_;

    // normal force
    Double fn0 = s->normal_force_;
    s->normal_force_inc_ = -kna * s->normal_disp_inc_;
    s->normal_force_ += s->normal_force_inc_;

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

    // tensile strength
    Double ten;
    

    //Define the softening tensile strength
    if (s->state_)
    {
        //Calculate ultimate(bound) displacement
        Double u_ul = 2 * G_I / tension_;
        if (s->normal_disp_ < u_ul && s->normal_disp_ > (tension_ / kn_))
        {
            //dt = (s->normal_disp_ - (tension_ / kn_)) / (u_ul - (tension_ / kn_));
            s->working_[Dqt] = std::max((s->normal_disp_ - (tension_ / kn_)) / (u_ul - (tension_ / kn_)), s->working_[Dqt]);
            dt = s->working_[Dqt];
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
        ten = -tension_ * (1- d_ts + 1e-14) * s->area_;
    }
    else 
    {
        dt = 0;
        ds = 0;
        d_ts = dt + ds - dt * ds;
        ten = -tension_ * (1 - d_ts) * s->area_;
    }
    // check tensile failure
    bool tenflag = false;
    Double f1;
    f1 = s->normal_force_ - ten;
    // Change the criterion to f1 criterion for tensile instead
    if (f1 <= 0) 
    {
      s->normal_force_  = ten;
      if (!s->normal_force_)
      {
        s->shear_force_ = DVect3(0,0,0);
        tenflag = true; // complete tensile failure
      }
      s->state_ |= tension_now;
      s->normal_force_inc_ = 0;
      s->shear_force_inc_ = DVect3(0,0,0);
    }

    // shear force
    if (!tenflag)
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
            Double u_uls = 2 * G_II / (tmax - tres) + (tres / ks_);
            if (s->shear_disp_.mag() < u_uls && s->shear_disp_.mag() >= (tmax/ks_))
            {
                //ds = (s->shear_disp_.mag() - (tmax / ks_)) / (u_uls - (tmax / ks_));
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
            cc = res_cohesion_ + (cohesion_ - res_cohesion_) * (1- d_ts);
            Double tan_friction_c = tan_res_friction_ + (tan_friction_ - tan_res_friction_) * (1- d_ts);
            Double tc = cc * s->area_ + s->normal_force_ * tan_friction_c;
            fsmax = tc;
            f2 = fsm - tc;
        }
        else {
            f2 = fsm - fsmax;
        }
        //Check if slip
        if (f2 >= 0.0) 
        {
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
                    s->normal_force_ += kna * dil * dusm;
                }
            }
        }
    } // if (!tenflg)
  }

} // namespace models


// EOF
