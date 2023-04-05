#pragma once
/** \file ivertex.h
    * \brief Interface for a vertex used in the faceted wall logic.
    *
    * \addtogroup wallsrc
    */
#include "base/src/farray.h"


namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IHalfFacet;
    //class IFWall;

    /** \brief Interface for a vertex used in the faceted wall logic.
        * This is the interface for a vertex used in the faceted wall logic. A vertex is simply a point
        * in space that can have a linear velocity. When a vertex has a linear velocity then the associated
        * wall translational and angular velocities are set to 0. The user is responsible for ensuring that 
        * the wall remains orientable and manifold if the positions or velocities are set through this interface.
        * \ingroup wallsrc
        */
    class IHalfVertex {
    public:
        /// TType type for type casting.
        inline static const TType type_ = 0x4c90671b;

        /// Return an IThing interface for this object.
        virtual IThing *        getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *  getIThing() const=0;

        /// Returns the DVect position of the vertex.
        virtual DVect           getPosition() const=0;
        /// Returns the DVect translational velocity of the vertex.
        virtual DVect           getVelocity() const=0;

        /// Set the vertex position to \a p.
        virtual void            setPosition(const DVect &p)=0;
        /// Set the vertex translational velocity to \a p.
        virtual void            setVelocity(const DVect &p)=0;

        /// Return a list of IHalfFacet objects that share this vertex.
        virtual void getFacetList(FArray<IHalfFacet *> *ret)=0;
        /// Return a list of const IHalfFacet objects that share this vertex.
        virtual void getFacetList(FArray<const IHalfFacet *> *ret) const=0;

        /// Return the pointer to the parent FWall.
        //virtual IFWall * getParentFWall()=0;
        //virtual const IFWall * getParentFWall() const=0;

        /// Return an attribute - used for plotting
        virtual QVariant     getAttribute(const QString &name) const=0;


    };
}// namespace pfc
// EoF

