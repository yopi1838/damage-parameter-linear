#pragma once
// ifelementthing.h

#include "fortconv/fortdefc.h"

/**
 * \file
 * \brief This is the interface for FElementThing, a wrapper for finite elements
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IBlockThing;

  /// \brief This is the interface for FElementThing, a C++ wrapper for finite elements.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $kfeid in the fortran block linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IFElementThing
  {
  public:

    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// type used in IThing typing system
    inline static const TType type_ = 0x52e81f5b;
    /// Return finite element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Returns id of associated block. 
    virtual quint64 getBlockID() const=0;
    /// Returns pointer to associated block. 
    virtual const IBlockThing * getBlock() const=0;
    /// Returns the material number.  Returns 0 if using host block material.
    virtual int       getMaterialNumber() const=0;
    /// Returns the constitutive model number.  Returns 0 if using host block model.
    virtual int       getModelNumber() const=0;
    /// Return the number of gauss points
    virtual int       getNumGauss() const = 0;
    ///  Returns stress of gauss point i
    virtual SymTensor getGaussStress(int i) const=0;
    ///  Returns position of gauss point i
    virtual DVect     getGaussPos(int i) const=0;
    ///  Returns state of gauss point i
    virtual int       getGaussState(int i) const=0;

  };

} // end namespace block
// EOF