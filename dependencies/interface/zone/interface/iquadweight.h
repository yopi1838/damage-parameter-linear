#pragma once
// iface.h

/**
  * \file
  * \brief Interface to zone faces
  */

#include "base/src/base.h"

namespace zone
{
  /// \brief Interface to face weight, used to define the location of a gridpoint in a given face
  /** Quadrilateral weighting function class\n
    * Tracks weighting function for point in four sided polygon.\n
    * Also tracks whether the point in question is inside the polygon, and whether the polygon is concave or not.\n
    * \ingroup Zone
  */
  class IQuadWeight
  {
  public:
    /// Given four vectors representing values at the vertices of the face, computes a vector
    ///   using the current vertex weights.
    virtual DVect3  getVector(const DVect3 &dvA,const DVect3 &dvB,const DVect3 &dvC,const DVect3 &dvD) const=0;
    /// Returns the weigth for a given vertex. Parameter \a by must be in the range 0 to 3.
    virtual double  getWeight(quint32 by) const=0;
    /// Set the four weights. Does not check is the sum of the weight is 1.0;
    virtual void  set(const double &w1,const double &w2,const double &w3,const double &w4) = 0;
  };

} // namespace zone
// EoF
