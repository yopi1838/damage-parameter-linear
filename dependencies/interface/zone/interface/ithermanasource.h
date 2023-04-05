#pragma once
// ithermanasource.h

/**
 * \file
 * \brief ThermAnaSource interface for thermal analytical source
 */

#include "base/src/base.h"
namespace itasca {
    class IThing;
}

namespace zone {
    using namespace itasca;

    struct ThermAnaSourceComponent;

    /// \brief Interface to the thermal analytical source
    /** This interface allows access to position information, and source strength/time/decay/etc
        * \ingroup kernel
        */
    class IThermAnaSource {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x6240891f;

        /// Returns an IThing interface to this object.
        virtual IThing* getIThing() = 0;
        virtual const IThing* getIThing() const = 0; ///< \copydoc getIThing

        ///  Get next source in the list
        virtual const IThermAnaSource* getNext() const = 0;

        /// Sets the current position of the object.
        virtual void  setLocation(const DVect& v) = 0;

        ///  Get2D or 3D position - different from getLocatin , which only returns 3D
        virtual DVect getPosition() const = 0;

        ///  Set the start time
        virtual double getStartTime() const = 0;
        ///  Return the start time
        virtual void  setStartTime(double t) = 0;

        ///  Set the source strength
        virtual double getStrength() const = 0;
        ///  Return the source strength
        virtual void  setStrength(double s) = 0;

        ///  Return number of components
        virtual quint64 getNumComponents() const = 0;
        ///  Return component i fraction.  Input is 0 based
        virtual double getComponentFraction(size_t i) const = 0;
        ///  Return component i decay.  Input is 0 based
        virtual double getComponentDecay(size_t i) const = 0;

        /// get thermal diffusivity constant ( =conductivity / (density*specific heat) )
        virtual double getDiffusivity() const = 0;
        ///  get thermal conductivity constant
        virtual double getThermalCond() const = 0;

    };
} // namespace 