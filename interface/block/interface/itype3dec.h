#pragma once
// itype3dec.h

/**
 * \file
 * \brief This interface is a central point to get type const char pointers.
*/


namespace block
{
  /// This interface is a central point to get type const char pointers.
  /// \ingroup THREEDEC
  class IType3DEC
  {
  public:
    /// block index type
    virtual TType getBlock() const=0;
    /// contact index type
    virtual TType getContact() const=0;
    /// face index type
    virtual TType getFace() const=0;
    /// vertex index type
    virtual TType getVertex() const=0;
    /// zone index type
    virtual TType getZone() const=0;
    /// boundary node index type
    virtual TType getBoundary() const=0;
    /// fluid flow pipe index type
    virtual TType getFPipe() const=0;
    /// finite element index type
    virtual TType getFElement() const=0;
    /// sub-contact index type
    virtual TType getSubContact() const=0;
    /// finite element face index type
    virtual TType getFEFace() const=0;
    /// joint plot face index type
    virtual TType getJointFace() const=0;
    /// joint plot edge index type
    virtual TType getJointEdge() const=0;
    /// joint plot node index type
    virtual TType getJointNode() const=0;
    /// loxial axial reinforcement index type
    virtual TType getReinforcement() const=0;
    /// cable element index type
    virtual TType getCableElement() const=0;
    /// cable node index type
    virtual TType getCableNode() const=0;
    /// beam element index type
    virtual TType getBeamElement() const=0;
    /// beam node index type
    virtual TType getBeamNode() const=0;
    /// liner element index type
    virtual TType getLinerElement() const=0;
    ///  index type
    virtual TType getLinerNode() const=0;
    /// liner interface index type
    virtual TType getLinerContact() const=0;
  };
} // namespace threecec

// EoF