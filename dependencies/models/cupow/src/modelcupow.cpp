#include "modelcupow.h"
#include "base/src/version.h"

namespace models {

    static const UInt   iNitMax = 1;
    static const Double dStrTol = 1.e-8;
    static const Double dSq3d2  = 1.224744871391589049; // sqrt(1.5)

    ModelCUPow::ModelCUPow() :
        a1_(0.0), a2_(0.0), n1_(0.0), n2_(0.0), rs1_(0.0), rs2_(0.0) {
    }

    UInt ModelCUPow::getMinorVersion() const {
        return version::update;
    }

    String ModelCUPow::getProperties(void) const {
        return ModelUbiquitous::getProperties() + 
            L",constant-1,constant-2,exponent-1,exponent-2,stress-reference-1,stress-reference-2";
    }

    Variant ModelCUPow::getProperty(UInt index) const {
        if (index <= 20)
            return ModelUbiquitous::getProperty(index);
        else {
            switch (index) {
            case 21: return a1_;
            case 22: return a2_;
            case 23: return n1_;
            case 24: return n2_;
            case 25: return rs1_;
            case 26: return rs2_;
            }
        }
        return(0.0);
    }

    void ModelCUPow::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
        if (index <= 20)
            ModelUbiquitous::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 21: a1_  = p.toDouble(); break;
            case 22: a2_  = p.toDouble(); break;
            case 23: n1_  = p.toDouble(); break;
            case 24: n2_  = p.toDouble(); break;
            case 25: rs1_ = p.toDouble(); break;
            case 26: rs2_ = p.toDouble(); break;
            }
        }
    }

    void ModelCUPow::copy(const ConstitutiveModel *m) {
        const ModelCUPow *vm = dynamic_cast<const ModelCUPow *>(m);
        if (!vm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelUbiquitous::copy(m);
        a1_  = vm->a1_;
        a2_  = vm->a2_;
        n1_  = vm->n1_;
        n2_  = vm->n2_;
        rs1_ = vm->rs1_;
        rs2_ = vm->rs2_;
    }

    void ModelCUPow::initialize(UByte d,State *s) {
        ModelUbiquitous::initialize(d,s);
    }

    void ModelCUPow::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);

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

        Double dCrtdel =  (s->isCreepInUse() ? s->getTimeStep() : 0.0);

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
        Double dS11n(0.0),dS22n(0.0),dS33n(0.0),dS12n(0.0),dS23n(0.0),dS13n(0.0);

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
            Double dE11v(0.0),dE22v(0.0),dE33v(0.0),dE12v(0.0),dE23v(0.0),dE13v(0.0);
            /* stress magnitude */
            if (s->isCreep()) {
                Double dSmag2 = dS11m*dS11m + dS22m*dS22m + dS33m*dS33m
                    + 2.0 * (dS12m*dS12m + dS23m*dS23m + dS13m*dS13m);
                if (fabs(dSmag2) > dStrTol) {
                    Double dSbar  = dSq3d2 * sqrt (dSmag2);
                    Double dEdc   = 0.0;
                    if (dSbar >= rs1_) dEdc += a1_ * pow (dSbar, n1_);
                    if (dSbar <= rs2_) dEdc += a2_ * pow (dSbar, n2_);
                    dEdc   = 1.5 * dEdc * dCrtdel / dSbar;
                    /* creep strain increments */
                    dE11v = dEdc * dS11m;
                    dE22v = dEdc * dS22m;
                    dE33v = dEdc * dS33m;
                    dE12v = dEdc * dS12m;
                    dE23v = dEdc * dS23m;
                    dE13v = dEdc * dS13m;
                }
            }
            /* new deviatoric stresses */
            dS11n = dS11d + g2_*(dE11d - dE11v);
            dS22n = dS22d + g2_*(dE22d - dE22v);
            dS33n = dS33d + g2_*(dE33d - dE33v);
            dS12n = s->stnS_.s12() + g2_*(s->stnE_.s12()  - dE12v);
            dS23n = s->stnS_.s23() + g2_*(s->stnE_.s23()  - dE23v);
            dS13n = s->stnS_.s13() + g2_*(s->stnE_.s13()  - dE13v);
        }
        /* end of iteration */

        /* new elastic isotropic stress */
        dS0 += bulk_ * dDevol;
        /* new visco-elastic stresses */
        s->stnS_.rs11() = dS11n + dS0;
        s->stnS_.rs22() = dS22n + dS0;
        s->stnS_.rs33() = dS33n + dS0;
        s->stnS_.rs12() = dS12n;
        s->stnS_.rs23() = dS23n;
        s->stnS_.rs13() = dS13n;

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

        DVect3 normv(normX_, normY_, normZ_);
        Axes3D aAxes = Axes3D::make(normv);
        ModelUbiquitous::stressToLocal(s,aAxes);
        dsp11_ = 0.0; dsp33_=0.0; dsp13_=0.0; dsp23_=0.0;
        Double fjt  = sig_ - jTension_;
        Double fjs  = tau_ + sig_*qphi_ - jCohesion_;
        Double fjd = fjs / jrc_;
        if (fjs > 0.0 && fjd >= fjt)
            ModelUbiquitous::shearCorrectionJoint(s,&jPlas,fjs);
        else if (fjt > 0.0 && fjt >= fjd)
            ModelUbiquitous::tensionCorrectionJoint(s,&jPlas,fjt);
        ModelUbiquitous::apexCorrectionJoint(s,&jPlas);

        if (jPlas > 0) 
            ModelUbiquitous::stressToGlobal(s,aAxes);

        if (s->isLarge()) 
            ModelUbiquitous::largeStrainCorrection(s,aAxes);

        if (iPlas || jPlas)
            s->viscous_ = false;
    }

}
// EOF