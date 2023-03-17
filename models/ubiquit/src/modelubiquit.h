#pragma once

#include "models/mohr/src/modelmohr.h"
#include "models/base/modelweakplanetype.h"

namespace models {
    class ModelUbiquitous : public ModelMohr, public ModelWeakplaneType {
    public:
        ModelUbiquitous(unsigned short option=0);
        //
        virtual String  getName() const { return(L"ubiquitous-joint"); }
        virtual String  getFullName() const { return(L"Ubiquitous Joint"); }
        virtual UInt    getMinorVersion() const;
        virtual String  getProperties() const; 
        virtual String  getStates() const;
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
        virtual ModelUbiquitous *clone() const { return(NEW("ModelUbiquitous") ModelUbiquitous()); }
        virtual void    copy(const ConstitutiveModel *mod);
        virtual void    initialize(UByte dim,State *s);
        virtual void    run(UByte dim,State *s); 
        // Optional
        virtual bool    supportsHystereticDamping() const { return(true); }
        virtual bool    supportsStressStrengthRatio() const { return false; }
        virtual bool    supportsPropertyScaling() const { return true; }
        virtual void    scaleProperties(const Double &scale,const std::vector<UInt> &props);

    protected:
        virtual bool updateJointParameters(bool bEUpdated);
        virtual void stressToLocal(State *s,const Axes3D &aAxes);
        virtual void stressToGlobal(State *s,const Axes3D &aAxes);
        virtual void apexCorrectionJoint(State *s,UInt *jplasticity);
        virtual void tensionCorrectionJoint(State *s,UInt *jPlasticity,const Double &fjt);
        virtual void shearCorrectionJoint(State *s,UInt *jPlasticity,const Double &fjs);
        virtual void largeStrainCorrection(State *s,const Axes3D &aAxes);

        Double jCohesion_,jFriction_,jDilation_,jTension_;
        Double normX_,normY_,normZ_;
        Double sig_,tau_,sp13_,sp23_,dsp11_,dsp33_,dsp13_,dsp23_;
        Double qphi_,qpsi_,jrc_,jc1_,jc2_;
    };
}
//EOF