#pragma once

#include "models/src/conmodel.h"

namespace models {

    class NormAxes3 {
        // Store a normal vector and keep a matching Axes3D object.
    public:
        NormAxes3() { norm_.fill(0.0); axes_ = Axes3D::make(DVect3(0.0, 0.0, 0.0));  }
        NormAxes3(const NormAxes3 &a2) { norm_ = a2.norm_; axes_ = a2.axes_; }
        virtual ~NormAxes3() {}
        const DVect3 &norm() const { return norm_; }
        void set_norm(const DVect3 &data) {
            norm_ = data;
            axes_ = Axes3D::make(norm_);
        }
        const Axes3D &axes() const { return axes_; }
    private:
        DVect3 norm_;
        Axes3D axes_;
    };

    class ModelWeakplaneType {
    public:
        ModelWeakplaneType();
        String getProperties() const {return L"dip,dip-direction,normal-x,normal-y,normal-z,normal,angle"; }
        virtual Variant getProperty(UInt index) const;
        virtual void    setProperty(UInt index,const Variant &p,UInt restoreVersion=0);

    protected:
        virtual void setDip(const Double&);
        virtual void setDd(const Double&);
        virtual void setNx(const Double&);
        virtual void setNy(const Double&);
        virtual void setNz(const Double&);
        virtual void setNorm(const DVect3&);
        virtual void setAngle(const Double&);

        Double dip_, dd_, angle_;
        NormAxes3 norma_;
    };
}

//EOF