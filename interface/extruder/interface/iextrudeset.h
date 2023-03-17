#pragma once

/**
* \file
* \brief Provides interface to IExtrudeSet. Contains containers for points, edges and blocks objects.
*/

#include "utility/interface/icontainer.h"
#include "utility/interface/igroup.h"
#include "base/src/farray.h"

namespace itasca {
    class IGroup;
    class IThing;
}
namespace itascaxd {
    class IGeomPoint;
    class IRange;
}

namespace extruder {
    using namespace itascaxd;

    class IExtrudePoint;
    class IExtrudeEdge;
    class IExtrudeRegion;

    /// \brief Provides interface to IExtrudeSet. Contains containers for points, edges and blocks objects.
    /// \ingroup FLAC3D
    class IExtrudeSet {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4cb61c35;

        /// returns its IThing pointer
        virtual IThing *        getIThing()=0;
        /// returns its const IThing pointer
        virtual const IThing *  getIThing() const=0;
        /// returns its const QObject pointer
        virtual const QObject * getQObject() const=0;
        /// returns its QObject pointer
        virtual QObject *       getQObject()=0;

        /// returns the set name
        virtual QString         getName() const=0;

        /// Adds a point to the point container and returns a pointer to the newly created object. The point is located at \b pos.
        virtual IExtrudePoint * createPoint(const DVect &pos)=0;
        /// Adds a edge to the edge container and returns a pointer to the newly created object. The edge is defined by verices \b end1 and \b end2.
        virtual IExtrudeEdge *  createEdge(IExtrudePoint *end1,IExtrudePoint *end2,bool useDefaultSize, QString* str)=0;
        /// Adds a block to the block container and returns a pointer to the newly created object. 
        //virtual IExtrudeRegion *  createBlock()=0;

        /// returns a pointer to the point container.
        virtual IContainer *         getPointContainer()=0;
        /// returns a const pointer to the point container.
        virtual const IContainer *   getPointContainer() const=0;
        /// given an \b id, returns a const pointer to the point object.
        virtual const IExtrudePoint *findPointWithID(quint64 id) const=0;
        /// given an \b id, returns a pointer to the point object.
        virtual IExtrudePoint *      findPointWithID(quint64 id)=0;
        /// given a position \b pos in space, returns a const pointer to the closest point object within a given \b radius.
        virtual const IGeomPoint *   findClosestPoint(const DVect &pos,const double &radius=limits<double>::max()) const=0;
        /// given a position \b pos in space, returns a pointer to the closest point object within a given \b radius.
        virtual IGeomPoint *        findClosestPoint(const DVect &pos,const double &radius=limits<double>::max())=0;
        /// Test if a potential closed polygon exists at pos. Returns vertices of the polygon
        virtual const QList<QList<const IExtrudePoint*>> findPotentialPolygonAt(DVect pos) const=0;
        /// Test if a (quad) block exists with 2 points on a diagonal
        //virtual IExtrudeRegion *      findBlockByEdge(ExtrudeEdge* edge)=0;

        /// returns a pointer to the edge container container.
        virtual IContainer *         getEdgeContainer()=0;
        /// returns a const pointer to the edge container container.
        virtual const IContainer *   getEdgeContainer() const=0;
        /// given an \b id, returns a const pointer to the edge object.
        virtual const IExtrudeEdge * findEdgeWithID(quint64 id) const=0;
        /// given an \b id, returns a pointer to the edge object.
        virtual IExtrudeEdge *       findEdgeWithID(quint64 id)=0;
        /// given a position \b pos in space, returns a const pointer to the closest edge object within a given \b radius.
        virtual const IExtrudeEdge * findClosestEdge(const DVect &pos,const double &radius=limits<double>::max()) const=0;
        /// given a position \b pos in space, returns a pointer to the closest edge object within a given \b radius.
        virtual IExtrudeEdge *       findClosestEdge(const DVect &pos,const double &radius=limits<double>::max())=0;
        /// given the two vertices \b end1 and \b end2 of an edge, returns a pointer to the matching edge object.
        virtual const IExtrudeEdge * findMatchingEdge(const IGeomPoint *end1,const IGeomPoint *end2) const=0;
        /// given the two vertices \b end1 and \b end2 of an edge, returns a pointer to the matching edge object.
        virtual IExtrudeEdge *       findMatchingEdge(const IGeomPoint *end1,const IGeomPoint *end2)=0;
        /// given one edge, populate \b connectedEdges with ID's of edges which are connected to it and to one another with the angle less than the break angle
        virtual quint32              getEdgesByBreakAngle(quint32 edgeID, double breakAngle, QList<quint32>* connectedEdges) const = 0;
        /// given list of edge IDs, combine them to single curved edge
        virtual quint32              combineEdgesToSingleEdge(QList<quint32>* connectedEdges, QString *edgeWarning=nullptr) = 0;
        /// Delete point from set
        virtual void                 deletePoint(IExtrudePoint *pointIn, uint *edgeCount=0, uint *blockCount=0)=0;
        /// Delete edge from set
        virtual void                 deleteEdge(IExtrudeEdge *edgeIn, uint *blockCount=0)=0;
        /// Delete block from set
        virtual void                 deleteBlock(IExtrudeRegion *b) = 0;
        /// Create a new regular block by 3 or 4 points
        virtual IExtrudeRegion *     createBlockByPoints(IExtrudePoint* p0In, IExtrudePoint* p1In, IExtrudePoint* p2In, IExtrudePoint* p3In, quint32* newedgecount=0, bool ignoreerror=false) = 0;
        /// Create a new block by given set of points (point IDs)
        virtual IExtrudeRegion *     createBlockByPoints(QSet<quint64> inputPts, QSet<QString> *errorsList = nullptr, QList<QString> *warningsList = nullptr) = 0;
        /// Create a new block at a given position
        virtual IExtrudeRegion *     createBlockAt(DVect pos, QSet<QString> *errorsList = nullptr, QList<QString> *outMessages = nullptr) = 0;
        /// Automatically create new blocks in all suitable closed polygons presents in the set
        virtual bool                 createBlocksAuto(IGroupID *group = nullptr, QSet<QString> *errorsList = nullptr, QList<QString> *outMessages = nullptr, const IRange *range = nullptr) = 0;

        /// returns number of points.
        virtual int getPointSize() const =0;
        /// returns number of edges.
        virtual int getEdgeSize()=0;
        /// returns number of blocks.
        virtual int getBlockSize()=0;

        //metadata     
        /// MetaData is used to store generic data and it contains a key-value pair of strings.
        /// Clear Meta Data
        virtual void clearMetaData()=0;
        /// Add or change MetaData key and value
        virtual void setMetaKeyValue(const QString &key, const QString &value)=0;    
        /// Returns the number of MetaData objects in the Metadata vector. 
        virtual int getMetaCount() const=0;
        /// Given an \b index to MetaData vector, returns the MetaData key;
        virtual QString getMetaKey(int index) const=0;
        /// Given an \b index to MetaData vector, returns the MetaData value;
        virtual QString getMetaValue(int index) const=0;
        /// Given an \b key, returns the corresponding index of the MetaData vector;
        virtual int findMetaKeyIndex(const QString &key) const=0;
        /// Given an \b key, searchs the MedaData vector and returns the corresponding MetaData value;
        virtual QString findMetaKeyValue(const QString &key) const=0;

        /// returns a pointer to the block container.
        virtual IContainer *           getRegionsContainer()=0;
        /// returns a const pointer to the block container.
        virtual const IContainer *     getRegionsContainer() const=0;
        /// given an \b id, returns a const pointer to the block object.
        virtual const IExtrudeRegion * findBlockWithID(quint64 id) const=0;
        /// given an \b id, returns a pointer to the block object.
        virtual IExtrudeRegion *       findBlockWithID(quint64 id)=0;
        /// given a position \b pos in space, returns a const pointer to the closest block object within a given \b radius.
        virtual const IExtrudeRegion * findClosestBlock(const DVect &pos,const double &radius=limits<double>::max()) const=0;
        /// given a position \b pos in space, returns a pointer to the closest block object within a given \b radius.
        virtual IExtrudeRegion *       findClosestBlock(const DVect &pos,const double &radius=limits<double>::max())=0;
        /// Get the locations of all of the region nodes.
        virtual void                   getRegionLocations(FArray<DVect> *ret,quint64 wr=0,int we=-1)=0;

        /// Return true if basic block geometry is good
        virtual bool valid(QList<QString> *problems = nullptr, bool areaCheck = false) const=0;

        /// Return true if any point-point are too close, by distance atol
        //virtual bool testPPMerge(double atol) const=0;
        /// Return true if any point-edge are too close, by distance atol
        //virtual bool testPEMerge(double atol) const=0;
        /// Return true if any edges are crossing
        //virtual bool testEEMerge(const IExtrudeEdge* e1 = nullptr, const double eetol = 0.05) const=0;
        /// Return true if points insde block area.
        //virtual bool testBlockInvalid() const=0;
        /// Return true if any validation checks fail
        //virtual bool testAuto(const double eetol = 0.05) const=0;

        /// Returns true is auto zone mode is on
        virtual bool   isAutoZone() const=0;
        /// Return AutoZone target average zone edge length
        virtual double getAutoZoneEdge() const=0;
        /// Return AutoZone direction: 0=all, 1=x,y only, 2=normal only.
        virtual int getAutoZoneDir() const=0;
        /// Returns AutoZone modes: 0 for edge, 1 for size, 2 for zones
        virtual int getAutoZoneMode() const=0;
        /// Return AutoZone size (number of zones across largest dimension of x,y)
        virtual int getAutoZoneSize() const=0;
        /// Return AutoZone target (maximum) total number of zones
        virtual int getAutoZoneTotal() const=0;    
        /// check if AutoZone Total can be performed
        virtual bool isAutoZoneTotalAllowed() const = 0;
        /// Return number of 2d zones in xy plane
        virtual double computeZones2D() const=0;
#ifdef THREED
        /// Return number of 3d zones
        virtual double computeTotalZones() const=0;
#endif
        /// Set AutoZone flag state
        //virtual void   setAutoZone(bool x)=0; // AP: setting "AutoZone active on" is not compatible with irregular blocks - disabling this functionality for now
        /// Set AutoZone mode: 0 for edge, 1 for size, 2 for zones
        virtual void   setAutoZoneMode(int mode)=0;
        /// Set AutoZone direction: 0=all, 1=x,y only, 2=normal only.
        virtual void   setAutoZoneDir(int dir)=0;
        /// Set AutoZone target zone average edge length
        virtual void   setAutoZoneEdge(double x)=0;
        /// Set AutoZone target size (number of zones across largest dimension of x,y)
        virtual void   setAutoZoneSize(int n)=0;
        /// Set AutoZone target target (maximum) total number of zones
        virtual void   setAutoZoneTotal(int nz)=0;
        
        /// Return extrusion origin point
        virtual const DVect &getOrigin() const=0;
        /// Return input (raw) Axes
        virtual const Axes &getAxesInput() const=0;
        /// Return internal Axes (modified for orthogonality and unit lengths)
        virtual Axes        getAxes() const=0;
       /// Return extrusion u-axis direction (unit length)
        virtual const DVect &getUAxis() const=0;
        /// Return extrusion v-axis direction (unit length)
        virtual const DVect &getVAxis() const=0;
        // Extrusion segments
#ifdef THREED
        /// Return number of extrusion segments, 1 or more
        virtual int           getSegmentCount() const=0;
        /// Return extrusion 3D normal (extrusion) direction (unit length)
        virtual const DVect3 &getNAxis() const=0;
        /// Return segment extrusion zero coordinate (default zero)
        virtual double        getSegmentZOrigin() const=0;
        /// Return length of indexed segment 
        virtual double        getSegmentLength(int seg) const=0;
        /// Return ratio of indexed segment 
        virtual double        getSegmentRatio(int seg) const=0;
        /// Return size (zone count) of indexed segment 
        virtual int           getSegmentSize(int seg) const=0;
        /// Return position of node before segment, relative to zorigin, indexed from zero
        virtual double        getSegmentPosition(int seg /* 0-based */ ) const=0; // origin excluded
        /// Return position of node at end of segment, relative to zorigin: 0...total_length
        virtual double        getSegmentDepth(int seg) const=0; // origin excluded
        /// Return group in given slot of indexed segment 
        virtual const IGroup *getSegmentGroup(int seg,const ISlotID &slot) const=0;
        /// Return group name in given slot of indexed segment 
        virtual QString       getSegmentGroupName(int seg,const ISlotID &slot) const=0;
        /// Remove group by name, in given slot from indexed segment
        virtual bool          removeSegmentGroup(int seg, const QString &name)=0;
        /// Return true if named group is in given slotin indexed segment
        virtual uint          getSegmentIsInGroup(int seg,const FArray<IGroupID> &ids,TType type=0,bool only=false) const=0;
        /// Returns complete list of groups associated with the segment.
        virtual uint          getSegmentGroupList(int seg,FArray<IGroupID> *list) const=0;
        /// Return group in given slot of indexed node 
        virtual const IGroup *getSegmentNodeGroup(int seg,const ISlotID &slot) const=0;
        /// Return group name in given slot of indexed node
        virtual QString       getSegmentNodeGroupName(int seg,const ISlotID &slot) const=0;
        /// Return group name in given slot of indexed node 
        virtual bool          removeSegmentNodeGroup(int seg, const QString &name)=0;
        /// Return true if given group in given slotis in indexed node
        virtual uint          getSegmentNodeIsInGroup(int seg,const FArray<IGroupID> &ids,TType type=0,bool only=false) const=0;
        /// Returns complete list of groups associated with the segment node.
        virtual uint          getSegmentNodeGroupList(int seg,FArray<IGroupID> *list) const=0;
        /// Return segment relative position of index gridpoint inside indexed segment
        virtual double        getDiscretizedSegment(int seg, int gp) const=0;
#endif

        /// Set extrusion origin
        virtual void setOrigin(DVect c)=0;
        // set one axis - compute 2
        /// Set extrusion u-axis direction (unit length), adjust v-axis and normal
        virtual void setUAxis(DVect u)=0;
        /// Set extrusion v-axis direction (unit length), adjust u-axis and normal
        virtual void setVAxis(DVect v)=0;
        /// Set extrusion u,v axes, adjust v-axis, compute normal
        virtual void setUVAxes(DVect u,DVect v)=0;
        /// Return true if u,v (and n in 3D) axes are unit length
        virtual bool testAxesBasisNormal()=0;
        /// Return true if u,v (and n in 3D) axes are orthogonal
        virtual bool testAxesBasisOrthogonal()=0;
#ifdef THREED
        /// Set extrusion normal-axis (extruding) direction (unit length), adjust u-axis and v-axis
        virtual void setNAxis(DVect n)=0;
        // set two axes - compute 1
        /// Set extrusion u,n axes, adjust n-axis, compute v-axis
        virtual void setUNAxes(DVect u,DVect n)=0;
        /// Set extrusion u,n axes, adjust n-axis, compute u-axis
        virtual void setVNAxes(DVect v,DVect n)=0;

        /// Set length of indexed segment, adjust following segment positions
        virtual void          setSegmentLength(int index, double len)=0;
        /// Set size (zones) of indexed segment
        virtual void          setSegmentSize(int index, int size)=0;
        /// Set zone ratio of indexed segment
        virtual void          setSegmentRatio(int index, double rat)=0;
        /// Add segment node to end, or split existing segment if position inside.
        virtual bool addSegmentNodeByPosition(double pos)=0;
#endif

        /// Return auto-validation state, true means validation checks disable changes that created invalidate geometry
        virtual bool   isAutoValidation() const=0;
        /// Set auto-validation state, true means validation checks disable changes that created invalidate geometry
        virtual void   setAutoValidation(bool x)=0;
        /// Return relative tolerance used in auto-validation
        virtual double getValidationTol() const=0;
        /// Set relative tolerance used in auto-validation
        virtual void   setValidationTol(double x)=0;
        /// Compute absolute tolerance used in validation
        virtual double computeValidationAbsTol() const=0;
        /// Set tolerance for combining multiple edges to a single curved edge (affects amount of control points on the new edge)
        virtual void   setEdgeCombineTolerance(const double tol) = 0;
        /// Return tolerance for combining multiple edges
        virtual double getEdgeCombineTolerance() const = 0;

        // test zone quality    

        /// Count number of problem zones, index 0=ok, 1,2=warnings, 3,4=bad. Return true if any bad.
        virtual bool countZoneQuality(int statecount[], bool areaCheck = false) const=0;
        /// Return true if any bad zones
        virtual bool anyZonesBad(bool areaCheck = false) const=0;
        /// Return descriptive string of warning and bad zone counts
        virtual QString getBadZonesReport() const=0;
        /// Return descriptive string of bad zone counts 
        virtual QString getZoneQualityReport(bool badonly=false) const=0;
        /// Convert countlists to string report
        virtual QString getZoneQualityReport(int statecount[]) const=0;


        // Signals
        /// Signal emitted when a point with \b id was added. \b id equal zero means ALL points were added (e.g. restore) 
        virtual void pointAdded(quint64 id)=0; 
        /// Signal emitted when a point with \b id was removed. \b id equal zero means ALL points were removed (e.g. new) 
        virtual void pointRemoved(quint64 id)=0; 
        /// Signal emitted when an edge with \b id was added. \b id equal zero means ALL edges were added (e.g. restore) 
        virtual void edgeAdded(quint64 id)=0; 
        /// Signal emitted when an edge with \b id was removed. \b id equal zero means ALL edges were removed (e.g. new) 
        virtual void edgeRemoved(quint64 id)=0;
        /// Signal emitted when a block with \b id was added. \b id equal zero means ALL blocks were added (e.g. restore) 
        virtual void blockAdded(quint64 id)=0;
        /// Signal emitted when a block with \b id was added after all edges were assigned. \b id equal zero means ALL blocks were added (e.g. restore) 
        virtual void blockAddedLater(quint64 id)=0;
        /// Signal emitted when a block with \b id was removed. \b id equal zero means ALL blocks were removed (e.g. new) 
        virtual void blockRemoved(quint64 id)=0; 
        /// Signal emitted when a point with \b id has changed its position
        virtual void pointMoved(quint64 id)=0;
        /// Signal emitted when an edge with \b id has changed its position (implied from pointMoved)
        virtual void edgeMoved(quint64 id)=0; 
        /// Signal emitted when an block with \b id has changed its position (implied from edgeMoved and blockMoved)
        virtual void blockMoved(quint64 izd)=0;
        /// Signal emitted when a point associated with an end of an edge with \b id has been added or removed.
        virtual void edgePointChanged(quint64 id)=0; 
        /// Signal emitted when an edge associated with a block with \b id has been added or removed.
        virtual void blockEdgeChanged(quint64 id)=0;
        /// Signal emitted when the Type property has changed in the edge. \b ID 0 means ALL edges have changed
        virtual void edgeTypeChanged(quint64 id)=0; 
        /// Signal emitted when a control point is added.
        virtual void sendEdgeControlPointAddedNotice(quint64 id, int index)=0;
        /// Signal emitted when the Size property has changed in the edge. \b ID 0 means ALL edges changed to \b size. (generally 0.)    
        virtual void edgeSizeChanged(quint64 id,quint32 size)=0;         
        /// Signal emitted when the ratio property has changed on the edge. \b ID 0 means ALL edges changed to \b rat. (generally 1.0).
        virtual void edgeRatioChanged(quint64 id,const double &rat)=0; 
        /// Signal emitted when the edge parameterization has changed in some way (could be anything, control points, type, etc).
        ///virtual void edgeParamChanged(quint64 id)=0; 
        /// Signal emitted when block multiplier has changed.
        virtual void blockMultiplierChanged(quint64 id,quint32 m)=0;
        /// Signal emitted when block orientation has changed.
        virtual void blockOrientationChanged(quint64 id)=0;
        /// Signal number of segments has changed
        /// Signal extrusion axes or origin has changed
        virtual void axesChanged()=0;
#ifdef THREED
        virtual void segmentsChanged(int count)=0;
        /// Signal segment size has changed
        virtual void segmentSizeChanged(int index, quint32 size)=0;
        /// Signal segment ratio has changed
        virtual void segmentRatioChanged(int index, const double &rat)=0;
        /// Signal segment length has changed
        virtual void segmentLengthChanged(int index, const double &length)=0;
        /// Signal extrusion Zorigin has changed
        virtual void segmentOriginChanged(const double &z0)=0;
#endif

        /// Signal AutoZone state has changed
        virtual void autoZoneChange(bool on, int mode, double dx, int n, int t)=0;

        /// Signal MetaData added
        virtual void metaAdded(int index)=0; // index=-1 means ALL added (restore)
        /// Signal MetaData removed
        virtual void metaRemoved(int index)=0; // index=-1 means ALL removed (new)
        /// Signal MetaData changed
        virtual void metaChanged(int index)=0;    
        /// Notice point groups changed
        virtual uint getPointGroupNotice() const=0;
        /// Notice edge groups changed
        virtual uint getEdgeGroupNotice() const=0;
        /// Notice block groups changed
        virtual uint getBlockGroupNotice() const=0;
        /// Notice that 2D geometry has changed (points/edges/blocks moved or removed)
        virtual uint  getTwoDGeomChangedNotice() const=0;
#ifdef THREED
        /// Notice segment groups changed
        virtual uint getSegmentGroupNotice() const=0;
        /// Notice node groups changed
        virtual uint getSegmentNodeGroupNotice() const=0;
#endif

    };
} // namespace f3
// EoF
