#pragma once
// ibody.h

// Interface for a Body which can be composed of multiple Pieces.
// Also properties.
// Other utility functions required by cell space.

#include "dim.h"
namespace itasca {
    class IFishParam;
    class IGroup;
    class IThing;
}

namespace itascaxd {
    using namespace itasca;
    class IPiece;

    /// \ingroup module
    class IBody {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b83;

        /// Enumerator for defining the geometric attributes of a body relative to a geometric shape.
        enum InOutMode { FullyOut=0, FullyIn, IntersectOut, IntersectIn, Surround};

        /// Return IThing interface associated with this piece.
        virtual IThing *        getIThing()=0;
        virtual const IThing *  getIThing() const=0; ///< \copydoc getIThing

        /// Get the first piece
        virtual IPiece *        getFirstPiece()=0;
        virtual const IPiece *  getFirstPiece() const=0; ///< \copydoc getFirstPiece
        /// Get the first IThing - may be different than getFirstPiece
        virtual IThing *        getFirstIThing()=0;
        virtual const IThing *  getFirstIThing() const=0; ///< \copydoc getFirstPiece
        /// Return a list of all the attribute names
        virtual QStringList     getAllAttributes() const =0;
        /// Return the value of the attribute.
        virtual QVariant        getAttribute(const QString &name) const = 0;

        // Return the extent of a body
        virtual DExtent3        getExtent() const=0;
        // Return a boolean indicating whether or not a box is completely inside the body
        virtual bool            isBoxIn(const DExtent &d,const double &tol=1e-7) const=0;
        // Return a boolean indicating whether or not the body intersects a DExtent
        virtual bool            intersectsBox(const DExtent &d,const double &tol=1e-7) const=0;
        // Return the in/out state
        virtual InOutMode       checkInOutSphere(const DVect &,const double &) const=0;

        virtual Quat            getOrient() const=0;
        
        // Get/set the fragment index
        virtual int             getFragmentIndex() const=0;
        virtual void            setFragmentIndex(int ind)=0;

        // Reset the time step contributions
        virtual void            resetTimeStepContributions()=0;

        // Get the stress from the previous timestep
#ifdef TWOD
        virtual void            getOldStress(std::array<double,3> &) const=0;
#else
        virtual void            getOldStress(std::array<double,6> &) const=0;
#endif
        // Get the volume 
        virtual double          getVolume() const=0;
        // Returns true if any DOF is fixed
        virtual bool            getAnyFixed() const=0;
        virtual bool            getAllFixed() const=0;
        virtual DVect           getPosition() const=0;
        virtual std::vector<quint64>    getSurfaceTriangles() const=0;

    };
} // namespace itascaxd
// EoF
