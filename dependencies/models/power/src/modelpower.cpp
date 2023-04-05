#include "modelpower.h"
#include "base/src/version.h"

namespace models {

    static const UInt   iNitMax = 1;
    static const Double dStrTol = 1.e-8;
    static const Double dSq3d2  = 1.224744871391589049; // sqrt(1.5)

    ModelPower::ModelPower(): a1_(0.0),a2_(0.0),n1_(0.0),n2_(0.0),rs1_(0.0),rs2_(0.0) {
    }

    UInt ModelPower::getMinorVersion() const {
        return version::update;
    }

    String ModelPower::getProperties(void) const {
        return ModelElastic::getProperties() + 
            L",constant-1,constant-2,exponent-1,exponent-2"
            L",stress-reference-1,stress-reference-2";
    }

    Variant ModelPower::getProperty(UInt index) const {
        if (index <= 4)
            return ModelElastic::getProperty(index);
        else {
            switch (index) { 
            case 5:  return(a1_);
            case 6:  return(a2_);
            case 7:  return(n1_);
            case 8:  return(n2_);
            case 9:  return(rs1_);
            case 10: return(rs2_);
            }
        }
        return(0.0);
    }

    void ModelPower::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 4)
            ModelElastic::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 5:  a1_  = p.toDouble();  break;
            case 6:  a2_  = p.toDouble();  break;
            case 7:  n1_  = p.toDouble();  break;
            case 8:  n2_  = p.toDouble();  break;
            case 9:  rs1_ = p.toDouble();  break;
            case 10: rs2_ = p.toDouble();  break;
            }
        }
    }

    void ModelPower::copy(const ConstitutiveModel *m) {
        const ModelPower *pm = dynamic_cast<const ModelPower *>(m);
        if (!pm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelElastic::copy(m);
        a1_      = pm->a1_;
        a2_      = pm->a2_;
        n1_      = pm->n1_;
        n2_      = pm->n2_;
        rs1_     = pm->rs1_;
        rs2_     = pm->rs2_;
    }

    void ModelPower::initialize(UByte d, State *s) {
        ConstitutiveModel::initialize(d,s);
        ModelElastic::updateParameters();

        if (rs1_ > rs2_) 
            throw std::runtime_error("Power Creep: stress-reference-1 > stress-reference-2 is not allowed.");
    }

    void ModelPower::run(UByte d,State *s) {
        ConstitutiveModel::run(d, s);  
        if ((d!=3)&&(d!=2)) throw std::runtime_error("Illegal dimension in ModelPower");

        /* volumetric and deviatoric strain increments */
        Double dDevol  = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33();
        Double dDevol3 = d1d3 * dDevol;
        Double dE11d  = s->stnE_.s11() - dDevol3;
        Double dE22d  = s->stnE_.s22() - dDevol3;
        Double dE33d  = s->stnE_.s33() - dDevol3;
        /* old isotropic and deviatoric stresses */
        Double dS0   = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        Double dS11d = s->stnS_.s11() - dS0;
        Double dS22d = s->stnS_.s22() - dS0;
        Double dS33d = s->stnS_.s33() - dS0;

        /* iteration to find new deviatoric stresses */
        Double dS11n=0, dS22n=0, dS33n=0, dS12n=0, dS23n=0, dS13n=0;
        for (UInt iT=1;iT<=iNitMax;iT++) {
            Double dS11m, dS22m, dS33m, dS12m, dS23m, dS13m;
            if (iT==1) {
                dS11m   = dS11d;
                dS22m   = dS22d;
                dS33m   = dS33d;
                dS12m   = s->stnS_.s12();
                dS23m   = s->stnS_.s23();
                dS13m   = s->stnS_.s13();
            } else {
                dS11m   = 0.5 * (dS11n + dS11d);
                dS22m   = 0.5 * (dS22n + dS22d);
                dS33m   = 0.5 * (dS33n + dS33d);
                dS12m   = 0.5 * (dS12n + s->stnS_.s12() );
                dS23m   = 0.5 * (dS23n + s->stnS_.s23() );
                dS13m   = 0.5 * (dS13n + s->stnS_.s13() );
            }
            Double dE11v = 0.0;
            Double dE22v = 0.0;
            Double dE33v = 0.0;
            Double dE12v = 0.0;
            Double dE23v = 0.0;
            Double dE13v = 0.0;
            /* stress magnitude */
            if (s->isCreepInUse()) {
                Double dSmag2 = dS11m*dS11m + dS22m*dS22m + dS33m*dS33m
                    + 2.0 * (dS12m*dS12m + dS23m*dS23m + dS13m*dS13m);
                if (fabs(dSmag2) > dStrTol) {
                    Double dSbar  = dSq3d2 * sqrt (dSmag2);
                    Double dEdc   = 0.0;
                    if (dSbar >= rs1_) dEdc += a1_ * pow (dSbar, n1_);
                    if (dSbar <= rs2_) dEdc += a2_ * pow (dSbar, n2_);
                    dEdc   = 1.5 * dEdc * s->getTimeStep() / dSbar;
                    /* creep strain increments */
                    dE11v = dEdc * dS11m;
                    dE22v = dEdc * dS22m;
                    dE33v = dEdc * dS33m;
                    dE12v = dEdc * dS12m;
                    dE23v = dEdc * dS23m;
                    dE13v = dEdc * dS13m;
                    FP_S;
                }
            }
            /* new deviatoric stresses */
            dS11n = dS11d + g2_ * (dE11d - dE11v);
            dS22n = dS22d + g2_ * (dE22d - dE22v);
            dS33n = dS33d + g2_ * (dE33d - dE33v);
            dS12n = s->stnS_.s12()  + g2_ * (s->stnE_.s12()  - dE12v);
            dS23n = s->stnS_.s23()  + g2_ * (s->stnE_.s23()  - dE23v);
            dS13n = s->stnS_.s13()  + g2_ * (s->stnE_.s13()  - dE13v);
        }
        /* end of iteration */
        /* new elastic isotropic stress */
        dS0 += bulk_ * dDevol;
        /* new stresses */
        s->stnS_.rs11()    = dS11n + dS0;
        s->stnS_.rs22()    = dS22n + dS0;
        s->stnS_.rs33()    = dS33n + dS0;
        s->stnS_.rs12()    = dS12n;
        s->stnS_.rs23()    = dS23n;
        s->stnS_.rs13()    = dS13n;
    }
}
// EOF