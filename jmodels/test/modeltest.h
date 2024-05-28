#pragma once

#include "models/src/conmodel.h"
#include "models/src/convert.h"

namespace models {
    class Model$classname$ : public ConstitutiveModel {
      public:
        Model$classname$(unsigned short option = 0);
        string getName() const override;
        string getFullName() const override;
        uint32 getMinorVersion() const override;
        string getProperties() const override;
        string getStates() const override;
        base::Property getProperty(uint32 index) const override;
        void setProperty(uint32 index, const base::Property& p, uint32 restoreVersion = 0) override;
        ConstitutiveModel* clone() const override { return new Model$classname$(); }
        double getConfinedModulus() const override { return bulk_ + shear_*d4d3; }
        double getShearModulus() const override { return shear_; }
        double getBulkModulus() const override { return bulk_; }
        void copy(const ConstitutiveModel* mod) override;
        void run(uint32 dim, State* s) override;
        void initialize(uint32 dim, State* s) override;
        // Optional
        bool isPropertyAdvanced(uint32 i) const override;
        bool supportsStrengthStressRatio() const override { return true; }
        bool supportsPropertyScaling() const override { return true; }
        double getStrengthStressRatio(const SymTensor& st) const override;
        void scaleProperties(const double& scale, const std::vector<uint32>& props) override;
        bool isKGEv() const override { return false; }

      private:
        double moduliReduction(const double& factor);
        void apexCorrection(const double& fric, State* s, DVect3* prin, uint32* iPlasticity = nullptr, bool bBrittle = false);
        void tensionCorrection(State* s, DVect3* prin, uint32* iPlasticity, const double& ftz, bool bBrittle = false);
        void shearCorrection(State* s, DVect3* prin, uint32* iPlasticity, const double& fs);

        double bulk_ = 0.0;
        double shear_ = 0.0;
        double cohesion_ = 0.0;
        double friction_ = 0.0;
        double dilation_ = 0.0;
        double tension_ = 0.0;
        bool brittle_ = false;

        double e1_ = 0.0;
        double e2_ = 0.0;
        double g2_ = 0.0;
        double nph_ = 0.0;
        double csn_ = 0.0;
        double nps_ = 0.0;
        double rc_ = 0.0;
        double sc1_ = 0.0;
        double sc2_ = 0.0;
        double sc3_ = 0.0;
    };
} // namespace models

// EOF
