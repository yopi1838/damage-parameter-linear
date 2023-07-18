#include "jmodelexample.h"
#include "state.h"
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
  return "jmodel2d";
#else
  return "jmodel2";
#endif
}

extern "C" __declspec(dllexport) unsigned getMajorVersion()
{
  return MAJOR_VERSION;
}

extern "C" __declspec(dllexport) unsigned getMinorVersion()
{
  return MINOR_VERSION;
}

extern "C" __declspec(dllexport) void *createInstance()
{
  jmodels::JModelexample *m = new jmodels::JModelexample();
  return (void *)m;
}
#endif

namespace jmodels
{
  static const Double d2d3 = 2.0 / 3.0;
  static const Double dPi  = 3.141592653589793238462643383279502884197169399;
  static const Double dDegRad = dPi / 180.0;

  // Plasticity Indicators
  static const UInt slip_now     = 0x01;  /* state logic */
  static const UInt tension_now  = 0x02;
  static const UInt slip_past    = 0x04;
  static const UInt tension_past = 0x08;

  JModelexample::JModelexample() :
    kn_(0),
    ks_(0),
    cohesion_(0),
    friction_(0),
    dilation_(0),
    tension_(0),
    zero_dilation_(0),
    res_cohesion_(0),
    res_friction_(0),
    res_dilation_(0),
    res_tension_(0),
    tan_friction_(0),
    tan_dilation_(0),
    tan_res_friction_(0),
    tan_res_dilation_(0),
    cleng_noupdate_(0),
    cleng_ini_(0),
    kn_soft_(0),
    unzero_(0),
    unmax_(0),
    kn_unload_(0),
    tens_soft_(0),
    tens_soft_rat_(0),
    n_ind_(0),
    usult_rat_(0),
    usult_(0),
    ks_soft_(0),
    uszero_(0),
    usmax_(0),
    ks_unload_(0),
    coh_soft_(0),
    coh_soft_rat_(0),
    s_ind_(0),
    state_ind_(0),
    n_energy_(0),
    s_energy_(0),
    usplast_(0),
    ns_indep_(0),
    cTab_(0), fTab_(0), dTab_(0), tTab_(0),
    iCTab_(0), iFTab_(0), iDTab_(0), iTTab_(0),
    ssoft1_(0),ssoft2_(0),ssoft3_(0),ssoft4_(0),
    dssoft1_(0),dssoft2_(0),dssoft3_(0),dssoft4_(0),    
    nsoft1_(0),nsoft2_(0),nsoft3_(0), nsoft4_(0),
    dnsoft1_(0),dnsoft2_(0),dnsoft3_(0), dnsoft4_(0),
    fks_unload_ (0),    
    ext1_(0), ext2_(0), ext3_(0), ext4_(0), ext5_(0),
    ext6_(0), ext7_(0), ext8_(0), ext9_(0), ext10_(0),
    tan_fric_soft_(0), tan_fric_soft_rat_(0),
    ntypesoft_(0), stypesoft_(0),
    gn_(0), gs_(0),
    fscorrtype_(0)
  {
  }

  String JModelexample::getName() const
  {
#ifdef JMODELDEBUG
    return L"jmodel2d";
#else
    return L"jmodel2";
#endif
  }

  String JModelexample::getFullName() const
  {
#ifdef JMODELDEBUG
    return L"jmodel2d";
#else
    return L"jmodel2";
#endif
  }

  UInt JModelexample::getMinorVersion() const
  {
    return MINOR_VERSION;
  }

  String JModelexample::getProperties() const
  {
    return L"jkn        ,jks           ,jcohesion     ,jfriction      ,jdilation  ,"
           L"jtension   ,zerdilation   ,rescohesion   ,resfriction    ,resdilation,"
           L"restension ,cleng_noupdate,cleng_ini     ,jkn_soft       ,unzero     ,"
           L"unmax      ,jkn_unload    ,jtens_soft    ,jtens_soft_rat ,n_ind      ,"
           L"dsult_rat  ,usult         ,jks_soft      ,uszero         ,usmax      ,"
           L"jks_unload ,jcoh_soft     ,jcoh_soft_rat ,s_ind          ,state_ind  ,"
           L"n_energy   ,s_energy      ,usplast       ,ns_indep      ,ctable     ,"
           L"ftable     ,dtable        ,ttable        ,"
           L"jc1     ,jc2      ,jc3        ,jc4        ,"
           L"us1     ,us2      ,us3        ,us4        ,"
           L"jt1     ,jt2      ,jt3        ,jt4        ,"
           L"un1     ,un2      ,un3        ,un4        ,"
           L"fks_unload ,"
           L"ext1       ,ext2        ,ext3          ,ext4           ,ext5        ,"
           L"ext6       ,ext7        ,ext8          ,ext9           ,ext10       ,"
           L"tan_fric_soft,tan_fric_soft_rat ,ntypesoft     ,stypesoft      ,"
           L"Gn         ,Gs          ,fscorrtype    ";
  }

  String JModelexample::getStates() const
  {
    return L"slip-n,tension-n,slip-p,tension-p";
  }

  Variant JModelexample::getProperty(UInt index) const
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
    case 10: return res_dilation_;
    case 11: return res_tension_;
    case 12: return cleng_noupdate_;
    case 13: return cleng_ini_;
    case 14: return kn_soft_;
    case 15: return unzero_;
    case 16: return unmax_;
    case 17: return kn_unload_;
    case 18: return tens_soft_;
    case 19: return tens_soft_rat_;
    case 20: return n_ind_;
    case 21: return usult_rat_;
    case 22: return usult_;
    case 23: return ks_soft_;
    case 24: return uszero_;
    case 25: return usmax_;
    case 26: return ks_unload_;
    case 27: return coh_soft_;
    case 28: return coh_soft_rat_;
    case 29: return s_ind_;
    case 30: return state_ind_;
    case 31: return n_energy_;
    case 32: return s_energy_;
    case 33: return usplast_;
    case 34: return ns_indep_;
    case 35: return cTab_;
    case 36: return fTab_;
    case 37: return dTab_;
    case 38: return tTab_;
    case 39: return ssoft1_;
    case 40: return ssoft2_;
    case 41: return ssoft3_;
    case 42: return ssoft4_;
    case 43: return dssoft1_;
    case 44: return dssoft2_;
    case 45: return dssoft3_;
    case 46: return dssoft4_;
    case 47: return nsoft1_;
    case 48: return nsoft2_;
    case 49: return nsoft3_;
    case 50: return nsoft4_;
    case 51: return dnsoft1_;
    case 52: return dnsoft2_;
    case 53: return dnsoft3_;
    case 54: return dnsoft4_;
    case 55: return fks_unload_;
    case 56: return ext1_;
    case 57: return ext2_;
    case 58: return ext3_;
    case 59: return ext4_;
    case 60: return ext5_;
    case 61: return ext6_;
    case 62: return ext7_;
    case 63: return ext8_;
    case 64: return ext9_;
    case 65: return ext10_;
    case 66: return tan_fric_soft_;
    case 67: return tan_fric_soft_rat_;
    case 68: return ntypesoft_;
    case 69: return stypesoft_;
    case 70: return gn_;
    case 71: return gs_;
    case 72: return fscorrtype_;
    }
    return 0.0;
  }

  void JModelexample::setProperty(UInt index,const Variant &prop,UInt)
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
    case 10: res_dilation_ = prop.toDouble();  break;
    case 11: res_tension_ = prop.toDouble();  break;
    case 12: cleng_noupdate_ = prop.toInt();  break;
    case 13: cleng_ini_ = prop.toDouble();  break;
    case 14: kn_soft_ = prop.toDouble();  break;
    case 15: unzero_ = prop.toDouble();  break;
    case 16: unmax_ = prop.toDouble();  break;
    case 17: kn_unload_ = prop.toDouble();  break;
    case 18: tens_soft_ = prop.toDouble();  break;
    case 19: tens_soft_rat_ = prop.toDouble();  break;
    case 20: n_ind_ = prop.toDouble();  break;
    case 21: usult_rat_ = prop.toDouble();  break;
    case 22: usult_ = prop.toDouble();  break;
    case 23: ks_soft_ = prop.toDouble();  break;
    case 24: uszero_ = prop.toDouble();  break;
    case 25: usmax_ = prop.toDouble();  break;
    case 26: ks_unload_ = prop.toDouble();  break;
    case 27: coh_soft_ = prop.toDouble();  break;
    case 28: coh_soft_rat_ = prop.toDouble();  break;
    case 29: s_ind_ = prop.toDouble();  break;
    case 30: state_ind_ = prop.toDouble();  break;
    case 31: n_energy_ = prop.toDouble();  break;
    case 32: s_energy_ = prop.toDouble();  break;
    case 33: usplast_ = prop.toDouble();  break;
    case 34: ns_indep_ = prop.toInt();  break;
    case 35: cTab_ = prop.toInt();  break;
    case 36: fTab_ = prop.toInt();  break;
    case 37: dTab_ = prop.toInt();  break;
    case 38: tTab_ = prop.toInt();  break;
    case 39: ssoft1_ = prop.toDouble();  break;
    case 40: ssoft2_ = prop.toDouble();  break;
    case 41: ssoft3_ = prop.toDouble();  break;
    case 42: ssoft4_ = prop.toDouble();  break;
    case 43: dssoft1_ = prop.toDouble();  break;
    case 44: dssoft2_ = prop.toDouble();  break;
    case 45: dssoft3_ = prop.toDouble();  break;
    case 46: dssoft4_ = prop.toDouble();  break;
    case 47: nsoft1_ = prop.toDouble();  break;
    case 48: nsoft2_ = prop.toDouble();  break;
    case 49: nsoft3_ = prop.toDouble();  break;
    case 50: nsoft4_ = prop.toDouble();  break;
    case 51: dnsoft1_ = prop.toDouble();  break;
    case 52: dnsoft2_ = prop.toDouble();  break;
    case 53: dnsoft3_ = prop.toDouble();  break;
    case 54: dnsoft4_ = prop.toDouble();  break;
    case 55: fks_unload_ = prop.toDouble();  break;
    case 56: ext1_ = prop.toDouble();  break;
    case 57: ext2_ = prop.toDouble();  break;
    case 58: ext3_ = prop.toDouble();  break;
    case 59: ext4_ = prop.toDouble();  break;
    case 60: ext5_ = prop.toDouble();  break;
    case 61: ext6_ = prop.toDouble();  break;
    case 62: ext7_ = prop.toDouble();  break;
    case 63: ext8_ = prop.toDouble();  break;
    case 64: ext9_ = prop.toDouble();  break;
    case 65: ext10_ = prop.toDouble();  break;
    case 66: tan_fric_soft_ = prop.toDouble();  break;
    case 67: tan_fric_soft_rat_ = prop.toDouble();  break;
    case 68: ntypesoft_ = prop.toInt();  break;
    case 69: stypesoft_ = prop.toInt();  break;
    case 70: gn_ = prop.toDouble();  break;
    case 71: gs_ = prop.toDouble();  break;
    case 72: fscorrtype_ = prop.toInt();  break;
    }
  }


  void JModelexample::copy(const JointModel *m)
  {
    JointModel::copy(m);
    const JModelexample *mm = dynamic_cast<const JModelexample *>(m);
    if (!mm) throw std::exception("Internal error: constitutive model dynamic cast failed.");
    kn_ = mm->kn_;
    ks_ = mm->ks_;
    cohesion_ = mm->cohesion_;
    friction_ = mm->friction_;
    dilation_ = mm->dilation_;
    tension_ = mm->tension_;
    zero_dilation_ = mm->zero_dilation_;
    res_cohesion_ = mm->res_cohesion_;
    res_friction_ = mm->res_friction_;
    res_dilation_ = mm->res_dilation_;
    res_tension_ = mm->res_tension_;
    tan_friction_ = mm->tan_friction_;
    tan_dilation_ = mm->tan_dilation_;
    tan_res_friction_ = mm->tan_res_friction_;
    tan_res_dilation_ = mm->tan_res_dilation_;
    cleng_noupdate_ = mm->cleng_noupdate_;
    cleng_ini_ = mm->cleng_ini_;
    kn_soft_ = mm->kn_soft_;
    unzero_ = mm->unzero_;
    unmax_ = mm->unmax_;
    kn_unload_ = mm->kn_unload_;
    tens_soft_ = mm->tens_soft_;
    tens_soft_rat_ = mm->tens_soft_rat_;
    n_ind_ = mm->n_ind_;
    usult_rat_ = mm->usult_rat_;
    usult_ = mm->usult_;
    ks_soft_ = mm->ks_soft_;
    uszero_ = mm->uszero_;
    usmax_ = mm->usmax_;
    ks_unload_ = mm->ks_unload_;
    coh_soft_ = mm->coh_soft_;
    coh_soft_rat_ = mm->coh_soft_rat_;
    s_ind_ = mm->s_ind_;
    state_ind_ = mm->state_ind_;
    n_energy_ = mm->n_energy_;
    s_energy_ = mm->s_energy_;
    usplast_ = mm->usplast_;
    ns_indep_ = mm->ns_indep_;
    cTab_ = mm->cTab_;
    fTab_ = mm->fTab_;
    dTab_ = mm->dTab_;
    tTab_ = mm->tTab_;
    ssoft1_ = mm->ssoft1_;
    ssoft2_ = mm->ssoft2_;
    ssoft3_ = mm->ssoft3_;
    ssoft4_ = mm->ssoft4_;
    dssoft1_ = mm->dssoft1_;
    dssoft2_ = mm->dssoft2_;
    dssoft3_ = mm->dssoft3_;
    dssoft4_ = mm->dssoft4_;
    nsoft1_ = mm->nsoft1_;
    nsoft2_ = mm->nsoft2_;
    nsoft3_ = mm->nsoft3_;
    nsoft4_ = mm->nsoft4_;
    dnsoft1_ = mm->dnsoft1_;
    dnsoft2_ = mm->dnsoft2_;
    dnsoft3_ = mm->dnsoft3_;
    dnsoft4_ = mm->dnsoft4_;
    fks_unload_ = mm->fks_unload_;
    ext1_ = mm->ext1_;
    ext2_ = mm->ext2_;
    ext3_ = mm->ext3_;
    ext4_ = mm->ext4_;
    ext5_ = mm->ext5_;
    ext6_ = mm->ext6_;
    ext7_ = mm->ext7_;
    ext8_ = mm->ext8_;
    ext9_ = mm->ext9_;
    ext10_ = mm->ext10_;
    tan_fric_soft_ = mm->tan_fric_soft_;
    tan_fric_soft_rat_ = mm->tan_fric_soft_rat_;
    ntypesoft_ = mm->ntypesoft_;
    stypesoft_ = mm->stypesoft_;
    gn_ = mm->gn_;
    gs_ = mm->gs_;
    fscorrtype_ = mm->fscorrtype_;

  }

  void JModelexample::initialize(UByte dim,State *s)
  {
    JointModel::initialize(dim,s);
    
    tan_friction_     = tan(friction_ * dDegRad);
    tan_res_friction_ = tan(res_friction_ * dDegRad);
    tan_dilation_     = tan(dilation_ * dDegRad);
    tan_res_dilation_ = tan(res_dilation_ * dDegRad);
    
    // initializations in the first use
    if (n_ind_ == 0) {
        
      n_ind_ = 1;
      s_ind_ = 1;
      state_ind_ = s->state_;
 
      // old linear softening  
      if (kn_) {
        // unult_ = (tension_ / kn_) * unult_rat_;
        // if (unult_rat_ > 1.0) {
        //   kn_soft_ = kn_ / (unult_rat_ - 1.0);
        // }
        // else {
        //   throw std::exception("jmsoft error: unult_rat <=1");      
        // }  
      }
      else {
        throw std::exception("jmsoft error: jkn=0");      
      }
      if (ks_) {
        // usult_ = (cohesion_ / ks_) * usult_rat_;
        // if (usult_rat_ > 1.0) {
        //   ks_soft_ = ks_ / (usult_rat_ - 1.0);
        // }
        // else {
        //   throw std::exception("jmsoft error: usult_rat <=1");      
        // }  
      }
      else {
        throw std::exception("jmsoft error: jks=0");      
      }
      
      kn_unload_ = kn_;
      ks_unload_ = ks_;
      tens_soft_ = tension_;
      coh_soft_ = cohesion_;
      tens_soft_rat_ = 1.0;
      coh_soft_rat_ = 1.0;
      // for now, keep friction softening equal to cohesion
      // tan_fric_soft_ = tan_friction_;
      // tan_fric_soft_rat_ = 1.0;
      usplast_ = 0.0;

      fks_unload_ = 1.0;

      // initial contact length/area ... not defined?
      // cleng_ini_ = s->area_;
      cleng_ini_ = 0.0;
      
      if (gn_) {
        if (ntypesoft_ == 0) ntypesoft_ = 1;
        if (!kn_ ) {
          throw std::exception("jmsoft error: jkn=0");      
        }
        if (!tension_ ) {
          throw std::exception("jmsoft error: jtens=0");      
        }
	    Double tensdrop = tension_ - res_tension_;
        Double unelmax = 0.0;
        unelmax = tension_/kn_;
        if (unelmax <= 0.0) {
          throw std::exception("jmsoft error: peak elast. normal displ. <= 0 ");      
        }
        Double gne = 0.0;
        gne = 0.5 * tensdrop * tensdrop / kn_; 
        Double gnpp = gn_ - gne;
        if (gnpp <= 0.0) {
          throw std::exception("jmsoft error: Gn post-peak < 0 ");      
        }
        Double dlin = (2.0*gnpp/tensdrop)/unelmax;
        ext4_ = gne;
        ext5_ = gnpp;

		dnsoft1_ = dnsoft2_ = dnsoft3_ = dnsoft4_ = 0.0;
		nsoft1_ = nsoft2_ = nsoft3_ = nsoft4_ = 0.0;
        if (ntypesoft_ == 1) {
          dnsoft1_ = dlin;
          nsoft1_ = 0.0;
        }
        else if (ntypesoft_ == 2) {
          dnsoft1_ = dlin / 3.0;
          nsoft1_ = 0.5;
          dnsoft2_ = dlin * (4.0/3.0);
          nsoft2_ = 0.0;
        }
        else if (ntypesoft_ == 3) {
          dnsoft1_ = dlin * 0.5;
          nsoft1_ = (1.0/3.0);
          dnsoft2_ = dlin * 1.5;
          nsoft2_ = 0.0;
        }
        else if (ntypesoft_ == 4) {
          dnsoft1_ = dlin / 3.0;
          nsoft1_ = 0.5;
          dnsoft2_ = dlin * (2.0/3.0);
          nsoft2_ = 0.25;
          dnsoft3_ = dlin * (5.0/3.0);
          nsoft3_ = 0.0;
        }
      }
      else if (dnsoft1_ > 0.0) {
        ntypesoft_ = 9;     
      }

      if (gs_) {
        if (stypesoft_ == 0) stypesoft_ = 1;
        if (!ks_ ) {
          throw std::exception("jmsoft error: jks=0");      
        }
        if (!cohesion_ ) {
          throw std::exception("jmsoft error: jcohesion=0");      
        }
        Double cohdrop = cohesion_ - res_cohesion_;
	    Double uselmax = 0.0;
        uselmax = cohesion_/ks_;
        if (uselmax <= 0.0) {
          throw std::exception("jmsoft error: peak elast. shear displ. <= 0 ");      
        }
        Double gse = 0.0;
        gse = 0.5 * cohdrop * cohdrop / ks_; 
        Double gspp = gs_ - gse;
        if (gspp <= 0.0) {
          throw std::exception("jmsoft error: Gs post-peak < 0 ");      
        }
        Double dlin = (2.0*gspp/cohdrop)/uselmax;
        ext6_ = gse;
        ext10_ = gspp;

		dssoft1_ = dssoft2_ = dssoft3_ = dssoft4_ = 0.0;
		ssoft1_ = ssoft2_ = ssoft3_ = ssoft4_ = 0.0;

        if (stypesoft_ == 1) {
          dssoft1_ = dlin;
          ssoft1_ = 0.0;
        }
        else if (stypesoft_ == 2) {
          dssoft1_ = dlin / 3.0;
          ssoft1_ = 0.5;
          dssoft2_ = dlin * (4.0/3.0);
          ssoft2_ = 0.0;
        }
        else if (stypesoft_ == 3) {
          dssoft1_ = dlin * 0.5;
          ssoft1_ = (1.0/3.0);
          dssoft2_ = dlin * 1.5;
          ssoft2_ = 0.0;
        }
        else if (stypesoft_ == 4) {
          dssoft1_ = dlin / 3.0;
          ssoft1_ = 0.5;
          dssoft2_ = dlin * (2.0/3.0);
          ssoft2_ = 0.25;
          dssoft3_ = dlin * (5.0/3.0);
          ssoft3_ = 0.0;
        }
      }
      else if (dssoft1_ > 0.0) {
        stypesoft_ = 9;     
      }

      // if (dnsoft1_ > 0.0 && dnsoft1_ < 1.0) {
      //   throw std::exception(" dnsoft1 must be > 1 ");      
      // }          
      // if (dssoft1_ > 0.0 && dssoft1_ < 1.0) {
      //   throw std::exception(" dssoft1 must be > 1 ");      
      // }          
      
      iCTab_ = iFTab_ = iDTab_ = iTTab_ = 0;
      if (cTab_ || fTab_ || dTab_ || tTab_) {
        if (cTab_) iCTab_ = s->getTableIndexFromID(cTab_);
        if (fTab_) iFTab_ = s->getTableIndexFromID(fTab_);
        if (dTab_) iDTab_ = s->getTableIndexFromID(dTab_);
        if (tTab_) iTTab_ = s->getTableIndexFromID(tTab_);
      }

    }
    
  }

  void JModelexample::run(UByte dim,State *s)
  {
    JointModel::run(dim,s);
    /* --- state indicator:                                  */
    /*     store 'now' info. as 'past' and turn 'now' info off ---*/
    if (s->state_ & slip_now) s->state_ |= slip_past;
    s->state_ &= ~slip_now;
    if (s->state_ & tension_now) s->state_ |= tension_past;
    s->state_ &= ~tension_now;
    
    if (cleng_ini_ <= 0.0) {
      cleng_ini_ = s->area_;
    }
    
    Double area = s->area_;
    if (cleng_noupdate_ && cleng_ini_) {
      area = cleng_ini_;
    }
    
    Double kna  = kn_ * area;
    Double ksa  = ks_ * area;
    
    Double un = s->normal_disp_;
    Double un_inc = s->normal_disp_inc_;
    DVect3 us = s->shear_disp_;
    Double usmag = s->shear_disp_.mag();

	Double cohdrop = cohesion_ - res_cohesion_;
    // uselmax, use peak cohesion or cohesion drop?
	Double uselmax = 0.0;
    // if (ks_) uselmax = cohdrop/ks_;
    // uselmax, use peak cohesion
    if (ks_) uselmax = cohesion_/ks_;

	Double tensdrop = tension_ - res_tension_;
    // unelmax, use peak tension or tension drop? 
    // it has to be peak tension, so that unrat starts at zero after peak
    Double unelmax = 0.0;
    // if (kn_) unelmax = tensdrop/kn_;
    if (kn_) unelmax = tension_/kn_;
        
    // max. normal displacement
    if (un > unmax_) unmax_ = un;
    
    // max. displ. in softenng curve
    Double dnsoftmax = 0.0;
    // dnsoftmax = max(dnsoft1_,dnsoft2_,dnsoft3_,dnsoft4_);
    if (dnsoft1_ > dnsoftmax) dnsoftmax = dnsoft1_;
    if (dnsoft2_ > dnsoftmax) dnsoftmax = dnsoft2_;
    if (dnsoft3_ > dnsoftmax) dnsoftmax = dnsoft3_;
    if (dnsoft4_ > dnsoftmax) dnsoftmax = dnsoft4_;
    Double dssoftmax = 0.0;
    // dssoftmax = max(dssoft1_,dssoft1_,dssoft3_,dssoft4_);
    if (dssoft1_ > dssoftmax) dssoftmax = dssoft1_;
    if (dssoft2_ > dssoftmax) dssoftmax = dssoft2_;
    if (dssoft3_ > dssoftmax) dssoftmax = dssoft3_;
    if (dssoft4_ > dssoftmax) dssoftmax = dssoft4_;
  
    bool tenflag = false;

    Double fn0 = s->normal_force_;
    DVect3 fs0 = s->shear_force_;


    // --- normal direction ---
    // if (unult_rat_ <= 1.0) {
    if (ntypesoft_ == 0) {
        
      // --- no softening, brittle --- 
    
      // normal force
      // fn0 = s->normal_force_;
      s->normal_force_inc_ = -kna * s->normal_disp_inc_;
      s->normal_force_ += s->normal_force_inc_;

      // correction for time step in which joint opens (or goes into tension)
      // s->dnop_ is part of s->normal_disp_inc_ at which separation or tension takes place
      s->dnop_ = s->normal_disp_inc_;
      if ((fn0 > 0.0) && (s->normal_force_ <= 0.0) && (s->normal_force_inc_ < 0.0))
      {
        s->dnop_ = -s->normal_disp_inc_ * fn0 / s->normal_force_inc_;
        if (s->dnop_ > s->normal_disp_inc_) s->dnop_ = s->normal_disp_inc_;
      }

      // tensile strength
      Double ten;
      if (s->state_)
        ten = -res_tension_ * area;
      else
        ten = -tension_ * area;

      // check tensile failure
      if (s->normal_force_ <= ten)
      {
        s->normal_force_  = ten;
        if (!s->normal_force_)
        {
          s->shear_force_ = DVect3(0,0,0);
          tenflag = true; // complete tensile failure
        }
        s->state_ |= tension_now;
        s->normal_force_inc_ = 0.0;
        s->shear_force_inc_ = DVect3(0,0,0);
      }
    }
    else {
    
      // --- with softening --- 
    
      // normal force
      fn0 = s->normal_force_;
      s->normal_force_inc_ = -kna * s->normal_disp_inc_;
      s->normal_force_ += s->normal_force_inc_;

      // correction for time step in which joint opens (or goes into tension)
      // s->dnop_ is part of s->normal_disp_inc_ at which separation or tension takes place
      s->dnop_ = s->normal_disp_inc_;
      if ((fn0 > 0.0) && (s->normal_force_ <= 0.0) && (s->normal_force_inc_ < 0.0))
      {
        s->dnop_ = -s->normal_disp_inc_ * fn0 / s->normal_force_inc_;
        if (s->dnop_ > s->normal_disp_inc_) s->dnop_ = s->normal_disp_inc_;
      }
     
      n_ind_ = 2;
      
      // contact not in compression
      // add unzero for insitu stress state ? 
      if (un > 0.0) {

        n_ind_ = 3;
 
        // check if unloading
        if (un_inc < 0.0 || kn_unload_ != kn_ ) {
          s->normal_force_inc_ = -(kn_unload_ * area) * s->normal_disp_inc_;
          s->normal_force_ = fn0 + s->normal_force_inc_;
          // step with change from compression to tension
          if (fn0 > 0.0) {
            s->normal_force_inc_ = -(kn_unload_ * area) * s->dnop_;
            s->normal_force_ = s->normal_force_inc_;
          }
          n_ind_ = 4;
        }

        Double unrat = 0.0;
        if (unelmax) unrat = (un-unelmax) / unelmax;
        if (unrat<0.0) unrat = 0.0;
		Double tensrat = 0.0;

        ext2_ = 0.0;
        ext3_ = 0.0;
        ext7_ = 0.0;
        ext8_ = 0.0;
        ext9_ = 0.0;

        // check softening
        if (-(s->normal_force_) > tens_soft_ * area) {

          if (dnsoft1_) {
            // find tensrat interpolating curve
            if (unrat>=0.0 && unrat<=dnsoft1_){
              n_ind_ = 62;
              Double dd = dnsoft1_-0.0;
              tensrat = 1.0 + (nsoft1_-1.0)*unrat/dd;
            }              
            else if (dnsoft2_ && unrat>dnsoft1_ && unrat<=dnsoft2_){
              n_ind_ = 63;
              Double dd = dnsoft2_-dnsoft1_;
              tensrat = nsoft1_ + (nsoft2_-nsoft1_)*(unrat-dnsoft1_)/dd;
            }              
            else if (dnsoft3_ && unrat>dnsoft2_ && unrat<=dnsoft3_){
              n_ind_ = 64;
              Double dd = dnsoft3_-dnsoft2_;
              tensrat = nsoft2_ + (nsoft3_-nsoft2_)*(unrat-dnsoft2_)/dd;
            }              
            // else if (unrat>dnsoft3_ && unrat<=unult_rat_){
            //   n_ind_ = 65;
            //   Double dd = unult_rat_-dnsoft3_;
            //   tensrat = nsoft3_ + (0.0-nsoft3_)*(unrat-dnsoft3_)/dd;
            // }
            // else if (unrat>unult_rat_){
            //   n_ind_ = 66;
            //   tensrat = 0.0;
            // }
            else if (dnsoft4_ && unrat>dnsoft3_ && unrat<=dnsoft4_){
              n_ind_ = 65;
              Double dd = dnsoft4_-dnsoft3_;
              tensrat = nsoft3_ + (nsoft4_-nsoft3_)*(unrat-dnsoft3_)/dd;
            }              
            if (unrat > dnsoftmax){
              n_ind_ = 66;
              tensrat = 0.0;
            }
            ext9_ = unrat;
            ext7_ = tensrat;
            // if not ns_indep, reduce tensrat ???
            // if (!ns_indep_) {
            //   if (tens_soft_rat_ < tensrat) tensrat = tens_soft_rat_;
            // }
            // tens_soft_rat_ should not increase
            if (tens_soft_rat_ < tensrat) tensrat = tens_soft_rat_;
            ext8_ = tensrat;
            
            // update softening strength and strength ratio
            tens_soft_ = res_tension_ + tensrat*tensdrop;
            if (tens_soft_ < 0.0) tens_soft_ = 0.0;
            if (unrat > dnsoftmax) tens_soft_ = res_tension_;
            if (tensdrop > 0.0) tens_soft_rat_ = (tens_soft_ - res_tension_) / tensdrop;
            ext2_ = tens_soft_rat_;
            ext3_ = tens_soft_;

            s->normal_force_ = -(tens_soft_ * area);
            s->normal_force_inc_ = 0.0; 
            if (un > 0.0) {
              kn_unload_ = -(s->normal_force_) / (un * area); 
            }
          }
          else { 
            // old linear softening
            // n_ind_ = 67;
            // if (un_inc > 0.0) tens_soft_ = tens_soft_ - kn_soft_ * un_inc;
            // if (tens_soft_ < 0.0) tens_soft_ = 0.0;

            // if (unrat > unult_rat_) tens_soft_ = res_tension_;
            // if (tensdrop > 0.0) tens_soft_rat_ = (tens_soft_ - res_tension_) / tensdrop;
            // ext2_ = tens_soft_rat_;
            // ext3_ = tens_soft_;

            // s->normal_force_ = -(tens_soft_ * area);
            // s->normal_force_inc_ = 0.0; 
            // if (un > 0.0) {
            //   kn_unload_ = -(s->normal_force_) / (un * area); 
            // }
          }

        }          
      
      }

      // check complete tensile failure
      if ((!s->normal_force_) && (!tens_soft_))
      {
        s->shear_force_ = DVect3(0,0,0);
        s->state_ |= tension_now;
        s->normal_force_inc_ = 0.0;
        s->shear_force_inc_ = DVect3(0,0,0);  
        tenflag = true; // complete tensile failure
        n_ind_ = 9;
      }
    
    }
    
    // --- shear direction ---
    
    // if (usult_rat_ <= 1.0) {
    if (stypesoft_ == 0) {
        
      // --- no softening, brittle -------------------------------------------- 
    
      s_ind_ = 2;
    
      // shear force
      if (!tenflag)
      {
        s->shear_force_inc_ = s->shear_disp_inc_ * -ksa;
        s->shear_force_ += s->shear_force_inc_;
        Double fsm = s->shear_force_.mag();
        // shear strength
        Double fsmax;
        if (!s->state_) {
          fsmax = cohesion_ * area + tan_friction_ * s->normal_force_;
          s_ind_ = 3;
        }
        else
        { // if residual friction is zero, take peak value
          Double resamueff = tan_res_friction_;
          if (!resamueff) resamueff = tan_friction_;
          fsmax = res_cohesion_ * area + resamueff * s->normal_force_;
          s_ind_ = 4;
        }
        if (fsmax < 0.0) {
          fsmax = 0.0;
          s_ind_ = 5;
        }
        //  check for slip
        if (fsm >= fsmax)
        {
          s_ind_ = 6;
          Double rat = 0.0;
          if (fsm) rat = fsmax / fsm;
          s->shear_force_ *= rat;
          s->state_ |= slip_now;
          s->shear_force_inc_ = DVect3(0,0,0);
          // dilation
          if (dilation_)
          {
            s_ind_ = 7;
            Double zdd = zero_dilation_;
            Double usm = s->shear_disp_.mag();
            if (!zdd) zdd = 1e20;
            if (usm < zdd)
            {
              s_ind_ = 8;
              Double dusm  = s->shear_disp_inc_.mag();
              Double dil = 0.0;
              if (!s->state_) dil = tan_dilation_;
              else
              {
                // if residual dilation is zero, take peak value
                //     Double resdileff = tan_res_dilation_;
                // Note: In CLJ1 in 3DEC, no residual dilation is defined
                Double resdileff = tan_dilation_;
                if (!resdileff) resdileff = tan_dilation_;
                dil = resdileff;
              }
              s->normal_force_ += kna * dil * dusm;
            }
          } // dilation
        } // fsm>fsmax
      } // if (!tenflg)
    }
    else {
    
      // --- with softening --------------------------------------------------- 
     
      s_ind_ = 3;
          
      // shear force
      if (!tenflag)
      {
        Double dusm = s->shear_disp_inc_.mag();
        s->shear_force_inc_ = s->shear_disp_inc_ * (-ksa);
        s->shear_force_ += s->shear_force_inc_;
        Double fsm = s->shear_force_.mag();
        Double dfsm = s->shear_force_inc_.mag();

        // check if unloading
        if (fks_unload_ < 1.0) {
          // project shear displacement increment on total shear displacement
          Double dusproj = us | s->shear_disp_inc_;
          if (dusproj < 0.0 || ks_unload_ != ks_ ) {
            s->shear_force_inc_ = s->shear_disp_inc_ * (-(ks_unload_ * area));
            s->shear_force_ = fs0 + s->shear_force_inc_;
            fsm = s->shear_force_.mag();
            dfsm = s->shear_force_inc_.mag();
            s_ind_ = 4;
          }
        }

		// peak friction
        Double tanfric = tan_friction_;
        // if residual friction is zero, take peak value
        Double tanresfric = tan_res_friction_;
        if (!tanresfric) tanresfric = tan_friction_;
		// current friction
		Double tanfricsoft = tanresfric + (tanfric - tanresfric) * coh_soft_rat_;
        // current shear strength
        Double fsmax;
        fsmax = coh_soft_ * area + tanfricsoft * s->normal_force_;
        if (fsmax < 0.0) {
          fsmax = 0.0;
          s_ind_ = 5;
        }

        //  check for slip
        if (fsm >= fsmax)
        {
          s_ind_ = 6;
          Double rat = 0.0;
          if (fsm) rat = fsmax / fsm;
          s->shear_force_ *= rat;
          s->state_ |= slip_now;
          s->shear_force_inc_ = DVect3(0,0,0);
          
          Double ratinc = 1.0;
          if (dfsm > 0.0) ratinc = (fsm-fsmax) / dfsm;
          if (ratinc > 1.0) ratinc = 1.0;
          if (ratinc < 0.0) ratinc = 0.0;
		  
		  // plastic displacement
		  Double dusplast = ratinc*dusm;
		  usplast_ = usplast_ + ratinc*dusm;
          
          Double usrat = 0.0;
          // if (uselmax) usrat = usmag / uselmax;
          if (uselmax) usrat = usplast_ / uselmax;
		  Double cohrat = 0.0;
		  
          ext1_ = usrat;  
          // ext2_ = cTab_;
          // ext3_ = 0.0;
          ext4_ = 0.0;
          ext5_ = 0.0;
          // table
          Double s_slope = 0.0;       
          if (iCTab_) {
            s_ind_ = 61;
            // s->getYFromX(iCTab_,sHP_);
            // s->getSlopeFromX(iCTab_,sHP_);
            if (usrat > 1.0) {
              s_slope = s->getSlopeFromX(iCTab_,usrat); 
              ext3_ = s_slope;
              ext4_ = s->getYFromX(iCTab_,usrat);            
            }
            // ??? if (dusrat > usult_rat_) s_slope = 0.0;             
            coh_soft_ = coh_soft_ + (s_slope/uselmax) * dusm * ratinc;
            if (coh_soft_ < 0.0) coh_soft_ = 0.0;
            // ext3_ = ratinc;
            // ext4_ = dusm;
            ext5_ = coh_soft_;
          }
          else if (dssoft1_) {
            // find cohrat interpolating curve
            if (usrat>=0.0 && usrat<=dssoft1_) {
              s_ind_ = 62;
              Double dd = dssoft1_-0.0;
              cohrat = 1.0 + (ssoft1_-1.0)*usrat/dd;
            }              
            else if (dssoft2_ && usrat>dssoft1_ && usrat<=dssoft2_) {
              s_ind_ = 63;
              Double dd = dssoft2_-dssoft1_;
              cohrat = ssoft1_ + (ssoft2_-ssoft1_)*(usrat-dssoft1_)/dd;
            }              
            else if (dssoft3_ && usrat>dssoft2_ && usrat<=dssoft3_) {
              s_ind_ = 64;
              Double dd = dssoft3_-dssoft2_;
              cohrat = ssoft2_ + (ssoft3_-ssoft2_)*(usrat-dssoft2_)/dd;
            }              
            // else if (usrat>dssoft3_ && usrat<=usult_rat_){
            //   s_ind_ = 65;
            //   Double dd = usult_rat_-dssoft3_;
            //   cohrat = ssoft3_ + (0.0-ssoft3_)*(usrat-dssoft3_)/dd;
            // }
            // else if (usrat>usult_rat_){
            //   s_ind_ = 66;
            //   cohrat = 0.0;
            // }
            else if (dssoft4_ && usrat>dssoft3_ && usrat<=dssoft4_) {
              s_ind_ = 65;
              Double dd = dssoft4_-dssoft3_;
              cohrat = ssoft3_ + (ssoft4_-ssoft3_)*(usrat-dssoft3_)/dd;
            }              
            if (usrat > dssoftmax) {
              s_ind_ = 66;
              cohrat = 0.0;
            }
            // if not ns_indep, reduce cohrat ???
            // if (!ns_indep_) {
            //  if (coh_soft_rat_ < cohrat) cohrat = coh_soft_rat_;
            // }
            // coh_soft_rat_ should not increase
            if (coh_soft_rat_ < cohrat) cohrat = coh_soft_rat_;
            
            // update softening strength and strength ratio           
            coh_soft_ = res_cohesion_ + cohrat*cohdrop;
            if (coh_soft_ < 0.0) coh_soft_ = 0.0;
            if (usrat > dssoftmax) coh_soft_ = res_cohesion_;
            if (cohdrop > 0.0) coh_soft_rat_ = (coh_soft_ - res_cohesion_)/cohdrop;
            ext4_ = coh_soft_rat_;
            ext5_ = coh_soft_;

          }          
          else { 
            // old linear softening
            // s_ind_ = 67;
            // coh_soft_ = coh_soft_ - ks_soft_ * dusm * ratinc;
            // if (coh_soft_ < 0.0) coh_soft_ = 0.0;
            // if (usrat>usult_rat_) coh_soft_ = res_cohesion_;
            // should correct fs with new fsmax?
          }          

          // correct fs with new strength, unless fscorrtype is set
          if (!fscorrtype_) {
	        Double tanfricsoft1 = tanresfric + (tanfric - tanresfric) * coh_soft_rat_;
            Double fsmax1 = coh_soft_ * area + tanfricsoft1 * s->normal_force_;
            if (fsmax1 < 0.0) {
              fsmax1 = 0.0;
              s_ind_ = 5;
            }
            if (fsmax1 < fsmax) {
              Double rat2 = 0.0;
              if (fsmax > 0.0) {
                rat2 = fsmax1 / fsmax;
                s->shear_force_ *= rat2;
              }
              ext6_ = rat2;
            } 
          }
          
          // dilation
          if (dilation_)
          {
            s_ind_ = 7;
            Double zdd = zero_dilation_;
            Double usm = s->shear_disp_.mag();
            if (!zdd) zdd = 1e20;
            if (usm < zdd)
            {
              s_ind_ = 8;
              Double dusm  = s->shear_disp_inc_.mag();
              Double dil = 0.0;
              if (!s->state_) dil = tan_dilation_;
              else
              {
                // if residual dilation is zero, take peak value
                //     Double resdileff = tan_res_dilation_;
                // Note: In CLJ1 in 3DEC, no residual dilation is defined
                Double resdileff = tan_dilation_;
                if (!resdileff) resdileff = tan_dilation_;
                dil = resdileff;
              }
              s->normal_force_ += kna * dil * dusm;
            }
          } // dilation

          // update unloading slope
          if (fks_unload_ < 1.0) {
            Double fsmag = s->shear_force_.mag();
            if (usmag > 0.0) {
              ks_unload_ = (fks_unload_ * ks_) + (1.0-fks_unload_) * (fsmag) / (usmag * area); 
            }
          }

        } // fsm>fsmax
      } // if (!tenflg)
      else {
        // tenflag
        s_ind_ = 9;
      }
          
      if (cohdrop > 0.0) coh_soft_rat_ = (coh_soft_ - res_cohesion_) / cohdrop;
    
    }
    
    // interaction between normal and shear softening, if not ns_indep
    if (!ns_indep_) {
      if (coh_soft_rat_ < tens_soft_rat_) {
        tens_soft_rat_ = coh_soft_rat_;
        tens_soft_ = res_tension_ + tens_soft_rat_ * tensdrop;
      }
      if (tens_soft_rat_ < coh_soft_rat_) {
        coh_soft_rat_ = tens_soft_rat_;
        coh_soft_ = res_cohesion_ + coh_soft_rat_ * cohdrop;
      }
    }
 
    // update variables    
    state_ind_ = s->state_;
         
    // energy
    n_energy_ += -0.5 * (fn0+s->normal_force_) * s->normal_disp_inc_;
    s_energy_ += -0.5 * ((fs0+s->shear_force_) | (s->shear_disp_inc_));

  }

} // namespace models


// EOF
