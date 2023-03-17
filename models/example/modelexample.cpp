#include "modelexample.h"
#include "version.txt"

// excerpt-export-start
int __stdcall DllMain(void *,unsigned, void *) {
    return 1;
}

extern "C" EXPORT_TAG const char *getName() {
#ifdef MODELDEBUG
    return "cmodelexampled";
#else
    return "cmodelexample";
#endif
}

extern "C" EXPORT_TAG unsigned getMajorVersion() {
    return MAJOR_VERSION;
}

extern "C" EXPORT_TAG unsigned getMinorVersion() {
    return UPDATE_VERSION;
}

extern "C" EXPORT_TAG void *createInstance() {
    models::ModelExample *m = new models::ModelExample();
    return (void *)m;
}
// excerpt-export-end

namespace models {

// excerpt-con-start
    ModelExample::ModelExample(unsigned short option) : ModelElastic(option) {
    }
// excerpt-con-end

    String ModelExample::getName() const { 
#ifdef MODELDEBUG
        return L"example-debug"; 
#else
        return L"example";
#endif
    }

    String ModelExample::getFullName() const { 
#ifdef MODELDEBUG
        return L"Example Debug";
#else
        return L"Example";
#endif
    }

    UInt ModelExample::getMinorVersion() const {
        return UPDATE_VERSION;
    }

    String ModelExample::getProperties() const {
        return ModelElastic::getProperties() + L",cohesion,friction,dilation,tension,flag-brittle";
    }

    String ModelExample::getStates() const {
        return L"shear-n,tension-n,shear-p,tension-p";
    }

    Variant ModelExample::getProperty(UInt index) const {
        if (index <= 4)
            return ModelElastic::getProperty(index);
        else {
            switch (index) {     
            case 5: return cohesion_;
            case 6: return friction_; 
            case 7: return dilation_;
            case 8: return tension_;
            case 9: return brittle_;
            }
        }
        return 0.0;
    }

    void ModelExample::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 4)
            ModelElastic::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 5: cohesion_ = p.toDouble(); break;
            case 6: friction_ = p.toDouble(); break;
            case 7: dilation_ = p.toDouble(); break;
            case 8: tension_  = p.toDouble(); break;
            case 9: brittle_  = p.toBool();   break;
            }
        }
    }

    bool ModelExample::isPropertyAdvanced(UInt i) const {
        if (i <= 4)
            return ModelElastic::isPropertyAdvanced(i);
        else if (i==9) 
            return true;
        return false;
    }

    void ModelExample::copy(const ConstitutiveModel *m) {
        const ModelExample *mm = dynamic_cast<const ModelExample *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        //
        ModelElastic::copy(m);
        //
        cohesion_ = mm->cohesion_;
        friction_ = mm->friction_;
        dilation_ = mm->dilation_;
        tension_  = mm->tension_;
        brittle_  = mm->brittle_;
    }

// excerpt-run-start
    void ModelExample::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        updateParameters();
    }

    void ModelExample::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);
// excerpt-state-start
        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
// excerpt-state-end
        UInt iPlas = 0;

        ModelElastic::elasticTrial(s);
        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        Double fs  = nph_*prin.z() - prin.x() - csn_;
        Double fsd = fs / rc_;
        Double ftz = prin.z() - tension_;
        if (fsd > 0.0 && fsd >= ftz)
            shearCorrection(s,&prin,&iPlas,fs);
        else if (ftz > 0.0 && ftz >= fsd)
            tensionCorrection(s,&prin,&iPlas,ftz,brittle_);
        apexCorrection(friction_,s,&prin,&iPlas,brittle_);

        if (iPlas) {
            s->stnS_ = info.resolve(prin);
            s->viscous_ = false;
        }
    }

    bool ModelExample::updateParameters() {
        Double rsin = std::sin(friction_ * degrad);
        nph_ = (1.0 + rsin) / (1.0 - rsin);
        csn_ = 2.0 * cohesion_ * sqrt(nph_);
        rsin = std::sin(dilation_ * degrad);
        nps_ = (1.0 + rsin) / (1.0 - rsin);
        rc_  = std::sqrt(1.0 + nph_*nph_);
        //
        if (friction_) {
            Double apex = cohesion_ / std::tan(friction_ * degrad);
            tension_ = std::min(tension_,apex);
        }
        //
        ModelElastic::updateParameters();
        //
        Double ra = e1_ - nps_ * e2_;
        Double rb = e2_ - nps_ * e1_;
        Double rd = ra - rb * nph_;
        sc1_  = ra / rd;
        sc3_  = rb / rd;
        sc2_  = e2_ * (1.0 - nps_) / rd;
        //
        return true;
    }

    bool ModelExample::updateParameters(bool bEUpdated, Double *sf1, Double *sf3) {
        if (cohesion_ < 0.0)
            throw std::runtime_error("Example model: cohesion is not allowed less than 0.");

        if (friction_ >= 90.0 || friction_ < 0.0)
            throw std::runtime_error("Example model: friction angle is not in the valid range of 0 to 90.");

        if (dilation_ > friction_)
            throw std::runtime_error("Example model: dilationn angle is not allowed greater than friction angle.");

        Double rsin = std::sin(friction_ * degrad);
        nph_ = (1.0 + rsin) / (1.0 - rsin);
        csn_ = 2.0 * cohesion_ * sqrt(nph_);
        rsin = std::sin(dilation_ * degrad);
        nps_ = (1.0 + rsin) / (1.0 - rsin);
        rc_  = std::sqrt(1.0 + nph_*nph_);
        //
        if (friction_) {
            Double apex = cohesion_ / std::tan(friction_*degrad);
            tension_ = std::min(tension_,apex);
        }
        //
        if (!bEUpdated) ModelElastic::updateParameters();
        //
        Double ra = e1_ - nps_*e2_;
        Double rb = e2_ - nps_*e1_;
        Double rd = ra  - rb*nph_;
        sc1_ = ra/rd;
        sc3_ = rb/rd;
        sc2_ = e2_*(1.0 - nps_)/rd;
        //
        if (sf1) *sf1 = -1.0/rd;
        if (sf3) *sf3 = nps_/rd;
        //
        return !bEUpdated;
    }
// excerpt-run-end

    Double ModelExample::moduliReduction(const Double &factor) {
        Double shear_new = ModelElastic::moduliReduction(factor);
        Double ra = e1_ - nps_*e2_;
        Double rb = e2_ - nps_*e1_;
        Double rd = ra - rb*nph_;
        sc1_  = ra/rd;
        sc3_  = rb/rd;
        sc2_  = e2_*(1.0 - nps_)/rd;
        return shear_new;
    }

    void ModelExample::apexCorrection(const Double &fric,State *s,DVect3 *prin,UInt *iPlasticity,bool bBrittle) {
        if (fric > 0.0) {
            Double apex = cohesion_ / tan(fric*degrad);
            if (prin->x()>=apex || prin->y()>=apex || prin->z()>=apex) {
                if(iPlasticity) *iPlasticity = 4;
                s->state_ |= tension_now;
                if (bBrittle) tension_ = 0.0;
                prin->rx() = apex;
                prin->ry() = apex;
                prin->rz() = apex;
            }
        }
    }

    void ModelExample::tensionCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &ftz,bool bBrittle) {
        s->state_ |= tension_now;
        if (bBrittle) tension_ = 0.0;
        Double ftx = prin->x() - tension_;
        Double fty = prin->y() - tension_;
        if (ftx > 0.0) {
            if(iPlasticity) *iPlasticity = 4;
            prin->rx()  = tension_;
            prin->ry()  = tension_;
            prin->rz()  = tension_;
        } else if (fty > 0.0) {
            if(iPlasticity) *iPlasticity = 3;
            prin->rx() -= (fty + ftz) * e2_/(e1_+e2_);
            prin->ry()  = tension_;
            prin->rz()  = tension_;
        } else {
            if(iPlasticity) *iPlasticity = 2;
            Double tco = ftz * e2_/e1_;
            prin->rx() -= tco;
            prin->ry() -= tco;
            prin->rz()  = tension_;
        }
    }

    void ModelExample::shearCorrection(State *s,DVect3 *prin,UInt *iPlasticity,const Double &fs) {
        if(iPlasticity) *iPlasticity = 1;
// excerpt-state2-start
        s->state_ |= shear_now;
// excerpt-state2-end
        prin->rx() += fs * sc1_;
        prin->ry() += fs * sc2_;
        prin->rz() += fs * sc3_;
    }

    Double ModelExample::getStressStrengthRatio(const SymTensor &st) const {
        DVect3 prin = st.getEigenInfo();
        Double rat = 10.0;
        Double tanf = std::tan(friction_*degrad); 
        Double tcut = friction_ ? std::min(tension_,(cohesion_/tanf)) : tension_; 
        if (tcut - prin.z() <= 0.0)
            rat = 0.0;
        else {
            Double sinf = std::sin(friction_*degrad);
            Double denom = 1.0 - sinf;
            Double nph = limits<Double>::max();
            if (denom) nph = (1.0 + sinf) / denom;
            Double sig1f = nph*prin.z() - 2.0*cohesion_*std::sqrt(nph);
            denom = prin.z() - prin.x();
            if (denom) rat = (prin.z() - sig1f) / denom;
        }
        rat = std::min(rat,10.0);
        return rat;
    }

    void ModelExample::scaleProperties(const Double &scale,const std::vector<UInt> &props) {
        for (UInt u=0;u<props.size();++u) {
            switch (props[u]) {
            case 5: cohesion_ *= scale;  break;
            case 6: friction_ = std::max(0.0,std::min(89.0,std::atan(std::tan(friction_*degrad)*scale)/degrad));  break;
            case 7: dilation_ = std::max(0.0,std::min(89.0,std::atan(std::tan(dilation_*degrad)*scale)/degrad));  break;
            case 8: tension_  *= scale;  break;
            }
            setValid(0);
        }
    }

} // namespace models
// EOF

// EOF
