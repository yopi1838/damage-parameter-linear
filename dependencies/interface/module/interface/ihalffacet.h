#pragma once
/** \file ifacet.h
    * \brief Interface for a facet used in the faceted wall logic.
    *
    * \defgroup wallsrc Wall Implementation
    * \addtogroup wallsrc
    */

namespace itasca {
    class IThing;
}

//namespace pfc {
//    class IFWall;
//}

namespace itascaxd {
    using namespace itasca;
    //using namespace pfc;

    class IPiece;
    class IPieceMechanical;
    class IPieceMechanicalData;
    class IHalfVertex;
    class IHalfEdge;
    //class ITriPoly;

    /** \brief Interface for a facet used in the faceted wall logic.
        * This is the interface for a triangular facet as used in the faceted wall logic. A facet consists
        * of three vertices and three edges and the next facet in the local list (for a single faceted wall)
        * can be obtained via the getNextIHalfFacet() method. The centroid is taken to be the average location 
        * of the facet vertices. Facets, at some level, are used as Pieces and/or PieceMechanical objects 
        * and, as such, must return a pointer.
        * \ingroup wallsrc
        */

    class IHalfFacet {
    public:
        /// TType type used for type casting.
        inline static const TType type_ = 0x4c90674d;

        /// Return an IThing interface for this object.
        virtual IThing *        getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *  getIThing() const=0;
        /// Return an IPiece pointer.
        virtual IPiece *                getIPiece()=0;
        /// Return a const IPiece pointer.
        virtual const IPiece *          getIPiece() const=0;
        /// Return an IPieceMechanical pointer.
        virtual IPieceMechanical *      getIPieceMechanical()=0;
        /// Return a const IPieceMechanical pointer.
        virtual const IPieceMechanical *getIPieceMechanical() const=0;
        /// Return an IPieceMechanicalData pointer.
        virtual IPieceMechanicalData *      getIPieceMechanicalData()=0;
        /// Return a const IPieceMechanicalData pointer.
        virtual const IPieceMechanicalData *getIPieceMechanicalData() const=0;

        /// Return the next IHalfFacet in the local list of facets.
        virtual IHalfFacet *        getNextIHalfFacet()=0;
        /// Return the next const IHalfFacet in the local list of facets.
        virtual const IHalfFacet *  getNextIHalfFacet() const=0;

        /// Return the IHalfVertex with index \a i that corresponds to the dimension.
        virtual IHalfVertex *       getIHalfVertex(int i)=0;
        /// Return the const IHalfVertex with index \a i that corresponds to the dimension.
        virtual const IHalfVertex * getIHalfVertex(int i) const=0;

        /// Return the IHalfEdge with index \a i. The origin of IHalfEdge \a i is IHalfVertex \a i.
        virtual IHalfEdge *         getIHalfEdge(int i)=0;
        /// Return the const IHalfEdge with index \a i. The origin of IHalfEdge \a i is IHalfVertex \a i.
        virtual const IHalfEdge *   getIHalfEdge(int i) const=0;

        /// Return the DVect centroid of the facet or the average position of all vertices.
        virtual DVect           getCentroid() const=0;

        /// Return the extent in DExtent3 format.
        virtual DExtent3        getExtent() const=0;
        /// Return the DExtent cell extent used for contact detection and remapping.
        virtual DExtent         getCellExtent() const=0;
        /// Return the DExtent cell extent used for contact detection and remapping.
        virtual DExtent         getToleranceExtent() const=0;

        /// Return the DVect normal of the facets.
        virtual DVect3          getNormal() const=0;

        /// Return a pointer the the parent faceted wall
        //virtual const IFWall *        getParentFWall() const=0;
        //virtual IFWall *              getParentFWall()=0;

        /// Return a pointer the the parent TriPoly
        //virtual const ITriPoly *      getParentTriPoly() const=0;

        /// Return a DVect that is the closest point to this facet.
        virtual DVect           calcClosestPoint(const DVect &) const=0;

        /// Return the centroid position.
        virtual DVect            getPosition() const = 0;
        /// Return the velocity at the centroid.
        virtual DVect            getVelocity() const = 0;
        /// Return the conveyor velocity
        virtual DVect            getConveyorVelocity() const = 0;
        /// Set the conveyor velocity
        virtual void             setConveyorVelocity(const DVect &) = 0;
        /// Return the string of the facet activity
        virtual QString          getActiveSideLabel() const = 0;
        /// Return the integer of the side activity.
        virtual int              getActiveSide() const = 0;
        /// Set the integer of the side activity.
        virtual void             setActiveSide(int t) = 0;

    };
}// namespace itascaxd
// EoF

