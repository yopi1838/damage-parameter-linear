#pragma once
// iblockthing.h

#include "fortconv/fortdefc.h"
#include "base/src/farray.h"

/**
 * \file
 * \brief This is the interface for BlockThing, a class that holds extra data not in the fortran arrays.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IZoneThing;
  class IFaceThing;
  class IVertexThing;
  class IContactThing;
  class IFElementThing;
  class IFEFaceThing;

  /// \brief This is the interface for BlockThing, a class that holds extra data not in the fortran arrays.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KBID in the fortran block linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IBlockThing
  {
  public:

    /// Master/Slave status
    enum MasterSlaveStatus {
      NONE,
      MASTER,
      SLAVE
    };

    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// type used in IThing typing system
    inline static const TType type_ = 0x4f01ef15;
    /// Return block fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Return Thing ID
    virtual quint64 getBlockID() const=0;
    /// Returns block centroid position
    virtual DVect3  getLocation() const=0;
    /// Return block material number
    virtual int getMaterialNumber() const=0;
    /// Return block model number
    virtual int getModelNumber() const=0;
    /// Return block fixity condition
    virtual bool getFix() const=0;
    /// Set block fixity condition
    virtual void setFix(bool b) = 0;
    ///  return string indicating fix condition
    virtual QString   getFixString() const = 0;
    /// Return block velocity
    virtual DVect3 getVelocity() const=0;
    /// Set block velocity (rigid)
    virtual void setVelocity(const DVect3 &)=0;
    /// Get displacement (average of node displacements)
    virtual DVect3    getDisplacement() const = 0;
    /// Return block angular velocity
    virtual DVect3 getAngularVelocity() const=0;
    /// Set block angular velocity (rigid)
    virtual void setAngularVelocity(const DVect3 &)=0;
    /// Return block volume
    virtual double getVolume() const=0;
    /// Return block mass
    virtual double getMass() const=0;
    /// Return block moment of inertia
    virtual DVect3 getInertia() const=0;
    /// Return moment of inertia tensor 
    virtual SymTensor  getInertiaTensor() const=0;
    /// Return block centroid force sum (rigid)
    virtual DVect3 getForce() const=0;
    /// Return block centroid moment (rigid)
    virtual DVect3 getMoment() const=0;
    /// set block centroid moment (rigid)
    virtual void setMoment(const DVect3 &)=0;
    /// Return load applied to block centroid (rigid)
    virtual DVect3 getLoad() const=0;
    /// Return load applied to block centroid (rigid)
    virtual void setLoad(const DVect3 &)=0;
    /// Return block master/slave condition: 0 = none, 1 = master, 2 = slave
    virtual int getMasterSlaveStatus() const=0;
    /// Return fortran index of master block (0 if none)
    virtual FInt getMasterOffset() const=0;
    /// Returns true if block is marked as excavated
    virtual bool      isExcavated() const=0;
    /// Returns true if block is removed
    virtual bool      isRemoved() const=0;
    /// Returns true if block is hidden
    virtual bool      getHidden() const=0;
    /// Returns true if block is a wall
    virtual bool      isWall() const=0;
    /// Returns true if block is rigid (unzoned)
    virtual bool      isRigid() const=0;
    /// Get maximum and minimum angle between block (not zone) faces in degrees.  Angle > 180 = concave
    virtual void      getEdgeAngle(double *max, double *min) const=0;
    /// Returns the minimum edge length
    virtual double    getMinEdge() const=0;
    /// Return maximum aspect ratio (edge length / volume^(1/3))
    virtual double getMaxAspect() const=0;
    /// Return minimum face area (rigid faces, not zones)
    virtual double getMinFaceArea() const=0;
    /// Return surface area
    virtual double getSurfaceArea() const = 0;
    /// Returns true if block has concave face or edge.
    /// Input edge/face angle above which block is considered concave
    virtual bool isConcave(double d = 180.0) const=0;
    /// Return true if BlockThing and Fortran block are in sync
    virtual bool isValid() const=0;
    /// Returns the boolean stored in utility bit \a bit in the block.
    /// \arg bit must be a bit index previouly returned from IBlockArray::claimBit().
    virtual bool                getBit(int bit) const=0;
    /// Sets the boolean stored in utility bit \a bit in the block.
    /// This function is const, to indicate that setting bit values does not impact the calculation
    ///   logic in any way.
    /// \arg bit must be a bit index previouly returned from IBlockArray::claimBit().
    /// Note - this is const through some hack so that we can use const BlockThings when getting data
    virtual void                setBit(int bit,bool b) const=0;
    /// Retrieves the value stored in the utility array at \a index.
    /// \arg index must be a value previously returned from IBlockArray::claimValue().
    virtual QVariant            getValue(int index) const=0;
    /// Sets the value stored in the utility array at \a index.
    /// This is const to indicate that these bit values do not modify the model in any way.
    /// \arg index must be a value previously returned from IBlockArray::claimValue().
     /// Note - this is const through some hack so that we can use const BlockThings when getting data
    virtual void                setValue(int index,const QVariant &v) const=0;
    /// Get density of block.  This is the only property for rigid blocks.
    /// All other properties should be retrieved through zones
    virtual double   getDensity() const=0;
    /// Set density of block.  This is the only property for rigid blocks.
    /// All other properties should be retrieved through zones
    virtual void     setDensity(double d)=0;
    /// Return density scaling factor
    virtual double    getDensityScalingFactor() const=0;
    /// Get array of FaceThings that make up this block.
    /// By default, this will attempt to return the zone faces if they exist.
    /// The user may optionally enter a boolean to indicate a rigid block.
    /// If true is input, the original rigid block faces are returned, even if the block is zoned.
    virtual FArray<const IFaceThing *> getFaces(bool rigid) const=0; 
    /// Get array of ZoneThings that make up this block.
    virtual FArray<const IZoneThing *> getZones() const=0;
    /// Get array of zone ids that make up this block.
    virtual FArray<quint64> getZoneIDs() const = 0;
    /// Return true if this block is a slave.  False otherwise
    virtual bool isSlave() const=0;
    /// Returns id of master block.  Return 0 if not a slave
    virtual quint64 getMasterID() const=0;
    /// Returns vertices on each original face
    virtual FArray< FArray<const IVertexThing *> > getFaceVertices() const=0;
    /// Returns all vertices
    virtual FArray<const IVertexThing *> getVertices() const=0;
    /// Return number of vertices
    virtual quint32 numGP() const = 0;  
    /// Get all of the ContactThings associated with the block
    virtual FArray<const IContactThing *> getContacts() const=0;
    /// Get list of finite elements associated with block.
    virtual FArray<const IFElementThing *> getFElements() const=0;
    /// Get list of finite element faces associated with block.
    virtual FArray<const IFEFaceThing *> getFEFaces() const=0;
    /// Return dynamic free field code (0-4?) 
    virtual uint getFreeField() const = 0;
    // Add a single gridpoint to a block.
    virtual FInt addGridpoint(int ifac, double x, double y, double z) = 0; 
    // Add a single tet element to a block.
    virtual void addTetZone(FInt n0, FInt n1, FInt n2, FInt n3) = 0;
    // Post processing stuff after gridpoints and tet zones added to a block
    virtual void block_import_post() = 0;
    /// Return if block is internal (has face blocks)
    virtual bool isInternal() const=0;
    /// Return if block is a face block
    virtual bool isFaceBlock() const = 0;
    /// Return host block if face block.  Otherwise return null
    virtual const IBlockThing * getHostBlock() const = 0;
    /// return host block ID for face blocks.  Otherwise return 0
    virtual quint64 getHostID() const=0;
    /// Find vertex of block
    virtual const IVertexThing * findVertex(DVect3 v) const = 0;
    /// get block condition
    virtual double   getBlockCondition(int i) const = 0;

  };

} // end namespace block
// EOF