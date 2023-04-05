#pragma once
// ifacething.h

#include "fortconv/fortdefc.h"

#include "fortconv/fortdefc.h"
#include "base/src/farray.h"

/**
 * \file
 * \brief This is the interface for FaceThing, a class that holds extra data not in the fortran arrays.
*/

namespace itasca
{
  class IThing;
}

namespace block
{

  class IBlockThing;
  class IZoneThing;
  class IVertexThing;
  class IFlowPlaneThing;

  /// \brief This is the interface for FaceThing, a class that holds extra data not in the fortran arrays.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KFCPP in the fortran block linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IFaceThing
  {
  public:

    /// offset for FaceThing ID
    static const FInt cpp   = 12; // C++ extension from Fortran array

    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// type used in IThing typing system
    inline static const TType type_ = 0x521615ab;
    /// Return block fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Return Thing ID.
    virtual quint64 getFaceID() const=0;
    /// Return host blockthing
    virtual IBlockThing*         getBlockThing() const=0;
    /// Return host zonething.  Returns NULL if unzoned (rigid).
    virtual IZoneThing*          getZoneThing() const=0;
    /// Return host blockthing id
    virtual quint64             getBlockThingID() const=0;
    /// Return host zonething id.  Returns 0 if unzoned (rigid).
    virtual quint64             getZoneThingID() const=0;
    /// Get a list of vertexthings that make up this face
    virtual FArray<const IVertexThing *> getVertices() const=0;
    /// Get list of vertexids
    virtual FArray<quint64> getVertexIDs() const = 0;
    /// Get a list of vertex locations that make up this face
    virtual FArray<DVect>        getVertexLocations() const=0;
    /// get location for specified vertex (0 based)
    virtual DVect                getVertexLocation(quint32 i) const=0;
    /// Return face joint set id number (this is joint id resulting from jset command)
    virtual quint32              getJointsetID() const=0;
    /// Return face fracture id number (this is id of a single fracture)
    virtual quint32              getFractureID() const=0;
    /// Return normal vector to face
    virtual DVect3               getNormal() const=0;
    /// Returns true if face or host zone or host block is hidden
    virtual bool                getHidden() const=0;
    /// Return the area of the face
    virtual double              getArea() const=0;
    /// Return offset of corresponding Finite Element face.  Required for ranges.
    virtual FInt                getFEFaceOffset() const=0;
    /// returns true if there is no f-f contact between the face and input block
    virtual bool   isFaceFace(const IBlockThing *bt) const=0;
    /// returns true if this is a zone face.  Returns false for an original block face
    /// note that this will return true even for rigid block faces that have been triangulated
    virtual bool isZoneFace() const = 0;
    /// Return true if FaceThing and Fortran face are in sync
    virtual bool isValid() const=0;
    /// Return flow plane associated with the face.  Return nullptr if none.
    virtual IFlowPlaneThing *    getFlowPlane() const=0;
    /// Return number of gridpoints
    virtual quint32 getNumGP() const=0;
    /// Return gridpoint n (0 < n < 2)
    virtual IVertexThing * getGP(quint32 n) const=0;
    /// Return true if face is on an excavated block
    virtual bool                 isExcavated() const=0;
    /// Return true if face is on a removed block
    virtual bool                 isRemoved() const=0;
    /// Return true if face is planar
    virtual bool                 isPlanar() const = 0;
    /// Return the minumum edge length of the face
    virtual double               getMinEdgeLength() const = 0;
    /// Return host fblockthing (nullptr if not a faceblock)
    virtual IBlockThing*         getFaceBlockThing() const = 0;
    /// Return the safety factor of the face
    virtual double               getFactorOfSafety() const = 0;
    /// Set the safety factor of the face
    virtual void                 setFactorOfSafety(double d) = 0;
  };

} // end namespace block
// EOF