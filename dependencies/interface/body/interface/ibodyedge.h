//ibodyedge.h
#pragma once
/**
* \file
* \brief Interface to provide to a specifics related to IBodyEdge (related to Body snapon tool but not general GeomEdge) 
*/


namespace body{
using namespace itasca;
using namespace itascaxd;
    class IBodyEdge {
    public:
       /// The type identification number for this class, for use in convert_cast() and convert_getcast().
       inline static const TType type_=0x54d4ede7;
       /// returns a const IThing pointer
       virtual const IThing    *   getIThing() const=0;
       /// returns a IThing pointer
       virtual IThing          *   getIThing()=0;
       ///set (zone size) and returns counts of successfull zonesizes set (propagation assumed)
       virtual quint32         setZoneSize(quint32 s, bool maintainFactor=false)=0;
       ///return zone size for the edge
       virtual quint32         getZoneSize() const=0;
       /// Set zone ratio for the edge (propagation)
       virtual void            zoneSize(quint32 s)=0;
       /// Set zone ratio on edge. Returns counts (propagation depends on setRatioIsolate flag)
       virtual quint32         setRatio(const double &r)=0;
       /// Return zone ratio of the edge
       virtual double          getRatio()const=0;     
       /// Return zone factor of the edge (ratio of zone-lengths  last over first)
       virtual double getFactor()const=0;
       /// Sets zone factor of the edge (ratio of zone-lengths  last over first)
       /// Returns counts (propagation depends on setRatioIsolate flag)
       virtual quint32         setFactor(const double &r)=0;
       /// Set zone ratio isolate flag (Stops propagating ratios by connected blocks)
       virtual void            setRatioIsolate(bool b)=0;
       /// Return ratio isolate flag
       virtual bool          getRatioIsolate()const=0; 
       /// return GUI specific select flag
       virtual bool getGUIselect() const=0;
       /// set GUI specific select flag
       virtual void setGUIselect(bool flag) const=0;
       /// get type of interpolating curve 0 - polyline, 1- CircleArc, 2-Spline
       virtual int getCurveType()const=0;
       /// sets interpolating curve type : 0 - polyline, 1- CircleArc, 2-Spline
       virtual void setCurveType(quint32 type)=0;
       /// returns number of control points
       virtual quint32 getControlPointsSize()const =0;
       /// returns total length of curved edge
       virtual double getCurveLength()const =0;
       /// add control point
       virtual void addControlPoint(const DVect &pos) =0;
       /// remove control point from the edge at index=ind;
       virtual void removeControlPoint(quint32 ind)=0;
       /// get control point position
       virtual DVect getControlPointPosition(quint32 index /* 1-based */ ) const=0;
       /// set control point new position
       virtual void setControlPointPosition(quint32 ind,const DVect &pos)=0;
       /// get index to control point from the position in space (clossest 
       virtual quint32 getControlPointIndex(const DVect &pos)const=0;
       /// get index to control point from the position in space
       virtual quint32 getControlPointIndex(double u)const=0;
       /// return closest distance and points of curved edge with the segment (a1,a2) 
       virtual double closestWithSegment(const DVect &a1, const DVect &a2, DVect &segmentPos, DVect &edgeCurvePos)const=0;
       /// return closest distance and points of curved edge with the ray (a1,direction). Vector direction must be non-zero length.
       virtual double closestWithRay(const DVect &a1, const DVect &direction, DVect &rayPos, DVect &edgeCurvePos)const=0;
       /// return local corrdinate (parameter U) of a control point of the edge. Index must be in [1,getControlPointsSize()].
       virtual double getControlPointU(quint32 index /* 1-based */)const=0;
   };
}
