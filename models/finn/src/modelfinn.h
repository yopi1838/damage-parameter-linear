#pragma once

#include "models/mohr/src/modelmohr.h"

namespace models {
  class ModelFinn : public ModelMohr {
    public:
      ModelFinn();
      virtual String        getName() const { return(L"finn"); }
      virtual String        getFullName() const { return(L"Finn"); }
      virtual UInt          getMinorVersion() const;
      virtual String        getProperties() const;
      virtual Variant       getProperty(UInt index) const;
      virtual void          setProperty(UInt index,const Variant &p,UInt restoreVersion=0);
      virtual ModelFinn     *clone() const { return(NEW("ModelFinn") ModelFinn()); }
      virtual void          copy(const ConstitutiveModel *mod);
      virtual void          initialize(UByte dim,State *s);
      virtual void          run(UByte dim,State *s); 
      virtual void          save(std::ostream &os) const;
      virtual void          restore(std::istream &is,UInt restoreVersion);
      // Optional
      virtual bool          isPropertyReadOnly(UInt i) const;
      virtual bool          isPropertyAdvanced(UInt i) const;
      virtual bool          supportsHystereticDamping() const { return(true); }  
      virtual bool          supportsStressStrengthRatio() const { return false; }
      virtual bool          supportsPropertyScaling() const { return false;}
      virtual bool          isLiquefaction() const { return true; }

    private:
      Double   sHP_,tHP_,fC1_,fC2_,fC3_,fC4_,eVD_,count_,latency_;
      String   cTable_,fTable_,dTable_,tTable_;
      UInt     switch_;
      Double   wE12_,wE23_,wE31_,wE1M2_,wE2M3_,wE3M1_,wE12P_,wE23P_,wE31P_;
      Double   wE1M2P_,wE2M3P_,wE3M1P_,uV1_,uV2_,uV3_,uV4_,uV5_,uV6_;
      Double   dEVD_,dPPrev_,latCnt_;
      Double   sf1_,sf3_;
      void     *iCohesion_,*iFriction_,*iDilation_,*iTension_;
  };
}
// EOF