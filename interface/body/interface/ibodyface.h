//ibodyface.h
#pragma once
/**
* \file
* \brief Interface to provide to a specifics related to IBodyFace (related to Body snapon tool specifics but not general GeomPoly) 
*/


namespace body{

    using namespace itasca;
    using namespace itascaxd;
    class IBodyBlock;

    class IBodyFace {
    public:
        inline static const TType type_ = 0x54a5a3df;
        /// returns a const IThing pointer
        virtual const IThing    *   getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing          *   getIThing()=0;
        /// Return x,y position of indexed gridpoint. Indexes must bound from 0 to blockMultiplier X edgeZoneSize.
        virtual DVect   getDiscretizedLocation(const IBodyBlock *block,quint32 ig, quint32 jg) const=0;
        /// Return bount for index from zero to maxIndex=edgeZoneSize X blockMultiplier 
        virtual UVect2 getZoneIndBound(const IBodyBlock *block)const=0;
        /// return GUI specific select flag
        virtual bool getGUIselect() const=0;
        /// set GUI specific select flag
        virtual void setGUIselect(bool flag) const=0;
        /// compute complete discretized location grid (including control points), szx and szy are [0,szx] and [0,szy], 
        virtual void getDiscretizedLocationCompleteGrid(const IBodyBlock *block, std::vector<std::vector<DVect>> &zpCoords, quint32 &szx, quint32 &szy)const=0;
        /// get surface line points. Return points in the array size (number +2) or (number of internal line points plus its ends).
        ///"from" and "to" end points in local face coordinates.
        virtual FArray<DVect> getSurfaceLine(const DVect2 &from, const DVect2 &to, quint32 number) const=0;

        /// get type of interpolating curved face 0 - NotCurved, 1- BicubicSpline
        virtual qint32 getCurvedFaceType()const = 0;
        /// sets interpolating curved face 0 - NotCurved, 1- BicubicSpline
        virtual void setCurveType(quint32 type) = 0;
        /// returns number of control points wich is allwas grid nXm on face (corner BodyPoint's are also counted there).
        virtual quint32 getControlPointsSize()const = 0;
        /// return size n of nXm grid of control face points
        virtual quint32 getControlPointsSizeN()const = 0;
        /// return size m of nXm grid of control face points
        virtual quint32 getControlPointsSizeM()const = 0;
        /// removes all control points from face making it NotCurved
        virtual void removeAllControlPonts() = 0;
        /// get position of a control point
        virtual DVect getControlPointPosition(quint32 i, quint32 j)const = 0;
        /// set position of a control point
        virtual void setControlPointPosition(quint32 i, quint32 j, const DVect &pos)= 0;
        /// get indexes to control point from the internal surface coordinates (u,v) (clossest CP will be found)
        virtual QPair<qint32,qint32> getControlPointIndex(double u, double v)const = 0;
        /// get indexes to control point from the position (clossest CP will be found)
        virtual QPair<qint32, qint32> getControlPointIndex(DVect &pos)const = 0;
    };
};
