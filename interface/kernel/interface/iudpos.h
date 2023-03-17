#pragma once
// iudpos.h

/**
 * \file
 * \brief IUDPos interface for user defined data that includes a position in space
 */

#include "base/src/base.h"
namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;
    
    class IUDData;

    /// \brief Interface to the position in space of user-defined data that requires it.
    /** This interface allows access to position information, if the user-defined data includes it.
        * \ingroup kernel
        */
    class IUDPos {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bd2;

        /// Returns an IThing interface to this object.
        virtual IThing *       getIThing()=0;
        virtual const IThing * getIThing() const=0; ///< \copydoc getIThing

        /// Returns the current position in space of the object.
        virtual DVect getPosition() const=0;
        /// Sets the current position of the object.
        virtual void  setPosition(const DVect &v)=0;
    };
} // namespace itascaxd

// EoF
