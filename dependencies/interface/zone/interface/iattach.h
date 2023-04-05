// iattach.h
#pragma once

/**
  * \file
  * \brief Interface to a attachment object
  */

#include "base/src/base.h"

namespace itasca {
    class IThing;
}
namespace zone {
    class IGp;
    class IFace;
    class IQuadWeight;

    /// \brief Interface to a attachment object
    /** Attachments can occur between 2 grid points, a grid pont and an edge or a gridpoint and a face.
     * \ingroup Zone
    */
    class IAttachPoint {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a1d;
        /// The specific types of attach conditions that can be derived from an attach point.
        enum class ClassType {
            Gp = 1,    ///< Direct Gp to Gp attachment.
#ifdef THREED
            Edge,    ///< Gp attached to the edge between two Gp.
#endif
            Face     ///< Gp attached to a face of a zone.
        };

        /// returns a const IThing pointer
        virtual const IThing *          getIThing() const = 0;
        /// returns a IThing pointer
        virtual IThing *                getIThing() = 0;
        /// returns a const pointer to the next attachement object, IAttachPoint, in the global attachment list, or 0 to indicate the end of the list.\n
        /// See IAttachList::getFirst().
        virtual const IAttachPoint *    getNext() const = 0;
        /// returns the attachment point location
        virtual DVect3                  getLocation() const = 0;
        /// returns \b true if the Gridpoint was 'nulled'
        virtual bool                    isMechNull() const = 0;
        /// returns a const pointer to the slave gridpoint.
        virtual const IGp *             getSlave() const = 0;
        /// Returns the type of attach this class represents.  gp-gp, gp-edge, or gp-face.
        virtual ClassType               classType() const = 0;
        /// Returns a const pointer to the master gridpoint in a <b> gridpoint to gridpoint attachment </b> \n
        /// Returns a const pointer to one of the grid points of a <b> gridpoint to edge attachment </b> \n
        /// Returns 0 for a <b> gridpoint to face attachment </b>
        virtual const IGp *             getMaster1() const = 0;
        /// Returns 0 for a <b> gridpoint to gridpoint attachment </b> \n
        /// Returns a const pointer to the other gridpoint of a <b> gridpoint to edge attachment </b> \n
        /// Returns 0 for a <b> gridpoint to face attachment </b>
        virtual const IGp *             getMaster2() const = 0;
        /// Returns 0 for a <b> gridpoint to gridpoint attachment </b> \n
        /// Returns 0 for a <b> gridpoint to edge attachment </b> \n
        /// Returns a const pointer to a IFace object for a <b> gridpoint to face attachment </b>
        virtual const IFace *           getFace()const = 0;
        /// Returns 0.0 for a <b> gridpoint to gridpoint attachment </b> \n
        /// Returns the weight for a <b> gridpoint to edge attachment </b> \n
        /// Returns 0.0 for a <b> gridpoint to face attachment </b>
        virtual double                  getWeight() const = 0;
        /// sets the weight for a <b> gridpoint to edge attachments </b> \n
        virtual void                    setWeight(double w) = 0;
        /// Returns 0 for a <b> gridpoint to gridpoint attachment </b> \n
        /// Returns 0 for a <b> gridpoint to edge attachment </b> \n
        /// Returns a IQuadWeight const pointer to a <b> gridpoint to face attachment </b>
        virtual const IQuadWeight *     getQuadWeight() const = 0;
        /// Returns a IQuadWeight pointer to a <b> gridpoint to face attachment </b>
        virtual IQuadWeight *           getQuadWeight() = 0;
        /// returns true if gridpoints are "snapped"
        virtual bool                    getSnap() const = 0;
        /// set condition to "snap" gridpoints
        virtual void                    setSnap(bool s) = 0;
    };
} /// namespace zone
// EoF
