#pragma once

#include "jmodelbase.h"

/**
* \file
* \brief The base class for constitutive model plug-ins.
*/

namespace jmodels
{
    /// The most important link between 3DEC and a user-written joint model is the member-function
    ///   run(unsigned dim, State *ps), which computes the mechanical response of the model
    ///   during cycling.\n
    /// A structure, State (defined in state.h?, is used to transfer information to and
    ///   from the model.\n
    /// State is also sent to the initialize() method, although not all fields are guaranteed
    ///   to be valid in that case.\n
    /// The members of State (all public) are as follows.\n
    /// Not all the information may be used by a particular code; the structure is intended 
    ///   to serve all Itasca codes.  
    /// \brief The structure used to pass information to the joint constitutive model.
    /// \ingroup models
  struct State
  {
  public:
    static const UInt max_working_ = 10;
    static const UInt max_iworking_ = 2;

    virtual Double getTimeStep() const=0;  ///< \brief The current time step.
    virtual bool      isThermal() const = 0; ///< \brief true if thermal is configured.
    virtual bool      isCreep() const = 0; ///< \brief true if creep is configured.
    virtual bool      isFluid() const = 0; ///< \brief true if fluid/flow is configured.
    virtual bool   trackEnergy() const = 0; ///< \brief true if energy tracking is on.
    /// Finding a table given an \a id number can be an expensive operation.\n
        /// In general, it is better to store a reference pointer using this function during initialize(),
        ///   and use it to access the table during run().
        /// Index pointers should *not* be saved and restored, and should be found during every initialize() call.
        ///   (Table data may have changed in the interim).
    virtual void * getTableIndexFromID(const String &s) const=0;
    /// The index pointer should be previously obtained from a getTableIndexFromID() call.
    virtual Double getYFromX(void *index,const Double &x) const=0;
    /// The index pointer should be previously obtained from a getTableIndexFromID() call.
    virtual Double getSlopeFromX(void *index,const Double &x) const=0;

    JMODEL_EXPORT State();
    UInt   state_;            ///< \brief Contact state mask.
    Double area_;             ///< \brief Contact area.
    Double normal_force_;     ///< \brief Contact normal force.
    DVect3 shear_force_;      ///< \brief Contact shear force.
    Double normal_disp_;      ///< \brief Normal displacement.
    DVect3 shear_disp_;       ///< \brief Shear displacement.
    Double normal_disp_inc_;  ///< \brief Normal displacement increment.
    DVect3 shear_disp_inc_;   ///< \brief Shear displacement increment.
    Double normal_force_inc_; ///< \brief Normal force increment (output only).
    DVect3 shear_force_inc_;  ///< \brief Shear force increment (output only).
    /// Fraction of normal displacement increment
    /// that causes contact tension or separation (output only)
    Double dnop_;             
    Double working_[max_working_];   ///< \brief This is a working area for values that must be stored between run() calls.
    Int    iworking_[max_iworking_]; ///< \brief This is a working area for values that must be stored between run() calls.
  };
} // namespace jmodels

// EoF
