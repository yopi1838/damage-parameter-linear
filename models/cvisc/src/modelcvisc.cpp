#include "modelcvisc.h"
#include "base/src/version.h"

namespace models {

    ModelCVisc::ModelCVisc() : ModelBurger(),
        cohesion_(0.0),friction_(0.0),dilation_(0.0),tension_(0.0),
        sHP_(0.0),tHP_(0.0),nph_(0.0),csn_(0.0),rc_(0.0),nps_(0.0) {
    }

    UInt ModelCVisc::getMinorVersion() const{
        return version::update;
    }

    String ModelCVisc::getProperties(void) const {
        return ModelBurger::getProperties() + 
            L",cohesion,friction,dilation,tension,strain-shear-plastic,strain-tensile-plastic -strain-tension-plastic";
    }

    String ModelCVisc::getStates(void) const {
        return L"shear-n,tension-n,shear-p,tension-p";
    }

    Variant ModelCVisc::getProperty(UInt index) const {
        if (index <= 11)
            return ModelBurger::getProperty(index);
        else {
            switch (index) {
            case 12: return cohesion_;
            case 13: return friction_;
            case 14: return dilation_;
            case 15: return tension_;
            case 16: return sHP_;
            case 17: return tHP_;
            }
        }
        return(0.0);
    }

    void ModelCVisc::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
        if (index <= 11)
            ModelBurger::setProperty(index,p,restoreVersion);
        else {
            switch (index) {
            case 12: cohesion_ = p.toDouble(); break;
            case 13: friction_ = p.toDouble(); break;
            case 14: dilation_ = p.toDouble(); break;
            case 15: tension_  = p.toDouble(); break;  
            case 16: sHP_ = p.toDouble(); break;
            case 17: tHP_ = p.toDouble(); break;
            }
        }
    }

    void ModelCVisc::copy(const ConstitutiveModel *m) {
        const ModelCVisc *vm = dynamic_cast<const ModelCVisc *>(m);
        if (!vm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        ModelBurger::copy(m);
        cohesion_ = vm->cohesion_;
        friction_ = vm->friction_;
        dilation_ = vm->dilation_;
        tension_  = vm->tension_;
        sHP_ = vm->sHP_;
        tHP_ = vm->tHP_;
    }

    void ModelCVisc::initialize(UByte d,State *s) {
        ModelBurger::initialize(d, s);

        Double rsin = std::sin(friction_ * degrad);
        nph_  = (1.0 + rsin) / (1.0 - rsin);
        csn_  = 2.0 * cohesion_ * sqrt(nph_);
        if (friction_) {
            Double apex = cohesion_ / std::tan(friction_ * degrad);
            tension_ = std::min(tension_,apex);
        }
        else if (cohesion_ == 0.0)
            tension_ = 0.0;
        rsin = std::sin(dilation_ * degrad);
        nps_ = (1.0 + rsin) / (1.0 - rsin);
        rc_ = std::sqrt(1.0 + nph_*nph_);
    }

    static const UInt Dqs = 6;
    static const UInt Dqt = 7;

    void ModelCVisc::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);

        if (s->state_ & shear_now) s->state_ |= shear_past;
        s->state_ &= ~shear_now;
        if (s->state_ & tension_now) s->state_ |= tension_past;
        s->state_ &= ~tension_now;
        UInt iPlas = 0;

        Double tempk=0, tempm=0;
        Double dCrtdel =  (s->isCreepInUse() ? s->getTimeStep() : 0.0);
        Double dSubZoneVolume = s->getSubZoneVolume();

        if (!s->sub_zone_) {
            s->working_[0] = 0.0;
            s->working_[1] = 0.0;
            s->working_[2] = 0.0;
            s->working_[3] = 0.0;
            s->working_[4] = 0.0;  
            s->working_[5] = 0.0;
            s->working_[Dqs] =0.0;
            s->working_[Dqt] =0.0;
        }

        if(Kviscosity_ <= 0.0) tempk = 0.0;
        else tempk = 1.0 / Kviscosity_ ;
        //
        if (Mviscosity_ <= 0.0) tempm = 0.0;
        else tempm = 1.0 / Mviscosity_ ;

        Double temp  = 0.5 * Kshear_ * dCrtdel * tempk;
        Double a_con = 1.0 + temp;
        Double b_con = 1.0 - temp;
        Double ba    =  b_con / a_con;
        Double bal   =  ba - 1.0;
        temp  = (tempm + tempk / a_con) * dCrtdel * 0.25 ;
        Double temp1 = 0.5 / Mshear_;
        Double x_con =  temp1 + temp;
        Double y_con =  temp1 - temp;
        Double z_con = dCrtdel *  tempk / (4.0 * a_con);
        Double c1dxc = 1.0 / x_con;

        //;--- partition strains ---
        Double dev   = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33() ;
        Double dev3  = d1d3 * dev;
        Double de11d = s->stnE_.s11() - dev3;
        Double de22d = s->stnE_.s22() - dev3;
        Double de33d = s->stnE_.s33() - dev3;
        //;--- partition stresses---
        Double s0   = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        Double s11d = s->stnS_.s11() - s0;
        Double s22d = s->stnS_.s22() - s0;
        Double s33d = s->stnS_.s33() - s0;
        //;--- remember old stresses ---
        Double s11old = s11d;
        Double s22old = s22d;
        Double s33old = s33d;
        Double s12old = s->stnS_.s12();
        Double s13old = s->stnS_.s13();  
        Double s23old = s->stnS_.s23();
        //;--- new trial deviator stresses assuming viscoelastic increments ---
        s11d = (de11d + s11d * y_con - Mekd_[0] * bal) * c1dxc ;
        s22d = (de22d + s22d * y_con - Mekd_[1]* bal) * c1dxc ;
        s33d = (de33d + s33d * y_con - Mekd_[2]* bal) * c1dxc ;
        Double s12i = (s->stnE_.s12()  +  s->stnS_.s12() * y_con - Mekd_[3]  * bal) * c1dxc ;
        Double s13i = (s->stnE_.s13()  +  s->stnS_.s13() * y_con - Mekd_[4]  * bal) * c1dxc ; 
        Double s23i = (s->stnE_.s23()  +  s->stnS_.s23() * y_con - Mekd_[5]  * bal) * c1dxc ;
        //;--- new trial isotropic stress assuming elastic increment ---
        s0 += bulk_ * dev;
        Double s11i  = s11d + s0;
        Double s22i  = s22d + s0;
        Double s33i  = s33d + s0;
        //; --- trial stresses ---
        s->stnS_.rs11() = s11i;
        s->stnS_.rs22() = s22i;
        s->stnS_.rs33() = s33i;
        s->stnS_.rs12() = s12i;
        s->stnS_.rs13() = s13i;
        s->stnS_.rs23() = s23i;

        if (canFail()) {
            SymTensorInfo info;
            DVect3 prin = s->stnS_.getEigenInfo(&info);

            Double e1_ = bulk_ + d2d3*c1dxc;
            Double e2_ = bulk_ - d1d3*c1dxc;
            Double ra = e1_ - nps_*e2_;
            Double rb = e2_ - nps_*e1_;
            Double rd = ra - rb*nph_;
            Double sc1_ = ra / rd;
            Double sc3_ = rb / rd;
            Double sc2_ = e2_*(1.0 - nps_)/rd;
            Double e21_ = e2_/e1_;
            Double sf1_ = 1.0/rd;
            Double sf3_ = - nps_/rd;

            Double fs  = - prin.x() + nph_ * prin.z() - csn_;
            Double fsd = fs / rc_;
            Double ftz = prin.z() - tension_;
            Double fty = prin.y() - tension_;
            Double ftx = prin.x() - tension_;

            if (fsd > 0.0 && fsd >= ftz) {
                iPlas = 1;
                s->state_ |= shear_now;
                prin.rx() += fs * sc1_;
                prin.ry() += fs * sc2_;
                prin.rz() += fs * sc3_;
            }
            else if (ftz > 0.0 && ftz >= fsd) {
                s->state_ |= tension_now;
                if (ftx > 0.0) {
                    iPlas = 4;
                    prin.rx()  = tension_;
                    prin.ry()  = tension_;
                    prin.rz()  = tension_;
                } else if (fty > 0.0) {
                    iPlas = 3;
                    prin.rx() -= (fty + ftz) * e2_/(e1_+e2_);
                    prin.ry()  = tension_;
                    prin.rz()  = tension_;
                } else {
                    iPlas = 2;
                    Double tco = ftz * e21_;
                    prin.rx() -= tco;
                    prin.ry() -= tco;
                    prin.rz()  = tension_;
                }
            }

            if (friction_ > 0.0) {
                Double apex = cohesion_ / tan(friction_*degrad);
                if (prin.x()>=apex || prin.y()>=apex || prin.z()>=apex) {
                    iPlas = 4;
                    s->state_ |= tension_now;
                    prin.rx() = apex;
                    prin.ry() = apex;
                    prin.rz() = apex;
                }
            }

            if (iPlas) {
                s->stnS_ = info.resolve(prin);

                if (iPlas == 1) {
                    Double dDe1p = fs * sf1_;
                    Double dDe3p = fs * sf3_;
                    Double dDepa = d1d3 * (dDe1p + dDe3p);
                    dDe1p -= dDepa;
                    dDe3p -= dDepa;
                    s->working_[Dqs]  += sqrt(0.5 * (dDe1p*dDe1p+dDepa*dDepa+dDe3p*dDe3p)) * dSubZoneVolume;
                }
                if (iPlas == 2) {
                    Double dAux = ftz / e1_;
                    s->working_[Dqt]  += dAux * dSubZoneVolume;
                }
                if (iPlas == 3) {
                    Double dAux = (ftz+fty) / (e1_+e2_);
                    s->working_[Dqt]  += dAux * dSubZoneVolume;
                }
                if (iPlas == 4) {
                    Double dAux = (ftz+fty+ftx) / bulk_;
                    s->working_[Dqt]  += dAux * dSubZoneVolume;
                }
            }
        }

        //--- sub-zone contribution to Kelvin-strains ---
        s0  = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        s->working_[0] += (Mekd_[0] * ba  +  (s->stnS_.s11() - s0 + s11old) * z_con) * s->getSubZoneVolume();
        s->working_[1] += (Mekd_[1] * ba  +  (s->stnS_.s22() - s0 + s22old) * z_con) * s->getSubZoneVolume();
        s->working_[2] += (Mekd_[2] * ba  +  (s->stnS_.s33() - s0 + s33old) * z_con) * s->getSubZoneVolume();
        s->working_[3] += (Mekd_[3] * ba  +  (s->stnS_.s12()      + s12old) * z_con) * s->getSubZoneVolume();
        s->working_[4] += (Mekd_[4] * ba  +  (s->stnS_.s13()      + s13old) * z_con) * s->getSubZoneVolume(); 
        s->working_[5] += (Mekd_[5] * ba  +  (s->stnS_.s23()      + s23old) * z_con) * s->getSubZoneVolume();

        //--- update stored Kelvin-strains and plastic strain ---
        if (s->sub_zone_ == s->total_sub_zones_-1) {
            Double Aux = 1./s->getZoneVolume();
            if (s->overlay_==2) Aux *= 0.5;
            Mekd_[0]= s->working_[0] * Aux;
            Mekd_[1]= s->working_[1] * Aux;
            Mekd_[2]= s->working_[2] * Aux;
            Mekd_[3]= s->working_[3] * Aux;
            Mekd_[4]= s->working_[4] * Aux; 
            Mekd_[5]= s->working_[5] * Aux;
            if (canFail()) {
                sHP_ += s->working_[Dqs]*Aux;
                tHP_ += s->working_[Dqt]*Aux;
            }
        }
    }
}
// EOF