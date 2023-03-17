#include "modelelastic.h"
#include "base/src/version.h"

namespace models {

    ModelElastic::ModelElastic(unsigned short option) : ConstitutiveModel(option) { 
    }

    UInt ModelElastic::getMinorVersion() const {
        return version::update;
    }

    Variant ModelElastic::getProperty(UInt index) const {
        switch (index) {
        case 1: return bulk_; 
        case 2: return shear_;
        case 3: { Double young;
            getYPfromBS(bulk_,shear_,&young,nullptr);
            return young; }
        case 4: { Double poisson;
            getYPfromBS(bulk_,shear_,nullptr,&poisson);
            return poisson; }
        }
        return(0.0);
    }

    void ModelElastic::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);

        switch (index)  {
        case 1: bulk_  = p.toDouble(); break;
        case 2: shear_ = p.toDouble(); break;
        case 3: { 
            Double young(0),poisson(0);
            getYPfromBS(bulk_,shear_,&young,&poisson);
            young = p.toDouble();
            if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_); } break;
        case 4: { 
            Double young(0),poisson(0);
            getYPfromBS(bulk_,shear_,&young,&poisson);
            poisson = p.toDouble();
            if (!young && !restoreVersion) 
                throw std::runtime_error("Young's Modulus must be non-zero before you can specify Poisson's Ratio.");
            if (!restoreVersion) getBSfromYP(young,poisson,&bulk_,&shear_); } break;
        }
    }

    void ModelElastic::copy(const ConstitutiveModel *m) {
        ConstitutiveModel::copy(m);
        const ModelElastic *em = dynamic_cast<const ModelElastic *>(m);
        if (!em) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        bulk_ = em->bulk_;
        shear_ = em->shear_;
    }

    void ModelElastic::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        updateParameters();
    }

    void ModelElastic::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);

        if (s->modulus_reduction_factor_ > 0.0)
            moduliReduction(s->modulus_reduction_factor_);

        elasticTrial(s);
        s->viscous_ = true;
    }

    void ModelElastic::elasticTrial(State *s) {
        Double e11 = s->stnE_.s11();
        Double e22 = s->stnE_.s22();
        Double e33 = s->stnE_.s33();
        s->stnS_.rs11() += e11 * e1_ + (e22 + e33) * e2_;
        s->stnS_.rs22() += (e11 + e33) * e2_ + e22 * e1_;
        s->stnS_.rs33() += (e11 + e22) * e2_ + e33 * e1_;    
        s->stnS_.rs12() += s->stnE_.s12() * g2_;
        s->stnS_.rs13() += s->stnE_.s13() * g2_;
        s->stnS_.rs23() += s->stnE_.s23() * g2_;
    }

    bool ModelElastic::updateParameters() {
        e1_ = bulk_ + shear_ * d4d3;
        e2_ = bulk_ - shear_ * d2d3;
        g2_ = 2.0 * shear_;
        return true;
    }

    Double ModelElastic::moduliReduction(const Double &factor) {
        Double shear_new = shear_ * factor;
        e1_ = bulk_ + shear_new * d4d3;
        e2_ = bulk_ - shear_new * d2d3;
        g2_ = 2.0 * shear_new;
        return shear_new;
    }

} // namespace models
// EOF
