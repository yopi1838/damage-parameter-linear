#pragma once
// isubcontactthing.h

#include "iblockdef.h"

/**
 * \file
 * \brief This is the interface for SubcontactThing, a class that holds extra data not in the fortran arrays.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IFaceThing;
  class IVertexThing;
  class IContactThing;
  class IBlockThing;
  class IFlowZoneThing;
  class IFlowPlaneVertexThing;

  /// \brief This is the interface for SubcontactThing, a class that holds extra data not in the fortran arrays.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KCXCPP in the fortran subcontact linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  ///           There is also two-way accessibility with the corresponding fortran data block
  /// \ingroup THREEDEC
  class ISubcontactThing
  {
  public:
    /// enumeration of possible subcontact types
    enum SubcontactType {VertexFace=0, EdgeEdge};
    /// type used in IThing typing system
    inline static const TType type_ = 0x4f54d345;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return contact fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Return Thing ID
    virtual quint64 getSubcontactID() const=0;
    /// Return host ContactThing 
    virtual const IContactThing * getHostContact() const=0;
    /// Return host contact id
    virtual quint64 getHostContactID() const=0;
    /// Returns subcontact position
    virtual DVect3              getLocation() const=0;
    /// Returns true if subcontact or host contact is hidden
    virtual bool                getHidden() const=0;
    /// Returns the face associated with this subcontact.  null for no face
    virtual IFaceThing * getFace() const=0;
    /// Returns the id of the face associated with this subcontact.  0 for no face
    virtual quint64 getFaceID() const=0;
    /// Returns the vertex associated with this subcontact.  null for no vertex
    virtual IVertexThing * getVertex() const=0;
    /// Returns the Vertex ID associated with the subcontact.  0 for no vertex
    virtual quint64 getVertexID() const=0;
    /// Return fracture ID (from DFN) associated with subcontact.  
    /// Returns 0 if not created with DFN.
    virtual quint64 getFractureID() const=0;
    /// Returns ID of DFN used to create contact. 0 if DFN was not used
    virtual quint64    getDFN_ID() const = 0;
    /// Returns name of DFN used to create contact.  Empty string if DFN was not used
    virtual QString    getDFN_name() const = 0;
    /// Return material number
    virtual int getMaterial() const=0;
    /// Return material as string
    virtual QString getMaterialString() const = 0;
    /// Return constitutive model number
    virtual int getConstitutive() const=0;
    /// Return the shear force vector
    virtual DVect3 getShearForce() const=0;
    /// Set the shear force vector
    virtual void setShearForce(const DVect3 &dv)=0;
    /// Return the normal force magnitude
    virtual double getNormalForce() const=0;
    // Set the normal force magnitude
    virtual void setNormalForce(double nf)=0;
    /// Return the shear displacement vector
    virtual DVect3 getShearDisplacement() const=0;
    /// Set the shear displacement vector
    virtual void setShearDisplacement(const DVect3 &dv)=0;
    /// Return the normal displacement
    virtual double getNormalDisplacement() const=0;
    /// Set the normal displacement
    virtual void setNormalDisplacement(double nd)=0;
    /// Return the area associated with the subcontact
    /// Manual says half area.  TODO: Check this!!
    virtual double getArea() const=0;
    /// Return the normal stress
    virtual double getNormalStress() const=0;
    /// Return the magnitude of shear stress
    virtual double getShearStress() const=0;
    /// Get the joint constitutive model as a string
    virtual QString getModel() const=0;
    /// Set joint constitutive model
    virtual void setModel(const QString &s)=0;
    /// Get the subcontact failure state as a string
    virtual QString getStateString() const=0;
    /// Get the subcontact failure state as an integer
    virtual int    getState() const=0;
    /// Set the subcontact failure state
    virtual void   setState(int i)=0;
    /// Return block 1 of the subcontact
    virtual const IBlockThing * getBlock1() const=0;
    /// Return block 2 of the subcontact
    virtual const IBlockThing * getBlock2() const=0;
    /// Return id of block 1 of the subcontact
    virtual quint64 getBlock1_ID() const=0;
    /// Return id of block 2 of the subcontact
    virtual quint64 getBlock2_ID() const=0;
    /// returns a list of all the valid properties of the current constitutive model assigned to this subcontact 
    virtual QStringList getProperties() const=0;
    /// Given a property name, returns the index of the property of the constitutive model assigned to this subcontact.
    /// \note Base 1, returns 0 if name is not found.
    virtual UInt        getPropertyIndex(const QString &prop) const=0;
    /// Returns value of subcontact property corresponding to \b index \n
    /// Returns 0 if \b index is not recognized
    virtual QVariant    getProperty(UInt index) const=0;
    /// Set property with name \b prop to value \b val
    virtual void setProperty(const QString &prop, QVariant val) = 0;
    /// Return the subcontact type
    virtual SubcontactType getSubcontactType() const=0;
    /// Return the pore pressure on the subcontact
    virtual double getPorePressure() const=0;
    /// Set the pore pressure on the subcontact
    virtual void setPorePressure(double d)=0;
    /// Return the flow zone associated with the subcontact.  Null if no flow
    virtual IFlowZoneThing * getFlowZone() const=0;
    /// Return the flow plane vertex associated with the subcontact.  Null if no flow
    virtual IFlowPlaneVertexThing* getFlowVertex() const = 0;
    /// Return true if subcontact is excavate.  False otherwise
    virtual bool isExcavated() const=0;
    /// Get the initial mechanical aperture
    virtual double getInitialAperture() const=0;
    /// Set the initial mechanical aperture
    virtual void setInitialAperture(double ap0)=0;
    /// Returns the boolean stored in utility bit \a bit in the subcontact.
    /// \arg bit must be a bit index previouly returned from ISubcobcontactArray::claimBit().
    virtual bool                getBit(int bit) const=0;
    /// Sets the boolean stored in utility bit \a bit in the subcontact.
    /// This function is const, to indicate that setting bit values does not impact the calculation
    ///   logic in any way.
    /// \arg bit must be a bit index previouly returned from ISubcontactArray::claimBit().
    /// Note - this is const through some hack so that we can use const SubcontactThings when getting data
    virtual void                setBit(int bit,bool b) const=0;
    /// Return the shear velocity vector
    virtual DVect3 getShearVelocity() const=0;
    /// Returns maximum shear displacement
    virtual double getMaxShear() const=0;
    /// Set maximum shear displacement
    virtual void setMaxShear(double d) = 0;
    /// Return energy
    virtual double getEnergy(UInt i) const = 0;
    virtual double getEnergy(const QString& e) const = 0;
    virtual double getTotalEnergy() const = 0;
    /// Return first fblockthing (nullptr if not a faceblock)
    virtual IBlockThing*         getFaceBlock1() const = 0;
    /// Return second fblockthing (nullptr if not a faceblock)
    virtual IBlockThing*         getFaceBlock2() const = 0;
    /// Return true if subcontact has non-zero strength and has not failed
    virtual bool isBonded() const = 0;
    /// Return ratio of shear strength to shear stress
    virtual double getFOS() const = 0;





  };

} // end namespace block
// EOF