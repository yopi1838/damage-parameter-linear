#pragma once

/**
  * \file
  * \brief Interface to maxwell damping
  */

namespace itascaxd {
    /// \brief Interface to maxwell damping 
    /// \ingroup Zone
    class IMaxwellDamp {
    public:
        virtual double getDamp(int n) const = 0;
        virtual double getFreq(int n) const = 0;
        virtual bool getOffIfYield() const = 0;
        virtual void setDamp(int n, const double &v, bool calc = true) = 0;
        virtual void setFreq(int n, const double &v, bool calc = true) = 0;
        virtual void setOffIfYield(bool b) = 0;
    };
} // namespace zone
// EoF
