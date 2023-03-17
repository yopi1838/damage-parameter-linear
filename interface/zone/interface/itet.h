//itet.h
#pragma once

/**
* \file
* \brief Interface to one of the tetrahedra used to implement mixed-discretization in a zone.
*/

#include "models/src/igenerictet.h"

class SymTensor;

namespace zone {
    class IZone;
    class IGp;
    using itasca::IThing;

    /// \brief Interface to one of the tetrahedra used to implement mixed-discretization in a zone.
    /** A hexahedral mixed-discretized zone has 10 internal tetrahedra.   Two overlays of five tetrahedra each.\n
    * Each of these tetrahedra has a stress state associated with them.\n
    * In general, the stress state reported for a zone is a volume-averaged state obtained from its component tetra.\n
    * A tetrahedral Nodal-Mixed-Discretized zone will have one of these internal tetra only.\n
    * It is possible to read and modify this internal data using this interface.
    * \ingroup Zone
    */
    class ITet : public models::IGenericTet {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bf7;
        /// Returns the IThing interface for this class implementation, in this case will always return nullptr.
        virtual const IThing *getIThing() const=0;
        /// Returns the IThing interface for this class implementation, in this case will always return nullptr.
        virtual IThing *getIThing()=0;
        virtual TType getType() const=0;
        /// Returns the next tetra in the overlay list.  See IZone::getOverlay1() and IZone::getOverlay2().
        virtual const ITet *getNext() const=0;
        /// Returns the stress state of this tetra.
        virtual SymTensor getStressTensor() const=0;
        /// Sets (overwrites) the stress state of this tetra.
        virtual void      setStressTensor(const SymTensor &s) =0; //set
        /// Returns the Maxwell stress state of this tetra.
        virtual SymTensor getMaxwellStress(uint i) const = 0;
        /// Sets (overwrites) the Maxwell stress state of this tetra.
        virtual void      setMaxwellStress(const SymTensor &s, uint i) = 0;
        /// Returns the volume of this tetra.  This is not calculated until cycling has begun.
        /// Before then the value will always return 0.0.
        virtual double    getVolume() const=0;
    };
} // namespace zone
// EoF

