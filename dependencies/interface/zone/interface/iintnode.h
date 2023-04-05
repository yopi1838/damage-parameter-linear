#pragma once

namespace itasca
{
  class IThing;
}

namespace zone
{
  using namespace itasca;

  class IFace;
  class IIntModel;
  class INode;
  class IQuadWeight;
  class IIntElem;

  /// \brief Class provides access to the an interface node 
  /** An interface node is a point on an interface that can interact with faces in vicinity.
  */
  /// \ingroup Zone
  class IIntNode
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a10;
    /// returns a const IThing pointer
    virtual const IThing *          getIThing() const=0;
    /// returns a IThing pointer
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer the next interface node in the global interface node list, or 0 to indicate the end of the list.\n
    /// \sa IInterface::getFirstINode()
    virtual const IIntNode *        getNext() const=0;
    /// Returns a pointer the next interface node in the global interface node list, or 0 to indicate the end of the list.\n
    /// \sa IInterface::getFirstINode()
    virtual IIntNode *              getNext() =0;
    /// Returns the accumulated Area "owned" by the node, valid only after cycling.
    virtual double                  getArea() const=0;
    /// Returns the normal vector used during contact detection for the node
    virtual DVect3                  getNormal() const=0;
    /// Returns the normal stress magnitude on the node.
    virtual double                  getNormalStress() const=0;
    /// Returns the shear stress on the node, see setShearStress()
    virtual DVect                   getShearStress() const=0;
    /// Sets the shear stress on the node, see getShearStress()
    virtual void                    setShearStress(const DVect &dv)=0;
    /// Returns additional normal stress, see setPreStress()
    virtual double                  getPreStress() const=0;
    /// Sets additional normal stress, see getPreStress()
    virtual void                    setPreStress(const double &d)=0;
    /// Returns the accumulated Shear Displacement, see setShearDisplacement()
    virtual DVect                   getShearDisplacement() const=0;
    /// Sets the accumulated Shear Displacement, see getShearDisplacement()
    virtual void                    setShearDisplacement(const DVect &dv)=0;
    /// Returns the contact tolerance, a dimensionless measure of how close a face 
    ///  has to be register a contact. See setContactTolerance().
    virtual double                  getContactTolerance() const=0;
    /// Sets the contact tolerance, a dimensionless measure of how close a face 
    ///  has to be register a contact. See getContactTolerance().
    virtual void                    setContactTolerance(const double &d)=0;
    /// Returns the interface node penetration with the contact face, valid only after cycling.\n
    /// A value less or equal to zero is returned if there is no contact
    virtual double                  getPenetration() const=0;
    /// Returns a const pointer to the face to which the interface node is attached.
    virtual const IFace *           getHost() const=0;
    /// Returns a pointer to the face to which the interface node is attached.
    virtual IFace *                 getHost() =0;
    /// Returns the weighting factor for the host face.
    virtual const IQuadWeight *     getHostWeight() const=0;
    /// Returns a const pointer to the face to which the interface node is in contact.
    virtual const IFace *           getTarget() const=0;
    /// Returns a pointer to the face to which the interface node is in contact.
    virtual IFace *                 getTarget() =0;
    /// Returns the weighting factor for the target face.
    virtual const IQuadWeight *     getTargetWeight() const=0;
    /// Returns the position of the interface node, see setPosition();
    virtual DVect                   getPosition() const=0;
    /// Sets the position of the interface node, see getPosition();
    virtual void                    setPosition(const DVect &dv)=0;
    /// Returns the displacement of the interface node.
    virtual DVect                   getDisplacement(bool local=false) const=0;
    /// Returns the velocity of the interface node, see setVelocity()
    virtual DVect                   getVelocity(bool local=false) const=0;
    /// Sets the velocity of the interface node, see getVelocity()
    virtual void                    setVelocity(const DVect &dv,bool local=false)=0;
    /// Returns a const pointer to the node constitutive model. 
    virtual const IIntModel *       getModel() const=0;
    /// Returns a const pointer to the node constitutive model. 
    virtual IIntModel       *       getModel()=0;
    /// Causes the position, velocity, and displacement of the node to immediately "snap"
    ///   to it's host faces position, velocity, and displacement.\n
    /// Useful if the host zone and gridpoint have been moved outside of the cycling logic.
    virtual void                    snapToHost()=0;
    /// Returns a const pointer to INode (itself).
    virtual const INode *           getINode() const=0;
    virtual double                  getPorePressure() const=0;
    virtual const IThing *          getOwner() const=0;
  };
} // namespace zone
// EoF

