#pragma once
/** \file iclumptemplate.h
    * \brief Interface for clump templates.
    *
    * \addtogroup clumpsrc
    */

namespace itasca {
    class IThing;
}
namespace itascaxd {
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;
    class IClump;
    class IPebble;

    /** \brief Interface for a clump template.
        * This is the interface for a clump template. A clump template is composed of a clump, a 
        * surface description, and a reference orientation
        * \ingroup clumpsrc
        */
    class IClumpTemplate {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x4ebdb825;

        /// Return an IThing interface for this object.
        virtual IThing *                    getIThing() = 0;
        /// Return a const IThing interface for this object.
        virtual const IThing *              getIThing() const = 0;
        /// Return the clump.
        virtual IClump *                    getClump() = 0;
        /// Return the const clump.
        virtual const IClump *              getClump() const = 0;
        /// Return a component of the inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D. This is as the clump template has been imported
        virtual double                      getMoiComp(int) const = 0;
        /// Return the inertia tensor in principal axis form.
        virtual DAVect                    getMoi() const=0;
        virtual double                    getVolume() const=0;
        virtual void                      setVolume(const double &d)=0;



#ifdef THREED
        /// Return the original orientation
        virtual const Quat &      getOrigOrientation() const=0;
#else
        /// Get the double corresponding to the orientation relative to the clump template orientation
        virtual const double &    getOrigOrientation() const=0;
#endif
        /// Return the clump template name
        virtual QString getClumpTemplateName() const=0;     
        /// Indicate whether or not a clump in the model domain refers to this clump template
        virtual bool              getIsReferredTo() const=0;
        /// Clone this template completely, not adding it to any container
        virtual IClumpTemplate *  cloneAll(const QString &name=QString(),bool add=true) const=0;
        /// Decrement the number of times this is referenced
        virtual void              doDecNumReferenced()=0;
        /// Increment the number of times this is referenced
        virtual void              doIncNumReferenced()=0;
        /// Set a component of the inertia tensor. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D. 
        virtual void              setMoiComp(const double &,int) = 0;
        /// Set the inertia tensor in principal axis form.
        virtual void              setMoi(const DAVect &)=0;

#ifdef THREED
        /// Get the inertia tensor. 
        virtual SymTensor         getMoiTensor() const = 0;
#endif
        /// Get the original position of the clump template centroid
        virtual DVect               getOrigPosition() const =0;
        /// Set the original position of the clump template centroid
        virtual void                setOrigPosition(const DVect &p) =0;
        /// Return the position of the pebble
        virtual DVect               getPebblePosition(const IPebble *) const=0;

    };
} // namespace pfc
// EoF