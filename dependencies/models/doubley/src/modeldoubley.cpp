#include "modeldoubley.h"
#include "base/src/version.h"

namespace models {

    ModelDoubleYield::ModelDoubleYield() : ModelStrainSoftening() {
    }

    String ModelDoubleYield::getProperties(void) const {
        return ModelStrainSoftening::getProperties() + 
            L",pressure-cap,multiplier,table-pressure-cap"
            L",strain-volumetric-plastic,bulk-maximum,shear-maximum";
    }

    UInt ModelDoubleYield::getMinorVersion() const {
        return version::update;
    }

    String ModelDoubleYield::getStates(void) const {
        return ModelStrainSoftening::getStates() + L", , , , ,volume-n,volume-p";
    }

    Variant ModelDoubleYield::getProperty(UInt index) const {
        switch (index) {
        case 1: return bulk_; 
        case 2: return shear_;
        case 3: { Double young;
            getYPfromBS(bulk_max,shear_max,&young,nullptr);
            return young; }
        case 4: { Double poisson;
            getYPfromBS(bulk_max,shear_max,nullptr,&poisson);
            return poisson; }
        case 5: return cohesion_;
        case 6: return friction_; 
        case 7: return dilation_;
        case 8: return tension_;
        case 9: return brittle_;
        case 10: return cTable_;
        case 11: return fTable_;
        case 12: return dTable_;
        case 13: return tTable_;
        case 14: return sHP_;
        case 15: return tHP_;
        case 16: return capPressure_;
        case 17: return multiplier_;
        case 18: return pTable_;
        case 19: return vHP_;
        case 20: return bulk_max;
        case 21: return shear_max;
        }
        return(0.0);
    }

    void ModelDoubleYield::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);

        switch (index)  {
        case 1: bulk_  = p.toDouble(); break;
        case 2: shear_ = p.toDouble(); break;
        case 3: { 
            Double young,poisson;
            getYPfromBS(bulk_max,shear_max,&young,&poisson);
            young = p.toDouble();
            if (!restoreVersion) getBSfromYP(young,poisson,&bulk_max,&shear_max); } break;
        case 4: { 
            Double young,poisson;
            getYPfromBS(bulk_max,shear_max,&young,&poisson);
            poisson = p.toDouble();
            if (!young && !restoreVersion) throw std::runtime_error("Young's Modulus must be non-zero before you can specify Poisson's Ratio.");
            if (!restoreVersion) getBSfromYP(young,poisson,&bulk_max,&shear_max); } break;
        case 5: cohesion_ = p.toDouble(); break;
        case 6: friction_ = p.toDouble(); break;
        case 7: dilation_ = p.toDouble(); break;
        case 8: tension_  = p.toDouble(); break;
        case 9: brittle_  = p.toBool();   break;
        case 10: cTable_  = p.toString(); break;
        case 11: fTable_  = p.toString(); break;
        case 12: dTable_  = p.toString(); break;
        case 13: tTable_  = p.toString(); break;
        case 14: sHP_     = p.toDouble(); break;
        case 15: tHP_     = p.toDouble(); break;
        case 16: capPressure_ = p.toDouble(); break;
        case 17: multiplier_  = p.toDouble(); break;
        case 18: pTable_ = p.toString();  break;
        case 19: vHP_   = p.toDouble();   break;
        case 20: bulk_max = p.toDouble(); break;
        case 21: shear_max = p.toDouble();break;
        }
    }

    bool ModelDoubleYield::isPropertyReadOnly(UInt i) const {
        if (i <= 15) 
            return ModelStrainSoftening::isPropertyReadOnly(i);
        return false;
    }

    bool ModelDoubleYield::isPropertyAdvanced(UInt i) const {
        if (i <= 15)
            return ModelStrainSoftening::isPropertyAdvanced(i);
        else if (i == 18) 
            return true;
        return false;
    }

    void ModelDoubleYield::copy(const ConstitutiveModel *m) {
        const ModelDoubleYield *mm = dynamic_cast<const ModelDoubleYield *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");

        ModelStrainSoftening::copy(m);

        capPressure_ = mm->capPressure_;
        multiplier_  = mm->multiplier_;
        pTable_      = mm->pTable_;
        vHP_         = mm->vHP_;
        bulk_max     = mm->bulk_max;
        shear_max    = mm->shear_max;
    }

    void ModelDoubleYield::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);

        if (bulk_max <= 0.0) 
            throw std::runtime_error("Double-Yield model: bulk-maximum must be greater than zero.");
        if (!shear_max  ) shear_max = 0.5*bulk_max;
        bulk_  = bulk_max;
        shear_ = shear_max;

        if (multiplier_ <= 0.0) multiplier_ = 5.0;

        iCohesion_ = iFriction_ = iDilation_ = iTension_ = iCapPressure_ = nullptr;

        if (cTable_.length()) iCohesion_ = s->getTableIndexFromID(cTable_);
        if (fTable_.length()) iFriction_ = s->getTableIndexFromID(fTable_);
        if (dTable_.length()) iDilation_ = s->getTableIndexFromID(dTable_);
        if (tTable_.length()) iTension_  = s->getTableIndexFromID(tTable_);
        if (iTension_ && brittle_)
            throw std::runtime_error("Internal error: flag-brittle not allowed on when assigning table-tension.");
        if (pTable_.length()) iCapPressure_ = s->getTableIndexFromID(pTable_);

        if (iTension_)  tension_  = s->getYFromX(iTension_, tHP_);
        if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
        if (iFriction_) friction_ = s->getYFromX(iFriction_,sHP_);
        if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);
        if (iCapPressure_) capPressure_ = s->getYFromX(iCapPressure_,vHP_);

        Double rsin = std::sin(friction_ * degrad);
        nph_  = (1.0 + rsin) / (1.0 - rsin);
        csn_  = 2.0 * cohesion_ * sqrt(nph_);
        if (friction_) {
            Double apex = cohesion_ / std::tan(friction_ * degrad);
            tension_ = std::min(tension_,apex);
        }
        else if (cohesion_ == 0.0)
            tension_ = 0.0;
        rsin = std::sin(dilation_ * degrad);
        nps_ = (1.0 + rsin) / (1.0 - rsin);
        rc_ = std::sqrt(1.0 + nph_*nph_);
    }

    static const UInt Dqs = 0;
    static const UInt Dqt = 1;
    static const UInt Dqv = 2;

    void ModelDoubleYield::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        if (s->state_ & volume_now) s->state_ |= volume_past;
        s->state_ &= ~volume_now;
        UInt iPlasS = 0;
        UInt iPlasT = 0;
        UInt iPlasV = 0;

        if (!s->sub_zone_) {
            s->working_[Dqs] = 0.0;
            s->working_[Dqt] = 0.0;
            s->working_[Dqv] = 0.0;
        }

        Double dLambdaS(0.0),dLambdaV(0.0),dLambdaT(0.0);
        Double dSubZoneVolume = s->getSubZoneVolume();

        if (iCapPressure_)  { // get current bulk stiffness
            Double dSlope;
            dSlope = s->getSlopeFromX(iCapPressure_,vHP_);
            bulk_ = multiplier_*dSlope;
            if (bulk_<=0.0) bulk_ = bulk_max;
            bulk_ = bulk_ < bulk_max ? bulk_ : bulk_max;
            if (!bulk_max)
                shear_ = 0.5 * bulk_;
            else
                shear_ = bulk_*shear_max/bulk_max;
        }

        Double shearNew_ = shear_;
        if (s->modulus_reduction_factor_ > 0.0)
            shearNew_ = shear_ * s->modulus_reduction_factor_;

        e1_ = bulk_ + d4d3*shearNew_;
        e2_ = bulk_ - d2d3*shearNew_;
        g2_ = 2.0*shearNew_;

        Double dRa  = e1_ - nps_*e2_;
        Double dRb  = e2_ - nps_*e1_;
        Double dRd  = dRa - dRb*nph_;
        Double dRe  = 1.0 - nph_;
        Double dRf  = 1.0 - nps_;
        Double dRg  = 1.0 / (dRd - bulk_*dRe*dRf);
        Double dSC2 = e2_ * (1.0 - nps_);

        ModelElastic::elasticTrial(s);
        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        Double fs  = nph_*prin.z() - prin.x() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        Double fv  = - d1d3*(prin.x() + prin.y() + prin.z()) - capPressure_;

        if (ftz <= 0.0) {
            if (fs > 0.0 && fv > 0.0) { // shear and volume failure
                iPlasS = 1; s->state_ |= shear_now;
                iPlasV = 1; s->state_ |= volume_now;
                dLambdaS = (fs - fv*dRe)*dRg;
                dLambdaV = fv/bulk_ - dLambdaS*dRf;
                prin.rx() += dLambdaS*dRa  + dLambdaV*bulk_;
                prin.ry() += dLambdaS*dSC2 + dLambdaV*bulk_;
                prin.rz() += dLambdaS*dRb  + dLambdaV*bulk_;
            } 
            else if (fs > 0.0 && fv <= 0.0) { // shear failure only
                iPlasS = 1; s->state_ |= shear_now;
                dLambdaS = fs / dRd;
                prin.rx() += dLambdaS * dRa;
                prin.ry() += dLambdaS * dSC2;
                prin.rz() += dLambdaS * dRb;
            } else if (fs <= 0.0 && fv > 0.0) { // volume failure only
                iPlasV = 1; s->state_ |= volume_now;
                dLambdaV = fv / bulk_;
                prin.rx() += fv;
                prin.ry() += fv;
                prin.rz() += fv;
            }
        } 
        else {
            if (fv <= 0.0) {
                if (fsd > 0.0 && fsd >= ftz) { // shear failure only
                    iPlasS = 1; s->state_ |= shear_now;
                    dLambdaS = fs / dRd;
                    prin.rx() += dLambdaS * dRa;
                    prin.ry() += dLambdaS * dSC2;
                    prin.rz() += dLambdaS * dRb;
                } 
                else { // tensile failure only
                    iPlasT = 1; s->state_ |= tension_now;
                    Double ftx = prin.x() - tension_;
                    Double fty = prin.y() - tension_;
                    if (ftx > 0.0) {
                        prin.rx()  = tension_;
                        prin.ry()  = tension_;
                        prin.rz()  = tension_;
                    } else if (fty > 0.0) {
                        prin.rx() -= (fty + ftz) * e2_/(e1_+e2_);
                        prin.ry()  = tension_;
                        prin.rz()  = tension_;
                    } else {
                        Double tco = ftz * e2_/e1_;
                        prin.rx() -= tco;
                        prin.ry() -= tco;
                        prin.rz()  = tension_;
                    }
                }
            }
            else {
                if (fs > 0.0) {  // yields on all yield surfaces
                    iPlasS = 1; s->state_ |= shear_now;
                    iPlasT = 1; s->state_ |= tension_now;
                    iPlasV = 1; s->state_ |= volume_now;
                    Double den = 1.0/(e2_ - e1_);
                    dLambdaS = (ftz*(1.0 + 2.0*nph_) + 3.0*fv - 2.0*ftz) *den;
                    dLambdaV = fv/bulk_ + 3.0*den*(fs - 2.0*fv - (1.0 + nph_)*ftz);
                    dLambdaT = (-ftz*(nps_*(1.0 + 2.0*nph_) + 2.0 + nph_) - 3.0*(1.0 + nps_)*fv + (1.0 + 2.0*nps_)*fs) *den;
                    prin.rx() = tension_*nph_ - csn_;
                    prin.ry() = -prin.x() - tension_ - 3.0*capPressure_;
                    prin.rz() = tension_;
                } else { // volume and tension failure
                    iPlasT = 1; s->state_ |= tension_now;
                    iPlasV = 1; s->state_ |= volume_now;
                    dLambdaV = (e1_*fv + bulk_*ftz) / (bulk_ * (e1_ - bulk_));
                    dLambdaT = (fv + ftz) / (e1_ - bulk_);
                    prin.rx() += 0.5*(3.0*fv + ftz);
                    prin.ry() += 0.5*(3.0*fv + ftz);
                    prin.rz()  = tension_;
                }
            }
        }

        if (iPlasS || iPlasV || iPlasT) {
            s->stnS_ = info.resolve(prin);
            s->viscous_ = false; 
            if (iPlasS) {
                Double dDe1p = dLambdaS;
                Double dDe3p = -dLambdaS * nps_;
                Double dDepa = d1d3 * (dDe1p + dDe3p);
                dDe1p -= dDepa;
                dDe3p -= dDepa;
                s->working_[Dqs]  += sqrt(0.5 * (dDe1p*dDe1p+dDepa*dDepa+dDe3p*dDe3p)) *dSubZoneVolume;
            }
            if (iPlasT) {
                Double dAux = dLambdaT;
                if (dAux < 0.) dAux = -dAux;
                s->working_[Dqt]  += dAux *dSubZoneVolume;
            }
            if (iPlasV) {
                Double dAux = dLambdaV;
                if (dAux < 0.) dAux = -dAux;
                s->working_[Dqv]  += dAux *dSubZoneVolume;
            }
        }

        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dAux = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dAux *= 0.5;
            sHP_ += s->working_[Dqs] * dAux;
            tHP_ += s->working_[Dqt] * dAux;
            vHP_ += s->working_[Dqv] * dAux;
            if (s->working_[Dqs] > 0.0) {
                if (iCohesion_) // Update Cohesion
                    cohesion_ = s->getYFromX(iCohesion_,sHP_);
                if (iFriction_) // update friction
                    friction_ = s->getYFromX(iFriction_,sHP_);
                if (iDilation_) // update dilation
                    dilation_ = s->getYFromX(iDilation_,sHP_);
            }
            Double dRSin = sin(friction_ * degrad);
            nph_ = (1.0 + dRSin) / (1.0 - dRSin);
            csn_ = 2.0 * cohesion_ * sqrt(nph_);
            rc_ = std::sqrt(1.0 + nph_*nph_);
            dRSin = sin(dilation_ * degrad);
            nps_ = (1.0 + dRSin) / (1.0 - dRSin);
            if (s->working_[Dqt] > 0.0 && iTension_)
                tension_ = s->getYFromX(iTension_,tHP_);
            if (friction_) {
                Double dApex = cohesion_ / tan(friction_ * degrad);
                tension_ = tension_ < dApex ? tension_ : dApex;
            }
            if (s->working_[Dqv] > 0.0) {
                if (iCapPressure_) { // update pressure cap
                    capPressure_ = s->getYFromX(iCapPressure_,vHP_);

                }
            }
        }
    }
}
// EOF
