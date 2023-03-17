#pragma once

/**
  * \file
  * \brief Interface to dowel structural elements
  */

#include "isel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    /// \brief Interface to dowel structural elements
    /** This interface inherits from ISEL.
    * \ingroup SEL
    */
    class ISELDowel {
    public:
        /// Type ID number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x5f983a6a;
        /// Returns a const IThing pointer.
        virtual const IThing* getIThing() const = 0;
        /// Returns a IThing pointer.
        virtual IThing* getIThing() = 0;
        /// Returns a const pointer to object base class.
        virtual const ISEL* getISEL() const = 0;
        /// Returns a pointer to object base class.
        virtual ISEL* getISEL() = 0;
        /// Returns the yield force
        virtual double           getPropYield() const = 0;
        /// Sets the yield force
        virtual void             setPropYield(const double& d) = 0;
        /// Returns the stiffness 
        virtual double           getPropK() const = 0;
        /// Sets the stiffness
        virtual void             setPropK(const double& e) = 0;
        /// Returns the rupture strain limit 
        virtual double           getPropStrainLimit() const = 0;
        /// Sets the rupture strain limit
        virtual void             setPropStrainLimit(const double& e) = 0;
        /// Returns the current yield state of the element. \n
        virtual ISEL::YieldState getRespYield() const = 0;
        /// Returns the force in the dowel (always positive)
        virtual double           getRespForce() const = 0;
        ///  Returns the strain in the dowel
        virtual double           getStrain() const = 0;
        ///  Returns strain rupture state (true for ruptured)
        virtual bool             getStrainFailure() const = 0;
        ///  Return the active length used in shear strain calculation.  Not the same as the actual length
        virtual double        getPropActiveLength() const = 0;
        ///  Set the active length used in shear strain calculation.  Not the same as the actual length
        virtual void          setPropActiveLength(const double& d) = 0;
        virtual QString       getState() const = 0;

        // get orientations and length for plotting
        virtual DVect        getOrientation() const = 0;
        virtual DVect        getHostCableOrientation() const = 0;
    };
} // namespace sel
// EoF
