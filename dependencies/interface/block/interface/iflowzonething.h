#pragma once
// iflowzonething.h

#include "base/src/farray.h"

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for FlowZoneThing.
            A FlowZoneThing represents one triangular zone in the flow plane.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IFlowPlaneThing;
  class IFlowPlaneVertexThing;

  class IFlowZoneThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52d95a3f;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return flow zone fortran offset.
    virtual FInt getFortranOffset() const=0;
    /// Return the plane area
    virtual double                      getArea() const=0;
    /// Return the host flow plane
    virtual const IFlowPlaneThing       * getFlowPlane() const=0;
    /// Return the id of the host flow plane
    virtual quint64 getFlowPlaneID() const=0;
    /// Return a vertex of the flow zone.  Input vertex number n (0-2)
    virtual const IFlowPlaneVertexThing * getFlowPlaneVertex(int n) const=0;
    /// Return a vertex id of the flow plane.  Input vertex number n (0-2)
    virtual quint64 getFlowPlaneVertexID(int n) const=0;
    /// Return fluid discharge vector for the flow zone
    virtual DVect3 getDischarge() const=0;
    /// Return fluid velocity vector for the flow zone
    virtual DVect3 getVelocity() const=0;
    /// Return fluid viscosity associated with flow zone
    virtual double getViscosity() const=0;
    /// Return true if zone is cracked
    virtual bool getCracked() const=0;
    /// Return the flow zone locaton
    virtual DVect3 getLocation() const = 0;
  };

} // end namespace block
// EOF