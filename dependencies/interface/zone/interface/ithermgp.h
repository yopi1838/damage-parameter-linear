// ithermgp.h
#pragma once

/**
* \file
* \brief Interface to access gridpoint thermal data
*/

namespace zone {
    /// \brief Interface to access gridpoint thermal data
    /// \ingroup Zone
    class IThermGp {
    public:
        virtual double getSource() const=0;
        virtual double getSourceDynamic() const=0;
        virtual double getSourceUser() const=0;
        virtual void   setSourceUser(const double &d)=0;
        virtual double getTemperature() const=0;
        virtual void   setTemperature(const double &d) =0;
        virtual double getTempChange() const=0;
        virtual void   setTempChange(const double &d)=0;
        /// Returns the grid point unbalanced flux
        virtual double getGpFlux() const=0;
        virtual double getTempAnalytical() const = 0;
        virtual void setTempAnalytical(double d) = 0;
    };
} // namespace zone
// EoF

