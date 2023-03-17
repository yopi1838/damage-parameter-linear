#include "modelswell.h"
#include "base/src/version.h"

namespace models {

    ModelSwell::ModelSwell() :
        a1_(0.0),c1_(0.0),a3_(0.0),c3_(0.0),m1_(0.0),m3_(0.0),pressure_(0.0),sv0_(0.0),
        dsxx_(0.0),dsyy_(0.0),dszz_(0.0),dsxy_(0.0),dsyz_(0.0),dszx_(0.0),
        ninc_(1),cinc_(0),modnum_(1) { 
    }

    UInt ModelSwell::getMinorVersion() const {
        return version::update;
    }

    String ModelSwell::getProperties(void) const {
        return ModelMohr::getProperties() + L"," +  ModelWeakplaneType::getProperties() + 
            L",constant-a-1,constant-c-1,constant-a-3,constant-c-3,constant-m-1,constant-m-3"
            L",pressure-reference,stress-local-vertical"
            L",stress-swelling-xx,stress-swelling-yy,stress-swelling-zz"
            L",stress-swelling-xy,stress-swelling-yz,stress-swelling-xz"
            L",count-swelling,number-start,flag-swell";
    }

    Variant ModelSwell::getProperty(UInt index) const {
        if (index <= 9)
            return ModelMohr::getProperty(index);
        else {
            switch (index) {
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15: 
            case 16: return ModelWeakplaneType::getProperty(index-9);
            case 17: return(a1_);
            case 18: return(c1_);
            case 19: return(a3_);
            case 20: return(c3_);
            case 21: return(m1_);
            case 22: return(m3_);
            case 23: return(pressure_);
            case 24: return(sv0_);
            case 25: return(dsxx_);
            case 26: return(dsyy_);
            case 27: return(dszz_);
            case 28: return(dsxy_);
            case 29: return(dsyz_);
            case 30: return(dszx_);          
            case 31: return(cinc_);
            case 32: return(ninc_);
            case 33: return(modnum_);
            }
        }
        return(0.0);
    }

    void ModelSwell::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 9)
            ModelMohr::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16: ModelWeakplaneType::setProperty(index-9,p,restoreVersion); break;
            case 17: a1_= p.toDouble(); break;
            case 18: c1_= p.toDouble(); break;
            case 19: a3_= p.toDouble(); break;
            case 20: c3_= p.toDouble(); break;
            case 21: m1_= p.toDouble(); break;
            case 22: m3_= p.toDouble(); break;
            case 23: pressure_= p.toDouble(); break;
            case 24: sv0_= p.toDouble(); break;
            case 25: dsxx_= p.toDouble(); break;
            case 26: dsyy_= p.toDouble(); break;
            case 27: dszz_= p.toDouble(); break;
            case 28: dsxy_= p.toDouble(); break;
            case 29: dsyz_= p.toDouble(); break;
            case 30: dszx_= p.toDouble(); break;
            case 31: cinc_= p.toInt(); break;
            case 32: ninc_= p.toInt(); break;     
            case 33: modnum_= p.toInt(); break;
            }
        }
    }

    bool ModelSwell::isPropertyReadOnly(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyReadOnly(i);
        else if (i >= 25 && i <= 31)
            return true;
        return false;
    }

    bool ModelSwell::isPropertyAdvanced(UInt i) const {
        if (i <= 9)
            return ModelMohr::isPropertyAdvanced(i);
        else if (i == 32 || i == 33) 
            return true;
        return false;
    }

    void ModelSwell::copy(const ConstitutiveModel *m) {
        const ModelSwell *em = dynamic_cast<const ModelSwell *>(m);
        if (!em) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        //
        dip_      = em->dip_;
        dd_       = em->dd_;
        norma_    = norma_;
        angle_    = em->angle_;

        a1_       = em->a1_;
        c1_       = em->c1_;
        a3_       = em->a3_;
        c3_       = em->c3_;
        m1_       = em->m1_;
        m3_       = em->m3_;
        pressure_ = em->pressure_;
        sv0_      = em->sv0_;
        dsxx_     = em->dsxx_;
        dsyy_     = em->dsyy_;
        dszz_     = em->dszz_;
        dsxy_     = em->dsxy_;
        dsyz_     = em->dsyz_;
        dszx_     = em->dszx_;       
        cinc_     = em->cinc_;
        ninc_     = em->ninc_;
        modnum_   = em->modnum_;
    }


    void ModelSwell::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        if ((d!=2)&&(d!=3)) throw std::runtime_error("Illegal dimension in ModelSwell");
        
        if (pressure_ <= 0.0) 
            throw std::runtime_error("Swell model: pressure-reference should be greater than zero.");

        if (ninc_ <= 0.0) 
            throw std::runtime_error("Swell model: number-start should be greater than zero.");

        Double dLenTemp = norma_.norm().mag();
        normX_ = norma_.norm().x();
        normY_ = norma_.norm().y();
        normZ_ = norma_.norm().z();
        if (dLenTemp) {
            normX_ /= dLenTemp;
            normY_ /= dLenTemp;
            normZ_ /= dLenTemp;
        }

        if (friction_) {
            Double dVal = tan(friction_ * degrad);
            Double dApex = cohesion_ / dVal;
            tension_ = tension_ < dApex ? tension_ : dApex;
        }
        else if (cohesion_ == 0.0)
            tension_ = 0.0;

        ModelMohr::updateParameters(false);
    }

    void ModelSwell::run(UByte d, State *s)
    {
        if (!isValid(d)) initialize(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;

        ModelElastic::elasticTrial(s);

        if (!sv0_) {
            //swelling logic
            DVect3 normv;
            normv.rx() = normX_;
            normv.ry() = normY_;
            normv.rz() = normZ_;
            SymTensor str = s->getEffectiveZoneStress();
            /* --- local components of traction vector on weak plane --- */
            Double dB3x  = str.s11() * normv.x() + str.s12() * normv.y() + str.s13() * normv.z();
            Double dB3y  = str.s12() * normv.x() + str.s22() * normv.y() + str.s23() * normv.z();
            Double dB3z  = str.s13() * normv.x() + str.s23() * normv.y() + str.s33() * normv.z();
            sv0_  = (normv.x()*dB3x) + (normv.y()*dB3y) + (normv.z()*dB3z) - s->getAveragePP();
            Double exx(0.0),eyy(0.0),ezz(0.0);
            if (sv0_ >= 0.0) {
                ezz = m1_;
                exx = m3_;
                eyy = exx;
            }
            else {
                if (modnum_== 1) {
                    if (a1_ > 0.0)
                        ezz = c1_*std::log10(a1_*(-1.0*sv0_/pressure_));
                    if (a3_ > 0.0)
                        exx = c3_*std::log10(a3_*(-1.0*sv0_/pressure_));
                    eyy = exx;
                }
                else if (modnum_== 2) {
                    ezz = c1_ - a1_*sv0_/pressure_;
                    exx = c3_ - a3_*sv0_/pressure_;
                    eyy = exx;
                }
                else
                    throw std::runtime_error("Swell model: illegal flag-swell.");
                if (m1_ > 0.0) ezz = std::min(ezz,m1_);
                if (m3_ > 0.0) {
                    exx = std::min(exx,m3_);
                    eyy = std::min(eyy,m3_);
                }
            }
            Double dsxx = -1.0*(e1_*exx + e2_*(eyy + ezz));
            Double dsyy = -1.0*(e1_*eyy + e2_*(ezz + exx));
            Double dszz = -1.0*(e1_*ezz + e2_*(exx + eyy));
            const Axes3D aAxes = norma_.axes();
            dsxx_ = aAxes.e1().x() * aAxes.e1().x() * dsxx + aAxes.e2().x() * aAxes.e2().x() * dsyy + aAxes.e3().x() * aAxes.e3().x() * dszz;
            dsxy_ = aAxes.e1().x() * aAxes.e1().y() * dsxx + aAxes.e2().x() * aAxes.e2().y() * dsyy + aAxes.e3().x() * aAxes.e3().y() * dszz;
            dszx_ = aAxes.e1().x() * aAxes.e1().z() * dsxx + aAxes.e2().x() * aAxes.e2().z() * dsyy + aAxes.e3().x() * aAxes.e3().z() * dszz;
            dsyy_ = aAxes.e1().y() * aAxes.e1().y() * dsxx + aAxes.e2().y() * aAxes.e2().y() * dsyy + aAxes.e3().y() * aAxes.e3().y() * dszz;
            dsyz_ = aAxes.e1().y() * aAxes.e1().z() * dsxx + aAxes.e2().y() * aAxes.e2().z() * dsyy + aAxes.e3().y() * aAxes.e3().z() * dszz;
            dszz_ = aAxes.e1().z() * aAxes.e1().z() * dsxx + aAxes.e2().z() * aAxes.e2().z() * dsyy + aAxes.e3().z() * aAxes.e3().z() * dszz;
        }

        if (cinc_ < ninc_) {
            s->stnS_.rs11() += dsxx_/ninc_;
            s->stnS_.rs12() += dsxy_/ninc_;
            s->stnS_.rs13() += dszx_/ninc_;
            s->stnS_.rs22() += dsyy_/ninc_;
            s->stnS_.rs23() += dsyz_/ninc_;
            s->stnS_.rs33() += dszz_/ninc_;
            if (s->sub_zone_==s->total_sub_zones_-1) cinc_++;
        }

        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        Double fs  = - prin.x() + nph_*prin.z() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        if (fsd > 0.0 && fsd >= ftz)
            ModelMohr::shearCorrection(s,&prin,&iPlas,fs);
        else if (ftz > 0.0 && ftz >= fsd)
            ModelMohr::tensionCorrection(s,&prin,&iPlas,ftz,brittle_);
        ModelMohr::apexCorrection(friction_,s,&prin,&iPlas,brittle_);

        if (iPlas) {
            s->stnS_ = info.resolve(prin);
            s->viscous_ = false;
        }
    }
}//namespace models
// EOF