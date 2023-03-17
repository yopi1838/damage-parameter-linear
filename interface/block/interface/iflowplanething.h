#pragma once
// iflowplanething.h

#include "iblockdef.h"

#include "base/src/farray.h"

/**
 * \file
 *  \brief This is the interface for FlowPlaneThing.
            A FlowPlaneThing represents the top of the fluid flow object hierarchy.
            Other flow objects such as flow plane zones and vertices can be obtained
            for each flow plane.
*/

namespace itasca
{
  class IThing;
}

namespace block
{
  class IFlowZoneThing;
  class IFlowPlaneVertexThing;
  class IContactThing;

  class IFlowPlaneThing
  {
  public:

    /// type used in IThing typing system
    inline static const TType type_ = 0x52d800d8;
    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// Return flow plane fortran offset.
    virtual FInt getFortranOffset() const=0;
    /// Return the plane area
    virtual double                      getArea() const=0;
    /// Return the contact associated with the flow plane
    virtual const IContactThing * getContact() const=0;
    /// Return the id of the contact associated with the flow plane
    virtual quint64 getContactID() const=0;
    /// Return local coordinate system x-axis in global system
    virtual DVect3 getLocalX() const=0;
    /// Return local coordinate system y-axis in global system
    virtual DVect3 getLocalY() const=0;
    /// Return flow zones associated with the flow plane
    virtual FArray<const IFlowZoneThing *> getFlowZones() const=0;
    /// Return flow vertices associated with the flow plane
    virtual FArray<const IFlowPlaneVertexThing *> getFlowPlaneVertices() const=0;
    /// Given a property name, returns the index of the property assigned to this flowplane.
    /// \note Base 1, returns 0 if name is not found.
    virtual quint32             getPropertyIndex(const QString &sc) const=0;
    /// Return property names as a single string with names separated by commas
    virtual QString             getProperties() const=0;
    /// Given an index, returns the name of the property assigned to this flowplane.
    /// \note Base 1, returns QString() if \a index \a is less then 1 or greater than the number of properties.
    virtual QString             getPropertyName(quint32 index) const=0;
    /// returns a list of all the valid properties assigned to this flowplane
    virtual QStringList         getPropertyNames() const=0;
    /// Returns value of flowplane property corresponding to \b index \n
    /// Returns 0 if \b index is not valid
    virtual QVariant            getProperty(uint index) const=0;
    /// Returns value of flowplane property corresponding to \b propName \n
    /// Returns 0 if \b propName is not recognized
    virtual QVariant            getProperty(const QString& propName) const=0;
    /// Set property corresponding to \b sprop equal to \b value 
    virtual bool                setProperty(QString sprop, QVariant value)=0;
    /// Set property corresponding to \b index equal to \b value 
    virtual bool                setProperty(uint index, QVariant value)=0;
    /// Return the flow plane location
    virtual DVect3              getLocation() const = 0;

  };

} // end namespace block
// EOF