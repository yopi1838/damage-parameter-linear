#include "modelchsoil.h"
#include "base/src/version.h"

namespace models {

    ModelCHSoil::ModelCHSoil() : ModelMohr(), 
        bulkr_(0.0),shearr_(0.0),youngr_(0.0),fricv_(0.0),
        pref_(0.0),pini_(0.0),m_(0.5),n_(0.5),rf_(0.9),
        fricf_(0.0),fric0_(0.0),dilaf_(0.0),
        iCohesion_(nullptr), iDilation_(nullptr), iTension_(nullptr),
        dilaw_(1),sHP_(0.0),tHP_(0.0),
        shearp_(0.0),sinr_(0.0),sincv_(0.0),sinr0_(0.0),sf1_(0.0),sf3_(0.0) {
    }

    String ModelCHSoil::getProperties(void) const {
        return  ModelMohr::getProperties() + 
            L",bulk-reference,shear-reference,young-reference"
            L",pressure-reference,pressure-initial"
            L",exponent-bulk,exponent-shear,failure-ratio"
            L",friction-mobilized,friction-0,dilation-mobilized"
            L",table-cohesion,table-dilation,table-tension,flag-dilation"
            L",strain-shear-plastic,strain-tensile-plastic"
            L",friction-critical";
    }

    UInt ModelCHSoil::getMinorVersion() const {
        return version::update;
    }

    Double ModelCHSoil::getConfinedModulus() const {
        Double bulk,shear;
        bulk = bulk_;
        shear = shear_;
        if (!bulk || !shear) {
            bulk  = bulkr_  * pref_ * std::pow (pini_/pref_,m_);
            shear = shearr_ * pref_ * std::pow (pini_/pref_,n_);
            Double dval = shear*d2d3;
            bulk  = bulk < dval ? dval : bulk;  // nu > 0
            dval   = 49.66*shear;
            bulk  = bulk > dval ? dval : bulk;  // nu < 0.49
        }
        return (bulk + shear*d4d3);
    }

    Variant ModelCHSoil::getProperty(UInt index) const {
        switch (index) {
        case  1: return bulk_;
        case  2: return shear_;
        case  3: 
            {
                Double young;
                getYPfromBS(bulk_,shear_,&young,0);
                return young;
            }
        case  4: 
            { 
                Double poisson;
                getYPfromBS(bulkr_,shearr_,0,&poisson);
                return poisson;
            }
        case  5: return cohesion_;
        case  6: return fricf_; 
        case  7: return dilaf_;
        case  8: return tension_;
        case  9: return brittle_;
        case 10: return(bulkr_);
        case 11: return(shearr_);
        case 12: 
            {
                Double youngr;
                getYPfromBS(bulkr_,shearr_,&youngr,0);
                return(youngr);
            }
        case 13: return(pref_);
        case 14: return(pini_);
        case 15: return(m_);
        case 16: return(n_);
        case 17: return(rf_);
        case 18: return(friction_);
        case 19: return(fric0_);
        case 20: return(dilation_);
        case 21: return(cTable_);
        case 22: return(dTable_);
        case 23: return(tTable_);
        case 24: return(dilaw_);
        case 25: return(sHP_);
        case 26: return(tHP_);
        case 27: return(fricv_);
        }
        return(0.0);
    }

    void ModelCHSoil::setProperty(UInt index,const Variant &p,UInt restoreVersion)  {
        ConstitutiveModel::setProperty(index,p,restoreVersion);
        switch (index) {
        case 1: bulk_ = p.toDouble();  break;
        case 2: shear_ = p.toDouble(); break;
        case 3:
            { 
                Double young,poisson;
                getYPfromBS(bulk_,shear_,&young,&poisson);
                young = p.toDouble();
                if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_);
            }
            break;
        case 4:
            { 
                Double youngr,poisson;
                getYPfromBS(bulkr_,shearr_,&youngr,&poisson);
                poisson = p.toDouble();
                if (!youngr && !restoreVersion) 
                    throw std::runtime_error("Young's Modulus must be non-zero before you can specify Poisson's Ratio.");
                if (!restoreVersion) getBSfromYP(youngr,poisson,&bulkr_,&shearr_);
            } 
            break;
        case  5: cohesion_ = p.toDouble(); break;
        case  6: fricf_    = p.toDouble(); break;
        case  7: dilaf_    = p.toDouble(); break;
        case  8: tension_  = p.toDouble(); break;
        case  9: brittle_  = p.toBool();   break;
        case 10: bulkr_    = p.toDouble(); break;
        case 11: shearr_   = p.toDouble(); break;
        case 12: 
            {
                Double youngr,poisson;
                getYPfromBS(bulkr_,shearr_,&youngr,&poisson);
                youngr_  = p.toDouble();  
                if (!restoreVersion) getBSfromYP(youngr_,poisson,&bulkr_,&shearr_);
            }
            break;
        case 13: pref_   = p.toDouble();  break;
        case 14: pini_   = p.toDouble();  break;
        case 15: m_      = p.toDouble();  break;
        case 16: n_      = p.toDouble();  break;
        case 17: rf_     = p.toDouble();  break;
        case 18: friction_ = p.toDouble();  break;
        case 19: fric0_    = p.toDouble();  break;
        case 20: dilation_ = p.toDouble();  break;
        case 21: cTable_ = p.toString();  break;
        case 22: dTable_ = p.toString();  break;
        case 23: tTable_ = p.toString();  break;
        case 24: dilaw_  = p.toInt();     break;
        case 25: sHP_    = p.toDouble();  break;
        case 26: tHP_    = p.toDouble();  break;
        case 27: fricv_  = p.toDouble();  break;
        }
    }

    bool ModelCHSoil::isPropertyReadOnly(UInt i) const {
        switch (i) {
        case 1:
        case 2:
        case 3:
            return true;
        default: return false;
        }
    }

    bool ModelCHSoil::isPropertyAdvanced(UInt i) const {
        switch (i) {
        case 15:  // m = 0.5
        case 16:  // n = 0.5
        case 17:  // Rf = 0.9
        case 18:  // friction_ (case 11)
        case 19:  // fric0_
        case 21:  // 0
        case 22:  // 0
        case 23:  // 0
        case 24:  // 1
        case 25:  // internally
        case 26:  // internally
        case 27:  // internal, need assignment only if dilaw = 2
            return true;
        default: return false;
        }
    }

    void ModelCHSoil::copy(const ConstitutiveModel *m) {
        const ModelCHSoil *mm = dynamic_cast<const ModelCHSoil *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        bulkr_  = mm->bulkr_;
        shearr_ = mm->shearr_;
        youngr_ = mm->youngr_;
        pref_   = mm->pref_;
        pini_   = mm->pini_;
        m_      = mm->m_;
        n_      = mm->n_;
        rf_     = mm->rf_;
        friction_ = mm->friction_;
        fric0_    = mm->fric0_;;
        dilation_ = mm->dilation_;
        sHP_    = mm->sHP_;
        tHP_    = mm->tHP_;
        fricv_  = mm->fricv_;
        dilaw_  = mm->dilaw_;
    }

    void ModelCHSoil::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);

        if (pref_ < 0.0)  pref_ = -pref_;
        if (pref_ == 0.0) pref_ = 100.0;         // default value is 100
        if (pini_ <= 0.0) pini_ = pref_ * 0.005; //
        if (rf_ <= 0.0) rf_ = 0.1;               // dRef
        if (rf_ > 1.0)  rf_ = 1.0;               // dRef
        m_ = m_ > 1.0 ? 1.0 : m_;
        n_ = n_ > 1.0 ? 1.0 : n_;

        if (!bulk_ && !shear_) {
            bulk_  = bulkr_  * pref_ * std::pow (pini_/pref_,m_);
            shear_ = shearr_ * pref_ * std::pow (pini_/pref_,n_);
            Double dval = d2d3*shear_;
            bulk_  = bulk_ < dval ? dval : bulk_;  // nu > 0
            dval   = 49.66*shear_;
            bulk_  = bulk_ > dval ? dval : bulk_;  // nu < 0.49
        }

        bool bEUpdated = false;
        bEUpdated = ModelElastic::updateParameters();

        iCohesion_ = iDilation_ = iTension_ = nullptr;

        if (cTable_.length()) iCohesion_ = s->getTableIndexFromID(cTable_);
        if (dTable_.length()) iDilation_ = s->getTableIndexFromID(dTable_);
        if (tTable_.length()) iTension_  = s->getTableIndexFromID(tTable_);
        if (iTension_ && brittle_)
            throw std::runtime_error("Internal error: flag-brittle not allowed on when assigning table-tension.");

        if (iTension_)  tension_  = s->getYFromX(iTension_, tHP_);
        if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
        if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);

        friction_ = friction_ < fricf_ ? friction_ : fricf_;
        dilaf_ = dilaf_ > fricf_ ? fricf_ : dilaf_;
        if (!dilaw_ && !iDilation_)    dilation_ = dilaf_;
        dilation_ = dilation_ > friction_ ? friction_ : dilation_;
        sinr_ = std::sin(fricf_ * degrad) / rf_;
        Double dRSin = std::sin(friction_ * degrad);
        if (!sHP_) fric0_ = friction_;  // default is the mobilized friction
        fric0_ = fric0_ > friction_ ? friction_ : fric0_;
        Double dRSin0 = std::sin(fric0_ * degrad);
        sinr0_ = dRSin0 / rf_;
        sHP_  = (pini_/shear_)*(sinr_-sinr0_) * (1.0/(1.0-(dRSin-dRSin0)/(sinr_-sinr0_))-1.0); // gamma_p

        if (dilaw_== 1) {
            Double dRa = std::sin(fricf_ * degrad);
            Double dRb = std::sin(dilaf_ * degrad);
            sincv_ = (dRa - dRb)/(1.0 - dRa * dRb);
        }
        else if (dilaw_ == 2) {
            if (friction_ < fricv_)
                dilation_ = 0.0;
            else {
                dilation_ = std::min(dilaf_,friction_);
                dilation_ = dilaf_ > friction_ ? friction_ : dilaf_;
            }
        }

        ModelMohr::updateParameters(bEUpdated,&sf1_,&sf3_);
    }

    static const UInt Dqs = 0;
    static const UInt Dqt = 1;

    void ModelCHSoil::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;
        if (!s->sub_zone_) {
            s->working_[Dqs] = 0.0;
            s->working_[Dqt] = 0.0;
        }
        Double dSubZoneVolume = s->getSubZoneVolume();

        ModelElastic::elasticTrial(s);
        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        Double fs  = - prin.x() + nph_*prin.z() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        Double fty = prin.y() - tension_;
        Double ftx = prin.x() - tension_;
        if (fsd > 0.0 && fsd >= ftz)
            ModelMohr::shearCorrection(s,&prin,&iPlas,fs);
        else if (ftz > 0.0 && ftz >= fsd)
            ModelMohr::tensionCorrection(s,&prin,&iPlas,ftz,brittle_);
        ModelMohr::apexCorrection(fricf_,s,&prin,&iPlas,brittle_);

        if (iPlas) {
            s->stnS_ = info.resolve(prin);
            s->viscous_ = false;

            if (iPlas == 1) {
                Double dDe1p = fs * sf1_;
                Double dDe3p = fs * sf3_;
                Double dDepa = d1d3 * (dDe1p + dDe3p);
                dDe1p -= dDepa;
                dDe3p -= dDepa;
                s->working_[Dqs]  += sqrt(0.5 * (dDe1p*dDe1p+dDepa*dDepa+dDe3p*dDe3p)) * dSubZoneVolume;
            }
            if (iPlas == 2) {
                Double dAux = ftz / e1_;
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
            if (iPlas == 3) {
                Double dAux = (ftz+fty) / (e1_+e2_);
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
            if (iPlas == 4) {
                Double dAux = (ftz+fty+ftx) / bulk_;
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
        }

        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dAux = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dAux *= 0.5;
            sHP_ += s->working_[Dqs] * dAux;
            tHP_ += s->working_[Dqt] * dAux;

            if (s->working_[Dqs] > 0.0) {
                if (iCohesion_) cohesion_  = s->getYFromX(iCohesion_,sHP_);
                // update friction
                Double temp1 = 1.0 / (1.0 + sHP_ * shear_ / ((sinr_-sinr0_)*pini_) );
                Double dsinphi = (sinr_-sinr0_)*(1.0-temp1) + sinr0_*rf_;
                if (dsinphi >  1.0) dsinphi =  1.0;
                if (dsinphi < -1.0) dsinphi = -1.0;
                shearp_ = temp1*temp1*shear_;
                friction_ = std::asin(dsinphi) / degrad;
                friction_ = friction_ < fricf_ ? friction_ : fricf_;
                if (friction_ < fric0_) shearp_ = 0.0;
                if (dilaw_ == 1) {
                    Double dsinphi2 = std::sin(friction_ * degrad);
                    Double dsinpsi = (dsinphi2 - sincv_)/(1. - dsinphi2 * sincv_);
                    if (dsinpsi >  1.0) dsinpsi =  1.0;
                    if (dsinpsi < -1.0) dsinpsi = -1.0;
                    dilation_ = std::asin(dsinpsi) / degrad;
                    dilation_ = dilation_ > friction_ ? friction_ : dilation_;
                }
                else if (dilaw_ == 2) {
                    if (friction_ < fricv_) {
                        dilation_ = 0.0;
                    }
                    else {
                        dilation_ = std::min(dilaf_,friction_);
                        dilation_ = dilaf_ > friction_ ? friction_ : dilaf_;
                    }
                }
                else {
                    if (iDilation_) { // update dilation
                        dilation_  = s->getYFromX(iDilation_,sHP_);
                        dilation_ = dilation_ > friction_ ? friction_ : dilation_;
                    } else {
                        dilation_ = dilaf_ > friction_ ? friction_ : dilaf_;
                    }
                }

                if (iCohesion_) { // update cohesion
                    dAux  = s->getYFromX(iCohesion_,sHP_);
                }

            }

            if (s->working_[Dqt] > 0.0 && iTension_)
                dAux  = s->getYFromX(iTension_,tHP_);

            ModelMohr::updateParameters(false,&sf1_,&sf3_);
        }
    }
}
// EOF