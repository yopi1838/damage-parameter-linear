#pragma once
// iflowplanevertexthing.h

#include "iblockdef.h"

/**
 * \file
 * \brief This is the interface for FlowPlaneVertexThing, a C++ wrapper for a flow plane vertex
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

  /// \brief This is the interface for FlowPlaneVertexThing, a C++ wrapper for a flow plane vertex
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KEXTCPP in the fortran flow plane vertex linked list.
  ///           This inherits from Thing so we can use groups and extra variables
  /// \ingroup THREEDEC
  class IFlowPlaneVertexThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52d8115b;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return flow plane vertex fortran offset.
    virtual FInt getFortranOffset() const=0;
    /// Returns true if vertex or host flow plane is hidden (host flow plane zone?)
    virtual bool         getHidden() const=0;
    /// Return the fluid aperture
    virtual double       getAperture() const=0; 
    /// Return the mechanical aperture
    virtual double       getMechanicalAperture() const=0; 
    /// set the mechanical aperture to input value
    virtual void         setMechanicalAperture(double d) = 0;
    /// aperture limited by specified maximum
    virtual double       getHydraulicAperture() const=0;
    /// Return the flow plane area associated with this vertex
    virtual double       getArea() const=0; 
    /// Return host flow plane
    virtual const IFlowPlaneThing * getFlowPlane() const=0;
    /// Return host flow plane ID
    virtual quint64 getFlowPlaneID() const=0;
    /// Return the first subcontact associated with this vertex
    virtual const ISubcontactThing * getSubcontact1() const=0;
    /// Return the second subcontact associated with this vertex (nullptr for no subcontact)
    virtual const ISubcontactThing * getSubcontact2() const=0;
    /// Return the first subcontact id associated with this vertex
    virtual quint64     getSubcontact1_ID() const=0;
    /// Return the second subcontact id associated with this vertex (0 for no subcontact)
    virtual quint64     getSubcontact2_ID() const=0;
    /// Return weight associated with first subcontact (0 - 1)
    virtual double       getWeight1() const=0;
    /// Return weight associated with second subcontact (0 - 1)
    virtual double       getWeight2() const=0;
    /// Return the flow know associated with this vertex
    virtual const IFlowKnotThing * getKnot() const=0;
    /// Return the id of flow know associated with this vertex
    virtual quint64     getKnotID() const=0;
    /// Return the local x and y coordinates in the coordinate system defined by the flow plane
    virtual DVect2       getLocalCoords() const=0;
    /// Return the proppant concentration associated with this vertex
    virtual double       getProppantConcentration() const=0;
    /// Return the proppant mass concentration associated with this vertex
    virtual double       getProppantMassConc() const = 0;
    /// Return the proppant thickness associated with this vertex
    virtual double       getProppantThickness() const=0;
    /// Return fluid viscosity
    virtual double       getViscosity() const=0;
    /// Set proppant volume concentration
    virtual void         setProppantConcentration(double c)=0;
    /// Set proppant mass concentration
    virtual void         setProppantMassConc(double c)=0;
    /// Given a property name, returns the index of the property assigned to this flowplane.
    /// \note Base 1, returns 0 if name is not found.
    virtual quint32             getPropertyIndex(const QString &sc) const = 0;
    /// Return property names as a single string with names separated by commas
    virtual QString             getProperties() const = 0;
    /// Given an index, returns the name of the property assigned to this flowplane.
    /// \note Base 1, returns QString() if \a index \a is less then 1 or greater than the number of properties.
    virtual QString             getPropertyName(quint32 index) const = 0;
    /// returns a list of all the valid properties assigned to this flowplane
    virtual QStringList         getPropertyNames() const = 0;
    /// Returns value of flowplane property corresponding to \b index \n
    /// Returns 0 if \b index is not valid
    virtual QVariant            getProperty(uint index) const = 0;
    /// Returns value of flowplane property corresponding to \b propName \n
    /// Returns 0 if \b propName is not recognized
    virtual QVariant            getProperty(const QString& propName) const = 0;
    /// Set property corresponding to \b sprop equal to \b value 
    virtual bool                setProperty(QString sprop, QVariant value) = 0;
    /// Set property corresponding to \b index equal to \b value 
    virtual bool                setProperty(uint index, QVariant value) = 0;
    /// Return the flow plane vertex location
    virtual DVect3           getLocation() const = 0;
    /// Return the F-factor
    virtual double       getFFactor() const = 0;
    /// set the F-factor
    virtual void         setFFactor(double d) = 0;
  };

} // end namespace block
// EOF