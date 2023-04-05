#pragma once
/** \file iedge.h
    * \brief Interface for an edge used in the faceted wall logic.
    *
    * \addtogroup wallsrc
    */

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IHalfVertex;
    class IHalfFacet;

    /** \brief Interface for an edge used in the faceted wall logic.
        * This is the interface for an edge used in the faceted wall logic. An edge is a line segment that starts 
        * at the vertex returned by getOrigin() and terminates at the vertex returned by getEnd().
        * \ingroup wallsrc
        */
    class IHalfEdge {
    public:
        /// TType type for type casting.
        inline static const TType type_ = 0x4c906744;

        /// Return an IThing interface for this object.
        virtual IThing *       getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing * getIThing() const=0;

        /// Return the IHalfVertex corresponding with the edge origin.
        virtual IHalfVertex *       getOrigin()=0;
        /// Return the const IHalfVertex corresponding with the edge origin.
        virtual const IHalfVertex * getOrigin() const=0;
        /// Return the IHalfVertex corresponding with the end of the edge.
        virtual IHalfVertex *       getEnd()=0;
        /// Return the const IHalfVertex corresponding with the end of the edge.
        virtual const IHalfVertex * getEnd() const=0;

        /// Return a pointer to the IHalfFacet that shares this edge.
        virtual IHalfFacet *        getPair()=0;
        /// Return a pointer to the IHalfFacet that shares this edge.
        virtual const IHalfFacet *  getPair() const=0;

    };
}// namespace pfc
// EoF

