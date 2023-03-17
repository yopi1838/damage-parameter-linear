#pragma once
// iextruderegion.h

#include "module/interface/ilinktype.h"

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IGeomPoly;
}

namespace extruder {
    using namespace itasca;
    using namespace itascaxd;

    class IExtrudeEdge;

    /// \brief Interface for IExtrudeEdge objects. 
    /// \ingroup FLAC3D
    class IExtrudeRegion {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4cb610e7;

        /// Return a const IThing interface for this object.
        virtual const IThing *   getIThing() const=0;
        /// Return an IThing interface for this object.
        virtual IThing *         getIThing()=0;
        /// Return a const IGeomPoly interface for this object.
        virtual const IGeomPoly *getIGeomPoly() const=0;
        /// Return a IGeomPoly interface for this object.
        virtual IGeomPoly *      getIGeomPoly()=0;

        /// return true if this is an irregular block
        virtual bool getIsIrregBlock() const = 0;
        /// Return block multiplier, 1=default, 2,3,4...
        virtual quint32 getMultiplier() const=0;
        /// Set block multiplier, 1=default, 2,3,4...
        virtual void    setMultiplier(quint32 m)=0;
        /// Return block zoning sizes
        virtual UVect2  getZones() const=0;
        /// Return total number of zones
        virtual quint64  numZones() const=0;
        /// Return x,y position of indexed gridpoint
        virtual DVect   getDiscretizedLocation(const IVect2 &index) const=0;
        /// Return x,y position of parametrized location, mu1=0-1, mu2=0-1.
        virtual DVect   getParametricLocation(const double &mu1, const double &mu2) const=0;
        /// Return center as defined by parametrization (0.5,0.5)
        virtual DVect   getCentroid() const=0;
        /// Compute u,v parametric position of given point inside block
        virtual DVect2 computeGridParameter(DVect p) const=0;     
        /// compute split parameter for edge on indexed side of block
        virtual DVect computeSplitEdgePosition(DVect2 f, int side) const=0;
        /// Return true if point interior to block
        virtual bool isPointInside(DVect pos)=0;
        /// Return neighboring region on indexed edge, if any.
        virtual IExtrudeRegion* getNeighborRegion(int index)=0;
        /// retun a list of unique line segments (QPair<DVect2,DVect2>(end1,end2)) for the unstructured mesh
        virtual QList<QPair<DVect2,DVect2>> getMeshLineSegments() const = 0;

        // Iterate list of bad zones
        /// Initialize search for warning or bad zones, pass in loop variables i,j
        virtual void resetSearchBadZones(int *i, int *j) const = 0;    
        /// Get next bad zone, and increment i,j loop variables, return false if done
        // engine bool specifies if the call is originating from the engine, and polling should happen - otherwise polling is suppressed.
        virtual bool nextSearchBadZone(int *i, int *j, int *ty, DVect *p1 ,DVect *p2, DVect *p3, DVect *p4,bool engine) const = 0; // false=search completed, return bad zone ccw points p1..p4
    };
} // namespace f3
// EoF
