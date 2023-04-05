#pragma once

/**
    * \file
    * \brief Interface to a particular particle trace being taken by the particle trace logic.
    */

#include "dim.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a particular particle trace being taken by the particle trace logic.
    /// \ingroup module
    class IParticleTrace {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b8d;

        /// Get IThing interface for the particle trace.
        virtual const IThing *getIThing() const=0;

        /// Unique ID associated with the particle trace
        virtual quint64 getID() const=0;

        /// Name of the particle trace - generally type and location. This name is assigned by the code.
        virtual QString getName() const=0;

        /// Label of the particle trace - to be displayed in plots.\n
        /// If unassigned this will return the same as getName(). Otherwise it will return a label assigned by the user.
        virtual QString getLabel() const=0;

        /// Returns the current position of the object.\n
        /// Returns (0,0,0) if the particle trace has a invalid position.
        virtual DVect getPosition() const=0;

        /// Returns a vector that contains the current velocity of the object.\n
        /// Returns (0,0,0) if the particle trace has a invalid velocity.
        virtual DVect getVelocity() const=0;

        /// Returns TRUE if the particle trace is in a state to take more values.\n
        /// Returns FALSE if not (for instance, a particle trace of a zone that has been deleted.)
        virtual bool isValid() const=0;

        /// This is the position in the particle trace temp file of the first record 
        ///<    containing values from this particle trace.
        virtual qint64 getStartPosition() const=0;

        /// This is the position in the particle trace record of the object.\n
        /// Note that this is distinct from the ID() of the particle trace.
        virtual int getStorageIndex() const=0;

        /// Returns the 3D region where the particle traces occured
        virtual DExtent getTraceExtent() const=0;

        /// Returns the maximum scalar velocity 
        virtual double getMaxVelocity() const=0;

        /// Returns the next Iparticle trace() object in the global list,
        ///<   or 0 if it is the last one.
        virtual const IParticleTrace *getNext() const=0;

    };
} // namespace itascaxd
// EoF
