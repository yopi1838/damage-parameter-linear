#pragma once
// iextrudeedge.h

#include "module/interface/ilinktype.h"

namespace itasca {
    class IThing;
}
namespace itascaxd {
    class IGeomEdge;
}

namespace extruder {
    using namespace itasca;
    using namespace itascaxd;

    class IExtrudePoint;
    class IExtrudeRegion;

    /// Edge types, simple, polyline or ellipse
    enum EdgeType { edgeTypeSimple=0, edgeTypePoly, edgeTypeEllipse };
    /// polyline subtypes: linear, arc or spline segments
    enum EdgePolyType { edgePolyTypeLinear=0, edgePolyTypeArc, edgePolyTypeSpline };

    /// \brief Interface for IExtrudeEdge objects. 
    /// \ingroup FLAC3D
    class IExtrudeEdge {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4cb610e6;

        /// Return a const IThing interface for this object.
        virtual const IThing *    getIThing() const=0;
        /// Return an IThing interface for this object.
        virtual IThing *          getIThing()=0;
        /// Return a const IGeomPoint interface for this object.
        virtual const IGeomEdge * getIGeomEdge() const=0;
        /// Return a IGeomPoint interface for this object.
        virtual IGeomEdge *       getIGeomEdge()=0;
        /// Are there any blocks attached?
        virtual bool        isBlocked() const=0;
        /// Is edge simple (linear, no control points)?
        virtual bool        isSimple() const=0;
        /// Is edge a polyline (linear and at least one control point)?
        virtual bool        isPolyline() const=0;
        /// Is edge a "curved" type (requiring sampling approximation)?
        virtual bool        isCurved() const=0;

        /// Adds \b point at \b end.
        virtual void                  setPointExtrude(IExtrudePoint *point,quint32 end)=0;
        /// Returns a pointer to IExtrudePoint at \b end
        virtual IExtrudePoint *       getIExtrudePoint(quint32 end)=0;
        /// Returns a const pointer to IExtrudePoint at \b end
        virtual const IExtrudePoint * getIExtrudePoint(quint32 end) const=0;

        /// Returns a interface to a CLinkType<> object, a const pointer to the next object in the linked list.
        virtual CLinkType<IExtrudeEdge> getNextExtrude(quint32 end) const=0;
        /// Returns a interface to a CLinkType<> object, a pointer to the next object in the linked list.
        virtual LinkType<IExtrudeEdge>  getNextExtrude(quint32 end)=0;
        /// Returns a interface to a CLinkType<> object, a const pointer to the first object in the linked list.
        virtual CLinkType<IExtrudeRegion> getStartExtrude() const=0;
        /// Returns a interface to a CLinkType<> object, a pointer to the first object in the linked list.
        virtual LinkType<IExtrudeRegion>  getStartExtrude()=0;

        /// Return (zone) size.
        virtual quint32         getSize() const=0;
        /// Return zone ratio
        virtual double          getRatio() const=0;
        /// Return zone ratio isolate flag (Stops propagating ratios by connected blocks)
        virtual bool            getRatioIsolate() const=0;
        /// Return x,y gridpoint position along edge
        virtual DVect           getDiscretizedLocation(quint32 index,quint32 multiplier) const=0;
        /// Return x,y parametric position, frac0=0 to 1
        virtual DVect           getParametricLocation(const double &frac0) const=0;
        /// Return list of edges connected by block opposite sides, used for assigning zone sizes and ratios
        virtual QList<quint64> getConnectedEdges() const=0;

        /// Set (zone) size.
        virtual quint32         setSize(quint32 s)=0;
        /// Set zone ratio on edge
        virtual quint32         setRatio(const double &r)=0;
        /// Set zone ratio isolate flag (Stops propagating ratios by connected blocks)
        virtual void            setRatioIsolate(bool b)=0;

        /// Return number of blocks connected to edge
        virtual int   getEdgeBlockCount() const=0; // 0-2 blocks connected to an edge
        /// Return x,y split position of block opposite edge to this edge, by parametric position mu.
        virtual DVect computeBlockSplitPoint(double mu, int index) const=0; // position for split on opposite edge of indexed block

        /// Return EdgeType: 0=simple, 1=polyline, 2=ellipse arc
        virtual quint32 getEdgeType() const=0;
        /// Return EdgeType: 0=simple, 1=polyline, 2=arc, 3=spline, 4=ellipse arc
        virtual quint32 getHybridType() const=0;
        /// Return ellipse arc center point
        virtual const IExtrudePoint* getEllipseArcCenter() const=0;
        /// Return ellipse arc "with" point
        virtual const IExtrudePoint* getEllipseArcWith() const=0;

        /// Return polyline length (linear segments only)
        virtual double getEdgePolylineLength() const=0;
        /// Return computed length of edge, including estimate for curved edges
        virtual double computeEdgePolylineCurveLength() const=0;

        /// Return PolylineType: 0=linear segments, 1=circle arcs, 2=cubic splines
        virtual quint32 getEdgePolylineType() const=0;
        /// Return number of segments in polyline
        virtual quint32 getEdgePolylineSize() const=0;
        /// Return position of control points, 1..size-1, index 0,size return endpoint positions
        virtual DVect getEdgePolylineControlPoint(int id) const=0;

        /// Return local U coordinate of controlpoint
        virtual double getEdgePolylineControlU(int id) const=0;
        /// Return local V coordinate of controlpoint
        virtual double getEdgePolylineControlV(int id) const=0; 
        /// Set x,y position of controlpoint
        virtual void setEdgePolylineControlPoint(int index, DVect p)=0;
        /// Set u,v local position of controlpoint
        virtual void setEdgePolylineControlLocal(int index, double u, double v)=0;
        /// Set u local position of controlpoint
        virtual void setEdgePolylineControlLocalU(int index, double u)=0;
        /// Set v local position of controlpoint
        virtual void setEdgePolylineControlLocalV(int index, double v)=0;

        /// Add controlpoint by x,y position
        virtual void addEdgePolylineControlPoint(DVect p)=0;
        /// Add controlpoint by u,v position
        virtual void addEdgePolylineControlLocal(double u, double v)=0;
        /// Remove indexed control point
        virtual void removeEdgePolylineControlPoint(int id)=0;

        /// Estimate parametric position (0,1) of position 
        virtual double findParameterForPosition(DVect pos) const=0;

        /// Set edge type as simple
        virtual void setEdgeTypeSimple()=0;
        /// Set EdgeType 
        virtual void setEdgeType(int edgeType)=0;
        /// Set EdgeType as polyline, and polyline type: 0=linear, 1=circle arcs, 2=cubic splines
        virtual void setEdgePolylineType(int curvetype=edgePolyTypeLinear)=0;

        /// Return block, besides given one, sharing this edge, 0 if none
        virtual IExtrudeRegion* getOtherBlock(IExtrudeRegion* b)=0;

        // Check if the edge is connected to an irregular block
        virtual bool belongsToIrregBlock() const=0;
    };

} // namespace f3
// EoF
