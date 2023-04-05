#include "modelpwipp.h"
#include "base/src/version.h"

namespace models {

    static const UInt    iItMax  = 4;
    static const Double dStrTol = 1.0e-8;
    static const Double root3 = 1.7320508075688772; // sqrt(3.0);

    ModelPWipp::ModelPWipp() : ModelDrucker(), temp_(0.0), 
        actE_(0.0), aWipp_(0.0), bWipp_(0.0), dWipp_(0.0), eDot_(0.0), 
        dGasC(0.0), nWipp_(0.0), ePrim_(0.0), eRate_(0.0), 
        sHP_(0.0), tHP_(0.0) {
    }

    String ModelPWipp::getProperties(void) const {
        return ModelDrucker::getProperties() +        
            L",activation-energy,constant-a,constant-b,constant-d,creep-rate-critical,constant-gas"
            L",exponent,temperature,creep-strain-primary,creep-rate-primary"
            L",strain-shear-plastic,strain-tension-plastic";
    }

    UInt ModelPWipp::getMinorVersion() const {
        return version::update;
    }

    Variant ModelPWipp::getProperty(UInt index) const {
        if (index <= 8)
            return ModelDrucker::getProperty(index);
        else {
            switch (index) { 
            case  9: return actE_;
            case 10: return aWipp_;
            case 11: return bWipp_;
            case 12: return dWipp_;
            case 13: return eDot_;
            case 14: return dGasC;
            case 15: return nWipp_;
            case 16: return temp_;
            case 17: return ePrim_;
            case 18: return eRate_;
            case 19: return sHP_;
            case 20: return tHP_;
            }
            return(0.0);
        }
    }


    void ModelPWipp::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 8)
            ModelDrucker::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case  9:  actE_  = p.toDouble();   break;
            case 10:  aWipp_ = p.toDouble();  break;
            case 11:  bWipp_ = p.toDouble();  break;
            case 12:  dWipp_ = p.toDouble();  break;
            case 13:  eDot_  = p.toDouble();  break;
            case 14: dGasC  = p.toDouble();  break;
            case 15: nWipp_ = p.toDouble();  break;
            case 16: temp_  = p.toDouble();  break;
            case 17: ePrim_ = p.toDouble();  break;
            case 18: eRate_ = p.toDouble();  break;
            case 19: sHP_   = p.toDouble();  break;
            case 20: tHP_   = p.toDouble();  break;
            }
        }
    }

    bool ModelPWipp::isPropertyReadOnly(UInt i) const {
        switch (i) {
        case 17:
        case 18:
        case 19:
        case 20: return true;
        default: return false;
        }
    }

    void ModelPWipp::copy(const ConstitutiveModel *m) {
        const ModelPWipp *pm = dynamic_cast<const ModelPWipp *>(m);
        if (!pm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelDrucker::copy(m);
        actE_ = pm->actE_;
        aWipp_ = pm->aWipp_;
        bWipp_ = pm->bWipp_;
        dWipp_ = pm->dWipp_;
        eDot_ = pm->eDot_;
        dGasC = pm->dGasC;
        nWipp_ = pm->nWipp_;
        temp_ = pm->temp_;
        ePrim_ = pm->ePrim_;
        eRate_ = pm->eRate_;
        sHP_ = pm->sHP_;
        tHP_ = pm->tHP_;
    }

    void ModelPWipp::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        if (shear_ <= 0.0 || bulk_ <= 0.0)
            throw std::runtime_error("WIPP-Drucker Model - Invalid modulus.");
    }

    static const UInt EPrime_sum = 0;
    static const UInt SHPSum     = 1;
    static const UInt THPSum     = 2;
    void ModelPWipp::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);
        if ((d!=3)&&(d!=2)) throw std::runtime_error("Illegal dimension in model pwipp.");

        Double dVdt = s->isCreepInUse() ? s->getTimeStep() : 0.0; //viscous
        Double dPdt = s->isCreepInUse() ? s->getTimeStep() : 1.0; //plastic
        Double dsubv = s->getSubZoneVolume();

        Double dTempUse = temp_;
        if (s->isThermal()) {
            dTempUse = s->getTemperature();
            temp_    = dTempUse;
        }
        Double dExc = 0.0;
        if (dTempUse)
            dExc = exp( -actE_ / (dGasC * dTempUse) );
        else
            throw std::runtime_error("Tempareture not assigned in model pwipp.");

        ModelDrucker::updateParameters();
        Double dPlas1 = gKQ_ * dPdt;

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        //UInt iPlas = 0;
        s->viscous_ = true;

        if (!s->sub_zone_) {
            s->working_[EPrime_sum] = 0.0;
            s->working_[SHPSum]     = 0.0;
            s->working_[THPSum]     = 0.0;
            eRate_      = 0.0;
        }

        SymTensor tEp(0.0),tSn(0.0);
        SymTensor tSd = s->stnS_.getDeviatoric();
        SymTensor tEd = s->stnE_.getDeviatoric();
        Double dDevol = s->stnE_.getTrace();
        Double dS0    = s->stnS_.getTrace()/3.0;
        Double dS00   = dS0;
        Double dEpn(0.0),dAlam(0.0);
        bool bSYield(false),bTYield(false);
        for (UInt it=1; it<=iItMax; it++) {           
            SymTensor tSm(0.0),tEv(0.0);;
            Double dEpm(0.0),dEdp(0.0);
            if (it == 1) {
                tSm   = tSd;
                dEpm  = ePrim_;
                dAlam = 0.0;
            } else {
                tSm  = tSd*0.5;
                tSm += tSn*0.5;
                dEpm = (ePrim_ + dEpn)*0.5;
            }
            /* --- stress magnitude --- */
            Double dSmag2 = tSm.getJ2();
            Double dSmag = std::sqrt(dSmag2);
            if (s->isCreepInUse()) {
                if (fabs(dSmag2) > dStrTol) {
                    Double dSBar = root3 * dSmag;
                    /* --- (secondary creep rate) --- */
                    Double dEds = std::pow (dSBar,nWipp_) * dWipp_;
                    dEds *= dExc;
                    /* --- (primary creep rate) --- */
                    if (dEds >= eDot_)
                        dEdp = (aWipp_ - bWipp_*dEpm) * dEds;
                    else
                        dEdp = aWipp_ * dEds - bWipp_ * eDot_ * dEpm;
                    if (dEdp < 0.0) dEdp = 0.0;
                    Double dEdc = 1.5 * dVdt * (dEds + dEdp) / dSBar;
                    /* --- creep strain increments --- */
                    tEv = tSm*dEdc;
                }
            }
            Double dTau  = dSmag;
            Double dCccc = 0.0;
            if (dTau > dStrTol) dCccc = 0.5 * dPdt * dAlam / dTau;
            tEp = tSm*dCccc;
            // --- new deviator stresses/
            tSn  = tSd;
            tSn += (tEd - tEv - tEp)*g2_;
            // --- new isotropic stress ---
            // default, overwritten if fails in tension
            dS0  = dS00 + (dDevol - dAlam*qPsi_*dPdt)*bulk_;

            if (canFail()) {
                tau_ = sqrt(tSn.getJ2());
                //if (bTYield) dS0  = tension_;
                Double fs  = tau_ + qPhi_*dS0 - kPhi_;
                Double fsd = fs / rc_;
                Double ft  = dS0 - tension_;
                if ((ft > 0.0 && ft >= fsd) && !bTYield) {
                    dS0  = tension_;
                    bTYield = true;
                    s->state_ |= tension_now;
                    dAlam = 0.0;
                    s->working_[THPSum] += ft / bulk_;
                }
                else {
                    if ((fs > 0.0 && fsd >= ft) || bSYield) {
                        bSYield = true;
                        s->state_ |= shear_now;
                        if (dPlas1) dAlam  += fs / dPlas1;
                    } 
                    else
                        dAlam = 0.0;
                }
            }
            /* --- new primary strain --- */
            eRate_ += dEdp*dsubv;
            dEpn    = ePrim_ + dEdp*dVdt;
            if (bTYield) break;
            if (!s->isCreepInUse() && it==2) break; // no more iteration needed if creep is off
        } // End of iteration loop

        /* --- convert back to x-y components --- */
        s->stnS_.rs11() = tSn.s11() + dS0;
        s->stnS_.rs22() = tSn.s22() + dS0;
        s->stnS_.rs33() = tSn.s33() + dS0;
        s->stnS_.rs12() = tSn.s12();
        s->stnS_.rs13() = tSn.s13();
        s->stnS_.rs23() = tSn.s23();
        /* --- store primary strain locally  --- */
        s->working_[EPrime_sum] += dEpn * dsubv;
        // --- accumulate plastic strain increments ---
        // if model cannot fail then dAlam is constantly equal to 0.0  
        if (dAlam != 0.0) 
            s->working_[SHPSum] += std::sqrt(tEp.getJ2())* dsubv;

        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dv = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dv *= 0.5;
            ePrim_  = s->working_[EPrime_sum] * dv;
            eRate_ *= dv;
            sHP_   += s->working_[SHPSum] *dv;
            tHP_   += s->working_[THPSum] *dv;
        }

        if( (s->state_ & shear_now) || (s->state_ & tension_now) )
            s->viscous_ = false; // inhibit viscous strains
    } 
}
// EOF