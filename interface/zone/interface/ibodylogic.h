//ibodylogic.h
#pragma once

#include "module/interface/dim.h"

namespace zone {
    class IBodyLogic {
    public:
        virtual void   computeBody() const=0;
        virtual uint   getBodyNotice() const=0;

        virtual DVect3 computeFaceGP(int fi, int g1, int g2) const=0;
        virtual DVect3 computeWedgeFaceGP(int fi, double g1, double g2) const=0;
        //  virtual DVect3 computeFaceSubGP(int fi, int g1, int g2, int s1, int s2, int m1, int m2) const=0;
        virtual DVect3 computeCellGP(int ci, int g1, int g2, int g3/*, int n1, int n2, int n3*/) const=0;

        virtual int getVertCount() const=0;
        virtual int getEdgeCount() const=0;
        virtual int getFaceCount() const=0;
        virtual int getCellCount() const=0;
        virtual int getDimCount() const=0;
        virtual int getSizeCount() const=0;
        virtual int getRatCount() const=0;
        virtual bool isVertUsed(int vi) const=0;
        virtual bool isEdgeUsed(int ei) const=0;
        virtual bool isFaceUsed(int fi) const=0;
        virtual bool isCellUsed(int ci) const=0;
        virtual DVect getVertexPos(int vi)const =0;
        virtual int getEdgeType(int ei) const=0; //0=linear, 1=arc
        virtual int getEdgeVertexIndex1(int ei) const=0;
        virtual int getEdgeVertexIndex2(int ei) const=0;
        virtual int getEdgeArcDimIndex(int ei) const=0;
        virtual int getEdgeArcCenterIndex(int ei) const=0;
        virtual int getEdgeSizeIndex(int ei) const=0;
        virtual int getEdgeRatioIndex(int ei) const=0;
        virtual int getFaceVertexIndex(int fi,int n) const=0;
        virtual int getFaceEdgeIndex(int fi,int n) const=0;
        virtual int getFaceISizeValue(int fi) const=0;
        virtual int getFaceJSizeValue(int fi) const=0;
        virtual bool isFaceSimple(int fi) const=0;
        virtual bool isFaceVisible(int fi) const=0;
        virtual int getCellVertexIndex(int ci,int n) const=0; // n=0..7
        virtual int getCellEdgeIndex(int ci,int n) const=0; // n=0..11
        virtual int getCellFaceIndex(int ci,int n) const=0; // n=0..5
        virtual bool isCellFilled(int ci) const=0;
        virtual DVect getCellCenter(int ci) const=0;
        //  virtual int getCellMultI(int ci) const=0;
        //  virtual int getCellMultJ(int ci) const=0;
        //  virtual int getCellMultK(int ci) const=0;
        virtual int getCellGroupIndex(int ci) const=0;
        //  virtual int getCellFaceMultI(int ci, int findex) const=0; // findex=0..5
        //  virtual int getCellFaceMultJ(int ci, int findex) const=0;
        virtual int getCellForm(int ci) const=0;
        virtual double getDimValue(int di) const=0;
        virtual double getRatioValue(int ri) const=0;
        virtual int getSizeValue(int ni) const=0;
        virtual int getGroupSize() const=0;
        virtual QString getGroupName(int gi) const=0;
        virtual QString getName() const=0;
    };
} // namespace zone
// EoF
