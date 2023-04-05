#pragma once
// ivertexthing.h

#include "iblockdef.h"
#include "base/src/farray.h"

/**
 * \file
 * \brief This is the interface for VertexThing.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IBlockThing;
  class IFaceThing;
  class IFlowKnotThing;

  /// \brief This is the interface for VertexThing.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KGCPP in the fortran vertex linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IVertexThing
  {
  public:
    ///offset for index to VertexThing ID
    static const FInt cpp   = 39; // C++ extension from Fortran array

    /// Enum of possible mechanical boundary conditions for a vertex
    enum {BoundFree=0, BoundLoad, BoundViscous, BoundVelocity};

    /// type used in IThing typing system
    inline static const TType type_ = 0x4ed7cfbd;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return vertex fortran offset. 
    virtual FInt getFortranOffset() const=0;
    /// Return Thing ID
    virtual quint64 getVertexID() const=0;
    /// Return vertex 3DEC id used for structural elements  (not the same as Thing ID)
    virtual FInt getThreedecID() const=0;
    /// Returns the boolean stored in utility bit \a bit in the zone.
    /// \arg bit must be a bit index previouly returned from IVertexArray::claimBit().
    virtual bool                getBit(int bit) const=0;
    /// Sets the boolean stored in utility bit \a bit in the zone.
    /// This function is const, to indicate that setting bit values does not impact the calculation
    ///   logic in any way.
    /// \arg bit must be a bit index previouly returned from IVertexArray::claimBit().
    /// Note - this is const through some hack so that we can use const VertexThings when getting data
    virtual void                setBit(int bit,bool b) const=0;
    /// Retrieves the value stored in the utility array at \a index.
    /// \arg index must be a value previously returned from IVertexArray::claimValue().
    virtual QVariant            getValue(int index) const=0;
    /// Sets the value stored in the utility array at \a index.
    /// This is const to indicate that these bit values do not modify the model in any way.
    /// \arg index must be a value previously returned from IVertexArray::claimValue().
     /// Note - this is const through some hack so that we can use const VertexThings when getting data
    virtual void                setValue(int index,const QVariant &v) const=0;
    /// Returns gridpoint position
    virtual DVect3              getLocation() const=0;
    /// Returns gridpoint pore pressure
    virtual double              getPP() const=0;
    /// set the gridpoint pore pressure
    virtual void                setPP(double pp)=0; 
    /// Returns gridpoint temperature.  Returns 0 if not configured for thermal
    virtual double              getTemperature() const=0;
    /// Sets gridpoint temperature.  Does nothing if not configured for thermal.
    virtual void                setTemperature(double temp)=0;
    /// Returns true if temperature is fixed.  Returns 0 if not configured for thermal
    virtual bool              getTemperatureFixed() const=0;
    /// Initialize gridpoint temperatures for analytical thermal solution
    virtual void         iniTemperatureAnalytical(double temp)=0;
    /// Returns gridpoint displacement
    virtual DVect3              getDisplacement() const=0;
    /// Set the gridpoint displacement
    virtual void                setDisplacement(const DVect3 & dv)=0;
    /// Returns gridpoint small strain displacement
    virtual DVect3              getSmallDisplacement() const=0;
    /// Returns gridpoint velocity
    virtual DVect3              getVelocity() const=0;
    /// Set the gridpoint velocity
    virtual void                setVelocity(const DVect3 & dv)=0;
    /// Returns gridpoint acceleration. Returns 0,0,0 if not configured for dynamic
    virtual DVect3              getAcceleration() const=0;
    /// Returns gridpoint unbalanced (??) force
    virtual DVect3              getForce() const=0;
    /// Add input vector to current gridpoijnt force
    virtual void       addForce(const DVect& df) = 0;
    /// Returns contents of gridpoint spare offset
    virtual double              getSpare() const=0;
    /// return fortran offset of host block
    virtual FInt                getBlock() const=0;
    /// return host BlockThing
    virtual IBlockThing * getBlockThing() const=0;
    /// return host BlockThing ID
    virtual quint64      getBlockThingID() const=0;
    /// return gridpoint stress for high-order zones
    virtual SymTensor    getHoStress(bool bEffective) const=0;
    /// Returns true if vertex or host block is hidden (TODO - check host zone)
    virtual bool         getHidden() const=0;
    /// Returns the load (force) applied to the vertex
    virtual DVect3       getAppliedLoad() const=0;
    /// Set the applied load on the vertex.
    /// This also automatically sets the BCType to BoundLoad if the force > 0
    virtual void         setAppliedLoad(const DVect3 &dv)=0;
    /// Returns the load (force) increment applied to the vertex
    /// This also automatically sets the BCType to BoundLoad if the force > 0 ??
    virtual DVect3       getAppliedLoadInc() const=0;
    /// Set the applied load increment on the vertex
    virtual void         setAppliedLoadInc(const DVect3 &dv)=0;
    /// Returns the reaction force at the vertex
    virtual DVect3       getReactionForce() const=0;
    /// Returns the velocity applied to the vertex
    virtual DVect3       getAppliedVelocity() const=0;
    /// Set the applied velocity on the vertex
    /// This also automatically sets the BCType to BoundVelocity for ALL COMPONENTS
    virtual void         setAppliedVelocity(const DVect3 &dv)=0;
    /// Set the input component of applied velocity on the vertex (0=x, 1=y, 2=z)
    /// This also automatically sets the BCType to BoundVelocity for the input component
    virtual void         setAppliedVelocity(double value, uint component)=0;
    /// Returns the unbalanced force at the vertex
    virtual DVect3       getFOB() const=0;
    /// \brief Return the type of boundary condition for this vertex in each direction
    /// Components of the vector correspond to BCs as given in the enum associated with this class
    /// Note that normal velocity is a special case.  Use GetBCNormal.
    virtual IVect3       getBCType() const=0;
    /// set the boundary condition type. 
    /// Components of the vector correspond to BCs as given in the enum associated with this class
    virtual void setBCType(const IVect3& iv)=0; 
    /// Return true if a normal velocity boundary condition is present
    virtual bool getBCNormal() const=0;
    /// Returns true if host block is marked as excavated
    virtual bool      isExcavated() const=0;
    /// Returns true if host block is removed
    virtual bool      isRemoved() const=0;
    /// Returns list of faces.
    /// Input true to get original faces or false to get triangulated faces
    ///  WARNING - this is slow since vertices don't store their faces, so a search is required
    virtual FArray<const IFaceThing *> getFaces(bool rigid) const=0;
    /// Return true if VertexThing and Fortran vertex are in sync
    virtual bool isValid() const=0;
    /// Return Flow Knot associated with this vertex. Returns null if none.
    virtual IFlowKnotThing * getFlowKnot() const=0;
    /// Return applied thermal flux
    virtual double getAppliedThermalFlux() const=0;
    /// Return thermal flux
    virtual double getThermalFlux() const = 0;
    /// Return thermal mass
    virtual double getThermalMass() const = 0;
    /// Return the position of the gridpoint with deformation exaggerated by factor if active is true
    virtual DVect3 getDeformedPosition(bool active, double factor) const=0;
    /// Return dynamic mass
    virtual double getDynamicMass() const = 0;
    /// Return total force magnitude
    virtual double getTotalForce() const = 0;
    /// return face block thing.  Returns nullptr if not on a faceblock
    virtual IBlockThing* getFaceBlockThing() const = 0;
    /// Return gridpoint stiffness
    virtual double getStiffness() const = 0;
    /// Set gridpoint stiffness
    virtual void   setStiffness(double value) = 0;
    /// Return face block thing.  Returns nullptr if not on a faceblock
    virtual double getMass() const = 0;
    /// Set vertex mass
    virtual void   setMass(double value) = 0;
    /// Return density scaling factor
    virtual double getDensityScalingFactor() const = 0;
    /// Set density scaling factor
    virtual void   setDensityScalingFactor(double value) = 0;
    /// Returns temperature change at gridpoint.  Returns 0 if not configured for thermal
    virtual double getDTemperature() const = 0;
    /// Add value to vertex stiffness.  
    /// This is separate stiffness storage used by FLAC3D structural elements
    virtual void         addToSelStiffness(double ss) = 0;
    /// Add value to vertex mass.  
    /// This is separate mass storage used by FLAC3D structural elements
    virtual void         addToSelMass(double sm) = 0;
    /// Return current local force ratio
    virtual double getForceRatio() const = 0;
    /// Set force ratio target
    virtual void setForceRatioTarget(double d) = 0;
    /// Return force ratio target
    virtual double getForceRatioTarget() const = 0;
    /// Return convergence ratio (0-1)
    virtual double getConvergence() const = 0;
  };

} // end namespace block
// EOF