#pragma once

namespace itasca {
    class IThing;
}

namespace rblock {
    using namespace itasca;
    class IRBlock;

    class IRBlockTemplate {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x55ef2418;
        /// Return an IThing interface for this object.
        virtual IThing *                getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *          getIThing() const=0;
        /// Return an IRBlock interface for this object.
        virtual IRBlock *               getIRBlock()=0;
        /// Return a const IRBlock interface for this object.
        virtual const IRBlock *         getIRBlock() const=0;
        /// Return the name of the template
        virtual QString                 getName() const=0;
         /// Return a component of the inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D. This is as the RBlock template has been imported
        virtual double                  getMoiCompTemp(int) const = 0;
        /// Return the inertia tensor in principal axis form.
        virtual DAVect                  getMoiTemp() const=0;
        virtual double                  getVolumeTemp() const=0;
        virtual void                    setVolumeTemp(const double &d)=0;
        /// Set a component of the inertia tensor. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D. 
        virtual void                    setMoiCompTemp(const double &,int) = 0;
        /// Set the inertia tensor in principal axis form.
        virtual void                    setMoiTemp(const DAVect &)=0;
#ifdef THREED
        virtual SymTensor               getMoiTensor() const = 0;
#endif


    };
} // namespace rblock
// EoF

