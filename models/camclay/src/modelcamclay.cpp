#include "modelcamclay.h"
#include "base/src/version.h"

namespace models {

    ModelCamClay::ModelCamClay(): bulk_(0.0),bulkB_(0.0),shear_(0.0),poisson_(0.0),
        Kappa_(0.0),Lambda_(0.0),MM_(0.0),MPC_(0.0),MP1_(0.0),
        MV_L_(0.0),MV_(0.0),EV_(0.0),EV_P_(0.0),MP_(0.0),MQ_(0.0) {
    }

    String ModelCamClay::getProperties(void) const {
        return L"bulk,shear,bulk-maximum,poisson,"
            L"kappa,lambda,ratio-critical-state,pressure-preconsolidation,"
            L"pressure-reference,specific-volume-reference,specific-volume,"
            L"strain-volumetric-plastic,pressure-effective,stress-deviatoric";
    }

    UInt ModelCamClay::getMinorVersion() const {
        return version::update;
    }

    String ModelCamClay::getStates(void) const {
        return L" , , , , , , , ,volume-n,volume-p";
    }


    Variant ModelCamClay::getProperty(UInt index) const {
        switch (index) {
        case 1:  return(bulk_);
        case 2:  return(shear_);
        case 3:  return(bulkB_);
        case 4:  return(poisson_);
        case 5:  return(Kappa_);
        case 6:  return(Lambda_);
        case 7:  return(MM_);
        case 8:  return(MPC_);
        case 9:  return(MP1_);
        case 10: return(MV_L_);
        case 11: return(MV_);
        case 12: return(EV_P_);
        case 13: return(MP_);
        case 14: return(MQ_);
        }
        return(0.0);
    }

    void ModelCamClay::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);
        switch (index) {
        case 1:  bulk_    = p.toDouble();  break;
        case 2:  shear_   = p.toDouble();  break;
        case 3:  bulkB_   = p.toDouble();  break;
        case 4:  poisson_ = p.toDouble();  break;
        case 5:  Kappa_   = p.toDouble();  break;
        case 6:  Lambda_  = p.toDouble();  break;
        case 7:  MM_      = p.toDouble();  break;
        case 8:  MPC_     = p.toDouble();  break;
        case 9:  MP1_     = p.toDouble();  break;
        case 10: MV_L_    = p.toDouble();  break;
        case 11: MV_      = p.toDouble();  break;
        case 12: EV_P_    = p.toDouble();  break;
        case 13: MP_      = p.toDouble();  break;
        case 14: MQ_      = p.toDouble();  break;
        }
    }

    bool ModelCamClay::isPropertyReadOnly(UInt i) const {
        switch (i) {
        case  1:
        case 11:
        case 12:
        case 14: return true;
        default: return false;
        }
    }

    bool ModelCamClay::isPropertyAdvanced(UInt) const {
        return false;
    }

    void ModelCamClay::copy(const ConstitutiveModel *m) {
        ConstitutiveModel::copy(m);
        const ModelCamClay *em = dynamic_cast<const ModelCamClay *>(m);
        if (!em) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        bulk_    = em->bulk_;
        shear_   = em->shear_;
        bulkB_   = em->bulkB_;
        poisson_ = em->poisson_;
        Kappa_   = em->Kappa_;
        Lambda_  = em->Lambda_;
        MM_      = em->MM_;
        MPC_     = em->MPC_;
        MP1_     = em->MP1_;
        MV_L_    = em->MV_L_;
        EV_P_    = em->EV_P_;
        MP_      = em->MP_;
        MQ_      = em->MQ_;
    }

    void ModelCamClay::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);

        // initialize mean pressure
        if (MP1_ == 0.0) MP1_ = 1.0;
        if (MP1_ < 0.0 || MPC_ < 0.0) throw std::runtime_error("Cam-clay: negtive mp1 or mpc");
        if (MV_L_ < 0.0 || Kappa_ == 0.0) throw std::runtime_error("Cam-clay: kappa is zero or negtive mv_l");
        if (Lambda_ <= Kappa_) throw std::runtime_error("Cam-clay: lambda cannot be equal to or less than kappa");
        if (bulkB_ == 0.0) throw std::runtime_error("Cam-clay: bulk_bound is zero");
        if (poisson_ < 0.0 || poisson_ > 0.5) throw std::runtime_error("Cam-clay: invalid poisson");

        /* --- initialize specific volume --- */
        if (MV_ == 0.0) {
            Double dP0 = MP_;           // initialized in camclay_ini_p
            if (dP0 <= 0.0) throw std::runtime_error("Cam-clay: Mean effective pressure is negative");
            else {
                if (MP_ > MPC_) throw std::runtime_error("Cam-clay: pressure-preconsolidation cannot be less than the initial mean effective pressure");
                MV_ = MV_L_ - Lambda_*log(MPC_/MP1_) + Kappa_*log(MPC_/dP0);
                if (MV_ < 0.0) throw std::runtime_error("Cam-clay:  Specific volume is negative");
            }
        }
        /* --- initialize current bulk modulus --- */
        if (bulk_ == 0.0) {
            Double dP0 = MP_;
            if (dP0 <= 0.0) throw std::runtime_error("Cam-clay: Mean effective pressure is negative");
            else {
                bulk_ = MV_ * std::max(dP0, MP1_*1.e-3) / Kappa_;
                if (poisson_ && !shear_)
                    shear_ = 1.5*bulk_*(1.0-2.0*poisson_)/(1.0+poisson_);
            }
        }
        /* --- initialize shear modulus --- */
        if (!shear_ && poisson_)
            shear_ = 1.5*bulk_*(1.0-2.0*poisson_)/(1.0+poisson_);
    }

    static const UInt Pav    = 0;
    static const UInt Qav    = 1;
    static const UInt Evav   = 2;
    static const UInt EvdPav = 3;
    static const UInt Pind   = 0;
    // The above are now indices into s->working_[] and s->iworkingp[].
    // This is necessary for thread safety.

    void ModelCamClay::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);
        if (s->state_ & volume_now) {
            s->state_ |= volume_past;
            s->state_ &= ~volume_now;
        }
        if (!s->sub_zone_) {
            s->iworking_[Pind] = 0;
            s->working_[Pav]   = 0.0;
            s->working_[Qav]   = 0.0;
            s->working_[Evav]  = 0.0;
            s->working_[EvdPav] = 0.0;
        }

        Double dA1 = bulk_ + d4d3*shear_;
        Double dA2 = bulk_ - d2d3*shear_;
        Double dG2 = 2.0*shear_;
        s->stnS_.rs11() += s->stnE_.s11() * dA1 + (s->stnE_.s22() + s->stnE_.s33()) * dA2;
        s->stnS_.rs22() += s->stnE_.s22() * dA1 + (s->stnE_.s11() + s->stnE_.s33()) * dA2;
        s->stnS_.rs33() += s->stnE_.s33() * dA1 + (s->stnE_.s11() + s->stnE_.s22()) * dA2;
        s->stnS_.rs12() += s->stnE_.s12() * dG2;
        s->stnS_.rs13() += s->stnE_.s13() * dG2;
        s->stnS_.rs23() += s->stnE_.s23() * dG2;

        s->viscous_ = true;  // allow viscous strains
        if (!canFail()) return;

        Double sMeanOld = s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33();
        /* --- mean pressure --- */
        Double dPVal = - (sMeanOld) * d1d3;
        /* --- deviatoric stresses --- */
        Double dDS11 = s->stnS_.s11() + dPVal;
        Double dDS22 = s->stnS_.s22() + dPVal;
        Double dDS33 = s->stnS_.s33() + dPVal;
        Double dDS12 = s->stnS_.s12();
        Double dDS13 = s->stnS_.s13();
        Double dDS23 = s->stnS_.s23();
        /* --- second deviatoric stress invariant --- */
        Double dJ2   = 0.5*(dDS11*dDS11+dDS22*dDS22+dDS33*dDS33)+(dDS12*dDS12+dDS23*dDS23+dDS13*dDS13);
        Double dQVal = sqrt(3.*dJ2);
        /* --- Cam-clay yield criterion --- */
        Double dM2 = MM_*MM_;
        Double dFS = dQVal*dQVal + dM2 * dPVal * (dPVal - MPC_);

        Double dEVVal  = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33();
        Double dPNew   = dPVal;
        Double dQNew   = dQVal;
        Double dEvdPVal = 0.0;

        /* --- detect small pc --- */
        Double p_small = MP1_*1.e-3;
        if (MPC_ <= p_small) {
            s->iworking_[Pind] = 1;
            s->state_ |= volume_now;
            s->stnS_.rs11() = 0.0;
            s->stnS_.rs22() = 0.0;
            s->stnS_.rs33() = 0.0;
            s->stnS_.rs12() = 0.0;
            s->stnS_.rs13() = 0.0;
            s->stnS_.rs23() = 0.0;
            dEvdPVal = dEVVal;
        } else {
            if (dFS > 0.0) {
                /* ---                        shear failure --- */
                s->iworking_[Pind] = 1;
                s->state_ |= volume_now;
                Double dSa    = 6.0 * shear_ * dQVal;
                Double dSc    = dM2 * (2.0 * dPVal - MPC_);
                Double dSb    = bulk_ * dSc;
                Double dBa    = dSa*dSa + dM2 * dSb*dSb;
                Double dBb    = dSa * dQVal + 0.5 * dSb * dSc;
                Double dBc    = dFS;
                Double dAlam  = 0.0;
                Double dAlam1 = 0.0;
                if (dBa != 0.0) {
                    Double dBd = dBb*dBb - dBa*dBc;
                    if (dBd < 0.0) throw std::runtime_error("Cam-clay:  Yield envelope cannot be reached");
                    dBd = std::sqrt(dBd);
                    Double dBe = dBb < 0.0 ? (dBb - dBd) : (dBb + dBd);
                    dAlam  = dBe / dBa;
                    dAlam1 = dBc / dBe;
                } else {
                    if (dBb != 0.0) {
                        dAlam  = 0.5 * dBc / dBb;
                        dAlam1 = dAlam;
                    } else throw std::runtime_error("Cam-clay:  Yield envelope cannot be reached");
                } 
                if (fabs(dAlam1) < fabs(dAlam)) dAlam = dAlam1;
                if (dAlam < 0.0) dAlam = 0.0;
                dQNew  = dQVal - dSa * dAlam;
                dPNew  = dPVal - dSb * dAlam;
                if (dQNew < 0.0) dQNew = 0.0;
                if (dPNew < 0.0) dPNew = 0.0;
                /* ---                        update stresses --- */
                if (dQVal == 0.0) {
                    Double dAdd = dPNew - dPVal;
                    s->stnS_.rs11() -= dAdd;
                    s->stnS_.rs22() -= dAdd;
                    s->stnS_.rs33() -= dAdd;
                } else {
                    Double dVal = dQNew / dQVal;
                    s->stnS_.rs11() = dDS11 * dVal - dPNew;
                    s->stnS_.rs22() = dDS22 * dVal - dPNew;
                    s->stnS_.rs33() = dDS33 * dVal - dPNew;
                    s->stnS_.rs12() = dDS12 * dVal;
                    s->stnS_.rs23() = dDS23 * dVal;
                    s->stnS_.rs13() = dDS13 * dVal;
                }
                dEvdPVal = dAlam * dSc;
            }
        }

        /* update zone stresses and strains --- */
        Double dVol = s->getSubZoneVolume();
        s->working_[Pav]    += dPNew * dVol;
        s->working_[Qav]    += dQNew * dVol;
        s->working_[Evav]   += dEVVal * dVol;
        s->working_[EvdPav] += dEvdPVal * dVol;

        /* --- the last zone has been processed, update parameters: --- */
        if (s->sub_zone_==s->total_sub_zones_-1) {
            dVol = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dVol *= 0.5;
            s->working_[Pav]    *= dVol;
            s->working_[Qav]    *= dVol;
            s->working_[Evav]   *= dVol;
            s->working_[EvdPav] *= dVol;
            MP_    = s->working_[Pav];
            MQ_    = s->working_[Qav];
            EV_   += s->working_[Evav];
            EV_P_ += s->working_[EvdPav];
            /* ---                  specific dVolume --- */
            Double dVold  = MV_;
            MV_    = dVold * (1.0 + s->working_[Evav]);
            if (MV_ < 0.0) throw std::runtime_error("Cam-clay:  Specific volume is negative");
            /* ---                  bulk modulus --- */
            bulk_  = MV_ * std::max(MP_,p_small) / Kappa_;
            if (bulk_ > bulkB_) throw std::runtime_error("Cam-clay:  Bulk modulus bound must be increased for stability");
            /* ---                  cap pressure --- */
            if (s->iworking_[Pind] == 1) {
                if (MP_ > MPC_ * 0.4) {
                    Double dVal = MV_ + Kappa_ * log(MP_ / MP1_);
                    MPC_ = MP1_ * exp((MV_L_ - dVal)/(Lambda_-Kappa_));
                } else {
                    MPC_ = MPC_ * (1.0+s->working_[EvdPav]*dVold/(Lambda_-Kappa_));
                }
                if (MPC_ <=  p_small) {
                    MPC_  =  p_small;
                    bulk_ = MV_ * p_small / Kappa_; // lower bound
                }
            }
            /* ---                  shear modulus --- */
            if (poisson_ != 0.0) {
                shear_ = 1.5*bulk_*(1.0-2.0*poisson_)/(1.0+poisson_);
            } else {
                Double dMaxg = 1.5 * bulk_;
                Double dMing = 0.0;
                shear_ = shear_ < dMaxg ? shear_ : dMaxg;
                shear_ = shear_ > dMing ? shear_ : dMing;
            }
        }

        if (s->state_ &  volume_now)
            s->viscous_ = false; // inhibit viscous strains
    }
}
// EOF