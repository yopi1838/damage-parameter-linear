//ifluidgp.h
#pragma once

/**
* \file
* \brief Interface to access fluid gridpoint data
*/

#include "base/src/base.h"

namespace zone {
    /// \brief Interface to access fluid gridpoint data
    /// \ingroup Zone
    class IFluidGp {
    public:
        /// Returns the Biot modulus, see setBiot()
        virtual double getBiot() const=0;
        /// Sets the Biot modulus, see getBiot()
        virtual void   setBiot(const double &d) =0;
        /// Returns the fluid mass for the gridpoint
        virtual double getThermMass() const=0;
        virtual double getSource() const=0;
        virtual double getSourceDynamic() const=0;
        virtual double getSourceUser() const=0;
        virtual void   setSourceUser(const double &d)=0;
        /// Returns the grid point unbalanced flow discharge, see  setGpFlow()
        virtual double getGpFlow() const=0;
        /// Sets the grid point unbalanced flow discharge, see  getGpFlow()
        virtual void   setGpFlow(const double &d) =0;
        /// Returns the grid point fluid bulk modulus, see setFModulus()
        virtual double getFModulus() const=0;
        /// Sets the grid point fluid bulk modulus, see getFModulus()
        virtual void   setFModulus(const double &d) =0;
        /// Returns the gridpoint fluid tension limit, see setFtens1()
        virtual double getFtens1() const=0;
        /// Sets the gridpoint fluid tension limit, see getFtens1()
        virtual void   setFtens1(const double &d) =0;
        /// Sets the gridpoint saturation.
        virtual void   setSat(const double &d)=0;
        /// Sets the gridpoint pore-pressure and the updates the accumulated pore pressure increment since the
        /// last mechanical step. See command FIX PP
        virtual void   updatePP(double d)=0;

        // fast flow items
        /// Returns the out of balance flow for fastflow logic
        virtual double getobv() const=0;
        /// sets the out of balance flow for fastflow logic
        virtual void   setobv(const double &d) = 0;
        /// Returns material confined modulus * nodal volume, for fastflow logic
        virtual double getcfmod() const=0;
        /// Returns the nodal volume (gp both fluid and mech)
        virtual double getnvol() const=0;
    };
} // namespace zone
// EoF

