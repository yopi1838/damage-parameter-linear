#pragma once

namespace itasca
{
  class IThing;
}

namespace zone
{
  using namespace itasca;

  class IFace;
  class IIntNode;

  /// \brief Class provides access to the an interface element
  /** An interface element is a triangular surface representing area of interface.
  */
  /// \ingroup Zone
  class IIntElem
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a12;
    /// returns a IThing pointer
    virtual const IThing *          getIThing() const=0;
    /// returns a const IThing pointer
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer the next interface element in the global interface element list, or 0 to indicate the end of the list.\n
    /// See IInterface::getFirstIntElement();
    virtual const IIntElem *        getNext() const=0;
    /// Returns a pointer the next interface element in the global interface element list, or 0 to indicate the end of the list.\n
    /// See IInterface::getFirstIntElement();
    virtual IIntElem *              getNext() =0;
    /// Returns a const pointer to interface node, where parameter \b i is 0,1 or 2
    virtual const IIntNode *        getVertex(const uint i) const=0;
    /// Returns a pointer to interface node, where parameter \b i is element the vertex 
    /// with valid values in the range 0 to 2
    virtual IIntNode *              getVertex(const uint i) =0;
    /// Returns a const pointer to the adjacent interface element joind across edge \b i. 
    /// Valid values for the edge are in the range 0 to 2.
    virtual const IIntElem *        getJoin(const uint i) const=0;
    /// Returns a pointer to the adjacent interface element joind across edge \b i. 
    /// Valid values for the edge are in the range 0 to 2.
    virtual IIntElem       *        getJoin(const uint i) =0;
    /// Returns the area of the element triangluar surface.
    virtual double                  getArea() const=0;
    /// Returns a const pointer to the face to which the interface element is attached.
    virtual const IFace *           getHost() const=0;
    /// Returns a pointer to the face to which the interface element is attached.
    virtual IFace *                 getHost() =0;
    /// Returns the current centroid of the interface element - the average of the three node position.
    virtual DVect3                  getLocation() const=0;
    virtual const IThing *          getOwner() const=0;
  };

} // namespace zone
// EoF
