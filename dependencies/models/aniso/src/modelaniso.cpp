#include "modelaniso.h"
#include "base/src/version.h"

namespace models {    
    ModelAnisotropic::ModelAnisotropic() : ModelWeakplaneType(), 
        E1_(0.0),E3_(0.0),Nu12_(0.0),Nu13_(0.0),G13_(0.0),
        A11_(0.0),A12_(0.0),A13_(0.0),A14_(0.0),A15_(0.0),A16_(0.0),
                  A22_(0.0),A23_(0.0),A24_(0.0),A25_(0.0),A26_(0.0),
                            A33_(0.0),A34_(0.0),A35_(0.0),A36_(0.0),
                                      A44_(0.0),A45_(0.0),A46_(0.0),
                                                A55_(0.0),A56_(0.0),
                                                          A66_(0.0) {
    }

    UInt ModelAnisotropic::getMinorVersion() const {
        return version::update;
    }

    String ModelAnisotropic::getProperties() const {
        return L"young-plane,young-normal,poisson-plane,poisson-normal,shear-normal,"
               + ModelWeakplaneType::getProperties();
    }

    String ModelAnisotropic::getStates() const {
        return L"";
    }

    Double ModelAnisotropic::getConfinedModulus(void) const {
        return getBulkModulus() + d4d3*getShearModulus();
    }

    Double ModelAnisotropic::getShearModulus(void) const {
        Double G12 = (Nu12_ != -1.0)? 0.5 * E1_ / (1.0+Nu12_) : 0.0;
        return (G12 > G13_ ? G12 : G13_);
    }

    Double ModelAnisotropic::getBulkModulus(void) const {
        Double M = E1_ > E3_ ? E1_ : E3_;
        Double N = Nu12_ > Nu13_ ? Nu12_ : Nu13_;
        return M/(3.0*(1.0-2.0*N));
    }

    Variant ModelAnisotropic::getProperty(UInt index) const {
        switch (index)  {
        case 1:  return(E1_);
        case 2:  return(E3_);
        case 3:  return(Nu12_);
        case 4:  return(Nu13_);
        case 5:  return(G13_);
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: return ModelWeakplaneType::getProperty(index-5);
        }  
        return 0.0;
    }

    void ModelAnisotropic::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
        ConstitutiveModel::setProperty(index,p,restoreVersion);
        switch (index) {
        case 1:  E1_   = p.toDouble(); break;
        case 2:  E3_   = p.toDouble(); break;
        case 3:  Nu12_ = p.toDouble(); break;
        case 4:  Nu13_ = p.toDouble(); break;
        case 5:  G13_  = p.toDouble(); break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: ModelWeakplaneType::setProperty(index-5,p,restoreVersion); break;
        }
    }

    void ModelAnisotropic::copy(const ConstitutiveModel *m) {
        ConstitutiveModel::copy(m);
        const ModelAnisotropic *mm = dynamic_cast<const ModelAnisotropic *>(m);
        if (!mm) throw std::runtime_error("Internal error: constitutive model dynamic cast failed.");
        //
        E1_   = mm->E1_;
        E3_   = mm->E3_;
        Nu12_ = mm->Nu12_;
        Nu13_ = mm->Nu13_;
        G13_  = mm->G13_;
        //
        dip_  = mm->dip_;
        dd_   = mm->dd_;
        norma_ = mm->norma_;
        angle_= mm->angle_;
    }

    void ModelAnisotropic::initialize(UByte d,State *s)  {
        ConstitutiveModel::initialize(d,s);

        if (E1_ <= 0.0 || E3_ <= 0.0 )
            throw std::runtime_error("Anisotropic Model - Invalid Young's modulus.");
        if (G13_ <= 0.0 )
            throw std::runtime_error("Anisotropic Model - Invalid shear modulus.");    
        if (Nu12_ >= 0.5 || Nu13_ >= 0.5 || Nu12_ < -1.0 || Nu13_ < -1.0)
            throw std::runtime_error("Anisotropic Model - Invalid Poisson ratio.");

        /* --- tranversely isotropic case --- */
        Double E2   = E1_;
        Double Nu23 = Nu13_;
        Double G23  = G13_;
        Double G12 = (Nu12_ != -1.0) ? 0.5 * E1_ / (1.0+Nu12_) : 0.0;
        if (E3_ <= 0.0) E3_ = E1_;

        buildMatrix(E1_,E2,E3_,Nu23,Nu13_,Nu12_,G23,G13_,G12,dd_,dip_);
    }

    void ModelAnisotropic::run(UByte d,State *s) {
        ConstitutiveModel::run(d,s);
        AnisotropicElasticTrial(s);
        s->viscous_ = false;
    }

    void ModelAnisotropic::AnisotropicElasticTrial(State *s) {
        Double DG12 = 2.0*s->stnE_.s12();
        Double DG13 = 2.0*s->stnE_.s13();
        Double DG23 = 2.0*s->stnE_.s23();
        Double DS11 = s->stnE_.s11();
        Double DS22 = s->stnE_.s22();
        Double DS33 = s->stnE_.s33();
        s->stnS_.rs11() += A11_*DS11 + A12_*DS22 + A13_*DS33 + A14_*DG12 + A15_*DG13 + A16_*DG23;
        s->stnS_.rs22() += A12_*DS11 + A22_*DS22 + A23_*DS33 + A24_*DG12 + A25_*DG13 + A26_*DG23;
        s->stnS_.rs33() += A13_*DS11 + A23_*DS22 + A33_*DS33 + A34_*DG12 + A35_*DG13 + A36_*DG23;
        s->stnS_.rs12() += A14_*DS11 + A24_*DS22 + A34_*DS33 + A44_*DG12 + A45_*DG13 + A46_*DG23;
        s->stnS_.rs13() += A15_*DS11 + A25_*DS22 + A35_*DS33 + A45_*DG12 + A55_*DG13 + A56_*DG23;
        s->stnS_.rs23() += A16_*DS11 + A26_*DS22 + A36_*DS33 + A46_*DG12 + A56_*DG13 + A66_*DG23;
    }

    /* inverts matrix b(6,6) */
    /* returns False if singular */
    bool ModelAnisotropic::xmatinv(Double b[6][6]) {
        bool flag;
        Double bii,bji;
        UInt n=6;
        UInt i,j,k;
        flag = true;
        for (i=0; i<n; i++) {
            bii = b[i][i];
            if (bii == 0.0) {
                flag = false;
                return(flag);
            }
            for (k=0; k<n; k++) b[i][k] = b[i][k] / bii;
            b[i][i] = 1.0 / bii;
            for (j=0; j<n; j++) {
                if (j != i) {
                    bji = b[j][i];
                    for (k=0; k<n; k++) b[j][k] = b[j][k] - bji * b[i][k];
                    b[j][i] = - bji * b[i][i];
                }
            }
        }
        return(flag);
    }

    /* multiplication of square matrices : B3 = B1*B2 */
    void ModelAnisotropic::xmatmul(Double b1[6][6],Double b2[6][6],Double b3[6][6]) {
        Double sum;
        UInt i,j,m,n;
        n = 6;
        for (i=0; i<n; i++) {
            for (j=0; j<n; j++) {
                sum = 0.0;
                for (m = 0; m<n; m++) sum = sum + b1[i][m] * b2[m][j];
                b3[i][j] = sum;
            }
        }
    }

    void ModelAnisotropic::buildMatrix(const Double &e1, const Double &e2, const Double &e3,
        const Double &v1, const Double &v2, const Double &v3,
        const Double &g1, const Double &g2, const Double &g3,
        const Double &dd, const Double &dip, const Double& rot) {
        Double adBB[6][6],adTT[6][6],adTTa[6][6],adTTb[6][6],adTTc[6][6],adTTw[6][6];

        for (UInt i=0;i<6;i++) {
            for (UInt j=0;j<6;j++) {
                adBB[i][j]  = 0.0;
                adTT[i][j]  = 0.0;
                adTTa[i][j] = 0.0;
                adTTb[i][j] = 0.0;
                adTTc[i][j] = 0.0;
                adTTw[i][j] = 0.0;
            }  
        }

        /* --- local compliance matrix --- */ //(2.20)
        if (e1) adBB[0][0] = 1.0 / e1;
        if (e2) adBB[0][1] = -v3 / e2;
        if (e3) adBB[0][2] = -v2 / e3;
        adBB[1][0] = adBB[0][1];//symetric terms
        if (e2) adBB[1][1] = 1.0 / e2;
        if (e3) adBB[1][2] = -v1 / e3;
        adBB[2][0] = adBB[0][2];
        adBB[2][1] = adBB[1][2];
        if (e3) adBB[2][2] = 1.0 / e3;
        if (g3) adBB[3][3] = 1.0 / g3;//shear terms
        if (g2) adBB[4][4] = 1.0 / g2;
        if (g1) adBB[5][5] = 1.0 / g1;
        /* --- invert to get local elasticity matrix --- */
        if (xmatinv(adBB)) {
            /* --- strain transformation matrix --- */
            /* (A) rotate about Znew to get Xnew along horiz. proj. of dip dir.*/
            Double ang = 90.0 - dd;
            ang = ang * degrad;
            Double C = cos (ang);
            Double S = sin (ang);
            Double C2 = C*C;
            Double S2 = S*S;
            Double SC = S*C;
            adTTb[0][0] = C2;
            adTTb[0][1] = S2;
            adTTb[0][3] = SC;
            adTTb[1][0] = S2;
            adTTb[1][1] = C2;
            adTTb[1][3] = -SC;
            adTTb[2][2] = 1.0;
            adTTb[3][0] = -2.0*SC;
            adTTb[3][1] = 2.0*SC;
            adTTb[3][3] = C2-S2;
            adTTb[4][4] = C;
            adTTb[4][5] = S;
            adTTb[5][4] = -S;
            adTTb[5][5] = C;
            /* (B) rotate about Ynew to get Xnew along dip dir.*/
            ang = - dip;
            ang = ang * degrad;
            C = cos(ang);
            S = sin(ang);
            C2 = C*C;
            S2 = S*S;
            SC = S*C;
            adTTc[0][0] = C2;
            adTTc[0][2] = S2;
            adTTc[0][4] = SC;
            adTTc[2][0] = S2;
            adTTc[2][2] = C2;
            adTTc[2][4] = -SC;
            adTTc[1][1] = 1.0;
            adTTc[4][0] = -2.0*SC;
            adTTc[4][2] = 2.0*SC;
            adTTc[4][4] = C2-S2;
            adTTc[3][3] = C;
            adTTc[3][5] = S;
            adTTc[5][3] = -S;
            adTTc[5][5] = C;

            /* --- composition of transformation matrices --- */
            if (rot == 0.0) 
                xmatmul(adTTc, adTTb, adTT); //adTT = adTTc * adTTb
            else {
                /* (C) rotate around Znew */
                ang = rot * degrad;
                Double C = cos(ang);
                Double S = sin(ang);
                Double C2 = C * C;
                Double S2 = S * S;
                Double SC = S * C;
                adTTa[0][0] = C2;
                adTTa[0][1] = S2;
                adTTa[0][3] = SC;
                adTTa[1][0] = S2;
                adTTa[1][1] = C2;
                adTTa[1][3] = -SC;
                adTTa[2][2] = 1.0;
                adTTa[3][0] = -2.0 * SC;
                adTTa[3][1] = 2.0 * SC;
                adTTa[3][3] = C2 - S2;
                adTTa[4][4] = C;
                adTTa[4][5] = S;
                adTTa[5][4] = -S;
                adTTa[5][5] = C;
                xmatmul(adTTa, adTTc, adTTw);
                xmatmul(adTTw, adTTb, adTT);
            }

            /* --- global elasticity matrix --- */
            xmatmul(adBB,adTT,adTTw);//adTTw=K

            for (UInt i=0;i<6;i++) {
                for (UInt j=0;j<6;j++) 
                    adTTa[i][j] = adTT[j][i];
            }
            //
            xmatmul(adTTa,adTTw,adBB);
            //
            A11_ = adBB[0][0];
            A12_ = adBB[0][1];
            A13_ = adBB[0][2];
            A14_ = adBB[0][3];
            A15_ = adBB[0][4];
            A16_ = adBB[0][5];
            A22_ = adBB[1][1];
            A23_ = adBB[1][2];
            A24_ = adBB[1][3];
            A25_ = adBB[1][4];
            A26_ = adBB[1][5];
            A33_ = adBB[2][2];
            A34_ = adBB[2][3];
            A35_ = adBB[2][4];
            A36_ = adBB[2][5];
            A44_ = adBB[3][3];
            A45_ = adBB[3][4];
            A46_ = adBB[3][5];
            A55_ = adBB[4][4];
            A56_ = adBB[4][5];
            A66_ = adBB[5][5];
        }
    }

}//namespace models