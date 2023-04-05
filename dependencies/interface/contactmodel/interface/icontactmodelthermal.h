#pragma once
/** \file  icontactmodelthermal.h
    * \brief Interface for a thermal contact model.
    *
    * \ingroup contactmodelsrc
    */

#include "module/interface/dim.h"
#include "base/src/baseqt.h"

namespace itascaxd
{
  class IContactModel;
  /** * \brief 
      * Interface for a thermal contact model.
      * \details 
      * A thermal contact model defines the thermal constitutive behavior at a contact. 
      * This interface provides basic functionality for accessing the thermal contact model properties.
      * \ingroup contactmodelsrc
      */
  class IContactModelThermal
  {
  public:
    /// Return an IContactModel interface for this object.
    virtual IContactModel * getContactModel()=0;
    /// Return a const IContactModel interface for this object.
    virtual const IContactModel * getContactModel() const =0;
    /// Returns the thermal contact model activity Distance.
    virtual double   getActivityDistance() const =0;

  };
} // namespace itascaxd
// EoF
