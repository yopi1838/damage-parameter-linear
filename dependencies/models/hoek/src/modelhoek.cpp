#include "modelhoek.h"
#include "base/src/version.h"

namespace models {

    ModelHoek::ModelHoek(): ModelMohr(), scale_(1.0),
        hb_mmi_(0.0), hb_mmc_(0.0),hb_ssi_(0.0), hb_ssc_(0.0),
        hb_aai_(0.0), hb_aac_(0.0),hb_sci_(0.0), hb_scc_(0.0),
        hb_cd_(0.0),hoek_psi_(0.0),
        hb_len(0.0),hb_sop(0),
        iMTab_(0),iSTab_(0),iATab_(0),iCTab_(0),iMULTab_(0),iTTab_(0),
        hb_evolution_(0.0), bEplas_s_(0) {
    }

    UInt ModelHoek::getMinorVersion() const {
        return version::update;
    }

    String ModelHoek::getProperties(void) const {
        return ModelMohr::getProperties() +
            L",constant-mb,constant-s,constant-a,constant-sci"
            L",table-mb,table-s,table-a,table-sci,table-multiplier,table-tension"
            L",flag-dilation,flag-evolution,flag-fos"
            L",constant-dilation,length-calibration"
            L",strain-plastic,current-mb,current-s,current-a,current-sci";
    }

    Variant ModelHoek::getProperty(UInt index) const {
        if (index <= 9)
            return ModelMohr::getProperty(index);
        else {
            switch (index) {
            case 10: return(hb_mmi_);
            case 11: return(hb_ssi_);
            case 12: return(hb_aai_);
            case 13: return(hb_sci_);
            case 14: return(mTab_);
            case 15: return(sTab_); 
            case 16: return(aTab_);
            case 17: return(cTab_);
            case 18: return(mULTab_);
            case 19: return(tTab_);
            case 20: return(hb_cd_);
            case 21: return(bEplas_s_);
            case 22: return(hb_sop);
            case 23: return(hoek_psi_);
            case 24: return(hb_len);
            case 25: return(hb_evolution_);
            case 26: return(hb_mmc_);
            case 27: return(hb_ssc_);
            case 28: return(hb_aac_);
            case 29: return(hb_scc_);
            }
            return(0.0);
        }
    }

    void ModelHoek::setProperty(UInt index, const Variant &p, UInt restoreVersion) 
    {
        if (index <= 9)
            ModelMohr::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 10: hb_mmi_       = p.toDouble(); break;
            case 11: hb_ssi_       = p.toDouble(); break;
            case 12: hb_aai_       = p.toDouble(); break;
            case 13: hb_sci_       = p.toDouble(); break;
            case 14: mTab_         = p.toString(); break;
            case 15: sTab_         = p.toString(); break;
            case 16: aTab_         = p.toString(); break;
            case 17: cTab_         = p.toString(); break;
            case 18: mULTab_       = p.toString(); break;
            case 19: tTab_         = p.toString(); break;
            case 20: hb_cd_        = p.toDouble(); break;
            case 21: bEplas_s_     = p.toInt();    break;
            case 22: hb_sop        = p.toInt();    break;
            case 23: hoek_psi_     = p.toDouble(); break;
            case 24: hb_len        = p.toDouble(); break;
            case 25: hb_evolution_ = p.toDouble(); break;
            case 26: hb_mmc_       = p.toDouble(); break;
            case 27: hb_ssc_       = p.toDouble(); break;
            case 28: hb_aac_       = p.toDouble(); break;
            case 29: hb_scc_       = p.toDouble(); break;
            }
        }
    }

    bool ModelHoek::isPropertyReadOnly(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyReadOnly(i);
        else if (i >= 26 && i <= 29)
            return true;
        return false;
    }

    bool ModelHoek::isPropertyAdvanced(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyAdvanced(i);
        else if (i >= 14 && i<= 25) 
            return true;
        return false;
    }

    void ModelHoek::copy(const ConstitutiveModel *m) {
        const ModelHoek *mm = dynamic_cast<const ModelHoek *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        //
        hb_mmi_       = mm->hb_mmi_;
        hb_ssi_       = mm->hb_ssi_;
        hb_aai_       = mm->hb_aai_;
        hb_sci_       = mm->hb_sci_;
        //
        mTab_         = mm->mTab_;
        sTab_         = mm->aTab_;
        aTab_         = mm->aTab_;
        cTab_         = mm->cTab_;
        mULTab_       = mm->mULTab_;
        tTab_         = mm->tTab_;
        //
        hb_cd_        = mm->hb_cd_;
        bEplas_s_     = mm->bEplas_s_;
        hb_sop        = mm->hb_sop;
        hoek_psi_     = mm->hoek_psi_;
        hb_len        = mm->hb_len;
        //
        hb_evolution_ = mm->hb_evolution_;
        hb_mmc_       = mm->hb_mmc_;
        hb_ssc_       = mm->hb_ssc_;
        hb_aac_       = mm->hb_aac_;
        hb_scc_       = mm->hb_scc_;        
    }

    void ModelHoek::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        ModelElastic::updateParameters();

        iMTab_ = iSTab_ = iATab_ = iCTab_ = iMULTab_ = iTTab_ = nullptr;

        if (mTab_.length())   iMTab_   = s->getTableIndexFromID(mTab_);
        if (sTab_.length())   iSTab_   = s->getTableIndexFromID(sTab_);
        if (aTab_.length())   iATab_   = s->getTableIndexFromID(aTab_);
        if (cTab_.length())   iCTab_   = s->getTableIndexFromID(cTab_);
        if (mULTab_.length()) iMULTab_ = s->getTableIndexFromID(mULTab_);
        if (tTab_.length())   iTTab_   = s->getTableIndexFromID(tTab_);
        if (iTTab_ && brittle_)
            throw std::runtime_error("Internal error: flag-brittle not allowed on when assigning table-tension.");

        Double dTmp = hb_evolution_;
        if (hb_len) dTmp *= hb_len / pow(s->getZoneVolume(), 1.0 / d);
        if (iMTab_)   hb_mmi_ = s->getYFromX(iMTab_, dTmp);
        if (iSTab_)   hb_ssi_ = s->getYFromX(iSTab_, dTmp);
        if (iATab_)   hb_aai_ = s->getYFromX(iATab_, dTmp);
        if (iCTab_)   hb_sci_ = s->getYFromX(iCTab_, dTmp);
        if (iTTab_)   tension_ = s->getYFromX(iTTab_, dTmp);

        if (hb_sci_ == 0.0) 
            throw std::runtime_error("Hoek-Brown property constant-sci cannot be zero.");
        if (hb_mmi_ == 0.0) 
            throw std::runtime_error("Hoek-Brown property constant-mb cannot be zero.");

        hb_mmc_ = hb_mmi_;
        hb_ssc_ = hb_ssi_;
        hb_aac_ = hb_aai_;
        hb_scc_ = hb_sci_;
    }

    static const UInt uiEvolution = 0;
    static const UInt uiS3        = 1;
    //
    void ModelHoek::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            ModelMohr::moduliReduction(s->modulus_reduction_factor_);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;
        if (!s->sub_zone_) {
            s->working_[uiEvolution] = 0.0;
            s->working_[uiS3]        = 0.0;
        }
        Double dSubZoneVolume = s->getSubZoneVolume();

        ModelElastic::elasticTrial(s);
        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);
        Double s3 = -prin.z();
        if (s3 < 0.0) s3 = 0.0;
        s->working_[uiS3] += s3 * dSubZoneVolume;

        if (scale_ && hb_sop) hb_scc_ = hb_sci_ * scale_;

        // friction
        Double h_lamc   = 1.0 + hb_aac_ * hb_mmc_ *pow(hb_mmc_*s3/hb_scc_ + hb_ssc_, hb_aac_-1);
        Double temp = (h_lamc-1.0)/(h_lamc+1.0);
        if (temp >  1.0) temp =  1.0;
        if (temp < -1.0) temp = -1.0;
        friction_ = asin(temp)/degrad;
        if (scale_ && !hb_sop) friction_ = atan(tan(friction_*degrad)*scale_)/degrad;
        // dilation
        Double psi = hoek_psi_;
        if(hb_cd_) {
            if (hb_cd_ < 0) psi = friction_;
            else            psi = hb_cd_ * friction_;
        }
        dilation_ = (psi <= friction_ ? psi : friction_);
        // cohesion
        Double temp1 = (1-hb_aac_) / hb_aac_;
        Double temp2 = 1.0 / hb_aac_;
        Double ucs   = s3 * (1-h_lamc) + pow(hb_mmc_*s3*pow(hb_scc_,temp1) + hb_ssc_*pow(hb_scc_,temp2) ,hb_aac_);
        cohesion_    = 0.5*ucs/sqrt(h_lamc);
        if (scale_ && !hb_sop) cohesion_ *= scale_;
        // tension
        temp = hb_scc_ * 1e20;
        if (hb_mmc_) temp = hb_ssc_ * hb_scc_/ hb_mmc_;
        tension_ = tension_ < temp ? tension_ : temp;

        Double sf1_,sf3_;
        ModelMohr::updateParameters(false,&sf1_,&sf3_);

        Double fs  = - prin.x() + nph_*prin.z() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        Double fty = prin.y() - tension_;
        Double ftx = prin.x() - tension_;
        if (fsd > 0.0 && fsd >= ftz)
            ModelMohr::shearCorrection(s,&prin,&iPlas,fs);
        else if (ftz > 0.0 && ftz >= fsd)
            ModelMohr::tensionCorrection(s,&prin,&iPlas,ftz,brittle_);
        ModelMohr::apexCorrection(friction_,s,&prin,&iPlas,brittle_);

        if (iPlas) {
            s->stnS_ = info.resolve(prin);
            s->viscous_ = false;
            if (iPlas == 1) {
                Double dDe3p = fs * sf3_;
                if (bEplas_s_) {
                    Double dDe1p = fs * sf1_;               
                    Double dDepa = d1d3 * (dDe1p + dDe3p);
                    dDe1p -= dDepa;
                    dDe3p -= dDepa;               
                    s->working_[uiEvolution] += sqrt(0.5*(dDe1p*dDe1p+dDepa*dDepa+dDe3p*dDe3p)) * dSubZoneVolume;
                }
                else
                    s->working_[uiEvolution] += abs(dDe3p) * dSubZoneVolume;
            }
            if (iPlas == 2 && !bEplas_s_) {
                Double dAux = ftz / e1_;
                s->working_[uiEvolution]  += dAux * dSubZoneVolume;
            }
            if (iPlas == 3 && !bEplas_s_) {
                Double dAux = (ftz+fty) / (e1_+e2_);
                s->working_[uiEvolution]  += dAux * dSubZoneVolume;
            }
            if (iPlas == 4  && !bEplas_s_) {
                Double dAux = (ftz+fty+ftx) / bulk_;
                s->working_[uiEvolution]  += dAux * dSubZoneVolume;
            }
        }

        if (s->sub_zone_ == s->total_sub_zones_-1) {
            Double dAux = 1.0 / s->getZoneVolume();
            if (s->overlay_ == 2) dAux *= 0.5;

            Double s3n     = s->working_[uiS3]*dAux;
            hb_evolution_ += s->working_[uiEvolution] * dAux;

            Double mult = 1.0;
            if (iMULTab_) mult = s->getYFromX(iMULTab_,fabs(s3n));
            Double dTmp = mult * hb_evolution_;
            if (hb_len) dTmp *= hb_len / pow(s->getZoneVolume(),1.0/d);

            if (iMTab_)   hb_mmc_ = s->getYFromX(iMTab_,dTmp);
            if (iSTab_)   hb_ssc_ = s->getYFromX(iSTab_,dTmp);
            if (iATab_)   hb_aac_ = s->getYFromX(iATab_,dTmp);
            if (iCTab_)   hb_scc_ = s->getYFromX(iCTab_,dTmp);
            if (iTTab_)   tension_= s->getYFromX(iTTab_,dTmp);
        }
    }

    void ModelHoek::scaleProperties(const Double &scale,const std::vector<UInt> &props) {
        scale_ = scale;
        for (UInt u=0;u<props.size();++u) {
            switch (props[u]) {
            case 13: hb_sop = 1;  break;
            case 29: hb_sop = 1;  break;
            default: hb_sop = 0;  break;
            }
        }
        setValid(0);
    }
}// namespace models
// EOF