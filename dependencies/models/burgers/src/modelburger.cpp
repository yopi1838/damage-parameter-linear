#include "modelburger.h"
#include "base/src/version.h"

namespace models {

    ModelBurger::ModelBurger() :bulk_(0.0),Kshear_(0.0),Mshear_(0.0),Kviscosity_(0.0),Mviscosity_(0.0) {
        Mekd_[0]= 0.0;
        Mekd_[1]= 0.0;
        Mekd_[2]= 0.0;
        Mekd_[3]= 0.0;
        Mekd_[4]= 0.0; 
        Mekd_[5]= 0.0; 
    }

    UInt ModelBurger::getMinorVersion() const {
        return version::update;
    }

    String ModelBurger::getProperties(void) const {
        return L"bulk,shear-kelvin,shear-maxwell,viscosity-kelvin,viscosity-maxwell,"
               L"strain-kelvin-xx,strain-kelvin-yy,strain-kelvin-zz,strain-kelvin-xy,strain-kelvin-xz,strain-kelvin-yz";
    }

    Variant ModelBurger::getProperty(UInt index) const {
        switch (index) {
        case 1:  return bulk_;
        case 2:  return Kshear_;
        case 3:  return Mshear_;
        case 4:  return Kviscosity_;
        case 5:  return Mviscosity_;
        case 6:  return Mekd_[0];
        case 7:  return Mekd_[1];
        case 8:  return Mekd_[2];
        case 9:  return Mekd_[3];
        case 10: return Mekd_[4];
        case 11: return Mekd_[5];
        }
        return(0.0);
    }

    void ModelBurger::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);
        switch (index) {
        case  1: bulk_ = p.toDouble(); break;
        case  2: Kshear_ = p.toDouble(); break;
        case  3: Mshear_ = p.toDouble(); break;
        case  4: Kviscosity_ = p.toDouble(); break;
        case  5: Mviscosity_ = p.toDouble(); break;
        case  6: Mekd_[0] = p.toDouble(); break;
        case  7: Mekd_[1] = p.toDouble(); break;
        case  8: Mekd_[2] = p.toDouble(); break;
        case  9: Mekd_[3] = p.toDouble(); break;
        case 10: Mekd_[4] = p.toDouble(); break;
        case 11: Mekd_[5] = p.toDouble(); break;
        }
    }

    void ModelBurger::copy(const ConstitutiveModel *m) {
        ConstitutiveModel::copy(m);
        const ModelBurger *vm = dynamic_cast<const ModelBurger *>(m);
        if (!vm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");  
        bulk_ = vm->bulk_;
        Kshear_ = vm->Kshear_;
        Mshear_ = vm->Mshear_;
        Kviscosity_ = vm->Kviscosity_;
        Mviscosity_ = vm->Mviscosity_;
        Mekd_[0]  = vm->Mekd_[0];
        Mekd_[1]  = vm->Mekd_[1];
        Mekd_[2]  = vm->Mekd_[2];
        Mekd_[3]  = vm->Mekd_[3];
        Mekd_[4]  = vm->Mekd_[4]; 
        Mekd_[5]  = vm->Mekd_[5]; 
    }

    void ModelBurger::initialize(UByte d, State *s) {
        ConstitutiveModel::initialize(d,s);
        if(Mshear_ <= 0.0) Mshear_ = 1e-20;
        if(Kshear_ <= 0.0) Kshear_ = 0.0;
        if(Kviscosity_ <= 0.0) {
            Kviscosity_ = 0.0;
            Kshear_     = 0.0;
        }
        if(Mviscosity_ <= 0.0) Mviscosity_ = 0.0;
    }

    void ModelBurger::run(UByte d, State *s) {
        ConstitutiveModel::run(d,s);

        // dEkd values now stored in s->working_[] array (necessary for thread safety)
        Double tempk=0, tempm=0;
        if (!s->sub_zone_) {
            s->working_[0] = 0.0;
            s->working_[1] = 0.0;
            s->working_[2] = 0.0;
            s->working_[3] = 0.0;
            s->working_[4] = 0.0;
            s->working_[5] = 0.0;
        }

        Double dCrtdel =  (s->isCreepInUse() ? s->getTimeStep() : 0.0);

        if (Kviscosity_ <= 0.0) tempk = 0.0;
        else tempk = 1.0/Kviscosity_ ;
        //
        if (Mviscosity_ <= 0.0) tempm = 0.0;
        else tempm = 1.0/Mviscosity_ ;

        Double temp = 0.5*Kshear_*dCrtdel*tempk;

        Double dA_con = 1.0 + temp;
        Double dB_con = 1.0 - temp;
        Double dBa    = dB_con/dA_con;
        Double dBal   = dBa - 1.0;
        temp  = (tempm + tempk / dA_con) * dCrtdel * 0.25 ;
        Double temp1  = 0.5 / Mshear_;
        Double dX_con = temp1 + temp;
        Double dY_con = temp1 - temp;
        Double dZ_con = dCrtdel * tempk/(4.0 * dA_con);
        Double c1dxc  = 1.0 / dX_con;
        // Partition Strains
        Double dDev = s->stnE_.s11() + s->stnE_.s22() + s->stnE_.s33() ;
        Double dDev3 = d1d3 * dDev;
        Double dE11d = s->stnE_.s11() - dDev3;
        Double dE22d = s->stnE_.s22() - dDev3;
        Double dE33d = s->stnE_.s33() - dDev3;
        // Partition Stresses
        Double dS0 = d1d3 * (s->stnS_.s11() + s->stnS_.s22() + s->stnS_.s33());
        Double dS11d = s->stnS_.s11() - dS0;
        Double dS22d = s->stnS_.s22() - dS0;
        Double dS33d = s->stnS_.s33() - dS0;
        // Remember old stresses
        Double dS11old = dS11d;
        Double dS22old = dS22d;
        Double dS33old = dS33d;
        Double dS12old = s->stnS_.s12();
        Double dS13old = 0.0;
        Double dS23old = 0.0;
        if (d==3) {                    
            dS13old = s->stnS_.s13();
            dS23old = s->stnS_.s23();
        }
        //new deviatoric stresses
        dS11d = (dS11d*dY_con + dE11d - Mekd_[0]*dBal)/dX_con;
        dS22d = (dS22d*dY_con + dE22d - Mekd_[1]*dBal)/dX_con;
        dS33d = (dS33d*dY_con + dE33d - Mekd_[2]*dBal)/dX_con;
        s->stnS_.rs12() = (s->stnS_.s12()*dY_con + s->stnE_.s12() - Mekd_[3]*dBal )*c1dxc;
        if (d==3) {                    
            s->stnS_.rs13() = (s->stnS_.s13()*dY_con + s->stnE_.s13() - Mekd_[4]*dBal )*c1dxc;
            s->stnS_.rs23() = (s->stnS_.s23()*dY_con + s->stnE_.s23() - Mekd_[5]*dBal )*c1dxc;
        }
        // isotropic stress is elastic
        dS0 = dS0 + bulk_ * dDev;
        // convert back to x-y components
        s->stnS_.rs11() = dS11d + dS0;
        s->stnS_.rs22() = dS22d + dS0;
        s->stnS_.rs33() = dS33d + dS0;
        // sub-zone contribution to kelvin-strains
        s->working_[0] += (Mekd_[0]*dBa + (dS11d + dS11old)*dZ_con) *s->getSubZoneVolume();
        s->working_[1] += (Mekd_[1]*dBa + (dS22d + dS22old)*dZ_con) *s->getSubZoneVolume();
        s->working_[2] += (Mekd_[2]*dBa + (dS33d + dS33old)*dZ_con) *s->getSubZoneVolume();
        s->working_[3] += (Mekd_[3]*dBa + (s->stnS_.s12() + dS12old)*dZ_con) *s->getSubZoneVolume();
        if (d==3) {                    
            s->working_[4] += (Mekd_[4]*dBa + (s->stnS_.s13() + dS13old)*dZ_con) *s->getSubZoneVolume();
            s->working_[5] += (Mekd_[5]*dBa + (s->stnS_.s23() + dS23old)*dZ_con) *s->getSubZoneVolume();
        }
        // update stored kelvin strains
        if (s->sub_zone_==s->total_sub_zones_-1) {
            Double Aux = 1.0/s->getZoneVolume();
            if (s->overlay_==2) Aux *= 0.5;
            Mekd_[0] = s->working_[0]*Aux;
            Mekd_[1] = s->working_[1]*Aux;
            Mekd_[2] = s->working_[2]*Aux;
            Mekd_[3] = s->working_[3]*Aux;
            if (d==3) {                    
                Mekd_[4] = s->working_[4]*Aux;
                Mekd_[5] = s->working_[5]*Aux;
            }
        } 
    }
} // namespace models
// EOF
