#include "modelwipp.h"
#include "base/src/version.h"

namespace models {

    static const UInt   iItMax  = 4;
    static const Double dStrTol = 1.0e-8;
    static const Double dSq3d2  = 1.224744871391589049; // sqrt(1.5)

    ModelWipp::ModelWipp(): actE_(0.0),aWipp_(0.0), bWipp_(0.0), dWipp_(0.0), eDot_(0.0), 
        gasC_(0.0),nWipp_(0.0), dTemp_(0.0), ePrim_(0.0), eRate_(0.0) { 
    }

    UInt ModelWipp::getMinorVersion() const {
        return version::update;
    }

    String ModelWipp::getProperties(void) const {
        return ModelElastic::getProperties() + 
            L",activation-energy,constant-a,constant-b,constant-d,creep-rate-critical"
            L",constant-gas,exponent,temperature,creep-strain-primary,creep-rate-primary";
    }

    Variant ModelWipp::getProperty(UInt index) const {
        if (index <= 4)
            return ModelElastic::getProperty(index);
        else {
            switch (index) { 
            case 5:  return(actE_);
            case 6:  return(aWipp_);
            case 7:  return(bWipp_);
            case 8:  return(dWipp_);
            case 9:  return(eDot_);
            case 10: return(gasC_);
            case 11: return(nWipp_);
            case 12: return(dTemp_);
            case 13: return(ePrim_);
            case 14: return(eRate_);
            }
        }
        return(0.0);
    }


    void ModelWipp::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 4)
            ModelElastic::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 5:  actE_ = p.toDouble();   break;
            case 6:  aWipp_ = p.toDouble();  break;
            case 7:  bWipp_ = p.toDouble();  break;
            case 8:  dWipp_ = p.toDouble();  break;
            case 9:  eDot_ = p.toDouble();   break;
            case 10: gasC_ = p.toDouble();   break;
            case 11: nWipp_ = p.toDouble();  break;
            case 12: dTemp_ = p.toDouble();   break;
            case 13: ePrim_ = p.toDouble();  break;
            case 14: eRate_ = p.toDouble();  break;
            }
        }
    }

    bool ModelWipp::isPropertyReadOnly(UInt i) const
    {
        switch (i) {
        case 13:
        case 14: return true;
        default: return false;
        }
    }

    void ModelWipp::copy(const ConstitutiveModel *m) {
        const ModelWipp *wm = dynamic_cast<const ModelWipp *>(m);
        if (!wm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelElastic::copy(m);
        actE_ = wm->actE_;
        aWipp_ = wm->aWipp_;
        bWipp_ = wm->bWipp_;
        dWipp_ = wm->dWipp_;
        eDot_ = wm->eDot_;
        gasC_ = wm->gasC_;
        nWipp_ = wm->nWipp_;
        dTemp_ = wm->dTemp_;
        ePrim_ = wm->ePrim_;
        eRate_ = wm->eRate_;
    }

    void ModelWipp::initialize(UByte d, State *s) {
        ConstitutiveModel::initialize(d,s);
        ModelElastic::updateParameters();
    }

    static const UInt EPrime = 0;
    static const UInt EPrime_sum = 1;
    //
    void ModelWipp::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);
        if ((d!=3)&&(d!=2)) throw std::runtime_error("Illegal dimension model wipp.");

        if (!s->sub_zone_) {
            s->working_[EPrime] = ePrim_;
            eRate_ = 0.0;
            s->working_[EPrime_sum] = 0.0;
        }

        Double dTempUse = dTemp_;
        if (s->isThermal()) {
            dTempUse = s->getTemperature();
            dTemp_    = dTempUse;
        }

        Double dExc = 0.0;
        if (dTempUse) 
            dExc = exp( -actE_ / (gasC_ * dTempUse) );
        else
            throw std::runtime_error("Tempreture not assigned in model wipp.");

        Double dVdt = s->isCreepInUse() ? s->getTimeStep() : 0.0;
        Double dsubv = s->getSubZoneVolume();

        /* --- volumetric and deviatoric strain increments --- */
        Double dDevol  = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33();
        Double dDevol3 = d1d3 * dDevol;
        Double dE11d  = s->stnE_.s11() - dDevol3;
        Double dE22d  = s->stnE_.s22() - dDevol3;
        Double dE33d  = s->stnE_.s33() - dDevol3;
        /* --- old isotropic and deviatoric stresses --- */
        Double dS0    = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        Double dS11d  = s->stnS_.s11() - dS0;
        Double dS22d  = s->stnS_.s22() - dS0;
        Double dS33d  = s->stnS_.s33() - dS0;
        Double dS11n  = 0.0;
        Double dS22n  = 0.0;
        Double dS33n  = 0.0;
        Double dS23n  = 0.0;
        Double dS12n  = 0.0;
        Double dS13n  = 0.0;
        Double dEpn   = 0.0;
        /* --- iteration to find new deviatoric stresses --- */
        for (UInt it=1;it<=iItMax;it++) {
            /* --- mean stress --- */
            Double dS11m,dS22m,dS33m,dS12m,dS13m,dS23m,dEpm;
            if (it==1) {
                dS11m  = dS11d;
                dS22m  = dS22d;
                dS33m  = dS33d;
                dS23m  = s->stnS_.s23();
                dS12m  = s->stnS_.s12();
                dS13m  = s->stnS_.s13();
                dEpm   = s->working_[EPrime];
            } else {
                dS11m  = 0.5 * (dS11n + dS11d);
                dS22m  = 0.5 * (dS22n + dS22d);
                dS33m  = 0.5 * (dS33n + dS33d);
                dS23m  = 0.5 * (dS23n + s->stnS_.s23() );
                dS12m  = 0.5 * (dS12n + s->stnS_.s12() );
                dS13m  = 0.5 * (dS13n + s->stnS_.s13() );
                dEpm   = 0.5 * (s->working_[EPrime] + dEpn);
            }
            Double dE11v = 0.0;
            Double dE22v = 0.0;
            Double dE33v = 0.0;
            Double dE12v = 0.0;
            Double dE23v = 0.0;
            Double dE13v = 0.0;
            /* --- stress magnitude --- */
            Double dEdp = 0.0;
            if (s->isCreepInUse()) {
                Double dSMag2 = dS11m*dS11m + dS22m*dS22m + dS33m*dS33m
                    + 2.0 * (dS12m*dS12m + dS23m*dS23m + dS13m*dS13m);
                if (fabs(dSMag2) > dStrTol) {
                    Double dSBar = dSq3d2 * sqrt (dSMag2);
                    Double dEds;
                    /* --- (secondary creep rate) --- */
                    dEds = pow (dSBar,nWipp_) * dWipp_;
                    dEds = dEds * dExc;
                    if (dEds >= eDot_) {
                        dEdp = (aWipp_ - bWipp_*dEpm) * dEds;
                    } else {
                        dEdp = aWipp_ * dEds - bWipp_ * eDot_ * dEpm;
                    }
                    Double dEdc = 1.5 * dVdt * (dEds + dEdp) / dSBar;
                    /* --- creep strain increments --- */
                    dE11v = dEdc * dS11m;
                    dE22v = dEdc * dS22m;
                    dE33v = dEdc * dS33m;
                    dE23v = dEdc * dS23m;
                    dE12v = dEdc * dS12m;
                    dE13v = dEdc * dS13m;
                } else dEdp = 0.0;
            } else dEdp = 0.0;
            /* --- new deviator stresses --- */
            dS11n  = dS11d       + g2_ * (dE11d       - dE11v);
            dS22n  = dS22d       + g2_ * (dE22d       - dE22v);
            dS33n  = dS33d       + g2_ * (dE33d       - dE33v);
            dS23n  = s->stnS_.s23()  + g2_ * (s->stnE_.s23()  - dE23v);
            dS12n  = s->stnS_.s12()  + g2_ * (s->stnE_.s12()  - dE12v);
            dS13n  = s->stnS_.s13()  + g2_ * (s->stnE_.s13()  - dE13v);
            /* --- new primary strain --- */
            eRate_ += dEdp * dsubv;
            dEpn   = s->working_[EPrime] + dEdp * dVdt;

            if (!s->isCreepInUse()) break; //no more iteration needed if creep is off
        }
        /* --- isotropic stress --- */
        dS0  += bulk_ * dDevol;
        /* --- convert back to x-y components --- */
        s->stnS_.rs11() = dS11n + dS0;
        s->stnS_.rs22() = dS22n + dS0;
        s->stnS_.rs33() = dS33n + dS0;
        s->stnS_.rs23() = dS23n;
        s->stnS_.rs12() = dS12n;
        s->stnS_.rs13() = dS13n;
        /* --- store primary strain locally  --- */
        s->working_[EPrime_sum] += dEpn * dsubv;
        /* --- average primary strain, and store globally --- */
        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dv = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dv *= 0.5;
            ePrim_  = s->working_[EPrime_sum] * dv;
            eRate_ *= dv;
        }
        s->viscous_ = true;  // allow viscous strains
    }
}// namespace models
// EOF