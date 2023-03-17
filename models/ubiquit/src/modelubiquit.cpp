#include "modelubiquit.h"
#include "base/src/version.h"

namespace models {

    ModelUbiquitous::ModelUbiquitous(unsigned short option) : ModelMohr(option), ModelWeakplaneType(), 
        jCohesion_(0.0),jFriction_(0.0),jDilation_(0.0),jTension_(0.0),
        normX_(0.0),normY_(0.0),normZ_(0.0),
        sig_(0.0),tau_(0.0),sp13_(0.0),sp23_(0.0),dsp11_(0.0),dsp33_(0.0),dsp13_(0.0),dsp23_(0.0), 
        qphi_(0.0),qpsi_(0.0),jrc_(0.0),jc1_(0.0),jc2_(0.0) {
    }

    UInt ModelUbiquitous::getMinorVersion() const {
        return version::update;
    }

    String ModelUbiquitous::getProperties(void) const {
        return ModelMohr::getProperties() 
            + L",joint-cohesion,joint-friction,joint-dilation,joint-tension,"
            + ModelWeakplaneType::getProperties();       
    }

    String ModelUbiquitous::getStates(void) const {
        return ModelMohr::getStates() + L",j-shear-n,j-tension-n,j-shear-p,j-tension-p";
    }

    Variant ModelUbiquitous::getProperty(UInt index) const {
        if (index <= 9)
            return ModelMohr::getProperty(index);
        else {
            switch (index) {
            case 10: return(jCohesion_);
            case 11: return(jFriction_);
            case 12: return(jDilation_);
            case 13: return(jTension_);
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20: return ModelWeakplaneType::getProperty(index-13);
            }
            return(0.0);
        }
    }

    void ModelUbiquitous::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 9)
            ModelMohr::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 10: jCohesion_ = p.toDouble();  break;
            case 11: jFriction_ = p.toDouble();  break;
            case 12: jDilation_ = p.toDouble();  break;
            case 13: jTension_ = p.toDouble();   break;
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20: ModelWeakplaneType::setProperty(index-13,p,restoreVersion); break;
            }
        }
    }

    void ModelUbiquitous::copy(const ConstitutiveModel *m) {
        const ModelUbiquitous *mm = dynamic_cast<const ModelUbiquitous *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelMohr::copy(m);
        jCohesion_= mm->jCohesion_;
        jFriction_= mm->jFriction_;
        jDilation_= mm->jDilation_;
        jTension_ = mm->jTension_;
        dip_      = mm->dip_;
        dd_       = mm->dd_;
        norma_    = mm->norma_;
        angle_    = mm->angle_;
    }


    void ModelUbiquitous::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        if ((d!=2)&&(d!=3)) throw std::runtime_error("Illegal dimension in the ubiquitous-joint model");

        ModelMohr::updateParameters();
        updateJointParameters(true);
    }

    void ModelUbiquitous::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            ModelMohr::moduliReduction(s->modulus_reduction_factor_);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;

        if (s->state_ & joint_shear_now) s->state_ |= joint_shear_past;
        s->state_ &= ~joint_shear_now;
        if (s->state_ & joint_tension_now) s->state_ |= joint_tension_past;
        s->state_ &= ~joint_tension_now;
        UInt jPlas = 0;

        ModelElastic::elasticTrial(s);
        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        Double fs  = - prin.x() + nph_ * prin.z() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        if (fsd > 0.0 && fsd >= ftz)
            ModelMohr::shearCorrection(s,&prin,&iPlas,fs);
        else if (ftz > 0.0 && ftz >= fsd)
            ModelMohr::tensionCorrection(s,&prin,&iPlas,ftz,brittle_);
        ModelMohr::apexCorrection(friction_,s,&prin,&iPlas,brittle_);

        if (iPlas)
            s->stnS_ = info.resolve(prin);

        stressToLocal(s, norma_.axes());

        dsp11_ = 0.0; dsp33_=0.0; dsp13_=0.0; dsp23_=0.0;
        Double fjt = sig_ - jTension_;
        Double fjs = tau_ + sig_*qphi_ - jCohesion_;
        Double fjd = fjs / jrc_;
        if (fjs > 0.0 && fjd >= fjt)
            shearCorrectionJoint(s,&jPlas,fjs);
        else if (fjt > 0.0 && fjt >= fjd)
            tensionCorrectionJoint(s,&jPlas,fjt);
        apexCorrectionJoint(s,&jPlas);

        if (jPlas > 0) 
            stressToGlobal(s, norma_.axes());

        if (s->isLarge()) 
            largeStrainCorrection(s, norma_.axes());

        if (iPlas || jPlas)
            s->viscous_ = false;
    }

    void ModelUbiquitous::scaleProperties(const Double &scale,const std::vector<UInt> &props) {
        for (UInt u=0;u<props.size();++u) {
            switch (props[u]) {
            case  5: cohesion_ *= scale;  break;
            case  6: friction_ = std::max(0.0,std::min(89.0,std::atan(std::tan(friction_*degrad)*scale)/degrad));  break;
            case  7: dilation_ = std::max(0.0,std::min(89.0,std::atan(std::tan(dilation_*degrad)*scale)/degrad));  break;
            case  8: tension_  *= scale;  break;
            case 10: jCohesion_ *= scale;  break;
            case 11: jFriction_ = std::max(0.0,std::min(89.0,std::atan(std::tan(jFriction_*degrad)*scale)/degrad));  break;
            case 12: jDilation_ = std::max(0.0,std::min(89.0,std::atan(std::tan(jDilation_*degrad)*scale)/degrad));  break;
            case 13: jTension_  *= scale;  break;
            }
            setValid(0);
        }
    }

    bool ModelUbiquitous::updateJointParameters(bool bEUpdated) {
        if (!bEUpdated) ModelElastic::updateParameters();

        if (jCohesion_ < 0.0)
            throw std::runtime_error("Ubiquitous-Joint type model: joint cohesion is not allowed less than 0.");

        if (jFriction_ >= 90.0 || jFriction_ < 0.0)
            throw std::runtime_error("Ubiquitous-Joint type model: joint friction angle is not in the valid range of 0 to 90.");

        if (jDilation_ > jFriction_)
            throw std::runtime_error("Ubiquitous-Joint type model: joint dilationn angle is not allowed greater than joint friction angle.");

        qphi_ = std::tan(jFriction_ * degrad);
        qpsi_ = std::tan(jDilation_ * degrad);
        jc1_ = 1. / (g2_ + e1_ * qphi_ * qpsi_);
        jc2_ = jc1_ * qpsi_;
        jrc_ = std::sqrt(1. + qphi_ * qphi_);

        if (jFriction_) {
            Double dapex = jCohesion_ / qphi_;
            jTension_ = jTension_ < dapex ? jTension_ : dapex;
        }
        else if (jCohesion_ == 0.0)
            jTension_ = 0.0;

        Double dLenTemp = norma_.norm().mag(); 
        if (dLenTemp==0.0) {
            DVect3 norm = Orientation3::getNormFromDipDD(dip_*degrad,dd_*degrad);
            norma_.set_norm(norm);
            angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
            dLenTemp = norma_.norm().mag();
        }
        normX_ = norma_.norm().x();
        normY_ = norma_.norm().y();
        normZ_ = norma_.norm().z();
        if (dLenTemp) {
            normX_ /= dLenTemp;
            normY_ /= dLenTemp;
            normZ_ /= dLenTemp;
        }
        return !bEUpdated;
    }

    void ModelUbiquitous::stressToLocal(State *s,const Axes3D &aAxes) {
        Double e1x = aAxes.e1().x();
        Double e1y = aAxes.e1().y();
        Double e1z = aAxes.e1().z();
        Double e2x = aAxes.e2().x();
        Double e2y = aAxes.e2().y();
        Double e2z = aAxes.e2().z();
        Double e3x = aAxes.e3().x();
        Double e3y = aAxes.e3().y();
        Double e3z = aAxes.e3().z();
        Double dB3x  = s->stnS_.s11()*e3x + s->stnS_.s12()*e3y + s->stnS_.s13()*e3z;
        Double dB3y  = s->stnS_.s12()*e3x + s->stnS_.s22()*e3y + s->stnS_.s23()*e3z;
        Double dB3z  = s->stnS_.s13()*e3x + s->stnS_.s23()*e3y + s->stnS_.s33()*e3z;
        sp13_ = e1x*dB3x + e1y*dB3y + e1z*dB3z;
        sp23_ = e2x*dB3x + e2y*dB3y + e2z*dB3z;
        sig_  = e3x*dB3x + e3y*dB3y + e3z*dB3z;
        tau_  = std::sqrt(sp13_*sp13_ + sp23_*sp23_);
    }

    void ModelUbiquitous::stressToGlobal(State *s,const Axes3D &aAxes) {
        Double e1x = aAxes.e1().x();
        Double e1y = aAxes.e1().y();
        Double e1z = aAxes.e1().z();
        Double e2x = aAxes.e2().x();
        Double e2y = aAxes.e2().y();
        Double e2z = aAxes.e2().z();
        Double e3x = aAxes.e3().x();
        Double e3y = aAxes.e3().y();
        Double e3z = aAxes.e3().z();
        Double dB1x = dsp11_*e1x + dsp13_*e3x;
        Double dB2x = dsp11_*e1y + dsp13_*e3y;
        Double dB3x = dsp11_*e1z + dsp13_*e3z;
        Double dB1y = dsp11_*e2x + dsp23_*e3x;
        Double dB2y = dsp11_*e2y + dsp23_*e3y;
        Double dB3y = dsp11_*e2z + dsp23_*e3z;
        Double dB1z = dsp13_*e1x + dsp23_*e2x + dsp33_*e3x;
        Double dB2z = dsp13_*e1y + dsp23_*e2y + dsp33_*e3y;
        Double dB3z = dsp13_*e1z + dsp23_*e2z + dsp33_*e3z;
        s->stnS_.rs11() += e1x*dB1x + e2x*dB1y + e3x*dB1z;
        s->stnS_.rs12() += e1x*dB2x + e2x*dB2y + e3x*dB2z;
        s->stnS_.rs13() += e1x*dB3x + e2x*dB3y + e3x*dB3z;
        s->stnS_.rs22() += e1y*dB2x + e2y*dB2y + e3y*dB2z;
        s->stnS_.rs23() += e1y*dB3x + e2y*dB3y + e3y*dB3z;
        s->stnS_.rs33() += e1z*dB3x + e2z*dB3y + e3z*dB3z;
    }

    void ModelUbiquitous::apexCorrectionJoint(State *s,UInt *jPlasticity) {
        if (jFriction_ > 0.0) {
            Double japex = jCohesion_ / tan(jFriction_*degrad);
            if ((sig_ + dsp33_)  > japex) { 
                s->state_ |= joint_tension_now;
                *jPlasticity = 2;
                dsp33_ = japex - sig_;
                dsp13_ = - sp13_;
                dsp23_ = - sp23_;
                dsp11_ = dsp33_ * e2_/e1_;
            }
        }
    }

    void ModelUbiquitous::tensionCorrectionJoint(State *s,UInt *jPlasticity,const Double &fjt) {
        s->state_ |= joint_tension_now;
        if(jPlasticity) *jPlasticity = 2;
        dsp33_ = -fjt;
        dsp11_ = dsp33_ * e2_/e1_;
    }

    void ModelUbiquitous::shearCorrectionJoint(State *s,UInt *jPlasticity,const Double &fjs) {
        s->state_ |= joint_shear_now;
        if(jPlasticity) *jPlasticity = 1;
        Double dRat = 0.0;
        if (tau_ > 1.0e-6 * jCohesion_)
            dRat = fjs * jc1_ * g2_ / tau_;
        dsp13_ = - dRat * sp13_;
        dsp23_ = - dRat * sp23_;
        dsp33_ = - fjs * jc2_ * e1_;
        dsp11_ = - fjs * jc2_ * e2_;
    }

    static const UInt StackX = 10;
    static const UInt StackY = 11;
    static const UInt StackZ = 12;
    //
    void ModelUbiquitous::largeStrainCorrection(State *s, const Axes3D &aAxes) {
        if (!s->sub_zone_) {
            s->working_[StackX] = 0.0;
            s->working_[StackY] = 0.0;
            s->working_[StackZ] = 0.0;
        }
        DVect3 rot = s->getRotation();
        Double e1x = aAxes.e1().x();
        Double e1y = aAxes.e1().y();
        Double e1z = aAxes.e1().z();
        Double e2x = aAxes.e2().x();
        Double e2y = aAxes.e2().y();
        Double e2z = aAxes.e2().z();
        Double e3x = aAxes.e3().x();
        Double e3y = aAxes.e3().y();
        Double e3z = aAxes.e3().z();
        s->working_[StackX] +=   (rot.x())*e3y + (rot.y())*e3z;
        s->working_[StackY] += - (rot.x())*e3x + (rot.z())*e3z;
        s->working_[StackZ] += - (rot.y())*e3x - (rot.z())*e3y;
        Double dB3x = s->stnE_.s11()*e3x + s->stnE_.s12()*e3y + s->stnE_.s13()*e3z;
        Double dB3y = s->stnE_.s12()*e3x + s->stnE_.s22()*e3y + s->stnE_.s23()*e3z;
        Double dB3z = s->stnE_.s13()*e3x + s->stnE_.s23()*e3y + s->stnE_.s33()*e3z;
        Double dSp13 = dB3x*e1x + dB3y*e1y + dB3z*e1z;
        Double dSp23 = dB3x*e2x + dB3y*e2y + dB3z*e2z;
        s->working_[StackX] -= (dSp13*e1x + dSp23*e2x);
        s->working_[StackY] -= (dSp13*e1y + dSp23*e2y);
        s->working_[StackZ] -= (dSp13*e1z + dSp23*e2z);
        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double dAux = 1.0 / s->total_sub_zones_;
            normX_ += s->working_[StackX] * dAux;
            normY_ += s->working_[StackY] * dAux;
            normZ_ += s->working_[StackZ] * dAux;
            norma_.set_norm(DVect3(normX_, normY_, normZ_));
            DVect2 dv2 = Orientation3::getDipDDFromNorm(DVect3(normX_,normY_,normZ_));
            dip_ = dv2.x() / degrad;
            dd_  = dv2.y() / degrad;
            angle_ = Orientation3::getJointAngleFromNorm(DVect3(normX_,normY_,normZ_)) / degrad;
        }
    }

}//namespace models
// EOF
