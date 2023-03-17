#include "modelfinn.h"
#include "base/src/version.h"

namespace models {

    static const Double dRSq6 = 0.408248290463863016; // 1.0 / sqrt(6.0);

    ModelFinn::ModelFinn() : ModelMohr(),sHP_(0.0),tHP_(0.0),
        fC1_(0.0),fC2_(0.0),fC3_(0.0),fC4_(0.0),eVD_(0.0),count_(0.0),
        latency_(0.0),switch_(0),
        wE12_(0.0),wE23_(0.0),wE31_(0.0),wE1M2_(0.0),wE2M3_(0.0),
        wE3M1_(0.0),wE12P_(0.0),wE23P_(0.0),wE31P_(0.0),
        wE1M2P_(0.0),wE2M3P_(0.0),wE3M1P_(0.0),uV1_(0.0),uV2_(0.0),uV3_(0.0),
        uV4_(0.0),uV5_(0.0),uV6_(0.0),dEVD_(0.0),dPPrev_(0.0),
        latCnt_(0.0),
        sf1_(0.0),sf3_(0.0),
        iCohesion_(0),iFriction_(0),iDilation_(0),iTension_(0) { 
    }

    String ModelFinn::getProperties(void) const {
        return ModelMohr::getProperties() + 
               L",table-cohesion,table-friction,table-dilation,table-tension"
               L",strain-shear-plastic,strain-tensile-plastic"
               L",constant-1,constant-2,constant-3,constant-4"
               L",strain-volumetric-irrecoverable,number-count"
               L",number-latency,flag-switch";
    }

    UInt ModelFinn::getMinorVersion() const  {
        return version::update;
    }

    Variant ModelFinn::getProperty(UInt index) const {
        if (index <= 9)
            return ModelMohr::getProperty(index);
        else {
            switch (index) {
            case 10: return(cTable_);
            case 11: return(fTable_);
            case 12: return(dTable_);
            case 13: return(tTable_);
            case 14: return(sHP_);
            case 15: return(tHP_);
            case 16: return(fC1_);
            case 17: return(fC2_);
            case 18: return(fC3_);
            case 19: return(fC4_);
            case 20: return(eVD_);
            case 21: return(count_);
            case 22: return(latency_);
            case 23: return(switch_);
            }
        }
        return(0.0);
    }

    void ModelFinn::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 9)
            ModelMohr::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 10: cTable_   = p.toString();  break;
            case 11: fTable_   = p.toString();  break;
            case 12: dTable_   = p.toString();  break;
            case 13: tTable_   = p.toString();  break;
            case 14: sHP_      = p.toDouble();  break;
            case 15: tHP_      = p.toDouble();  break;
            case 16: fC1_      = p.toDouble();  break;
            case 17: fC2_      = p.toDouble();  break;
            case 18: fC3_      = p.toDouble();  break;
            case 19: fC4_      = p.toDouble();  break;
            case 20: eVD_      = p.toDouble();  break;
            case 21: count_    = p.toDouble();  break;
            case 22: latency_  = p.toDouble();  break;
            case 23: switch_   = p.toInt();     break;
            }
        }
    }

    void ModelFinn::save(std::ostream &os) const {
        static const uint ver = 0;
        os.write(reinterpret_cast<const char *>(&ver      ),sizeof(ver     ));
        os.write(reinterpret_cast<const char *>(&wE12_    ),sizeof(wE12_   ));
        os.write(reinterpret_cast<const char *>(&wE23_    ),sizeof(wE23_   ));
        os.write(reinterpret_cast<const char *>(&wE31_    ),sizeof(wE31_   ));
        os.write(reinterpret_cast<const char *>(&wE1M2_   ),sizeof(wE1M2_  ));
        os.write(reinterpret_cast<const char *>(&wE2M3_   ),sizeof(wE2M3_  ));
        os.write(reinterpret_cast<const char *>(&wE3M1_   ),sizeof(wE3M1_  ));
        os.write(reinterpret_cast<const char *>(&wE12P_   ),sizeof(wE12P_  ));
        os.write(reinterpret_cast<const char *>(&wE23P_   ),sizeof(wE23P_  ));
        os.write(reinterpret_cast<const char *>(&wE31P_   ),sizeof(wE31P_  ));
        os.write(reinterpret_cast<const char *>(&wE1M2P_  ),sizeof(wE1M2P_ ));
        os.write(reinterpret_cast<const char *>(&wE2M3P_  ),sizeof(wE2M3P_ ));
        os.write(reinterpret_cast<const char *>(&wE3M1P_  ),sizeof(wE3M1P_ ));
        os.write(reinterpret_cast<const char *>(&uV1_     ),sizeof(uV1_    ));
        os.write(reinterpret_cast<const char *>(&uV2_     ),sizeof(uV2_    ));
        os.write(reinterpret_cast<const char *>(&uV3_     ),sizeof(uV3_    ));
        os.write(reinterpret_cast<const char *>(&uV4_     ),sizeof(uV4_    ));
        os.write(reinterpret_cast<const char *>(&uV5_     ),sizeof(uV5_    ));
        os.write(reinterpret_cast<const char *>(&uV6_     ),sizeof(uV6_    ));
        os.write(reinterpret_cast<const char *>(&dEVD_    ),sizeof(dEVD_   ));
        os.write(reinterpret_cast<const char *>(&dPPrev_  ),sizeof(dPPrev_ ));
    }

    void ModelFinn::restore(std::istream &is,UInt) {
        uint ver = 0;
        is.read(reinterpret_cast<char *>(&ver),sizeof(ver));
        is.read(reinterpret_cast<char *>(&wE12_    ),sizeof(wE12_   ));
        is.read(reinterpret_cast<char *>(&wE23_    ),sizeof(wE23_   ));
        is.read(reinterpret_cast<char *>(&wE31_    ),sizeof(wE31_   ));
        is.read(reinterpret_cast<char *>(&wE1M2_   ),sizeof(wE1M2_  ));
        is.read(reinterpret_cast<char *>(&wE2M3_   ),sizeof(wE2M3_  ));
        is.read(reinterpret_cast<char *>(&wE3M1_   ),sizeof(wE3M1_  ));
        is.read(reinterpret_cast<char *>(&wE12P_   ),sizeof(wE12P_  ));
        is.read(reinterpret_cast<char *>(&wE23P_   ),sizeof(wE23P_  ));
        is.read(reinterpret_cast<char *>(&wE31P_   ),sizeof(wE31P_  ));
        is.read(reinterpret_cast<char *>(&wE1M2P_  ),sizeof(wE1M2P_ ));
        is.read(reinterpret_cast<char *>(&wE2M3P_  ),sizeof(wE2M3P_ ));
        is.read(reinterpret_cast<char *>(&wE3M1P_  ),sizeof(wE3M1P_ ));
        is.read(reinterpret_cast<char *>(&uV1_     ),sizeof(uV1_    ));
        is.read(reinterpret_cast<char *>(&uV2_     ),sizeof(uV2_    ));
        is.read(reinterpret_cast<char *>(&uV3_     ),sizeof(uV3_    ));
        is.read(reinterpret_cast<char *>(&uV4_     ),sizeof(uV4_    ));
        is.read(reinterpret_cast<char *>(&uV5_     ),sizeof(uV5_    ));
        is.read(reinterpret_cast<char *>(&uV6_     ),sizeof(uV6_    ));
        is.read(reinterpret_cast<char *>(&dEVD_    ),sizeof(dEVD_   ));
        is.read(reinterpret_cast<char *>(&dPPrev_  ),sizeof(dPPrev_ ));
    }

    bool ModelFinn::isPropertyReadOnly(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyReadOnly(i);
        else {
            switch (i) {
            case 14:
            case 15:
            case 20:
            case 21: return true;
            default: return false;
            }
        }
    }

    bool ModelFinn::isPropertyAdvanced(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyAdvanced(i);
        else {        
            if (i >= 10 && i <= 13) 
                return true;
            else
                return false;
        }
    }

    void ModelFinn::copy(const ConstitutiveModel *m) {
        const ModelFinn *fm = dynamic_cast<const ModelFinn *>(m);
        if (!fm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        cTable_ = fm->cTable_;
        fTable_ = fm->fTable_;
        dTable_ = fm->dTable_;
        tTable_ = fm->tTable_;
        sHP_ = fm->sHP_;
        tHP_ = fm->tHP_;
        fC1_ = fm->fC1_;
        fC2_ = fm->fC2_;
        fC3_ = fm->fC3_;
        fC4_ = fm->fC4_;
        eVD_ = fm->eVD_;
        count_ = fm->count_;
        latency_ = fm->latency_;
        switch_ = fm->switch_;
        //
        wE12_ = fm->wE12_;
        wE23_ = fm->wE23_;
        wE31_ = fm->wE31_;
        wE1M2_ = fm->wE1M2_;
        wE2M3_ = fm->wE2M3_;
        wE3M1_ = fm->wE3M1_;
        wE12P_ = fm->wE12P_;
        wE23P_ = fm->wE23P_;
        wE31P_ = fm->wE31P_;
        wE1M2P_ = fm->wE1M2P_;
        wE2M3P_ = fm->wE2M3P_;
        wE3M1P_ = fm->wE3M1P_;
        uV1_ = fm->uV1_;
        uV2_ = fm->uV2_;
        uV3_ = fm->uV3_;
        uV4_ = fm->uV4_;
        uV5_ = fm->uV5_;
        uV6_ = fm->uV6_;
        dEVD_ = fm->dEVD_;
        dPPrev_ = fm->dPPrev_;
        latCnt_ = fm->latCnt_;
    }

    void ModelFinn::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        if ((d!=2)&&(d!=3))throw std::runtime_error("Illegal dimension in Finn constitutive model");

        iCohesion_ = iFriction_ = iDilation_ = iTension_ = nullptr;

        if (cTable_.length()) iCohesion_ = s->getTableIndexFromID(cTable_);
        if (fTable_.length()) iFriction_ = s->getTableIndexFromID(fTable_);
        if (dTable_.length()) iDilation_ = s->getTableIndexFromID(dTable_);
        if (tTable_.length()) iTension_  = s->getTableIndexFromID(tTable_);
        if (iTension_ && brittle_)
            throw std::runtime_error("Internal error: flag-brittle not allowed on when assigning table-tension.");

        if (iTension_)  tension_  = s->getYFromX(iTension_, tHP_);
        if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
        if (iFriction_) friction_ = s->getYFromX(iFriction_,sHP_);
        if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);

        if (friction_) {
            Double dVal = tan(friction_ * degrad);
            Double dApex = cohesion_ / dVal;
            tension_ = tension_ < dApex ? tension_ : dApex;
        }
        else if (cohesion_ == 0.0)
            tension_ = 0.0;

        ModelMohr::updateParameters(false,&sf1_,&sf3_);
    }

    static const UInt Dqs    = 0;
    static const UInt Dqt    = 1;
    static const UInt WE12a  = 2;
    static const UInt WE23a  = 3;
    static const UInt WE31a  = 4;
    static const UInt WE1M2a = 5;
    static const UInt WE2M3a = 6;
    static const UInt WE3M1a = 7;
    //
    void ModelFinn::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;
        Double dSubZoneVolume = s->getSubZoneVolume();

        if (!s->sub_zone_) {
            s->working_[Dqs]    = 0.0;
            s->working_[Dqt]    = 0.0;
            s->working_[WE12a]  = 0.0;
            s->working_[WE23a]  = 0.0;
            s->working_[WE31a]  = 0.0;
            s->working_[WE1M2a] = 0.0;
            s->working_[WE2M3a] = 0.0;
            s->working_[WE3M1a] = 0.0;
        }

        if (d==3) {
            ModelElastic::elasticTrial(s);
            Double strD = 3.0*bulk_*dEVD_;
            s->stnS_.rs11() += strD;
            s->stnS_.rs22() += strD;
            s->stnS_.rs33() += strD;
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

            // --- accumulate FINN strains
            s->working_[WE12a]  += s->stnE_.s12() * dSubZoneVolume;
            s->working_[WE23a]  += s->stnE_.s23() * dSubZoneVolume;
            s->working_[WE31a]  += s->stnE_.s13() * dSubZoneVolume;
            s->working_[WE1M2a] += (s->stnE_.s11() - s->stnE_.s22()) * dSubZoneVolume;
            s->working_[WE2M3a] += (s->stnE_.s22() - s->stnE_.s33()) * dSubZoneVolume;
            s->working_[WE3M1a] += (s->stnE_.s33() - s->stnE_.s11()) * dSubZoneVolume;

            /* --- plastic parameter incrementation and properties update --- */
            if (s->sub_zone_==s->total_sub_zones_-1) 
            {
                Double dAux = 1.0 / s->getZoneVolume();
                if (s->overlay_==2) dAux *= 0.5;

                sHP_ += s->working_[Dqs] * dAux;
                tHP_ += s->working_[Dqt] * dAux;

                if (s->working_[Dqs] > 0.0) {
                    if (iCohesion_) cohesion_ = s->getYFromX(iCohesion_,sHP_);
                    if (iFriction_) friction_ = s->getYFromX(iFriction_,sHP_);
                    if (iDilation_) dilation_ = s->getYFromX(iDilation_,sHP_);
                }
                if (s->working_[Dqt] > 0.0) {
                    if (iTension_) {
                        dAux = s->getYFromX(iTension_,tHP_);
                        tension_ = tension_ < dAux ? tension_ : dAux;
                    }
                }

                if (((iCohesion_ || iFriction_ || iDilation_) && s->working_[Dqs] > 0.0) || (iTension_ && s->working_[Dqt] > 0.0))
                    ModelMohr::updateParameters(false,&sf1_,&sf3_);

                //     Do FINN update
                dEVD_   = 0.0; // all subzones used by now
                wE12_  += s->working_[WE12a] * dAux;
                wE23_  += s->working_[WE23a] * dAux;
                wE31_  += s->working_[WE31a] * dAux;
                wE1M2_ += s->working_[WE1M2a] * dRSq6 * dAux;
                wE2M3_ += s->working_[WE2M3a] * dRSq6 * dAux;
                wE3M1_ += s->working_[WE3M1a] * dRSq6 * dAux;

                latCnt_++; // latency
                if (latCnt_ >= latency_) {
                    Double dDif1 = wE12_ - wE12P_;
                    Double dDif2 = wE23_ - wE23P_;
                    Double dDif3 = wE31_ - wE31P_;
                    Double dDif4 = wE1M2_ - wE1M2P_;
                    Double dDif5 = wE2M3_ - wE2M3P_;
                    Double dDif6 = wE3M1_ - wE3M1P_;
                    Double dDP;
                    if (!count_) // (first step)
                        dDP = - dDif1*dDif1 - dDif2*dDif2 - dDif3*dDif3 - dDif4*dDif4 - dDif5*dDif5 - dDif6*dDif6;
                    else
                        dDP =  dDif1*uV1_ + dDif2*uV2_ + dDif3*uV3_ + dDif4*uV4_ + dDif5*uV5_ + dDif6*uV6_;
                    if (dDP < 0.0) {
                        dDP = -dDP;
                        if (dDP < dPPrev_) {
                            Double dZuv =  uV1_*uV1_ + uV2_*uV2_ + uV3_*uV3_ + uV4_*uV4_ + uV5_*uV5_ + uV6_*uV6_;
                            if (dZuv > 1e-20) {
                                count_++;
                                dDP = (dDif1*uV1_ + dDif2*uV2_ + dDif3*uV3_ + dDif4*uV4_ + dDif5*uV5_ + dDif6*uV6_)/sqrt(dZuv);
                                Double dGam = fabs(dDP);
                                if (!switch_) 
                                    dDP  =  fC1_*(dGam - fC2_*eVD_) + fC3_*eVD_*eVD_/(dGam + fC4_*eVD_);
                                else {
                                    Double dGamma = dGam - fC3_;
                                    dDP  = (dGamma > 0) ? fC1_*dGamma*exp(-fC2_*eVD_/dGamma) : 0.0;
                                }
                                if (dDP < 0.0) dDP = 0.0;
                                //dDP = 0.5*dDP; MOO removed per discussion with Christine.
                                eVD_  += dDP;
                                dEVD_  = dDP / 3.0;
                            }
                            uV1_ = dDif1; uV2_ = dDif2; uV3_ = dDif3;
                            uV4_ = dDif4; uV5_ = dDif5; uV6_ = dDif6;
                            wE12P_  = wE12_;
                            wE23P_  = wE23_;
                            wE31P_  = wE31_;
                            wE1M2P_ = wE1M2_;
                            wE2M3P_ = wE2M3_;
                            wE3M1P_ = wE3M1_;
                            dPPrev_ = 0.0;
                            latCnt_ = 0.0;
                        } 
                        else
                            dPPrev_ = dDP;
                    }
                }
            } //  if (s->sub_zone_==s->total_sub_zones_-1) 
        } // end of 3D section
        else if (d==2) { // 2D Section 
            Double Tcs2=0.0,Tsi2=0.0,Tdc2=0.0,Tdss=0.0;
            iPlas    = 0;
            s->viscous_ = true;

            // Add in FINN increments
            Double dFde11 = s->stnE_.s11() + dEVD_;
            Double dFde22 = s->stnE_.s22() + dEVD_;
            Double dFde33 = s->stnE_.s33() + dEVD_;

            /* --- trial elastic stresses --- */
            s->stnS_.rs11() += dFde11 * e1_ + (dFde22 + dFde33) * e2_;
            s->stnS_.rs22() += (dFde11 + dFde33) * e2_ + dFde22 * e1_;
            s->stnS_.rs33() += (dFde11 + dFde22) * e2_ + dFde33 * e1_;
            s->stnS_.rs12() += s->stnE_.s12() * g2_;
            s->stnS_.rs13()  = 0.0;
            s->stnS_.rs23()  = 0.0;

            if (canFail()) {
                /* --- calculate and sort principal stresses and principal directions --- */
                Double dPrinMin,dPrinMid,dPrinMax;
                UInt icase=0;

                Double Tsdif  = s->stnS_.s11() - s->stnS_.s22();
                Double Ts0    = (s->stnS_.s11() + s->stnS_.s22()) / 2.0;
                Double Trad   = sqrt(Tsdif*Tsdif + 4.0*s->stnS_.s12()*s->stnS_.s12()) / 2.0;
                Double Tsi    = Ts0 - Trad;
                Double Tsii   = Ts0 + Trad;
                Double Tpsdif = Tsi - Tsii;

                if (s->stnS_.s33() > Tsii) {
                    //--- s33 is minor p.s. ---
                    icase    = 3;
                    dPrinMin = Tsi;
                    dPrinMid = Tsii;
                    dPrinMax = s->stnS_.s33();
                } else if (s->stnS_.s33() < Tsi) {
                    //--- s33 is major p.s. ---
                    icase    = 2;
                    dPrinMin = s->stnS_.s33();
                    dPrinMid = Tsi;
                    dPrinMax = Tsii;
                } else {
                    //--- s33 is intermediate ---
                    icase    = 1;
                    dPrinMin = Tsi;
                    dPrinMid = s->stnS_.s33();
                    dPrinMax = Tsii;
                }

                /* --- Mohr-Coulomb failure criterion --- */
                Double dFSurf  =  nph_ * dPrinMax - dPrinMin - csn_;
                Double dFSurfd = dFSurf / rc_;
                /* --- Tensile failure criteria --- */
                Double dTSurf1 =  dPrinMin - tension_;
                Double dTSurf3 =  dPrinMax - tension_;
                /* --- tests for failure */
                if (dTSurf3 <= 0.) {
                    if (dFSurfd > 0.) {
                        iPlas = 1;
                        /* --- shear failure: correction to principal stresses ---*/
                        s->state_ |= shear_now;
                        dPrinMin += dFSurf * sc1_;
                        dPrinMid += dFSurf * sc2_;
                        dPrinMax += dFSurf * sc3_;
                    }
                } else if (dTSurf1 <= 0) {
                    if (dFSurfd > 0 && dFSurfd > dTSurf3) {
                        iPlas = 1;
                        /* --- shear failure: correction to principal stresses ---*/
                        s->state_ |= shear_now;
                        dPrinMin += dFSurf * sc1_;
                        dPrinMid += dFSurf * sc2_;
                        dPrinMax += dFSurf * sc3_;
                    } else {
                        iPlas = 2;
                        /* --- tension failure: correction to principal stresses ---*/
                        s->state_ |= tension_now;
                        Double dTco = e2_ * dTSurf3 / e1_;
                        dPrinMin -= dTco;
                        dPrinMid -= dTco;
                        dPrinMax  = tension_;
                    }
                } else {
                    iPlas = 2;
                    /* --- tension failure: correction to principal stresses ---*/
                    s->state_ |= tension_now;
                    Double dTco = e2_ * dTSurf3 / e1_;
                    dPrinMin -= dTco;
                    dPrinMid -= dTco;
                    dPrinMax  = tension_;
                }

                if (iPlas) {
                    //--- direction cosines ---
                    if (Tpsdif == 0.0) {
                        Tcs2 = 1.0;
                        Tsi2 = 0.0;
                    } else {
                        Tcs2 = Tsdif / Tpsdif;
                        Tsi2 = 2.0 * s->stnS_.s12() / Tpsdif;
                    }
                    //--- resolve back to global axes ---
                    switch (icase) {
                    case 1:
                        Tdc2  = (dPrinMin - dPrinMax) * Tcs2;
                        Tdss  =  dPrinMin + dPrinMax;
                        s->stnS_.rs11()  = 0.5 * (Tdss + Tdc2);
                        s->stnS_.rs22()  = 0.5 * (Tdss - Tdc2);
                        s->stnS_.rs12()  = 0.5 * (dPrinMin  - dPrinMax) * Tsi2;
                        s->stnS_.rs33()  = dPrinMid;
                        break;
                    case 2:
                        Tdc2  = (dPrinMid - dPrinMax) * Tcs2;
                        Tdss  =  dPrinMid + dPrinMax;
                        s->stnS_.rs11()  = 0.5 * (Tdss + Tdc2);
                        s->stnS_.rs22()  = 0.5 * (Tdss - Tdc2);
                        s->stnS_.rs12()  = 0.5 * (dPrinMid  - dPrinMax) * Tsi2;
                        s->stnS_.rs33()  = dPrinMin;
                        break;
                    case 3:
                        Tdc2  = (dPrinMin - dPrinMid) *Tcs2;
                        Tdss  =  dPrinMin + dPrinMid;
                        s->stnS_.rs11()  = 0.5 * (Tdss + Tdc2);
                        s->stnS_.rs22()  = 0.5 * (Tdss - Tdc2);
                        s->stnS_.rs12()  = 0.5 * (dPrinMin  - dPrinMid) * Tsi2;
                        s->stnS_.rs33()  = dPrinMax;
                        break;
                    }
                    s->viscous_ = false;
                    /* --- hardening parameter accumulation --- */
                    if (iPlas == 1) {
                        /* --- shear parameter --- */
                        Double dDE1p = -dFSurf * sf1_;
                        Double dDE3p = -dFSurf * sf3_;
                        Double dDEPa = d1d3 * (dDE1p + dDE3p);
                        dDE1p -= dDEPa;
                        dDE3p -= dDEPa;
                        s->working_[Dqs]  += sqrt(0.5 * (dDE1p*dDE1p+dDEPa*dDEPa+dDE3p*dDE3p)) * s->getSubZoneVolume();
                    }
                    if (iPlas == 2) {
                        /* --- tensile parameter --- */
                        Double dAux = -dTSurf3 / e1_;
                        if (dAux < 0.) dAux = -dAux;
                        s->working_[Dqt]  += dAux * s->getSubZoneVolume();
                    }
                }
            }
            // --- accumulate FINN strains

            s->working_[WE12a]  += s->stnE_.s12() * s->getSubZoneVolume();
            s->working_[WE23a]   = 0.0;
            s->working_[WE31a]   = 0.0;
            s->working_[WE1M2a] += (s->stnE_.s11() - s->stnE_.s22()) * s->getSubZoneVolume();
            s->working_[WE2M3a]  = 0.0;
            s->working_[WE3M1a]  = 0.0;

            /* --- plastic parameter incrementation and properties update --- */
            if (s->sub_zone_==s->total_sub_zones_-1) {
                Double dAux = 1.0 / s->getZoneVolume();
                if (s->overlay_==2) dAux = dAux * 0.5;
                sHP_ += s->working_[Dqs] * dAux;
                tHP_ += s->working_[Dqt] * dAux;
                if (s->working_[Dqs] > 0.0) {
                    if (iCohesion_) {
                        cohesion_ = s->getYFromX(iCohesion_,sHP_);
                    }
                    if (iFriction_) {
                        friction_ = s->getYFromX(iFriction_,sHP_);
                    }
                    if (iDilation_) {
                        dilation_ = s->getYFromX(iDilation_,sHP_);
                    }
                }
                Double dRSin = sin(friction_ * degrad);
                nph_  = (1.0 + dRSin) / (1.0 - dRSin);
                csn_  = 2.0 * cohesion_ * sqrt(nph_);
                if (friction_) {
                    Double dApex = cohesion_ * cos(friction_ * degrad) / dRSin;
                    tension_ = tension_ < dApex ? tension_ : dApex;
                }
                dRSin = sin(dilation_ * degrad);
                Double rnps_ = (1.0 + dRSin) / (1.0 - dRSin);
                Double dRa   = e1_ - rnps_ * e2_;
                Double dRb   = e2_ - rnps_ * e1_;
                Double dRd   = dRa - dRb * nph_;
                sc1_ = dRa / dRd;
                sc3_ = dRb / dRd;
                sc2_ = e2_ * (1.0 - rnps_) / dRd;
                sf1_ = 1.0 / dRd;
                sf3_ = -rnps_ / dRd;
                rc_  = sqrt(1.0 + nph_ * nph_);
                if (s->working_[Dqt] > 0.0) {
                    if (iTension_)  tension_ = s->getYFromX(iTension_,tHP_);
                }
                // Do FINN update
                dEVD_   = 0.0; // all subzones used by now
                wE12_  += s->working_[WE12a] * dAux;
                wE23_  += s->working_[WE23a] * dAux;
                wE31_  += s->working_[WE31a] * dAux;
                wE1M2_ += s->working_[WE1M2a] * dRSq6 * dAux;
                wE2M3_ += s->working_[WE2M3a] * dRSq6 * dAux;
                wE3M1_ += s->working_[WE3M1a] * dRSq6 * dAux;

                latCnt_++; // latency
                if (latCnt_ >= latency_) {
                    Double dDif1 = wE12_ - wE12P_;
                    Double dDif2 = wE23_ - wE23P_;
                    Double dDif3 = wE31_ - wE31P_;
                    Double dDif4 = wE1M2_ - wE1M2P_;
                    Double dDif5 = wE2M3_ - wE2M3P_;
                    Double dDif6 = wE3M1_ - wE3M1P_;
                    Double dDP;
                    if (!count_) // (first step)
                        dDP = - dDif1*dDif1 - dDif2*dDif2 - dDif3*dDif3 - dDif4*dDif4 - dDif5*dDif5 - dDif6*dDif6;
                    else
                        dDP =  dDif1*uV1_ + dDif2*uV2_ + dDif3*uV3_ + dDif4*uV4_ + dDif5*uV5_ + dDif6*uV6_;
                    if (dDP < 0.0) {
                        dDP = -dDP;
                        if (dDP < dPPrev_) {
                            Double dZuv =  uV1_*uV1_ + uV2_*uV2_ + uV3_*uV3_ + uV4_*uV4_ + uV5_*uV5_ + uV6_*uV6_;
                            if (dZuv > 1e-20) {
                                count_++;
                                dDP = (dDif1*uV1_ + dDif2*uV2_ + dDif3*uV3_ + dDif4*uV4_ + dDif5*uV5_ + dDif6*uV6_)/sqrt(dZuv);
                                Double dGam = fabs(dDP);
                                if (!switch_) {
                                    dDP =  fC1_*(dGam - fC2_*eVD_) + fC3_*eVD_*eVD_/(dGam + fC4_*eVD_);
                                } else {
                                    Double dGamma = dGam - fC3_;
                                    dDP  = (dGamma > 0) ? fC1_*dGamma*exp(-fC2_*eVD_/dGamma) : 0.0;
                                }
                                if (dDP < 0.0) dDP = 0.0;
                                //dDP = 0.5*dDP;  MOO removed per discussion with Christine.
                                eVD_  += dDP;
                                dEVD_  = dDP / 3.0;
                            }
                            uV1_ = dDif1; uV2_ = dDif2; uV3_ = dDif3;
                            uV4_ = dDif4; uV5_ = dDif5; uV6_ = dDif6;
                            wE12P_  = wE12_;
                            wE23P_  = wE23_;
                            wE31P_  = wE31_;
                            wE1M2P_ = wE1M2_;
                            wE2M3P_ = wE2M3_;
                            wE3M1P_ = wE3M1_;
                            dPPrev_ = 0.0;
                            latCnt_ = 0.0;
                        } else
                            dPPrev_ = dDP;
                    }
                }
            }
        } // end of 2D section
    }
} // namespace models
// EOF