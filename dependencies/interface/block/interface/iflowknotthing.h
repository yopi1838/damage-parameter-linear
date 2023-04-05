#pragma once
// iflowknotthing.h

#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 * \brief This is the interface for FlowKnotThing, a C++ wrapper for a flow knot
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class ISubcontactThing;
  class IFlowKnotThing;
  class IFlowPlaneThing;
  class IFlowPlaneVertexThing;

  /// \brief This is the interface for FlowKnotThing, a C++ wrapper for a flow knot
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KNOTCPP in the fortran flow knot linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IFlowKnotThing
  {
  public:

    /// Possible pore pressure (fluid) conditions for a flow knot
    enum pp_condition {PP_Free, PP_Fix, PP_Flux};

    /// Possible thermal conditions for a flow knot
    enum therm_condition {T_Free, T_ConstTemp, T_ConstFlux};

    /// type used in IThing typing system
    inline static const TType type_ = 0x52d96559;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return flow knot fortran offset.
    virtual FInt getFortranOffset() const=0;
    /// Return location.
    virtual DVect3         getLocation() const=0;
    /// Return the fluid pore pressure
    virtual double          getPP() const=0; 
    /// Set the fluid pore pressure
    virtual void          setPP(double pp) = 0;
    /// Return the pore pressure condition
    virtual pp_condition    getPPCondition() const=0; 
    /// Set the pore pressure condition
    virtual void    setPPCondition(pp_condition pc)= 0;
    /// Return the thermal condition
    virtual therm_condition getThermalCondition() const=0; 
    virtual void            setThermalCondition(therm_condition tc) = 0;
    /// Return the additional fluid flux source value
    virtual double          getFluidFlux() const=0;
    /// Set the additional fluid flux source value
    virtual void            setFluidFlux(double f) = 0;
    /// Return the maximum unbalanced volume during the simulation
    virtual double          getUnbalancedVolumeMax() const=0;
    /// Return the maximum unbalanced volume for the current cycle
    virtual double          getUnbalancedVolume() const=0;
    /// Return the area associated with this knot
    virtual double          getArea() const=0;
    /// Return the volume associated with this knot
    virtual double          getVolume() const=0;
    /// Return the stiffness associated with this knot
    virtual double          getStiffness() const=0;
    /// Return the total transmissivity of elements connected to the knot
    virtual double       getTrans() const=0;
    /// Return the temperature
    virtual double       getTemp() const=0;
    /// Set the temperature
    virtual void       setTemp(double T)  = 0;
    /// Return heat flux
    virtual double       getThermalFlux() const=0;
    /// Set the heat flux
    virtual void         setThermalFlux(double d) = 0;
    /// Return heat flow sum
    virtual double       getThermalFlowSum() const=0;
    /// Return unbalanced flux
    virtual double       getUnbalancedThermalFlux() const=0;
    /// Return the heat transfer coefficient
    virtual double       getHeatTransferCoefficient() const=0;
    /// Set the heat transfer coefficient
    virtual void         setHeatTransferCoefficient(double d) = 0;
    /// Return the timestep associated with this knot
    virtual double       getTimestep() const = 0;
    /// Get (joint) fluid bulk modulus for this flow knot
    virtual double       getBulk() const = 0;
    /// Set (joint) fluid bulk modulus for this flow knot
    virtual void         setBulk(double d) = 0;
    /// Get proppant volume concentration applied at flow knot
    virtual double          getAppliedProppantConcentration() const=0;
    /// Set proppant volume concentration applied at flow knot
    virtual void            setAppliedProppantConcentration(double d) const=0;
    /// Get proppant fixity condition (true = fixed, false otherwise)
    virtual bool            getProppantFixed() const=0;
    /// Set proppant fixity condition (true = fixed, false otherwise)
    virtual void            setProppantFixed(bool b)=0;
    /// Return list of flow plane vertices associated with this flow knot
    virtual FArray<const IFlowPlaneVertexThing *> getVertices() const=0;
 
  };

} // end namespace block
// EOF