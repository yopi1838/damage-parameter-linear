#pragma once
/** \file igenericzone.h
  * \brief Generic base class for the zone interface made available to the constitutive model system.
  * \ingroup models
  */

namespace itasca
{
  class IThing;
}

namespace models
{
  using itasca::IThing;

  /** \brief Generic base class for the tetra interface made available to the constitutive model system.
    *   
    * A program using the constitutive model system is responsible for providing an implementation of this interface.
    * It may provide non, in which case the relevant functions should return nullptr.
    * \ingroup models
    */
  class IGenericTet
  {
  public:
    /// The type of IGenericTet, used in the alternate dynamic typing system based on IThing.
    inline static const TType type_ = 0x4e134150;
    /// Returns a pointer to a const IThing interface representing this object, or nullptr if such an interface is not supported.
    virtual const IThing *getIThing() const=0;
    /// Returns a pointer to an IThing interface representing this object, or nullptr if such an interface is not supported.
    virtual IThing *      getIThing()=0;
    /// Returns the actual type of the fully derived class represented by this object.
    virtual TType         getType() const=0;
  };
} // namespace models
// EoF