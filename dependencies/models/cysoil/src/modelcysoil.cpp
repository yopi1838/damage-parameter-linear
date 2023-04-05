#include "modelcysoil.h"
#include "base/src/version.h"

namespace models 
{
    ModelCYSoil::ModelCYSoil() : ModelMohr() {
    }

    String ModelCYSoil::getProperties(void) const {
        return ModelMohr::getProperties() + 
            + L",shear-reference,pressure-reference,exponent,pressure-cap,multiplier"
            + L",failure-ratio,alpha,beta,pressure-initial,over-consolidation-ratio"
            + L",friction-critical,friction-mobilized,friction-0,dilation-mobilized"
            + L",flag-cap,flag-shear,flag-dilation"
            + L",table-cohesion,table-friction,table-dilation,table-tension,table-pressure-cap"
            + L",shear-maximum,shear-minimum"
            + L",strain-shear-plastic,strain-tensile-plastic,strain-volumetric-plastic"
            + L",stress-deviatoric,pressure-effective"
            + L",void-initial,void-maximum,void";
    }

    UInt ModelCYSoil::getMinorVersion() const {
        return version::update;
    }

    String ModelCYSoil::getStates(void) const {
        return ModelMohr::getStates() + L", , , , ,volume-n,volume-p";
    }

    Variant ModelCYSoil::getProperty(UInt index) const {
        switch (index) { 
        case 1: return bulk_; 
        case 2: return shear_;
        case 3: { Double young;
            getYPfromBS(bulk_,shear_,&young,nullptr);
            return young; }
        case  4: return poisson_;
        case  5: return cohesion_;
        case  6: return frictUlt_; 
        case  7: return dilatUlt_;
        case  8: return tension_;
        case  9: return brittle_;
        case 10: return Gref_;
        case 11: return pref_;
        case 12: return m_;
        case 13: return capPressure_;
        case 14: return multiplier_;
        case 15: return rf_;
        case 16: return alpha_;
        case 17: return beta_;
        case 18: return pini_;
        case 19: return ocr_;
        case 20: return frictCV_;
        case 21: return friction_;
        case 22: return frict0_;
        case 23: return dilation_;
        case 24: return cFlag_;
        case 25: return sFlag_;
        case 26: return dFlag_;
        case 27: return cTab_;
        case 28: return fTab_;
        case 29: return dTab_;
        case 30: return tTab_;                       
        case 31: return pTab_;
        case 32: return shear_max;
        case 33: return shear_min;
        case 34: return sHP_;
        case 35: return tHP_;
        case 36: return vHP_;
        case 37: return qnew_;
        case 38: return pnew_;
        case 39: return eini_;
        case 40: return emax_;
        case 41: return enow_;
        }
        return (0.0);
    }

    void ModelCYSoil::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);

        switch (index)  {
        case 1: bulk_  = p.toDouble(); break;
        case 2: shear_ = p.toDouble(); break;
        case 3: { 
            Double young,poisson;
            getYPfromBS(bulk_,shear_,&young,&poisson);
            young = p.toDouble();
            if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_); } break;
        case 4: poisson_ = p.toDouble(); break;
        case 5: cohesion_ = p.toDouble(); break;
        case 6: frictUlt_ = p.toDouble(); break;
        case 7: dilatUlt_ = p.toDouble(); break;
        case 8: tension_  = p.toDouble(); break;
        case 9: brittle_  = p.toBool();   break;
        case 10: Gref_        = p.toDouble(); break;
        case 11: pref_        = p.toDouble(); break;
        case 12: m_           = p.toDouble(); break;
        case 13: capPressure_ = p.toDouble(); break;
        case 14: multiplier_  = p.toDouble(); break;
        case 15: rf_          = p.toDouble(); break;
        case 16: alpha_       = p.toDouble(); break;
        case 17: beta_        = p.toDouble(); break;
        case 18: pini_        = p.toDouble(); break;
        case 19: ocr_         = p.toDouble(); break;
        case 20: frictCV_     = p.toDouble(); break;
        case 21: friction_    = p.toDouble(); break;
        case 22: frict0_      = p.toDouble(); break;
        case 23: dilation_    = p.toDouble(); break;
        case 24: cFlag_       = p.toInt();    break;
        case 25: sFlag_       = p.toInt();    break;
        case 26: dFlag_       = p.toInt();    break;
        case 27: cTab_        = p.toString(); break;
        case 28: fTab_        = p.toString(); break;
        case 29: dTab_        = p.toString(); break;       
        case 30: tTab_        = p.toString(); break;
        case 31: pTab_        = p.toString(); break;
        case 32: shear_max = p.toDouble(); 
                 bulk_max= shear_max*(1.0+poisson_)/(1.5-3.0*poisson_);
                 break;
        case 33: shear_min    = p.toDouble(); break;
        case 34: sHP_         = p.toDouble(); break;
        case 35: tHP_         = p.toDouble(); break;
        case 36: vHP_         = p.toDouble(); break;
        case 37: qnew_        = p.toDouble(); break;
        case 38: pnew_        = p.toDouble(); break;
        case 39: eini_        = p.toDouble(); break;
        case 40: emax_        = p.toDouble(); break;
        case 41: enow_        = p.toDouble(); break;
        }
    }

    bool ModelCYSoil::isPropertyReadOnly(UInt i) const {
        switch (i) {
        case  1:
        case  2:
        case  3:
        case 37:
        case 38:
        case 39: return true;
        default: return false;
        }
    }

    bool ModelCYSoil::isPropertyAdvanced(UInt i) const {
        switch (i) {
        case 12: // 0.5, upper-limit is 0.99, exponent 
        case 13: // if not input and 35:cap_flag = 1, it will be initialized to p_ini*ocr 
        case 14: // 5.0, with cap; reset to 0.0 if without cap
        case 15: // 0.9, failure ratio
        case 16: // 1.0, alpha        
        case 17: // 1.0, beta
        case 19: // 1.0, ocr
        case 20: // internal, need input only if 37:dilation_flag = 2
        case 24: // 0, default
                 // 0: no cap,   p_ini must be input
                 // 1: with cap, capPressure must be input, if not, capPressure will be calulated by p_ini*ocr
        case 25: // 0, default
                 // 0: moblized friction angle with hardening
                 // 1: fixed friction angle, the mobilzied friction is equal to the ultimate friction angle
        case 26: // 0, default
                 // 0: built-in dilation law, friction_cv calculated from ultimate friction and dilation angles
                 // 1: fixed mobilized dilation angle, equel to the ultimate dilation angle
                 // 2: built-in dilation law, the diffrence from 0) is that friction_cv now is an constant input parameter
        case 27: // 0, cohesion table id
        case 28: // 0, dilation table id
        case 29: // 0, friction table id
        case 30: // 0, tension limit table id
        case 31: // 0, cap pressure table id 
        case 32: // internal, upper-bound of elastic shear modulus
        case 33: // internal, lower-bound of elastic shear modulus
        case 34: // internal, accumulated palstic shear strain
        case 35: // 0.0, accumulated tensile strain
        case 36: // internal, accumulated plastic volume strain
        case 39: // 1.0,   initial void ratio
        case 40: // 999.0, maximum void ratio
            return true;
        default: return false;
        }
    }

    void ModelCYSoil::copy(const ConstitutiveModel *m) {
        const ModelCYSoil *mm = dynamic_cast<const ModelCYSoil *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        shear_max     = mm->shear_max;
        shear_min     = mm->shear_min;
        frictUlt_     = mm->frictUlt_;
        dilatUlt_     = mm->dilatUlt_;
        capPressure_  = mm->capPressure_;
        multiplier_   = mm->multiplier_;
        cTab_         = mm->cTab_;
        dTab_         = mm->dTab_;
        fTab_         = mm->fTab_;
        pTab_         = mm->pTab_;
        tTab_         = mm->tTab_;
        vHP_          = mm->vHP_;
        sHP_          = mm->sHP_;
        tHP_          = mm->tHP_;
        frictCV_      = mm->frictCV_;
        friction_     = mm->friction_;
        dilation_     = mm->dilation_;
        alpha_        = mm->alpha_;
        qnew_         = mm->qnew_;
        pnew_         = mm->pnew_;
        Gref_         = mm->Gref_;
        pref_         = mm->pref_;
        m_            = mm->m_;
        rf_           = mm->rf_;
        beta_         = mm->beta_;
        pini_         = mm->pini_;
        ocr_          = mm->ocr_;
        cFlag_        = mm->cFlag_;
        sFlag_        = mm->sFlag_;
        dFlag_        = mm->dFlag_;
        eini_         = mm->eini_;
        emax_         = mm->emax_;
        enow_         = mm->enow_;
        bulk_max      = mm->bulk_max;
        frict0_       = mm->frict0_;
    }

    void ModelCYSoil::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        //
        if (frictUlt_ < 0.1) frictUlt_ = 0.1;
        if (friction_ > frictUlt_) friction_ = frictUlt_;
        if (frict0_ > frictUlt_)   frict0_ = frictUlt_;
        if (!sHP_) frict0_ = friction_;  // default is the mobilized friction
        dSfrictUlt_     = sin(frictUlt_ * degrad);
        if (dFlag_==0) {
            Double dSdilaUlt_ = sin(dilatUlt_ * degrad);
            dSfrictCV_ = (dSfrictUlt_ - dSdilaUlt_) / (1.0 - dSfrictUlt_*dSdilaUlt_);
            if (dSfrictCV_ >  1.0) dSfrictCV_ =  1.0;
            if (dSfrictCV_ < -1.0) dSfrictCV_ = -1.0;
            frictCV_ = std::asin(dSfrictCV_) / degrad;
        }
        else if (dFlag_==2) 
            dSfrictCV_   = sin(frictCV_   * degrad);

        dSfriction_  = sin(friction_  * degrad);
        dSFrict0_    = sin(frict0_    * degrad);
        dSFrictdiff_ = dSfrictUlt_ - dSFrict0_;

        icohesion_ = idilation_ = ifriction_ = itension_ = icapPressure_ = nullptr;

        if (cTab_.length()) icohesion_    = s->getTableIndexFromID(cTab_);
        if (dTab_.length()) idilation_    = s->getTableIndexFromID(dTab_);
        if (fTab_.length()) ifriction_    = s->getTableIndexFromID(fTab_);
        if (pTab_.length()) icapPressure_ = s->getTableIndexFromID(pTab_);
        if (tTab_.length()) itension_     = s->getTableIndexFromID(tTab_);

        if (!alpha_) throw std::runtime_error("cap-yield-soil model: alpha should be a positive value.");
        if (pref_ <= 0.0) throw std::runtime_error("cap-yield-soil model: pressure-reference should be a positive value.");
        if (!rf_)  rf_ = 0.9;
        if (Gref_ <= 0.0) throw std::runtime_error("cap-yield-soil model: shear-reference not assigned or not a positive value.");
        if (ocr_ < 1.0) ocr_ = 1.0;
        if (!cFlag_) {
            if (pini_ <= 0.0) throw std::runtime_error("cap-yield-soil model: pressure-initial must be a positive value without a cap.");
        }
        else if (!icapPressure_) { //if with a cap
            if (capPressure_ <= 0.0) {
                if (pini_ > 0.0 && ocr_ > 0.0) {
                    double Mm = 6.0 * dSfriction_ / (3.0 - dSfriction_);
                    double qa = (Mm/alpha_) * (pini_ + cohesion_/std::tan(frictUlt_*degrad));
                    capPressure_ = ocr_ * std::sqrt(pini_*pini_ + qa*qa);
                }
                else
                    throw std::runtime_error("cap-yield-soil model: capPressure or pressure-initial must be a positive value with a cap option.");
            }
        }
        if (m_ <= 0.0) throw std::runtime_error("cap-yield-soil model: exponent-bulk not assigned or not a positive value.");
        if (m_ > 0.99)    m_    = 0.99; //limit
        if (!beta_) beta_ = 1.0;
        if (beta_ < 0.1)  beta_ = 0.1;  //limit
        if (cFlag_) {
            if( !multiplier_) multiplier_ = 5.0;  //defualt with cap
        }
        else                  multiplier_ = 0.0;  //default without cap
        double s2b = (1.0 + poisson_) / (1.5-3.0*poisson_);
        dcoe_   = multiplier_ /((1.0-m_)*(1.0+multiplier_)*Gref_*s2b);
        if (!vHP_ && cFlag_) vHP_ = dcoe_ * pow(capPressure_/pref_,1.0-m_);
        if (eini_ < 0.0) eini_ = 0.0;
        if (emax_ < 0.0) emax_ = 999.0;
        if (!enow_) {   
            enow_ = eini_;
            ev_ = 0.0;
        }
        else {
            if (enow_ > emax_) enow_ = emax_;
            ev_ = log((1.0+enow_)/(1.0+eini_));
        }
        double p_used = cFlag_? capPressure_ : pini_;
        if (!shear_max)  {       
            // defalt shear_upper limits to 10 times of the initial value
            shear_max = 10.0 * Gref_*(1.+multiplier_)*pref_*pow(p_used/pref_,m_);
        }
        if (!shear_min)  {
            // defult shear_upper limits to 0.1 times of the initial value
            shear_min = 0.1 * Gref_*(1.+multiplier_)*pref_*pow(p_used/pref_,m_);
        }
        if (shear_max < shear_min) throw std::runtime_error("cap-yield-soil model: shear-maximum is less than shear_lower, one or two of them should be reassigned.");
        if (!bulk_max) bulk_max = shear_max * s2b;

        updMobVars(s);

        if ((!ifriction_)&&(rf_)&&(!sHP_)&& (dSFrictdiff_)) {
            // built-in friction hardening.
            double dSFricdiff2_ = dSfriction_ - dSFrict0_;
            double dDen = dSFrictdiff_ - dSFricdiff2_*rf_;
            double dPress = cFlag_ ? capPressure_ : pini_;
            shear_ = Gref_*(1.+multiplier_)*pref_*pow(dPress/pref_,m_); 
            shear_ = shear_ < shear_max ? shear_ : shear_max;
            shear_ = shear_ > shear_min ? shear_ : shear_min;
            double dGe  = beta_*(1.0+multiplier_)*Gref_;
            sHP_ = dSFrictdiff_*dSFricdiff2_/(dDen*dGe);
        }
    }

    static const unsigned Dqs = 0;
    static const unsigned Dqt = 1;
    static const unsigned Dqv = 2;
    static const unsigned Dev = 3;
    //
    void ModelCYSoil::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        //UInt iPlasS,iPlasV,iPlasT;
        //Double dLambdaS,dLambdaV,dLambdaT;
        //Double dEpc1,dEpc2,dEpc3,dEps1,dEps2,dEps3;

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        if (s->state_ & volume_now) s->state_ |= volume_past;
        s->state_ &= ~volume_now;

        double dsubv = s->getSubZoneVolume();

        if (!s->sub_zone_) {
            s->working_[Dqs] = 0.0;
            s->working_[Dqt] = 0.0;
            s->working_[Dqv] = 0.0;
            s->working_[Dev] = 0.0;
        }

        s->working_[Dev] += (s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33()) * dsubv;
        ModelMohr::updateParameters();

        ModelElastic::elasticTrial(s);       

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        // q-p space
        double dDelta   = (3.0 + dSfriction_)/(3.0 - dSfriction_);
        double dDelStar = (3.0 + dSdilation_)/(3.0 - dSdilation_);
        double dMM    = 6.0 * dSfriction_/(3.0 - dSfriction_);
        double dMStar = 6.0 * dSdilation_/(3.0 - dSdilation_);

        double dNN    = dMM / std::tan(frictUlt_*degrad);
        double dQVal = -(prin.x() + (dDelta - 1.0) * prin.y() - dDelta * prin.z());
        double dPVal = -(prin.x() + prin.y() + prin.z()) * d1d3;
        double alpha = alpha_;
        double alphaSquare = alpha*alpha;
        // Tensile failure criteria
        double ft  = prin.z() - tension_;
        // Mohr-Coulomb failure criterion
        double fs = dMM*dPVal - dQVal + dNN*cohesion_;
        // Cap volumetric failure criterion
        double fv = cFlag_? dQVal*dQVal/alphaSquare + dPVal*dPVal - capPressure_*capPressure_ : -1.0e-10;

        // tests for failure
        unsigned iPlasS = 0, iPlasV = 0, iPlasT = 0;
        double dLambdaS = 0.0, dLambdaV = 0.0, dLambdaT = 0.0;
        double dEpc1 = 0.0, dEpc2 = 0.0, dEpc3 = 0.0;
        double dEps1 = 0.0, dEps2 = 0.0, dEps3 = 0.0;
        pnew_ = dPVal;
        qnew_ = dQVal;

        if (ft > 0.0)
            ModelMohr::tensionCorrection(s,&prin,&iPlasT,ft,brittle_); 
        else if (fs < 0. && fv <= 0.) {  
            //shearing failure region
            iPlasS = 1;
            s->state_ |= shear_now;
            // compute lambdaS now
            double dDel1 = 1.0+dDelta*dDelStar+(dDelta-1.0)*(dDelStar-1.0);
            dLambdaS = fs / (bulk_ * dMM * dMStar + g2_ * dDel1);
            qnew_ = dQVal + g2_ * dLambdaS * dDel1;
            pnew_ = dPVal - bulk_ * dLambdaS * dMStar;
            dEps1 = dLambdaS * (- d1d3 * dMStar +            1.0);
            dEps2 = 0.0;
            dEps3 = dLambdaS * (- d1d3 * dMStar - dDelStar     );
            prin.rx() -= e1_ * dEps1 + e2_ * dEps3;
            prin.ry() -= e2_ * (dEps1 + dEps3);
            prin.rz() -= e1_ * dEps3 + e2_ * dEps1;
        } else if (cFlag_ && fv > 0. && fs >= 0.) {  
            //cap yielding only
            iPlasV = 1;
            s->state_ |= volume_now;
            // calculate pnew_ and qnew_
            if (dPVal) {
                double dQP = dQVal/dPVal;
                double vtemp = std::sqrt(dQP * dQP / alphaSquare + 1.0);
                pnew_ = capPressure_ / vtemp;
                qnew_ = dQP * pnew_;
                double dDPK = (dPVal - pnew_) / bulk_;
                dLambdaV = dDPK/(2.0 * pnew_);
                double dQNA = 2.0 * qnew_ / alphaSquare;
                double dPN23 = d2d3 * pnew_;
                dEpc1 = dLambdaV * (- dPN23 - dQNA                );
                dEpc2 = dLambdaV * (- dPN23 - dQNA * (dDelta - 1.0));
                dEpc3 = dLambdaV * (- dPN23 + dQNA *  dDelta      );
                prin.rx() -= e1_ * dEpc1 + e2_ * (dEpc2 + dEpc3);
                prin.ry() -= e1_ * dEpc2 + e2_ * (dEpc1 + dEpc3);
                prin.rz() -= e1_ * dEpc3 + e2_ * (dEpc1 + dEpc2);
            }
        } else if (cFlag_ && fv > 0. && fs < 0.) { 
            //both cap and shearing yielding
            iPlasV = 1;
            iPlasS = 1;
            s->state_ |= volume_now;
            s->state_ |= shear_now;
            double malp  = dMM*dMM + alphaSquare;
            double amalp = dNN*cohesion_;
            double temp  = capPressure_*capPressure_*malp - amalp*amalp;
            pnew_ = (-dMM*amalp + alpha*std::sqrt(temp<0.0?0.0:temp))/malp;
            if (pnew_) {
                qnew_ = dMM*pnew_ + amalp;
                // compute lambdaS and lambdaV now
                double dDel1 = 1.0 + dDelta * dDelStar+(dDelta-1.0)*(dDelStar-1.0);
                double dDel2 = 1.0 + dDelta * dDelta  +(dDelta-1.0)*(dDelta  -1.0);
                double dDel12 = dDel1/dDel2; 
                double d2pnew = 2.0*pnew_;
                double dMPN = dMStar/d2pnew;
                double dQNA = 2.0*qnew_ / alphaSquare;
                double dPN23 = d2d3*pnew_;
                double dDPK = (dPVal - pnew_) / bulk_;
                double dDQG = (dQVal - qnew_) / g2_;
                dLambdaS = (dDPK*dQNA/d2pnew - dDQG/dDel12)/(dDel12+dMPN*dQNA);
                dLambdaV = dDPK/d2pnew - dLambdaS * dMPN;
                dEpc1 = dLambdaV * (- dPN23 - dQNA                );
                dEpc2 = dLambdaV * (- dPN23 - dQNA * (dDelta - 1.0));
                dEpc3 = dLambdaV * (- dPN23 + dQNA *  dDelta      );
                dEps1 = dLambdaS * (- d1d3 * dMStar +          1.0);
                dEps2 = 0.0;
                dEps3 = dLambdaS * (- d1d3 * dMStar - dDelStar     );
            }
            prin.rx() -= e1_ * (dEpc1 + dEps1) + e2_ * (dEpc2         + dEpc3 + dEps3);
            prin.ry() -= e1_ * (dEpc2 + dEps2) + e2_ * (dEpc1 + dEps1 + dEpc3 + dEps3);
            prin.rz() -= e1_ * (dEpc3 + dEps3) + e2_ * (dEpc1 + dEps1 + dEpc2        );
        }

        ModelMohr::apexCorrection(friction_,s,&prin,&iPlasT,brittle_);

        if (iPlasS || iPlasV || iPlasT) {
            // transform back to refrence frame
            s->stnS_ = info.resolve(prin);
            // hardening parameter accumulation
            if (iPlasS) {
                // shear parameter
                double dDepa = d1d3 * (dEps1 + dEps3);
                dEps1 -= dDepa;
                dEps3 -= dDepa;
                s->working_[Dqs]  += sqrt(0.5 * (dEps1*dEps1+dDepa*dDepa+dEps3*dEps3)) * dsubv;
            }
            if (iPlasT) {
                // tensile parameter
                double dAux = dLambdaT;
                if (dAux < 0.) dAux = -dAux;
                s->working_[Dqt]  += dAux * dsubv;
            }
            if (iPlasV) {
                // volume parameter
                double dAux = 2.0 * pnew_ * dLambdaV;
                s->working_[Dqv]  += dAux * dsubv;
            }
        }
        // plastic parameter incrementation and properties update
        if (s->sub_zone_==s->total_sub_zones_-1) {
            double dAux = 1.0 / s->getZoneVolume();
            if (s->overlay_==2) dAux *= 0.5;

            sHP_ += s->working_[Dqs] * dAux;
            tHP_ += s->working_[Dqt] * dAux;
            vHP_ += s->working_[Dqv] * dAux;
            ev_  += s->working_[Dev] * dAux;
            enow_ = (1.0 + eini_) * exp(ev_) - 1.0;
            if (enow_ > emax_) enow_ = emax_;

            updMobVars(s);
        }
    }

    void ModelCYSoil::updMobVars(State *s) {
        if (icapPressure_)  { // get current bulk stiffness
            //Double dSlope;
            //dSlope = s->getSlopeFromX(icapPressure_,vHP_);// plastic modulus, e.g.,"h" in Double yield
            //bulk_ = multiplier_*dSlope; // Kc = R * h
            //if (bulk_<=0.0) bulk_ = bulk_max;
            capPressure_ = s->getYFromX(icapPressure_,vHP_);
            shear_ = Gref_*(1.+multiplier_)*pref_*pow(capPressure_/pref_,m_);
        } else {
            if (cFlag_) {
                capPressure_ = pref_*pow(vHP_/dcoe_,1.0/(1.0-m_));
                shear_ = Gref_*(1.+multiplier_)*pref_*pow(capPressure_/pref_,m_); //needed for updating moduli
            } 
            else {
                shear_ = Gref_*(1.+multiplier_)*pref_*pow(pini_/pref_,m_);
            }  
        }

        shear_ = shear_ < shear_max ? shear_ : shear_max;
        shear_ = shear_ > shear_min ? shear_ : shear_min;
        bulk_ = shear_*bulk_max/shear_max;

        if (ifriction_) { // update mobilized friction
            friction_ = s->getYFromX(ifriction_,sHP_);
            //sin(mobilized friction)
            dSfriction_ = sin(friction_ * degrad);
        } 
        else {
            if (sFlag_) {
                friction_ = frictUlt_;
                dSfriction_ = sin(friction_ * degrad);
            } else {
                double dGe  = beta_*(1.0+multiplier_)*Gref_;
                double dDen = dSFrictdiff_ + sHP_* dGe * rf_;
                if (dDen) 
                    dSfriction_ = dSFrict0_ + sHP_* dSFrictdiff_ * dGe / dDen;
                else 
                    dSfriction_ = sin(frict0_ * degrad);
                if (dSfriction_ > dSfrictUlt_) dSfriction_ = dSfrictUlt_;
                if (dSfriction_ >  1.0) dSfriction_ =  1.0;
                if (dSfriction_ < -1.0) dSfriction_ = -1.0;
                friction_ = std::asin(dSfriction_) / degrad;
            }
        }

        if (icohesion_) cohesion_ = s->getYFromX(icohesion_,sHP_);
        if (itension_)  tension_ = s->getYFromX(itension_,tHP_);
        if (friction_)  
            tension_ = std::min(tension_,cohesion_/tan(friction_ * degrad));
        else if (cohesion_ == 0.0)
            tension_ = 0.0;
        if (idilation_) {
            // update mobilized dilation
            dilation_ = s->getYFromX(idilation_,sHP_);
            dSdilation_ = sin(dilation_ * degrad);
        } 
        else {
            if (dFlag_==1) {
                dilation_ = dilatUlt_;
                dSdilation_ = sin(dilation_ * degrad);
            } 
            else {
                dSdilation_ = (dSfriction_ - dSfrictCV_) / (1.0-dSfriction_*dSfrictCV_); //Rowe dilatancy law
                if (dSdilation_ >  1.0) dSdilation_ =  1.0;
                if (dSdilation_ < -1.0) dSdilation_ = -1.0;
                dilation_ = std::asin(dSdilation_) / degrad;
            }
        }
        if (dilation_ > friction_) {
            dilation_ = friction_;
            dSdilation_ = sin(dilation_ * degrad);
        }
        if (enow_ >= emax_) {
            dilation_ = 0.0;
            dSdilation_ = 0.0;
        }
    }
}
// EOF

