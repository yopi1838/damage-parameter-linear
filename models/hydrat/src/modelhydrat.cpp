#include "modelhydrat.h"
#include "base/src/version.h"

namespace models {

    static constexpr Double dSqrt3 = 1.73205080756887729352744634150587; // sqrt(3.0);

    ModelHydrat::ModelHydrat() : ModelDrucker(), 
        bulk_cte_(0.0),shear_cte_(0.0),young_cte_(0.0),tension_cte_(0.0),
        alpha_0_(0.0),c_(0.0),a_(0.0),dAlpha_min_(0.0),young_(0.0),poisson_(0.0),compression_(0.0) {
    }

    String ModelHydrat::getProperties() const {
        return ModelDrucker::getProperties() +            
            L",bulk-reference,shear-reference,young-reference,tension-reference"
            L",hydration-minimum,constant-c,constant-a"
            L",hydration-difference-minimum,compression";
    }

    UInt ModelHydrat::getMinorVersion() const {
        return version::update;
    }

    Variant ModelHydrat::getProperty(UInt index) const {
        if (index <= 8) {
            if (index == 3) 
                return young_;
            else if (index == 4) 
                return poisson_;
            else 
                return ModelDrucker::getProperty(index);
        }
        else {
            switch (index) { 
            case  9: return bulk_cte_;
            case 10: return shear_cte_;
            case 11: return young_cte_;
            case 12: return tension_cte_;
            case 13: return alpha_0_;
            case 14: return c_;
            case 15: return a_;
            case 16: return dAlpha_min_;
            case 17: return compression_;
            }
        }
        return(0.0);
    }

    void ModelHydrat::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 8) {
            if (index == 3) 
                young_ = p.toDouble();
            else if (index == 4) { 
                getYPfromBS(bulk_cte_,shear_cte_,&young_cte_,&poisson_);
                poisson_ = p.toDouble();
                if (!restoreVersion) getBSfromYP(young_cte_,poisson_,&bulk_cte_,&shear_cte_);
            } 
            else
                ModelDrucker::setProperty(index,p,restoreVersion);
        }
        else {
            switch (index) {
            case  9: bulk_cte_    = p.toDouble();  break;
            case 10: shear_cte_   = p.toDouble();  break;
            case 11: {
                getYPfromBS(bulk_cte_,shear_cte_,&young_cte_,&poisson_);
                young_cte_ = p.toDouble();
                getBSfromYP(young_cte_,poisson_,&bulk_cte_,&shear_cte_); } break;
            case 12: tension_cte_ = p.toDouble();  break;
            case 13: alpha_0_     = p.toDouble();  break;
            case 14: c_           = p.toDouble();  break;
            case 15: a_           = p.toDouble();  break;
            case 16: dAlpha_min_  = p.toDouble();  break;
            case 17: compression_ = p.toDouble();  break;
            }
        }
    }

    bool ModelHydrat::isPropertyReadOnly(UInt i) const {
        if (i <= 8)
            return ModelDrucker::isPropertyReadOnly(i);
        else
            return false;
    }

    void ModelHydrat::copy(const ConstitutiveModel *m) {
        const ModelHydrat *dm = dynamic_cast<const ModelHydrat *>(m);
        if (!dm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelDrucker::copy(m);
        bulk_cte_     = dm->bulk_cte_;
        shear_cte_    = dm->shear_cte_;
        young_cte_    = dm->young_cte_;
        alpha_0_      = dm->alpha_0_;
        c_            = dm->c_;
        a_            = dm->a_;
        tension_cte_  = dm->tension_cte_;
        dAlpha_min_   = dm->dAlpha_min_;
        compression_  = dm->compression_;
    }

    void ModelHydrat::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);

        getYPfromBS(bulk_cte_,shear_cte_,&young_cte_,&poisson_);

        if (!bulk_)       bulk_    = bulk_cte_;
        if (!shear_)      shear_   = shear_cte_;
        if (!tension_)    tension_ = tension_cte_;
        if (!dAlpha_min_) dAlpha_min_ = 1e-10;

        ModelDrucker::updateParameters();
    }

    void ModelHydrat::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);
        if ((d!=3)&&(d!=2))throw std::runtime_error("Illegal dimension in ModelHydrat");

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
            ModelDrucker::shearCorrection(s,&iPlas,fs,dev);
        else if (ft > 0.0 && ft >= fsd)
            ModelDrucker::tensionCorrection(s,&iPlas,ft);
        ModelDrucker::apexCorrection(s,&iPlas);

        if (iPlas) s->viscous_ = false;

        Double dAlpha = s->getTMUtility(0);
        if (s->sub_zone_ == s->total_sub_zones_-1 && dAlpha) {
            s->modulus_changed_ = true;
            Double ddAlpha = dAlpha - alpha_0_;
            if (ddAlpha < dAlpha_min_) ddAlpha = dAlpha_min_;
            ddAlpha /= (1.0 - alpha_0_);
            young_ = young_cte_ * pow(ddAlpha, a_);
            getBSfromYP(young_,poisson_,&bulk_,&shear_);
            tension_ = tension_cte_ * ddAlpha;

            compression_ = 0.85 * (tension_cte_/c_) * pow(ddAlpha, 1.5);
            compression_ = tension_ >= compression_ ? 1.001 * tension_ : compression_;
            Double dAux = dSqrt3 / (compression_ + tension_);
            qPhi_ = dAux * (compression_ - tension_);
            kPhi_ = d2d3 * dAux * compression_ * tension_;

            ModelDrucker::updateParameters();
            // specific tension cut-off
            Double sTeno3 = tension_ / 3.0;
            Double dApex = qPhi_ ? kPhi_/qPhi_ : sTeno3;
            tension_ = dApex < sTeno3 ? dApex : sTeno3;
        }
    }
}//namespace models
// EOF
