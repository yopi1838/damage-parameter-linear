#pragma once
// ifelementthing.h

#include "fortconv/fortdefc.h"

/**
 * \file
 * \brief This is the interface for FEFaceThing, a wrapper for finite element faces
*/

namespace itasca
{
  class IThing;
}

namespace block
{

  class IFElementThing;

  /// \brief This is the interface for FEFaceThing, a C++ wrapper for finite element faces.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $kfefcpp in the fortran block linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IFEFaceThing
  {
  public:

    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// type used in IThing typing system
    inline static const TType type_ = 0x52e81f69;
    /// Return finite element fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Return the finite element face number (1-6)
    virtual uint      getFaceNumber() const=0;
    /// Return the host finite element
    virtual const IFElementThing * getFElementThing() const=0;

  };

} // end namespace block
// EOF