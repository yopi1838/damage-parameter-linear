#pragma once
// iccfdhex.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
}

namespace ccfd {
    using namespace itasca;

    class ICcfdHex {
    public:
        static const TType type_ = 0x4fe9b4a9;

        virtual const IThing *               getIThing()           const = 0;
        virtual IThing *                     getIThing()                 = 0;

        //virtual const DVect3 & getDragForce()                      const = 0;
        //virtual void           setDragForce(const DVect3 v)              = 0;
        //virtual const double & getPorosity()                       const = 0;
        //virtual void           setPorosity(const double &)               = 0;
        //virtual const double & getVolume()                         const = 0;
        //virtual void           setVolume(const double &)                 = 0;
        //virtual const DVect3 & getPressureGradient()               const = 0;
        //virtual void           getPressureGradient(const DVect3 &)       = 0;
        //virtual const DVect3 & getFluidVelocity()                  const = 0;
        //virtual void           setFluidVelocity(const DVect3 &)          = 0;
        //virtual const DVect3 & getOldFluidVelocity()               const = 0;
        //virtual void           setOldFluidVelocity(const DVect3 &)       = 0;
        //virtual const DVect3  *getNode(int i)                      const = 0;
        //virtual const DVect3 & getCentroid()                       const = 0;
        //virtual const double & getFluidViscosity()                 const = 0;
        //virtual void           setFluidViscosity(const double &)         = 0;
        //virtual const double & getFluidDensity()                   const = 0;
        //virtual void           setFluidDensity(const double &)           = 0;
        //virtual const double & getFluidPressure()                  const = 0;
        //virtual void          setFluidPressure(const double &)           = 0;

    };

} // namespace ccfd
// EoF

