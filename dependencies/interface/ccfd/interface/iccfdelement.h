#pragma once
// iccfdelement.h
#include "base/src/farray.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IBody;
}

namespace ccfd {
    using namespace itasca;
    using namespace itascaxd;

    class ICcfdElement {
    public:
        inline static const TType type_ = 0x4fe8e071;
        virtual const IThing *               getIThing()                const = 0;
        virtual IThing *                     getIThing()                      = 0;
        virtual const IBody *                getIBody()                 const = 0;
        virtual IBody *                      getIBody()                       = 0;

        virtual int            getNumberOfNodes()                       const = 0;                                                       
        virtual int            getNumberOfFaces()                       const = 0;                                                       
        virtual DVect          getDragForce()                           const = 0;
        virtual void           setDragForce(const DVect &v)                   = 0;
        virtual double         getPorosity()                            const = 0;
        virtual void           setPorosity(const double &)                    = 0;
        virtual double         getVolume()                              const = 0;
        virtual void           setVolume(const double &)                      = 0;
        virtual DVect          getFluidPressureGradient()               const = 0;
        virtual void           setFluidPressureGradient(const DVect3 &)       = 0;
        virtual DVect          getFluidVelocity()                       const = 0;
        virtual void           setFluidVelocity(const DVect3 &)               = 0;
        virtual const DVect  & getOldFluidVelocity()                    const = 0;
        virtual void           setOldFluidVelocity(const DVect3 &)            = 0;
        virtual const DVect  & getNode(int i)                           const = 0;
        virtual  DVect         getCentroid()                            const = 0;
        virtual double         getFluidViscosity()                      const = 0;
        virtual void           setFluidViscosity(const double &)              = 0;
        virtual double         getFluidDensity()                        const = 0;
        virtual void           setFluidDensity(const double &)                = 0;
        virtual double         getFluidPressure()                       const = 0;
        virtual void           setFluidPressure(const double &)               = 0;

        virtual int            nNodesOnFace()                           const = 0;
        virtual DVect          faceNode(int face, int node)             const = 0;
        virtual const DVect  & faceNodeAddress(int face, int node)      const = 0;
        virtual bool           isExternalFace(int n)                    const = 0;
        virtual ICcfdElement * adjacentToFace(int n)                    const = 0;
        virtual void           getFaceConnectedList(FArray<ICcfdElement*> *ret) const = 0;
        virtual void           getEdgeConnectedList(FArray<ICcfdElement*> *ret) const = 0;
        virtual void           getNodeConnectedList(FArray<ICcfdElement*> *ret) const = 0;
    };

} // namespace ccfd
// EoF

