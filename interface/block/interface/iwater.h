#pragma once
//iwater.h

/**
  * \file
  * \brief This interface provides access to water table data.
  */


#include "iblockdef.h"

namespace block
{
  /// \brief This interface provides access to functions used to define a water surface.
  /// \ingroup THREEDEC
  class IWater
  {
  public:
    /// Returns the tag for the water surface geometry change notice
    virtual FInt   getWaterNotice() const=0;
    /// returns status of surface
    virtual bool   getActive() const=0;
    /// returns definintion of plane if water surface is a single plane
    virtual bool   getPlane() const=0;
    /// gets origin of water surface if it is a single plane
    virtual RVect3 getOrigin() const=0;
    /// return normal of water surface if it is a single plane
    virtual RVect3 getNormal() const=0;
    /// returns index to the first triangle used to define the water surface
    virtual FInt   getTriangle() const=0;
    /// retuns the index to the next triangle
    virtual FInt   getNextTriangle(FInt triangle) const=0;
    /// returns the cordinates of the specified vertex for a surface triangle
    virtual RVect3 getVertex(UByte vert,FInt triangle) const=0;
    /// return water type identifier
    virtual quint32  getWaterType() const=0;
  };
} // namespace block
// EoF