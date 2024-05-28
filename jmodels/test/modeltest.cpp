#include "modeltest.h"
#include "version.txt"

// excerpt-export-start
int __stdcall DllMain(void *,unsigned, void *) {
    return 1;
}

extern "C" EXPORT_TAG const char *getName() {
#ifdef MODELDEBUG
    return "cmodel$modelkeyword$d";
#else
    return "cmodel$modelkeyword$";
#endif
}

extern "C" EXPORT_TAG unsigned getMajorVersion() {
    return MAJOR_VERSION;
}

extern "C" EXPORT_TAG unsigned getMinorVersion() {
    return UPDATE_VERSION;
}

extern "C" EXPORT_TAG void *createInstance() {
    models::Model$classname$ *m = new models::Model$classname$();
    return (void *)m;
}
// excerpt-export-end

namespace models {

// excerpt-con-start
    Model$classname$::Model$classname$(unsigned short option) : ConstitutiveModel(option) {
    }
// excerpt-con-end

    string Model$classname$::getName() const { 
#ifdef MODELDEBUG
        return "$modelkeyword$-debug"; 
#else
        return "$modelkeyword$";
#endif
    }

    string Model$classname$::getFullName() const { 
#ifdef MODELDEBUG
        return "$classname$ Debug";
#else
        return "$classname$";
#endif
    }

    uint32 Model$classname$::getMinorVersion() const {
        return UPDATE_VERSION;
    }

    string Model$classname$::getProperties() const {
        return "bulk,shear,young,poisson,cohesion,friction,dilation,tension,flag-brittle";
    }

    string Model$classname$::getStates() const {
        return "shear-n,tension-n,shear-p,tension-p";
    }

    base::Property Model$classname$::getProperty(uint32 index) const {
        switch (index) {
        case 1:
            return bulk_;
        case 2:
            return shear_;
        case 3: {
            double young;
            getYPfromBS(bulk_, shear_, &young, nullptr);
            return young;
        }
        case 4: {
            double poisson;
            getYPfromBS(bulk_, shear_, nullptr, &poisson);
            return poisson;
        }
        case 5:
            return cohesion_;
        case 6:
            return friction_;
        case 7:
            return dilation_;
        case 8:
            return tension_;
        case 9:
            return brittle_;
        }
        return 0.0;
    }

    void Model$classname$::setProperty(uint32 index, const base::Property& p, uint32 restoreVersion) {
        ConstitutiveModel::setProperty(index, p, restoreVersion);
        switch (index) {
        case 1:
            bulk_ = p.to<double>();
            break;
        case 2:
            shear_ = p.to<double>();
            break;
        case 3: {
            double young(0), poisson(0);
            getYPfromBS(bulk_, shear_, &young, &poisson);
            young = p.to<double>();
            if (!restoreVersion)
                getBSfromYP(young, poisson, &bulk_, &shear_);
        } break;
        case 4: {
            double young(0), poisson(0);
            getYPfromBS(bulk_, shear_, &young, &poisson);
            poisson = p.to<double>();
            if (!young && !restoreVersion)
                throw std::runtime_error(
                    "Young's Modulus must be non-zero before you can specify Poisson's Ratio.");
            if (!restoreVersion)
                getBSfromYP(young, poisson, &bulk_, &shear_);
        } break;
        case 5:
            cohesion_ = p.to<double>();
            break;
        case 6:
            friction_ = p.to<double>();
            break;
        case 7:
            dilation_ = p.to<double>();
            break;
        case 8:
            tension_ = p.to<double>();
            break;
        case 9:
            brittle_ = p.to<bool>();
            break;
        }
    }

    bool Model$classname$::isPropertyAdvanced(uint32 i) const {
        if (i==9) 
            return true;
        return false;
    }

    void Model$classname$::copy(const ConstitutiveModel *m) {
        const Model$classname$ *mm = dynamic_cast<const Model$classname$ *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        //
        bulk_ = mm->bulk_;
        shear_ = mm->shear_;
        cohesion_ = mm->cohesion_;
        friction_ = mm->friction_;
        dilation_ = mm->dilation_;
        tension_  = mm->tension_;
        brittle_  = mm->brittle_;
    }

// excerpt-run-start
    void Model$classname$::initialize(uint32 d,State *s) {
        ConstitutiveModel::initialize(d,s);

        e1_ = bulk_ + shear_ * d4d3;
        e2_ = bulk_ - shear_ * d2d3;
        g2_ = 2.0 * shear_;

        double rsin = std::sin(friction_ * degrad);
        nph_ = (1.0 + rsin) / (1.0 - rsin);
        csn_ = 2.0 * cohesion_ * sqrt(nph_);
        rsin = std::sin(dilation_ * degrad);
        nps_ = (1.0 + rsin) / (1.0 - rsin);
        rc_ = std::sqrt(1.0 + nph_ * nph_);
        if (friction_) {
            double apex = cohesion_ / std::tan(friction_ * degrad);
            tension_ = std::min(tension_, apex);
        }
        double ra = e1_ - nps_ * e2_;
        double rb = e2_ - nps_ * e1_;
        double rd = ra - rb * nph_;
        sc1_ = ra / rd;
        sc3_ = rb / rd;
        sc2_ = e2_ * (1.0 - nps_) / rd;
    }

    void Model$classname$::run(uint32 d,State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);
// excerpt-state-start
        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
// excerpt-state-end		
        uint32 iPlas = 0;

        double e11 = s->stnE_.s11();
        double e22 = s->stnE_.s22();
        double e33 = s->stnE_.s33();
        s->stnS_.rs11() += e11 * e1_ + (e22 + e33) * e2_;
        s->stnS_.rs22() += (e11 + e33) * e2_ + e22 * e1_;
        s->stnS_.rs33() += (e11 + e22) * e2_ + e33 * e1_;
        s->stnS_.rs12() += s->stnE_.s12() * g2_;
        s->stnS_.rs13() += s->stnE_.s13() * g2_;
        s->stnS_.rs23() += s->stnE_.s23() * g2_;

        s->viscous_ = true;        
        if (!canFail()) return;

        SymTensorInfo info;
        DVect3 prin = s->stnS_.getEigenInfo(&info);

        double fs  = nph_*prin.z() - prin.x() - csn_;
        double fsd = fs / rc_;
        double ftz = prin.z() - tension_;
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
//excerpt-run-end	

    double Model$classname$::moduliReduction(const double &factor) {
        double shear_new = shear_ * factor;
        e1_ = bulk_ + shear_new * d4d3;
        e2_ = bulk_ - shear_new * d2d3;
        g2_ = 2.0 * shear_new;
        double ra = e1_ - nps_*e2_;
        double rb = e2_ - nps_*e1_;
        double rd = ra - rb*nph_;
        sc1_  = ra/rd;
        sc3_  = rb/rd;
        sc2_  = e2_*(1.0 - nps_)/rd;
        return shear_new;
    }

    void Model$classname$::apexCorrection(const double &fric,State *s,DVect3 *prin,uint32 *iPlasticity,bool bBrittle) {
        if (fric > 0.0) {
            double apex = cohesion_ / tan(fric*degrad);
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

    void Model$classname$::tensionCorrection(State *s,DVect3 *prin,uint32 *iPlasticity,const double &ftz,bool bBrittle) {
        s->state_ |= tension_now;
        if (bBrittle) tension_ = 0.0;
        double ftx = prin->x() - tension_;
        double fty = prin->y() - tension_;
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
            double tco = ftz * e2_/e1_;
            prin->rx() -= tco;
            prin->ry() -= tco;
            prin->rz()  = tension_;
        }
    }

    void Model$classname$::shearCorrection(State *s,DVect3 *prin,uint32 *iPlasticity,const double &fs) {
// excerpt-state2-start		
        if(iPlasticity) *iPlasticity = 1;
// excerpt-state2-end		
        s->state_ |= shear_now;
        prin->rx() += fs * sc1_;
        prin->ry() += fs * sc2_;
        prin->rz() += fs * sc3_;
    }

    double Model$classname$::getStrengthStressRatio(const SymTensor &st) const {
        DVect3 prin = st.getEigenInfo();
        double rat = 10.0;
        double tanf = std::tan(friction_*degrad); 
        double tcut = friction_ ? std::min(tension_,(cohesion_/tanf)) : tension_; 
        if (tcut - prin.z() <= 0.0)
            rat = 0.0;
        else {
            double sinf = std::sin(friction_*degrad);
            double denom = 1.0 - sinf;
            double nph = limits<double>::max();
            if (denom) nph = (1.0 + sinf) / denom;
            double sig1f = nph*prin.z() - 2.0*cohesion_*std::sqrt(nph);
            denom = prin.z() - prin.x();
            if (denom) rat = (prin.z() - sig1f) / denom;
        }
        rat = std::min(rat,10.0);
        return rat;
    }

    void Model$classname$::scaleProperties(const double &scale,const std::vector<uint32> &props) {
        for (uint32 u=0;u<props.size();++u) {
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
