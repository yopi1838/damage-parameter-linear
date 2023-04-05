#pragma once
/** \file itripoly.h
    * \brief Interface for a TriPoly.
    *
    * \addtogroup pfcmodulesrc
    */

#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IGroup;
}

namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IBody;
    class IHalfFacet;
    class IHalfVertex;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /** \brief Interface for a TriPoly.
        *
        * This is the interface for a polygon in 2D or a triangularly faceted polyhedron in 3D. 
        * Each facet is composed of three edges and three vertices. A half-edge data structure is
        * used to efficiently store and retrieve adjacency information (e.g., facets adjacent to another facet, 
        * facets that share a vertex, etc.). This implementation requires that a TriPoly is a manifold, orientable
        * triangular mesh throughout. The position of a TriPoly is the average location of all vertices if open or
        * the center of mass assuming a constant density of closed. 
        *
        * \ingroup pfcmodulesrc
        */

    class ITriPoly {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x4e70b69e;
        /// Return an IThing interface for this object.
        virtual IThing *                getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *          getIThing() const=0;
        /// Return the next ITriPoly object in the global list, or 0 if it is the last set.
        virtual ITriPoly *              getNextITriPoly()=0;
        /// Return the next ITriPoly object in the global list, or 0 if it is the last set.
        virtual const ITriPoly *        getNextITriPoly() const=0;
        /// Return the first IHalfFacet associated with this TriPoly. 
        virtual IHalfFacet *            getFirstIHalfFacet()=0;
        /// Return the const first IHalfFacet associated with this TriPoly.
        virtual const IHalfFacet *      getFirstIHalfFacet() const=0;
        /// Return the DExtent3 extent of the TriPoly.
        virtual DExtent3                getExtent() const=0;

        /// Return the number of IHalfFacets.
        virtual int                     getNumberFacets() const=0;
        /// Return a list of IHalfFacets.
        virtual void                    getFacetList(FArray<IHalfFacet *> *ret)=0;
        /// Return a list of const IHalfFacets.
        virtual void                    getFacetList(FArray<const IHalfFacet *> *ret) const=0;

        /// Remove the IHalfFacet from the TriPoly
        virtual void                    removeIHalfFacet(IHalfFacet *fac) =0;
        /// Add a facet from vertices.
        virtual IHalfFacet *            addFacetFromVertices(DVect verts[dim],QString &error,quint64 id=0,bool ignoreEnd=false,bool allowStar=false)=0;

        /// Return a list of IHalfVertex objects.
        virtual void                    getVertexList(FArray<IHalfVertex *> *ret)=0;
        /// Return a list of const IHalfVertex object.
        virtual void                    getVertexList(FArray<const IHalfVertex *> *ret) const=0;

        /// Return the TriPoly position. This is implementation dependent.
        virtual DVect                   getPosition() const = 0;
        /// Return the TriPoly volume.
        virtual double                  getVolume() const = 0;
        /// Set the TriPoly position to \a p.
        virtual void                    setPosition(const DVect &p) = 0;

        /// Rotate the wall about the axis \a axis through the rotation point \a rotp by the angle \a w (in radians).
        virtual void rotate(const DVect &rotp,const DVect &axis,const double &w,bool noThrow = false) = 0;

    };
} // namespace pfc
// EoF

