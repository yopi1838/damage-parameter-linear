#include "modelviscous.h"
#include "base/src/version.h"

namespace models {

    ModelViscous::ModelViscous(): viscosity_(0.0), gD2V_(0.0) { 
    }

    UInt ModelViscous::getMinorVersion() const {
        return version::update;
    }

    String ModelViscous::getProperties(void) const {
        return ModelElastic::getProperties() + L",viscosity";
    }

    Variant ModelViscous::getProperty(UInt index) const {
        if (index <= 4)
            return ModelElastic::getProperty(index);
        else if (index == 5)   
            return(viscosity_);
        //
        return(0.0);
    }

    void ModelViscous::setProperty(UInt index,const Variant &p,UInt restoreVersion) {
        if (index <= 4)
            ModelElastic::setProperty(index,p,restoreVersion);
        else if (index == 5)
            viscosity_ = p.toDouble();
    }

    void ModelViscous::copy(const ConstitutiveModel *m) {
        const ModelViscous *vm = dynamic_cast<const ModelViscous *>(m);
        if (!vm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelElastic::copy(m);
        viscosity_ = vm->viscosity_;
    }

    void ModelViscous::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);
        ModelElastic::updateParameters();
        if (viscosity_ <= 0.0) gD2V_ = 0.0;
        else                   gD2V_ = 0.5*shear_/viscosity_;
    }

    void ModelViscous::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);
        if ((d!=3)&&(d!=2)) throw std::runtime_error("Illegal dimension in ModelViscous");

        Double dD = gD2V_ * (s->isCreepInUse() ? s->getTimeStep() : 0.0);
        if (dD > 0.5)  throw std::runtime_error("Timestep too large for ModelViscous");

        Double dVic1 = 1.0 - dD;
        Double dVic2 = 1.0 / (1.0 + dD);

        /* volumetric and deviatoric strain increments */
        Double dDevol  = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33();
        Double dDevol3 = d1d3 * dDevol;
        Double dE11d = s->stnE_.s11() - dDevol3;
        Double dE22d = s->stnE_.s22() - dDevol3;
        Double dE33d = s->stnE_.s33() - dDevol3;
        /* old isotropic and deviatoric stresses */
        Double dS0 = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        Double dS11d = s->stnS_.s11() - dS0;
        Double dS22d = s->stnS_.s22() - dS0;
        Double dS33d = s->stnS_.s33() - dS0;
        /* new deviatoric stresses */
        dS11d = (dS11d*dVic1 + g2_*dE11d) *dVic2;
        dS22d = (dS22d*dVic1 + g2_*dE22d) *dVic2;
        dS33d = (dS33d*dVic1 + g2_*dE33d) *dVic2;
        s->stnS_.rs12()  = (s->stnS_.s12()*dVic1 + g2_*s->stnE_.s12()) *dVic2;
        s->stnS_.rs23()  = (s->stnS_.s23()*dVic1 + g2_*s->stnE_.s23()) *dVic2;
        s->stnS_.rs13()  = (s->stnS_.s13()*dVic1 + g2_*s->stnE_.s13()) *dVic2;
        /* new elastic isotropic stress */
        dS0 += bulk_ * dDevol;
        /* total stresses */
        s->stnS_.rs11() = dS11d + dS0;
        s->stnS_.rs22() = dS22d + dS0;
        s->stnS_.rs33() = dS33d + dS0;
    }

}//namespace models
// EOF
