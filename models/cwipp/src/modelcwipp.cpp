#include "modelcwipp.h"
#include "base/src/version.h"

namespace models {

  static const Int    it_max = 4;
  static const Double dSq3D2   = 1.224744871391589049; // sqrt(1.5)
  static const Double str_tol  = 1.0e-4;
  static const Double str_tolv = 1.0e-8;

  ModelCWipp::ModelCWipp() : bulk_(0.0), shear_(0.0), temp_(0.0),
                             act_e_(0.0), awipp_(0.0), bwipp_(0.0), dwipp_(0.0), edot_(0.0),
                             gas_c_(0.0), nwipp_(0.0), kf_(0.0), gf_(0.0), rhof_(0.0),
                             b0_sk_(0.0), b1_sk_(0.0), b2_sk_(0.0), rho_(0.0), e_prim_(0.0),
                             e_rate_(0.0), k1_(0.0), g1_(0.0), f_d_(0.0) { }


  UInt ModelCWipp::getMinorVersion() const {
    return (version::update);
  }

  String ModelCWipp::getProperties() const {
    return(L"bulk,shear,young,poisson,"
           L"activation-energy -act_energy,"
           L"constant-a -a_wipp,constant-b -b_wipp,constant-d -d_wipp,"
           L"creep-rate-critical -e_dot_star,constant-gas -gas_c,"
           L"exponent -n_wipp,bulk-final -b_f,shear-final -s_f,density-final -d_f,"
           L"compaction-0 -b0_sk,"
           L"compaction-1 -b1_sk,compaction-2 -b2_sk ,temperature,density-salt -rho,"
           L"compaction-bulk -s_k1,compaction-shear -s_g1,density-fractional -frac_d");
  }

  String ModelCWipp::getStates() const  {
    return(L"");
  }


  Variant ModelCWipp::getProperty(UInt index) const {
    switch (index)  {
      case 1:  return(bulk_);
      case 2:  return(shear_);
      case 3: {
        Double young;
        getYPfromBS(bulk_,shear_,&young,0);
        return(young);
      } 
      break;
      case 4: {
        Double poisson;
        getYPfromBS(bulk_,shear_,0,&poisson);
        return(poisson);
      } break;
      case 5:  return(act_e_);
      case 6:  return(awipp_);
      case 7:  return(bwipp_);
      case 8:  return(dwipp_);
      case 9:  return(edot_);
      case 10: return(gas_c_);
      case 11: return(nwipp_);
      case 12: return(kf_);
      case 13: return(gf_);
      case 14: return(rhof_);
      case 15: return(b0_sk_);
      case 16: return(b1_sk_);
      case 17: return(b2_sk_);
      case 18: return(temp_);
      case 19: return(rho_);
      case 20: return(k1_);
      case 21: return(g1_);
      case 22: return(f_d_);
    }
    return(0.0);
  }


  void ModelCWipp::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
    ConstitutiveModel::setProperty(index,p,restoreVersion);
    switch (index) {
      case 1:  bulk_ = p.toDouble();   break;
      case 2:  shear_ = p.toDouble();  break;
      case 3: // YOUNG
        { 
          Double poisson,young;
          getYPfromBS(bulk_,shear_,&young,&poisson);
          young = p.toDouble();
          if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_);
        }
        break;
      case 4: 
        { 
          Double poisson,young;
          getYPfromBS(bulk_,shear_,&young,&poisson);
          poisson = p.toDouble();
          if (!young && !restoreVersion) 
              throw std::runtime_error("Young's Modulus must be non-zero before you can specify Poisson's Ratio.");
          if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_);
        }
        break;
      case 5:  act_e_ = p.toDouble();    break;
      case 6:  awipp_ = p.toDouble();   break;
      case 7:  bwipp_ = p.toDouble();   break;
      case 8:  dwipp_ = p.toDouble();   break;
      case 9:  edot_ = p.toDouble();    break;
      case 10: gas_c_ = p.toDouble();    break;
      case 11: nwipp_ = p.toDouble();   break;
      case 12: kf_ = p.toDouble();      break;
      case 13: gf_ = p.toDouble();      break;
      case 14: rhof_ = p.toDouble();    break;
      case 15: b0_sk_ = p.toDouble();   break;
      case 16: b1_sk_ = p.toDouble();   break;
      case 17: b2_sk_ = p.toDouble();   break;
      case 18: temp_ = p.toDouble();    break;
      case 19: rho_ = p.toDouble();     break;
      case 20: k1_ = p.toDouble();      break;
      case 21: g1_ = p.toDouble();      break;
      case 22: f_d_ = p.toDouble();     break;
    }
  }

  bool ModelCWipp::isPropertyReadOnly(UInt i) const
  {
    switch (i) {
      case 20:
      case 21:
      case 22: return true;
      default: return false;
    }
  }

  void ModelCWipp::copy(const ConstitutiveModel *cm) {
    ConstitutiveModel::copy(cm);
    const ModelCWipp *cwm = dynamic_cast<const ModelCWipp *>(cm);
    if (!cwm) throw std::runtime_error("Internal error: dynamic cast failed.");
    bulk_    = cwm->bulk_;
    shear_   = cwm->shear_;
    act_e_    = cwm->act_e_;
    awipp_   = cwm->awipp_;
    bwipp_   = cwm->bwipp_;
    dwipp_   = cwm->dwipp_;
    edot_    = cwm->edot_;
    gas_c_    = cwm->gas_c_;
    nwipp_   = cwm->nwipp_;
    kf_      = cwm->kf_;
    gf_      = cwm->gf_;
    rhof_    = cwm->rhof_;
    b0_sk_   = cwm->b0_sk_;
    b1_sk_   = cwm->b1_sk_;
    b2_sk_   = cwm->b2_sk_;
    temp_    = cwm->temp_;
    rho_     = cwm->rho_;
    e_prim_  = cwm->e_prim_;
    e_rate_  = cwm->e_rate_;
    k1_      = cwm->k1_;
    g1_      = cwm->g1_;
    f_d_     = cwm->f_d_;
  }

  void ModelCWipp::initialize(UByte d,State *s) {
    ConstitutiveModel::initialize(d,s);
    if (rho_ > rhof_)
      rho_ = rhof_;
    if (k1_ == 0.0 || g1_ == 0.0) {
      if (rho_ == rhof_) {
        k1_    = 0.0;
        g1_    = 0.0;
        bulk_  = kf_;
        shear_ = gf_;
      } 
      else {
        Double dVal = rhof_ - rho_;
        k1_   = log(kf_/bulk_)  / dVal;
        g1_   = log(gf_/shear_) / dVal;
        dVal  = rho_ - rhof_;
        bulk_  = kf_ * exp(k1_ * dVal);
        shear_ = gf_ * exp(g1_ * dVal);
        shear_ = shear_ < 1.5 * bulk_ ? shear_ : 1.5 * bulk_;
        shear_ = shear_ > 0.0 ? shear_ : 0.0;
      }
    }
    if (rhof_) 
      f_d_ = rho_ / rhof_;
    else
      throw std::runtime_error("Property does not correspond to model cwipp.");
  }

  static const Int EPrime     = 0;
  static const Int EPrime_sum = 1;
  static const Int Exc        = 2;
  static const Int Arhoc      = 3;
  static const Int G2         = 4;

  void ModelCWipp::run(UByte d,State *s) {
    ConstitutiveModel::run(d,s);
    if (!s->sub_zone_) {
        s->working_[EPrime] = e_prim_;
        e_rate_ = 0.0;
        s->working_[EPrime_sum] = 0.0;
        s->working_[Arhoc]  = 0.0;
        s->working_[G2]     = 2.0 * shear_;
        s->working_[Exc]    = 0.0;
    }
    if (s->isCreepInUse()) 
    { /* --- temperature dependent part --- */
        Double temp_Use = temp_;
        if (s->isThermal())
        {
          temp_Use = s->getTemperature();
          temp_ = temp_Use;
        }
        if (temp_Use) 
            s->working_[Exc] = exp( -act_e_ / (gas_c_ * temp_Use) );
        else
             throw std::runtime_error("Temperature not assigned in model cwipp.");
    }
    Double dVdt = s->isCreepInUse() ? s->getTimeStep() : 0.0; //viscous
    Double dsubv = s->getSubZoneVolume();
    /* --- volumetric and deviatoric strain increments --- */
    Double dDevol  = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33();
    Double dDevol3 = d1d3 * dDevol;
    Double dE11d  = s->stnE_.s11() - dDevol3;
    Double dE22d  = s->stnE_.s22() - dDevol3;
    Double dE33d  = s->stnE_.s33() - dDevol3;
    /* --- old isotropic and deviatoric stresses --- */
    Double dS0 = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
    Double dShTol = dS0 * dS0 * str_tol;
    Double dS0n = 0.0;
    Double dS11d = s->stnS_.s11() - dS0;
    Double dS22d = s->stnS_.s22() - dS0;
    Double dS33d = s->stnS_.s33() - dS0;
    Double dS11n = 0.0;
    Double dS22n = 0.0;
    Double dS33n = 0.0;
    Double dS23n = 0.0;
    Double dS12n = 0.0;
    Double dS13n = 0.0;
    Double dEpn  = 0.0;
    if (s->isCreepInUse())
    { /* --- new isotropic stress --- */
      Double dDevolc=0.0;
      if (rho_ < rhof_ && dS0 < 0.0) {
        Double dDCVal = s->getTimeStep() * b0_sk_ * exp(b2_sk_ * rho_)
                      * (1.0 + dDevol * 0.5 * (1.0 - b2_sk_ * rho_)) / rho_;
        Double dVal  = exp(-b1_sk_ * dS0);
        Double dAVal = dDCVal * (1.0 - dVal);
        Double dBVal = dDCVal * 0.5 * b1_sk_ * dVal;
        dVal  = bulk_ * exp(-k1_ * rho_ * dDevol * 0.5);
        Double dSmafG2 = 1.0 + dVal * dBVal;
        if (dSmafG2 > str_tolv)
        {
          Double dSig = dVal * (dDevol - dAVal) / dSmafG2;
          dDevolc = dAVal + dBVal * dSig;
          dS0n = dS0 + dSig;
        } 
        else 
        {
          dDevolc = s->getTimeStep() * b0_sk_ * (1.0 - exp(-b1_sk_ * dS0))
                        * exp(b2_sk_ * rho_) / rho_;
          Double dSig = bulk_ * (dDevol - dDevolc);
          dS0n = dS0 + dSig;
        }
      } 
      else 
      {
        dDevolc = 0.0;
        dS0n  = dS0 + bulk_ * (dDevol - dDevolc);
      }
      /* --- iteration to find new deviatoric stresses --- */
      for (Int it = 1;it<=it_max;it++) {
        Double dS11m,dS22m,dS33m,dS23m,dS12m,dS13m,dEpm;
        /* --- average stress --- */
        if (it == 1) 
        {
          dS11m  = dS11d;
          dS22m  = dS22d;
          dS33m  = dS33d;
          dS23m  = s->stnS_.s23();
          dS12m  = s->stnS_.s12();
          dS13m  = s->stnS_.s13();
          dEpm   = s->working_[EPrime];
        } 
        else 
        {
          dS11m  = 0.5 * (dS11n + dS11d);
          dS22m  = 0.5 * (dS22n + dS22d);
          dS33m  = 0.5 * (dS33n + dS33d);
          dS23m  = 0.5 * (dS23n + s->stnS_.s23() );
          dS12m  = 0.5 * (dS12n + s->stnS_.s12() );
          dS13m  = 0.5 * (dS13n + s->stnS_.s13() );
          dEpm   = 0.5 * (s->working_[EPrime] + dEpn);
        }
        /* --- stress magnitude --- */
        Double dSmafG2 = dS11m*dS11m + dS22m*dS22m + dS33m*dS33m
                + 2.0 * (dS12m*dS12m + dS23m*dS23m + dS13m*dS13m);
        Double dE11v=0.0,dE22v=0.0,dE33v=0.0,dE12v=0.0,dE13v=0.0,dE23v=0.0,dEdp=0.0;
        if (dSmafG2 > dShTol) 
        {
          Double dSBar = dSq3D2 * sqrt (dSmafG2);
          Double dVal = dSBar / f_d_;
          /* --- (secondary creep rate) --- */
          Double dEds = pow (dVal,nwipp_) * dwipp_ * s->working_[Exc];
          /* --- (primary creep rate) --- */
          if (dEds >= edot_) 
            dEdp = (awipp_ - bwipp_*dEpm) * dEds;
          else
            dEdp = awipp_ * dEds - bwipp_ * edot_ * dEpm;
          Double dEdc = 1.5 * dVdt * (dEds + dEdp);
          /* --- creep strain increments --- */
          dE11v = (dEdc - dDevolc) * dS11m / dSBar;
          dE22v = (dEdc - dDevolc) * dS22m / dSBar;
          dE33v = (dEdc - dDevolc) * dS33m / dSBar;
          dE23v = (dEdc - dDevolc) * dS23m / dSBar;
          dE12v = (dEdc - dDevolc) * dS12m / dSBar;
          dE13v = (dEdc - dDevolc) * dS13m / dSBar;
        }
        /* --- new deviator stresses --- */
        dS11n  = dS11d       + s->working_[G2] * (dE11d       - dE11v);
        dS22n  = dS22d       + s->working_[G2] * (dE22d       - dE22v);
        dS33n  = dS33d       + s->working_[G2] * (dE33d       - dE33v);
        dS23n  = s->stnS_.s23()  + s->working_[G2] * (s->stnE_.s23()  - dE23v);
        dS12n  = s->stnS_.s12()  + s->working_[G2] * (s->stnE_.s12()  - dE12v);
        dS13n  = s->stnS_.s13()  + s->working_[G2] * (s->stnE_.s13()  - dE13v);
        /* --- new primary strain --- */
        e_rate_ += dEdp * dsubv;
        dEpn   = s->working_[EPrime] + dEdp * dVdt;

        if (!s->isCreepInUse()) break; //no more iteration needed if creep is off
      }
    } 
    else // if (isCreepInUse())
    {
      dS0n   = dS0   + bulk_ * dDevol;
      dS11n  = dS11d       + s->working_[G2] * dE11d;
      dS22n  = dS22d       + s->working_[G2] * dE22d;
      dS33n  = dS33d       + s->working_[G2] * dE33d;
      dS23n  = s->stnS_.s23()  + s->working_[G2] * s->stnE_.s23() ;
      dS12n  = s->stnS_.s12()  + s->working_[G2] * s->stnE_.s12() ;
      dS13n  = s->stnS_.s13()  + s->working_[G2] * s->stnE_.s13() ;
    } // if (isCreepInUse())
    /* --- total stresses --- */
    s->stnS_.rs11() = dS11n + dS0n;
    s->stnS_.rs22() = dS22n + dS0n;
    s->stnS_.rs33() = dS33n + dS0n;
    s->stnS_.rs23() = dS23n;
    s->stnS_.rs12() = dS12n;
    s->stnS_.rs13() = dS13n;
    /* --- current density accumulation --- */
    s->working_[Arhoc] += dsubv * dDevol;
    /* --- store primary strain locally  --- */
    if (s->isCreepInUse()) s->working_[EPrime_sum] += dEpn * dsubv;
    /* --- update density, bulk, shear and store globally --- */
    if (s->sub_zone_==s->total_sub_zones_-1)  {
      Double dVal = 1.0 / s->getZoneVolume();
      if (s->overlay_==2) dVal *= 0.5;
      if (s->isCreepInUse()) 
      {
        e_prim_ = s->working_[EPrime_sum] * dVal;
        e_rate_ *= dVal;
      }
      Double rho_M   = rho_ / (1.0 + s->working_[Arhoc]*dVal);
      rho_M   = rho_ > rho_M ? rho_ : rho_M;
      rho_    = rho_M < rhof_ ? rho_M : rhof_;
      f_d_    = rho_ / rhof_;
      if (k1_ == 0.0 && g1_ == 0.0) 
      {
        bulk_  = kf_;
        shear_ = gf_;
      }
      else 
      {
        dVal  = rho_ - rhof_;
        bulk_  = kf_ * exp(k1_ * dVal);
        shear_ = gf_ * exp(g1_ * dVal);
        shear_ = shear_ < 1.5 * bulk_ ? shear_ : 1.5 * bulk_;
        shear_ = shear_ > 0.0 ? shear_ : 0.0;
      }
      s->viscous_ = true;  // allow viscous strains
    }
  }
} // namespace models

// EOF
