#pragma once
// igeompoly.h

/// \file
/// \brief The interface for a geometry polygon representing a closed collection of edges.
   
#include "ilinktype.h"
#include "dim.h"
#include "base/src/farray.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IGeomPoint;
    class IGeomEdge;
    class GeomBase;
    /// \brief A class representing a single closed polygon made up of a list of edges.
    /// \ingroup kernel
    class IGeomPoly {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bce;

        /// Return an IThing interface for this object.
        virtual IThing *         getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *   getIThing() const=0;

        /// Returns the number of edges associated with this polygon.  This is also the number of points if the polygon is closed.
        virtual quint64           getSize() const=0;
        /// Returns the number of fake edges associated with this polygon. Only irregular blocks with internal polygons (boundaries) have fake edges
        virtual quint64           getNumFakeEdges() const =0;
        /// Returns the edge at index \a index in the list of edges associated with the polygon.  Will return 0 if index >= getSize().
        virtual IGeomEdge *       getEdge(quint32 index)=0;
        /// Returns the edge at index \a index in the list of edges associated with the polygon.  Will return 0 if index >= getSize().
        virtual const IGeomEdge * getEdge(quint32 index) const=0;
        /// Adds an edge to the polygon.  The point at one end of the edge *must* match the point at the free end of the last edge of the polygon.
        /// Return true if successful, false if not.  By default throws an exception if the edge cannot be added validly.
        /// Set \a throwException to false if you do not want an exception thrown.
        /// Set \a forceFlip to true if you want to force the edge to be flipped on entry - only applies to the first edge added.
        virtual bool              addEdge(IGeomEdge *edge,bool throwException=true,bool forceFlip=false)=0;
        /// Removes the last edge from the polygon, reducing it's size by one and rendering it invalid (if it was previously valid).
        /// Returns a pointed to the edge removed.
        virtual IGeomEdge *       removeEdge()=0;
        /// Checks the validity of the polygon - the polygon must be fully closed, and each edge must match a point in the next edge.
        virtual bool              isValid() const=0;
        /// Returns the point at index /a index in the polygon, assuming it is valid.
        virtual IGeomPoint *      getPoint(quint32 index)=0;
        /// Returns the point at index /a index in the polygon, assuming it is valid.
        virtual const IGeomPoint *getPoint(quint32 index) const=0;
        /// When following the edge order around the polygon, the order of vertices could go from end1 to end2 or from end2 to end1.
        /// Returns true if the vertex order is from end2 to end1, false otherwise.
        virtual bool              getFlip(quint32 index) const=0;
        /// Returns the vector from point \a index to point \a index+1 as you go around the polygon.
        virtual DVect             getVector(quint32 index) const=0;
        /// Returns the centroid, calculated as the average of the point positions.
        virtual DVect             getCentroid() const=0;
        /// Returns the area of the polygon, assuming the polygon is planar and *radially* convex about the centroid.
        virtual double            getArea() const=0;
        /// Returns the normal vector or the polygon, based on an average of each vertex if the polygon is not planar.
        virtual DAVect            getDANormal() const=0;
        /// Returns the normal vector or the polygon as used in ThingData.
        virtual DVect3            getNormal() const=0;
        /// Returns true if the polygon is planar within \a tolerance,by checking the distance of vertices from the average plane.
        /// Returns false if maxdist*cendist > tolerance*area, where maxdist is the maximum distance of a vertex from the average plane,
        ///    cendist is the distance from the centroid to that vertex, and area is the area of the polygon assuming it was planar.
        virtual bool              isPlanar(const double &tolerance=1e-7) const=0;
        /// Returns true if the polygon is convex within \a tolerance.
        virtual bool              isConvex(const double &tolerance=1e-7) const=0;
        /// Returns true if the point \a v, after being projected onto the plane of the polygon, falls inside the polygon boundaries.
        /// \a on (if provided) is filled with the projected point.
        /// \a tolerance is used to determine if the projected point is inside or outside.
        virtual bool              isIn(const DVect &v,DVect *on=0,const double &tolerance=1e-7) const=0;
        /// Returns the closest point to /a v on the polygon, which could be on a point, on an edge, or on the surface of the polygon.
        virtual DVect             getClosestPoint(const DVect &v) const=0;

        /// Returns the connect index, computed.
        virtual int               getConnectIndex() const { return 0; }
        /// Returns the manifold index, computed.
        virtual int               getManifoldIndex() const { return 0; }
        /// Returns the front side index, computed.
        virtual int               getFrontSideIndex() const { return 0; }
        /// Returns the back side index, computed.
        virtual int               getBackSideIndex() const { return 0; }
         
        /// Returns an interface to a CLinkType<> object, a const pointer to the next object in the linked list.
        virtual CLinkType<IGeomPoly> getNext(quint32 index) const=0;
        /// Returns an interface to a LinkType<> object, a pointer to the next object in the linked list.
        virtual LinkType<IGeomPoly>  getNext(quint32 index) =0;
        /// Return const GeomBase 
        virtual const GeomBase * getGeomBase() const=0;
        /// Convert to global coordinate the point on the surface. If (u,v) = (0,0) it will be position of centroid, 
        /// If (u,v) = (1,0) -- position of point(0), (u,v) = (0,1) -- position of point(1) . Returns zero vector if polygon is not valid.
        virtual DVect convertToGlobal(double u, double v) const =0;
#ifdef THREED
        /// Convert to local polygon coordinate (u,v) of a 3d position point on the polygon. 
        /// No check if the point is on the polygon, or polygon is planar and convex. 
        virtual DVect2 convertToLocal(const DVect &pos) const =0;
#endif

        /// Return a list of triangulated vertices for plotting 
        virtual void             triangulatedList(FArray<DVect> * ret, const double &tolerance = 1e-7)=0;

        virtual quint32 index(const IGeomEdge *edge) const = 0;
        virtual quint32 index(const IGeomPoly *vert) const = 0;
        virtual void setEdge(quint32 ind, IGeomEdge *edge) = 0;

    };
} // namespace itascaxd
// EoF
