#pragma once
//ibodyset.h
#include "utility/interface/icontainer.h"
#include "../interface/ibodyblock.h"
/**
 * \file
 * \brief The interface for a container of a single "set" of geometry elements, representing points, edges, polygons, etc.
 */

namespace itasca {
    class IThing;
    class IParse;
}
namespace itascaxd{
    class IGeomPoint;
    class IGeomEdge;
    class IGeomPoly;
    class IRange;
    class IGeomSet;
    class GeomBase;
}


namespace body {
    using namespace itasca;
    using namespace itascaxd;

    class IBodyEdge;
    class IBodyFace;
    /// \brief A container for a single "set" of body-block  elements.
    /// A body-block set is a named collection of body-block elements (points, edges, polygons, blocks) as an organization layer.
    /// \ingroup kernel
    class IBodySet {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x53a89c9f;

        virtual const GeomBase *getGeomBase() const=0;

        /// Return an IThing interface for this object.
        virtual IThing *        getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *  getIThing() const=0;
        /// Return the next IBodySet object in the global list, or 0 if it is the last set.
        virtual IBodySet *      getNext()=0;
        /// Return the next IBodySet object in the global list, or 0 if it is the last set.
        virtual const IBodySet *getNext() const=0;

        /// Returns the name associated with the geometry set.
        virtual QString         getName() const=0;
        /// Returns a notification code (see INoticeManager) indicating a geomtric change on some element of the set.
        /// This includes object creation/destruction, point movement, etc.
        virtual uint            getGeometryCode() const=0;
        /// Returns a notification code (see INoticeManager) indicating that a group assignemnt on some element of the set.
        virtual uint            getGroupCode() const=0;
        /// Returns a notification code (see INoticeManager) indicating a possible extra variable change on some element of the set.
        virtual uint            getExtraCode() const=0;
        /// Returns notification code for edgeRatio changed
        virtual uint            getEdgeRatioChangedCode() const=0;
        /// Returns notification code for edge zone size changed
        virtual uint            getEdgeZoneSizeChangedCode() const =0;
        ///Returns notification code for block zone multiplier  changed
        virtual uint            getZoneMultiplierCode()const =0;
        /// Creates a new IGeomPoint object associated with the set, at position \a pos.  If \a id is specified it is created with that id number, else one is assigned.
        /// Will cause an error if \a id is specified and a point with that id number already exists.
        virtual IGeomPoint *      createPoint(const DVect &pos)=0;
        /// Returns the first IGeomPoint of all those contained in the set.  Call IGeomPoint::getNextPoint() to traverse the list.  May return 0.
        virtual IGeomPoint *      getFirstPoint()=0;
        /// Returns the first IGeomPoint of all those contained in the set.  Call IGeomPoint::getNextPoint() to traverse the list.  May return 0.
        virtual const IGeomPoint *getFirstPoint() const=0;
        /// Find the IGeomPoint with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual IGeomPoint *      findPointWithID(quint64 id)=0;
        /// Find the IGeomPoint with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual const IGeomPoint *findPointWithID(quint64 id) const=0;
        /// Find the IGeomPoint who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual IGeomPoint *      findClosestPoint(const DVect &pos,const double &radius=limits<double>::max(),bool validate=true)=0;
        /// Find the IGeomPoint who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual const IGeomPoint *findClosestPoint(const DVect &pos,const double &radius=limits<double>::max(),bool validate=true) const=0;
        /// Returns an IContainer interface pointer for the container of all IGeomPoint objects.
        virtual IContainer *      getPointContainer()=0;
        /// Returns an IContainer interface pointer for the container of all IGeomPoint objects.
        virtual const IContainer *getPointContainer() const=0;

        /// Creates a new IGeomEdge object associated with this container, from point \a end1 to point \a end2.
        /// Either \a end1 or \a end2 may be null, in which case the created edge is not valid.
        /// If an \a id number is not specified it is given one automatically.
        /// If an IGeomEdge object with that id number already exists, an exception occurs.
        virtual IGeomEdge *       createEdge(IGeomPoint *end1,IGeomPoint *end2)=0;
        /// Returns the first IGeomEdge object of all those contained in the set.  Call IGeomEdge::getNextEdge to traverse the list. May return 0.
        virtual IGeomEdge *       getFirstEdge()=0;
        /// Returns the first IGeomEdge object of all those contained in the set.  Call IGeomEdge::getNextEdge to traverse the list. May return 0.
        virtual const IGeomEdge * getFirstEdge() const=0;
        /// Find the IGeomEdge with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual IGeomEdge *       findEdgeWithID(quint64 id)=0;
        /// Find the IGeomEdge with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual const IGeomEdge * findEdgeWithID(quint64 id) const=0;
        /// Find the IGeomEdge who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual IGeomEdge *       findClosestEdge(const DVect &pos,const double &radius=limits<double>::max())=0;
        /// Find the IGeomEdge who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual const IGeomEdge * findClosestEdge(const DVect &pos,const double &radius=limits<double>::max()) const=0;
        /// Searches for an existing edge data structure with nodes matching end1 and end2 (either way).
        virtual IGeomEdge *       findMatchingEdge(const IGeomPoint *end1,const IGeomPoint *end2)=0;
        /// Searches for an existing edge data structure with nodes matching end1 and end2 (either way).
        virtual const IGeomEdge * findMatchingEdge(const IGeomPoint *end1,const IGeomPoint *end2) const=0;
        /// Returns an IContainer interface pointer for the container of all IGeomEdge objects.
        virtual IContainer *      getEdgeContainer()=0;
        /// Returns an IContainer interface pointer for the container of all IGeomEdge objects.
        virtual const IContainer *getEdgeContainer() const=0;

        /// Creates an IGeomPoly object associated with this set, in an invalid state.
        /// If an \a id number is not specified it is given one automatically.
        /// If an IGeomPoly object with that id number already exists, an exception occurs.
        virtual IGeomPoly *       createPoly()=0;
        /// Returns the first IGeomPoly object of all those contained in the set.  Call IGeomPoly::getNextPoly to traverse the list. May return 0.
        virtual IGeomPoly *       getFirstPoly()=0;
        /// Returns the first IGeomPoly object of all those contained in the set.  Call IGeomPoly::getNextPoly to traverse the list. May return 0.
        virtual const IGeomPoly * getFirstPoly() const=0;
        /// Find the IGeomPoly with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual IGeomPoly *       findPolyWithID(quint64 id)=0;
        /// Find the IGeomPoly with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual const IGeomPoly * findPolyWithID(quint64 id) const=0;
        /// Find the IGeomPoly who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual IGeomPoly *       findClosestPoly(const DVect &pos,const double &radius=limits<double>::max())=0;
        /// Find the IGeomPoly who's location is closest to \a pos, within a limiting distance of \a radius.  May return 0.
        /// Currently takes linear time, but hopefully will take nearly constant time in the future.
        virtual const IGeomPoly * findClosestPoly(const DVect &pos,const double &radius=limits<double>::max()) const=0;
        /// Adds a node to the polygon, by creating and adding an edge object from the free end of the last edge to this point.
        /// Returns the edge created.  The first point added will create an edge of length zero (from the point to the point).
        /// The second point added will expand the first edge, and so will return 0 since no new edge is created.
        /// Additional points will create additional edges, returning the edges created.
        virtual IGeomEdge *       addNodeToPoly(IGeomPoly *poly,IGeomPoint *point)=0;
        /// Closes the IGeomPoly by created an edge connecting the free end of the first edge to the free end of the second.
        /// Will cause an exception if this is not possible for any reason.  Returns the edge created.
        virtual IGeomEdge *       closePoly(IGeomPoly *poly)=0;
        /// Returns an IContainer interface pointer for the container of all IGeomPoly objects.
        virtual IContainer *      getPolyContainer()=0;
        /// Returns an IContainer interface pointer for the container of all IGeomPoly objects.
        virtual const IContainer *getPolyContainer() const=0;
        /// Returns a signal that gets called every time a point is moved.
        virtual IContainer::signal_type *getPointMovedSignal() const=0;
        /// Returns a signal that gets called every time an edge is moved.
        virtual IContainer::signal_type *getEdgeMovedSignal() const=0;
        /// Returns a signal that gets called every time a polygon is moved.
        virtual IContainer::signal_type *getPolyMovedSignal() const=0;
        /// Return a list of all of those polygons in the current set that are edge connected with /a poly.
        virtual QList<const IGeomPoly *> getEdgeConnectedMesh(const IGeomPoly *face) const=0;
        /// Return a list of all of those edges in the current set that are vertex connected with /a edge.
        virtual QList<const IGeomEdge *> getVertexConnectedMesh(const IGeomEdge *edge) const=0;
        /// Return a list of all of those edges in the current set that are vertex connected with /a point.
        virtual QList<const IGeomEdge *>               getVertexConnectedEdges(const IGeomPoint *point) const = 0;
        // returns true if atleast a point is shared (no check for valid pointers)
        virtual bool sharePoint(const IGeomEdge *edge1, const IGeomEdge *edge2, const IGeomPoint **shared=0) const = 0;
        /// Return pair of blocks connected to the face 
        virtual QPair<IBodyBlock *,IBodyBlock *> getFaceConnectedBlocks(const IGeomPoly *face) const=0;
        /// Finds the face that matches the provided points. If no match can be found returns zero (null-pointer).
        virtual  IGeomPoly * findMatchingFace(const FArray<const IGeomPoint*> &points)=0;
        /// Finds the face that matches the provided points. If no match can be found returns zero (null-pointer).
        virtual const IGeomPoly * findMatchingFace(const FArray<const IGeomPoint*> &points) const=0;

        /// Creates an IBodyBlock object associated with this set from array of points
        /// If an \a id number is not specified it is given one automatically.
        /// If an IBodyBlock object with that id number already exists, an exception occurs.
        /// blocktype enumerator specifies type of the block created.
        /// If validateFlag is set (set by default) it will check first if the proposed block is valid (does not intersect others and shares atleast one common face)
        /// against all others blocks: if it is not valid nothing will happen and null (0) will be returned.
        virtual IBodyBlock *       createBlock(FArray<DVect> *points=0, IBodyBlock::blockType blocktype=IBodyBlock::blockType::Hexahedron, IGroupID *addgroup=0)=0;

        /// return value of validateFlag (used by createBlock)
        virtual bool getValidateFlag()const =0;
        /// sets validate flag (used by createBlock)
        virtual void setValidateFlag(bool flag) =0;

        /// erase block and all its faces, edges, points if they are free (not belong to other blocks).
        virtual void       deleteBlock(IBodyBlock *block) =0;
        /// Returns the first IBodyBlock object of all those contained in the set.  Call IGeomPoly::getNextBlock to traverse the list. May return 0.
        virtual IBodyBlock *       getFirstBlock()=0;
        /// Returns the first IBodyBlock object of all those contained in the set.  Call IGeomPoly::getNextBlock to traverse the list. May return 0.
        virtual const IBodyBlock * getFirstBlock() const=0;
        /// Find the IBodyBlock with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual IBodyBlock *       findBlockWithID(quint64 id)=0;
        /// Find the IBodyBlock with ID number matching \a id.  This search will take less than linear time.  May return 0.
        virtual const IBodyBlock * findBlockWithID(quint64 id) const=0;
        /// Returns an IContainer interface pointer for the container of all IBodyBlock objects.
        virtual IContainer *      getBlockContainer()=0;
        virtual const IContainer *getBlockContainer() const =0;
        /// Set tollerance for considering points be the same in all spatiall seach
        /// if it set to 0.0 -- default tolerance will be used
        virtual void  setTolerance(double tolerance)=0;
        /// get flag auto-tolerance on/off -- if it is on, tolerance will be set based on recommended tolerance allways.
        virtual bool              getAutoTolerance()const=0;
        /// Return the recommended/default tolerance. For the current tolerance setting, call getTolerance().
        virtual double              getRecommendedTolerance()const=0;
        /// Get tolerance used in the body set methods
        virtual double              getTolerance()const=0;
        /// Adds vector to all points  in the filter
        virtual void Translate(const DVect &vec, const IRange *filter=0)=0;
        /// Makes X component of all points in the filter val
        virtual void MoveX(double val, const IRange *filter=0)=0;
        /// Makes Y component of all points in the filter val
        virtual void MoveY(double val, const IRange *filter=0)=0;
        /// Makes Z component of all points in the filter val
        virtual void MoveZ(double val, const IRange *filter=0)=0;
        /// On the supplied faceID provides a proxy block that can be added 
        /// to the BodySet by "body create ..." command.  Returns true if provided proxyBlock is valid(does not intersect others blocks).
        /// If flag useAdjacent is true --it will try produce proxi block with best fit possible attachment to adjacent blocks as well.
        /// if useAdjacent is true, the parameter blockType is ignored (allways Default)
        virtual bool getProxyBlock(quint64 faceID,FArray<DVect> &proxyBlock, bool useAdjacent=false,IBodyBlock::blockType blocktype=IBodyBlock::blockType::Default) const =0;
        /// On the supplied faceID returns a list of proxy blocks: blocks extruded from the face along external normal; blocks attached to all 
        /// pars (face --adjacent face); blocks attached to all triplet faces (face -- adjacent face -- attached by edge face) 
        /// to the BodySet by "body create ..." command.  Returns true if provided proxyBlock is valid(does not intersect others blocks).
        /// All pairs of faces (and triplets of faces) are taken into account if dihedral angle with the face is less than diherdalCut (default 3Pi/4).
        /// adjacentFaces, if non-zero and if falidateFlag=false, will contain corresponding faces id, used to create proxy blocks (validation of the proxy blocks will be not done and validB not set).
        /// if adjacentFaces non-zero and if falidateFlag=true (default case), will contain all faces that are detected as adjacent using tolerance (see getTolerance()) 
        virtual QList<FArray<DVect>> getProxyBlocks( quint64 faceID, QList<bool> *validB, IBodyBlock::blockType blocktype=IBodyBlock::blockType::Default, QList<FArray<quint64>> *adjacentFaces=0,QList<IBodyBlock::blockValidState> *proxyState=0) const =0;
        /// For the supplied  faces (facesIDs) returns proxy blocks to create a layer of new blocks attached the faces. Returns true if success. "Message" is empty if success.
        /// All faces must be external and all adjacent pair of faces must form angle more the dihedralcut.
        /// Populates proxyBlocks with (face -- proxy block) map. Blocks are extruded from the faces in the direction of simple average of external normals of the faces.
        /// Distance, if supplied, is the extrusion distance. By default it is computed from average diagonal of attached blocks extent. 
        ///If distance supplied with value 0.0L the default distance will be returned.
        virtual bool proxyBlocksForExtrusionFromFaces(QMap<const IGeomPoly *,FArray<DVect>> &proxyBlocks, const QList<quint64> &facesIDs, QString &message, double *distance=0)const =0;
        /// get dihedralCut that is in use by getProxyBlocks  (default 3Pi/4). If flag rad=false, returns angle in degree.
        virtual double getDihedralCut(bool rad=true)const=0;
        /// set dihedralCut that is in use by getProxyBlocks  (default 3Pi/4). If flag rad=false, assumes angle in degree.
        virtual void setDihedralCut(double cut,bool rad=true)=0;
        /// computeMidpoints: inpout - block, it's face, input midpoint. Output  midfacepointOut(exactly on bilinear surface), edges midpoints.
        /// on a quad:calculates edges midpoints from a face  midpoint using bilinear survace interpolation. Should work on both planar and non-planar faces-quads. 
        /// the input midpoint should be somewhere near the surface. It will be projected on a bilinear surface  along the normal line. edges midpoints are not nessery "mid points" 
        /// but somewhere on a edge according to bilinear split.  
        /// on a triangle : input face midpoint does not mater. midfacePointOut -- will be face centroid, all edges midpoints will be edges centroids -- true mid points.
        virtual void computeMidpoints(const IBodyBlock *block, const IGeomPoly *face, const DVect &midfacePointIn, DVect &midfacePointOut, FArray<DVect> &edgesMidp)const=0;
        //returns current triangle flag
        virtual bool getArrestTriangle()const =0;
        /// returns list of external geometric set object, that the body set works with
        virtual QList<const IGeomSet *> getExternalGeomSets()const=0;
        /// returns a list of external objects along the line. All objects returned will be sorted by the distance from the "beg" point. 
        virtual void listObjectAlongLine(const DVect &beg, const DVect &e, const double &radius, FArray<const IGeomPoint *> *retPoints, 
                             FArray<QPair<const IGeomEdge *, DVect>> *retEdges, FArray<QPair<const IGeomPoly *, DVect>> *intersectPolyPoints)const =0;
        /// returns list of external objects allong the cone. All object returned will be sorted by the distance from the "begining" point. All objects before "begining" will be discarded
        /// origin -- point of origin of the cone. If "depth" privided -- only object withing distance "depth" from the begining along the cone line ( "origin","begining") will be returned.
        /// if whatToReturnFlag = 0 (default), it will return external objects only (from geom. sets), if whatToreturnFlag = 1 it will return objects from the body set only, 
        /// if whatToReturnFlag >= 2 -- it will return objects from geom.sets and the body sets.
        virtual void listObjectAlongCone(const DVect &origin, const DVect &begining, const double &angle, FArray<const IGeomPoint *> *retPoints, 
                              FArray<QPair<const IGeomEdge *, DVect>> *retEdges, FArray<QPair<const IGeomPoly *, DVect>> *intersectPolyPoints, const double *depth=0,
                              quint32 whatToReturnFlag=0, FArray<const IBodyBlock*> *retBlocks=0)const =0;
        /// returns list of external objects allong the cylinder. All object returned will be sorted by the distance from the "begining" point. All objects before "begining" will be discarded
        /// origin -- point of origin of the cylinder(defines cylinder direction along with begining -- from origin to begining). If "depth" privided -- only object withing distance "depth" from the begining along the cylinder line ( "origin","begining") will be returned.
        /// if whatToReturnFlag = 0 (default), it will return external objects only (from geom. sets), if whatToreturnFlag = 1 it will return objects from the body set only, 
        /// if whatToReturnFlag >= 2 -- it will return objects from geom.sets and the body sets.
        virtual void listObjectAlongCylinder(const DVect &origin, const DVect &begining, const double &radius, FArray<const IGeomPoint *> *retPoints, 
                             FArray<QPair<const IGeomEdge *, DVect>> *retEdges, FArray<QPair<const IGeomPoly *, DVect>> *intersectPolyPoints, const double *depth=0,
                             quint32 whatToReturnFlag=0, FArray<const IBodyBlock*> *retBlocks=0)const =0;
        /// returns list of selected control points in the form <edge,indexCP>, where edge is a pointer to an edge and indexCP -- control point index.
        virtual void selectedEdgeControlPoints( QList<QPair<const IBodyEdge *, quint32>>  &selectedCP) const=0;
        //selects control point of the edge
        virtual void selectEdgeControlPoint(const IBodyEdge *,quint32 indexCP)const=0 ;
        //deselects control point of the edge
        virtual void deselectEdgeControlPoint(const IBodyEdge *,quint32 indexCP)const=0 ;
        //clears all selection of control points
        virtual void clearAllSelectedControlPoints()const=0;

        /// returns a set of selected control points where each element is encoded: (face ID << 32) + (n << 16) + m.
        /// returns a hash set for speed in finding a particular selection; 
        virtual void selectedFaceControlPoints(QSet<quint64> &selectedEncodedCpIds) const = 0;
        /// returns the number of selected face control points
        virtual quint64 countSelectedFaceControlPoints() const=0;
        /// return whether the specified control point is selected
        virtual bool isSelectedFaceControlPoint(quint64 encodedFaceControlPointId) const=0;
        /// return the ID of the face that holds selected control points
        virtual quint64 selectedControlPointFaceID() const=0;
        //selects control point of the face
        virtual void selectFaceControlPoint(const IBodyFace *, quint32 indexN, quint32 indexM)const = 0;
        //deselects control point of the face
        virtual void deselectFaceControlPoint(const IBodyFace *, quint32 indexN, quint32 indexM)const = 0;
        /// encode face id, n and m into a quint64 control point id
        static quint64 encodeFaceCpID(quint64 faceID, quint16 n, quint16 m) { return (faceID << 32) + (n << 16) + m; }
        /// decode a quint64 control point id into its component parts
        static void decodeFaceCpID(quint64 cpID, quint64 *faceID, quint16 *n, quint16 *m) {
                *faceID = cpID >> 32; *n = (cpID & 0x00000000FFFF0000) >> 16; *m = (cpID & 0x000000000000FFFF); }

        /// return a list of objects within a selection rectangle by using deep selection; 
        /// a boolean tells whether perspective or parallel projection is being used;
        /// 4 points, in counter-clockwise order, define a rectangle at the near side of the viewing frustrum; 
        /// 4 points, in counter-clockwise order, define a rectangle at the far side of the frustrum, for use with perspective projection;
        /// a vector indicates the direction of the selection rectangle (near to far); 
        /// the type of object to return is indicated;
        /// this function should not do the actual selection, just return a list of objects.
        virtual QList<const IThing *> marqueeDeepSelect(bool perspective, const FArray<DVect>& nearSelRect, 
                        const FArray<DVect>& farSelRect, const DVect& selectionDirection, TType objType)const=0;
        ///return flag if dihedral measure is used in validation of the block.
        virtual bool getDihedralBasedValidation()const=0;
        ///imports flac3d grid file, returns zero on success, throws exceptions if somthing wrong. Register input file if fileRecord=true (default).
        virtual int importFromFlac3D(IParse *par, const QString &fileName,bool fileRecord=true)=0;
        ///validates cellspace for edges faces and blocks and computes validState for all blocks. May provide number of not blocks.
        virtual void validateAllBlocks(quint32 *numberOfNotValid=0)=0;
        /// Make all hex by splitting blocks (all quad face are split)
        virtual bool makeAllHexBySplitBlocks(QString *message=0)=0;
    }; 
}