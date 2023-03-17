#pragma once
//ibodyblock
/**
* \file
* \brief Interface to provide access to a IBodyBlock 
*/

#include "utility/interface/igroup.h"
#include  "base/src/farray.h"

namespace itasca {
    class IThing;
}
namespace itascaxd{
    class IGeomPoly;
    class IGeomEdge;
    class IGeomPoint;
}

namespace body{
using namespace itasca;
using namespace itascaxd;
    /// \brief Interface to provide acces to BodyBlock 
    class IBodyBlock{
       
       public:
       /// Type of block enumeration ({Hexahedron=1, Tetrahedron, Wedge, Default}). Default usually means Hexahedron
       enum blockType{Hexahedron=1, Tetrahedron, Wedge, Default};
       /// Type of valid state
       enum blockValidState { 
              WrongTopologyFaceAttachment=-7, 
              WrongNumberOfNodes,
              IntersectWithOthers,
              HasConcaveFaces,
              SelfIntersections,
              NegativeOrZeroVolume,
              BadDegeneracy,
              AFaceHasAttachedBlocksFromOneSide,
              Contained, // the block is contained inside another block
              ValidBlock,
              ValidButNotAttached,
              DefaultState
              };
       
       /// The type identification number for this class, for use in convert_cast() and convert_getcast().
       inline static const TType type_=0x53a34354;
       /// returns a const IThing pointer
       virtual const IThing    *   getIThing() const=0;
       /// returns a IThing pointer
       virtual IThing          *   getIThing()=0;
       /// Returns the number of faces associated with this block. 6-for hexahedron, 5-wedge block, 4-for tetrahedron
       virtual quint32           getSize() const=0;
       /// Returns the number of edges associated with this block. 12-for hexahedron, 9-wedge block, 6-for tetrahedron
       virtual quint32           getSizeEdges() const=0;
       /// Returns the number of points associated with this block. 8-for hexahedron, 6-wedge block, 4-for tetrahedron
       virtual quint32           getSizePoints() const=0;
       /// Returns the face at index \a index in the list of faces associated with the block.  Will return 0 if index >= getSize().
       virtual IGeomPoly*       getFace(quint32 index)=0;
       /// Returns the face at index \a index in the list of faces associated with the block.  Will return 0 if index >= getSize().
       virtual const IGeomPoly*       getFace(quint32 index)const =0;
       /// Returns the edge at index \a index in the list of edges associated with the block.  Will return 0 if index >= getSizeEdges().
       virtual const IGeomEdge * getEdge(const quint32 index) const=0;
       /// Returns the edge at index \a index in the list of edges associated with the block.  Will return 0 if index >= getSizeEdges().
       virtual IGeomEdge * getEdge(quint32 index)=0;
       /// Returns the point at index \a index in the list of edges associated with the block.  Will return 0 if index >= getSizePonts().
       virtual const IGeomPoint * getPoint(quint32 index) const=0;
       /// Returns the point at index \a index in the list of edges associated with the block.  Will return 0 if index >= getSizePonts().
       virtual IGeomPoint * getPoint(quint32 index)=0;
       /// Checks the validity of the block - the block must form fully closed convex volume.
       virtual bool              isValid() const=0;
       /// Returns the centroid, calculated as the average of the point positions.
       virtual DVect             getCentroid() const=0;
       /// Returns the area of the block (sum of faces areas), assuming the block is valid.
       virtual double            getArea() const=0;
       /// Returns volume of the block: for hexahedron block with nonplanar faces uses exact formula from trilinear/bilinear interpolation of the volume of the block.
       virtual double            getVolume(double tol=1.0e-9) const=0;
       /// Returns validState flag
       virtual blockValidState  getValidState()const=0;
       /// Return validState string message
       virtual QString getValidStateString()const=0;
       /// Place into edgesOrder Array of pairs of point indicies, each of the pair  correspondes to  an edge between two points
       virtual void              getEdgesOrder(FArray<QPair<quint32,quint32>> &edgesOrder)const=0;
       /// Place ino facesOrder array of array of point indicies, each of array of point indicies correspond to points of the face
       virtual void              getFacesOrder( FArray< FArray<quint32> > &facesOrder)const=0;
       /// Returns true if the point \a v, is inside Block, check by projection of the point on each face plane and checking if it is inside each face
       /// \a on (if provided) is filled with the projected point.
       /// \a tolerance is used to determine if the projected point is inside or outside.
       virtual bool              isIn(const DVect &v,const double &tolerance=1e-7) const=0;
       /// Returns the closest point to /a v 
       virtual DVect             getClosestPoint(const DVect &v) const=0;
        /// Return block multiplier, 1=default, 2,3,4...
        virtual quint32 getMultiplier() const=0;
        /// Set block multiplier, 1=default, 2,3,4...
        virtual void    setMultiplier(quint32 m)=0;
        /// return GUI specific select flag
        virtual bool getGUIselect() const=0;
        /// set GUI specific select flag
        virtual void setGUIselect(bool flag) const=0;
        ///return "degenerecy OK flag" relevant for zoning wedges and tetrahedrons (for hexahedron is always true -- zoning possible)
        virtual bool getOKdegeneracyFlag()const=0;
        ///sets "degenerecy OK flag" relevant for zoning wedges and tetrahedrons (for hexahedron is always true -- zoning possible)
        virtual void setOKdegeneracyFlag(bool OK)const=0;
        /// Returns the oposite face of the block -- this is the face that does not share edges or points accros the block with supplied face. 
        /// Returns zero if supplied face is not part of the block or the block is tetrahedra or the block is wedge and supplied face is quad face.
        virtual const IGeomPoly * getTheOpositeFace(const IGeomPoly *face) const=0;
    };
}
