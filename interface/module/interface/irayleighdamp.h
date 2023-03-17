#pragma once

/**
  * \file
  * \brief Interface to maxwell damping
  */

namespace itascaxd {
    /// \brief Interface to hysteretic damping 
    /// \ingroup Zone
    class IRayleighDamp {
    public:
        virtual double getAlpha() const = 0;
        virtual double getBeta() const = 0;
        virtual uint getOption() const = 0;
        virtual void setAlpha(const double &a) = 0;
        virtual void setBeta(const double &b) = 0;
        virtual void setOption(uint op) = 0;
    };
} // namespace zone
// EoF
