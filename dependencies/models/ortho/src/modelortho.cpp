#include "modelortho.h"
#include "base/src/version.h"

namespace models {

    ModelOrthotropic::ModelOrthotropic()
        :ModelAnisotropic(), Rot_(0.0), E2_(0.0), Nu23_(0.0), G12_(0.0), G23_(0.0) { }

    String ModelOrthotropic::getProperties(void) const {
        return L"young-1,young-2,young-3,poisson-12,poisson-13,poisson-23,shear-12,shear-13,shear-23,"
               L"dip,dip-direction,normal-x,normal-y,normal-z,normal,angle,rotation,";
    }

    Variant ModelOrthotropic::getProperty(UInt index) const {
        switch (index) {
        case 1:  return(E1_);
        case 2:  return(E2_);
        case 3:  return(E3_);
        case 4:  return(Nu12_);
        case 5:  return(Nu13_);
        case 6:  return(Nu23_);
        case 7:  return(G12_);
        case 8:  return(G13_);
        case 9:  return(G23_);
        case 10:
        case 11:  
        case 12:
        case 13:
        case 14:
        case 15:
        case 16: return ModelWeakplaneType::getProperty(index-9);
        case 17: return(Rot_); 
        default: return 0.0;
        }
    }

    void ModelOrthotropic::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
        DVect3 norm; 
        ConstitutiveModel::setProperty(index,p,restoreVersion);
        switch (index) {
        case 1:  E1_ = p.toDouble();   break;
        case 2:  E2_ = p.toDouble();   break;
        case 3:  E3_ = p.toDouble();   break;
        case 4:  Nu12_ = p.toDouble(); break;
        case 5:  Nu13_ = p.toDouble(); break;
        case 6:  Nu23_ = p.toDouble(); break;
        case 7:  G12_ = p.toDouble();  break;
        case 8:  G13_ = p.toDouble();  break;
        case 9:  G23_ = p.toDouble();  break;
        case 10:
        case 11:
        case 12:  
        case 13:
        case 14:
        case 15:
        case 16: ModelWeakplaneType::setProperty(index-9,p,restoreVersion); break;
        case 17: Rot_ = p.toDouble(); break;
        }
    }


    Double ModelOrthotropic::getShearModulus(void) const {
        Double GMAX = std::max(G12_,G13_);
        GMAX = std::max(GMAX,G23_);
        return GMAX;
    }

    Double ModelOrthotropic::getBulkModulus(void) const {
        Double M = std::max(E1_,E2_);
        M = std::max(M,E3_);
        Double N = std::max(Nu12_,Nu13_);
        N = std::max(N,Nu23_);
        return M/(3.0*(1.0-2.0*N));
    }

    void ModelOrthotropic::initialize(UByte d,State *s) {
        ConstitutiveModel::initialize(d,s);

        if (E1_ <= 0.0 || E2_ <= 0.0 || E3_ <= 0.0 )
            throw std::runtime_error("Orthotropic Model - Invalid Young's modulus.");
        if (G12_ <= 0.0 || G13_ <= 0.0 || G23_ <= 0.0 )
            throw std::runtime_error("Orthotropic Model - Invalid shear modulus.");    
        if (Nu12_ > 0.5 || Nu13_ > 0.5 || Nu23_ > 0.5 || Nu12_ <= -1. || Nu13_ <= -1. || Nu23_ <= -1.)
            throw std::runtime_error("Orthotropic Model - Invalid Poisson ratio.");

        buildMatrix(E1_,E2_,E3_,Nu23_,Nu13_,Nu12_,G23_,G13_,G12_,dd_,dip_,Rot_);

    }

    void ModelOrthotropic::copy(const ConstitutiveModel *m) {
        ConstitutiveModel::copy(m);

        const ModelOrthotropic *mm = dynamic_cast<const ModelOrthotropic *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");

        E1_   = mm->E1_;
        E2_   = mm->E2_;
        E3_   = mm->E3_;
        Nu12_ = mm->Nu12_;
        Nu13_ = mm->Nu13_;
        Nu23_ = mm->Nu23_;
        G12_  = mm->G12_;
        G13_  = mm->G13_;
        G23_  = mm->G23_;
        dip_  = mm->dip_;
        dd_   = mm->dd_;
        Rot_  = mm->Rot_;
        norma_ = mm->norma_;
        angle_= mm->angle_;
    }
}
// EOF