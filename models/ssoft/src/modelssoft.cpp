#include "modelssoft.h"
#include "base/src/version.h"

namespace models {

    ModelStrainSoftening::ModelStrainSoftening() : ModelMohr() {
    }

    String ModelStrainSoftening::getProperties(void) const {
        return ModelMohr::getProperties() + 
            L",table-cohesion,table-friction,table-dilation,table-tension"
            L",strain-shear-plastic,strain-tension-plastic";
    }

    UInt ModelStrainSoftening::getMinorVersion() const {
        return version::update;
    }

    Variant ModelStrainSoftening::getProperty(UInt index) const {
        if (index <= 9)
            return ModelMohr::getProperty(index);
        else {
            switch (index) {
            case 10: return cTable_;
            case 11: return fTable_;
            case 12: return dTable_;
            case 13: return tTable_;
            case 14: return sHP_;
            case 15: return tHP_;
            }
            return(0.0);
        }
    }

    void ModelStrainSoftening::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 9)
            ModelMohr::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 10: cTable_ = p.toString();  break;
            case 11: fTable_ = p.toString();  break;
            case 12: dTable_ = p.toString();  break;
            case 13: tTable_ = p.toString();  break;
            case 14: sHP_    = p.toDouble();  break;
            case 15: tHP_    = p.toDouble();  break;
            }
        }
    }

    bool ModelStrainSoftening::isPropertyReadOnly(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyReadOnly(i);
        else if (i == 14 || i == 15)
            return true;
        return false;
    }

    bool ModelStrainSoftening::isPropertyAdvanced(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyAdvanced(i);
        else if (i <= 13) 
            return true;
        return false;
    }

    void ModelStrainSoftening::copy(const ConstitutiveModel *m) {
        const ModelStrainSoftening *mm = dynamic_cast<const ModelStrainSoftening *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        cTable_ = mm->cTable_;
        fTable_ = mm->fTable_;
        dTable_ = mm->dTable_;
        tTable_ = mm->tTable_;
        sHP_    = mm->sHP_;
        tHP_    = mm->tHP_;
    }

    void ModelStrainSoftening::initialize(UByte d, State *s) {
        ConstitutiveModel::initialize(d,s);

        iCohesion_ = iFriction_ = iDilation_ = iTension_ = nullptr;

        if (cTable_.length()) iCohesion_ = s->getTableIndexFromID(cTable_);
        if (fTable_.length()) iFriction_ = s->getTableIndexFromID(fTable_);
        if (dTable_.length()) iDilation_ = s->getTableIndexFromID(dTable_);
        if (tTable_.length()) iTension_  = s->getTableIndexFromID(tTable_);
        if(iTension_ && brittle_) 
            throw std::runtime_error("Internal error: flag-brittle not allowed on when assigning table-tension.");

        if (iTension_)  tension_  = s->getYFromX(iTension_, tHP_);
        if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
        if (iFriction_) friction_ = s->getYFromX(iFriction_,sHP_);
        if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);

        if (friction_) {
            Double dApex = cohesion_ / tan(friction_ * degrad);
            tension_ = tension_ < dApex ? tension_ : dApex;
        }
        else if (cohesion_ == 0.0)
            tension_ = 0.0;
    }

    static const UInt Dqs = 0;
    static const UInt Dqt = 1;
    //
    void ModelStrainSoftening::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        bool noUpdateE = false;
        if (s->modulus_reduction_factor_ > 0.0) {
            ModelElastic::moduliReduction(s->modulus_reduction_factor_);
            noUpdateE = true;
        }
        ModelMohr::updateParameters(noUpdateE,&sf1_,&sf3_);

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

        Double fs  = - prin.x() + nph_ * prin.z() - csn_;
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
            s->viscous_ = false;
            s->stnS_ = info.resolve(prin);
            //
            if (iPlas==1) {
                Double dDe1p = fs * sf1_;
                Double dDe3p = fs * sf3_;
                Double dDepa = d1d3 * (dDe1p + dDe3p);
                dDe1p -= dDepa;
                dDe3p -= dDepa;
                s->working_[Dqs]  += sqrt(0.5 * (dDe1p*dDe1p+dDepa*dDepa+dDe3p*dDe3p)) * dSubZoneVolume;
            }
            else if (iPlas==2) {
                Double dAux = ftz / e1_;
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
            else if (iPlas==3) {
                Double dAux = (ftz+fty) / (e1_+e2_);
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
            else if (iPlas==4) {
                Double dAux = (ftz+fty+ftx) / (e1_+2.0*e2_);
                s->working_[Dqt]  += dAux * dSubZoneVolume;
            }
        }

        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dAux = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dAux *= 0.5;

            sHP_ += s->working_[Dqs] * dAux;
            tHP_ += s->working_[Dqt] * dAux;

            if (s->working_[Dqs] > 0.0) {
                if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
                if (iFriction_) friction_ = s->getYFromX(iFriction_,sHP_);
                if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);
            }
            if (s->working_[Dqt] > 0.0 && iTension_)
                tension_ = s->getYFromX(iTension_,tHP_);
        }
    }
}

// EOF