#pragma once
// iextrudeblock.h

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
    class IExtrudeBlock {
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

        /// Return block multiplier, 1=default, 2,3,4...
        virtual quint32 getMultiplier() const=0;
        /// Set block multiplier, 1=default, 2,3,4...
        virtual void    setMultiplier(quint32 m)=0;
        /// Return block zoning sizes
        virtual UVect2  getZones() const=0;
        /// Return x,y position of indexed gridpoint
        virtual DVect   getDiscretizedLocation(const IVect2 &index) const=0;
        /// Return x,y position of parametrized location, mu1=0-1, mu2=0-1.
        virtual DVect   getParametricLocation(const double &mu1, const double &mu2) const=0;
        /// Return center as defined by parametrization (0.5,0.5)
        virtual DVect   getCentroid() const=0;

        /// Return (index+1)%4 for opposite edge of quads, trouble for triangles!
        virtual quint32 getOppositeEdge(quint32 index) const=0;
        // Assuming point v falls inside the poly, then returns the internal ratio that point represents on the set of opposite edges.
        // x() is the ratio for edges 0 and 2.  y() is the ratio for edges 1 and 3.
        // If the poly is triangular, the x() and y() is for 0, 1 and edge 2 depends on how the triangle distributes edge dependencies.
        /// 
        //    virtual DVect2  getInternalRatio(const DVect &v,const double &tolerance=limits<float>::epsilon()) const=0;

        /// Compute u,v parametric position of given point inside block
        virtual DVect2 computeGridParameter(DVect p) const=0;     
        /// compute split parameter for edge on indexed side of block
        virtual DVect computeSplitEdgePosition(DVect2 f, int side) const=0;

        /// Return true if point interior to block
        virtual bool isPointInside(DVect pos)=0;

        /// Return neighboring block on indexed edge, if any.
        virtual IExtrudeBlock* getNeighborBlock(int index)=0;    

        // test zone quality    
        /// test warning/bad zones, return counts
        void countZoneQuality(int statecount[]) const; // summary: zone states by type: 0=ok, 1=aspect ratio, 2=overlay1, 3=overlay2, 4=inverted/twisted
        /// Return true if any bad zones
        bool anyBadZones(bool engine);
        // Iterate list of bad zones
        /// Initialize search for warning or bad zones, pass in loop variables i,j
        virtual void resetSearchBadZones(int *i, int *j) const = 0;    
        /// Get next bad zone, and increment i,j loop variables, return false if done
        // engine bool specifies if the call is originating from the engine, and polling should happen - otherwise polling is suppressed.
        virtual bool nextSearchBadZone(int *i, int *j, int *ty, DVect *p1 ,DVect *p2, DVect *p3, DVect *p4,bool engine) const = 0; // false=search completed, return bad zone ccw points p1..p4
    };
} // namespace f3
// EoF
