#include "modeldrucker.h"
#include "base/src/version.h"

namespace models {

    ModelDrucker::ModelDrucker() : qPhi_(0.0), kPhi_(0.0), qPsi_(0.0), tension_(0.0), 
        gKQ_(0.0), kQ_(0.0), rc_(0.0), tau_(0.0), sig_(0.0) {
    }

    UInt ModelDrucker::getMinorVersion() const {
        return version::update;
    }

    String ModelDrucker::getProperties(void) const {
        return ModelElastic::getProperties() + 
               L",friction-drucker,cohesion-drucker,dilation-drucker,tension";
    }

    String ModelDrucker::getStates(void) const {
        return L"shear-n,tension-n,shear-p,tension-p";
    }

    Variant ModelDrucker::getProperty(UInt index) const {
        if (index <= 4)
            return ModelElastic::getProperty(index);
        else {
            switch (index) {  
            case 5:  return(qPhi_);
            case 6:  return(kPhi_);
            case 7:  return(qPsi_);
            case 8:  return(tension_);
            }
        }
        return(0.0);
    }

    void ModelDrucker::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 4)
            ModelElastic::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 5: qPhi_ = p.toDouble();  break;
            case 6: kPhi_ = p.toDouble();  break;
            case 7: qPsi_ = p.toDouble();  break;
            case 8: tension_ = p.toDouble();  break;
            }
        }
    }

    void ModelDrucker::copy(const ConstitutiveModel *m) {
        const ModelDrucker *mm = dynamic_cast<const ModelDrucker *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelElastic::copy(m);
        qPhi_ = mm->qPhi_;
        kPhi_ = mm->kPhi_;
        qPsi_ = mm->qPsi_;
        tension_ = mm->tension_;
    }


    void ModelDrucker::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        if (shear_ <= 0.0 || bulk_ <= 0.0)
            throw std::runtime_error("Drucker Model - Invalid modulus.");

        updateParameters();
    }

    void ModelDrucker::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);
        if ((d!=3)&&(d!=2)) throw std::runtime_error("Illegal dimension in ModelDrucker");

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;

        ModelElastic::elasticTrial(s);

        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensor dev(0.0);
        Double I1(0.0);
        Double J2 = s->stnS_.getJ2(&dev, &I1);
        sig_ = I1/3.0;
        tau_ = std::sqrt(J2);
        Double fs  = qPhi_*sig_ + tau_ - kPhi_;
        Double fsd = fs / rc_;
        Double ft  = sig_ - tension_;
        if (fsd > 0.0 && fsd >= ft)
            shearCorrection(s,&iPlas,fs,dev);
        else if (ft > 0.0 && ft >= fsd)
            ModelDrucker::tensionCorrection(s,&iPlas,ft);
        apexCorrection(s,&iPlas);

        if (iPlas) s->viscous_ = false;
    }

    bool ModelDrucker::updateParameters()
    {
        ModelElastic::updateParameters();

        kQ_ = bulk_ * qPsi_;
        gKQ_ = shear_ + kQ_ * qPhi_;

        rc_ = std::sqrt(qPhi_*qPhi_ + 1.0);
        Double dApex = tension_;
        if (qPhi_) dApex = kPhi_ / qPhi_;
        else if (kPhi_ == 0.0) dApex = 0.0;
        tension_ = dApex < tension_ ? dApex : tension_;

        return true;
    }

    Double ModelDrucker::moduliReduction(const Double &factor)
    {
        Double shear_new = ModelElastic::moduliReduction(factor);
        gKQ_ = shear_new + kQ_*qPhi_;
        return shear_new;
    }

    void ModelDrucker::apexCorrection(State *s,UInt *iPlasticity)
    {
        if (qPhi_)  {
            Double apex = kPhi_/qPhi_;
            if (sig_ >= apex) {
                if(iPlasticity) *iPlasticity = 2;
                s->state_ |= tension_now;
                s->stnS_.rs11() = apex;
                s->stnS_.rs22() = apex;
                s->stnS_.rs33() = apex;
                s->stnS_.rs12() = 0.0;
                s->stnS_.rs13() = 0.0;
                s->stnS_.rs23() = 0.0;
            }
        }
    }

    void ModelDrucker::tensionCorrection(State *s,UInt *iPlasticity,const Double &ft)
    {
        if(iPlasticity) *iPlasticity = 2;
        s->state_ |= tension_now;
        sig_ = tension_;
        s->stnS_.rs11() -= ft;
        s->stnS_.rs22() -= ft;
        s->stnS_.rs33() -= ft;
    }

    void ModelDrucker::shearCorrection(State *s,UInt *iPlasticity,const Double &fs, const SymTensor &dev)
    {
        if(iPlasticity) *iPlasticity = 1;
        s->state_ |= shear_now;
        Double lambda = fs/gKQ_;
        Double tauOld = tau_;
        tau_ -= lambda*shear_;
        sig_ -= lambda*kQ_;
        if (tauOld) {
            Double dRat = tau_/tauOld;
            s->stnS_.rs11() = dev.s11()*dRat + sig_;
            s->stnS_.rs22() = dev.s22()*dRat + sig_;
            s->stnS_.rs33() = dev.s33()*dRat + sig_;
            s->stnS_.rs12() = dev.s12()*dRat;
            s->stnS_.rs13() = dev.s13()*dRat;
            s->stnS_.rs23() = dev.s23()*dRat;
        }
    }

    Double ModelDrucker::getStressStrengthRatio(const SymTensor &st) const {
        DVect3 prin = st.getEigenInfo();
        Double rat = 10.0;
        Double tcut = qPhi_ ? std::min(tension_,(kPhi_/qPhi_)) : tension_; 
        if (tcut - prin.z() <= 0.0)
            rat = 0.0;
        else {
            Double sig1f = limits<Double>::max();
            if (qPhi_) 
                sig1f = 2.0* (kPhi_/qPhi_) - prin.z();
            Double denom = prin.z() - prin.x();
            if (denom) 
                rat = std::min(10.0,(prin.z() - sig1f) / denom);
        }
        rat = std::min(rat,10.0);
        return(rat);
    }

    //void ModelDrucker::scaleProperties(const Double &scale,const std::vector<UInt> &props) {
    //    for (UInt u=0; u<props.size(); ++u) {
    //        switch (props[u]) {
    //        case 5: qPhi_ *= scale;    break;
    //        case 6: kPhi_ *= scale;    break;
    //        case 7: qPsi_ *= scale;    break;
    //        case 8: tension_ *= scale; break;  
    //        }
    //    }
    //    setValid(0);
    //}
}
// EOF
