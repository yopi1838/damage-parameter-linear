#pragma once
/** \file ifwall.h
    * \brief Interface for a faceted wall.
    *
    * \addtogroup wallsrc
    */
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IGroup;
    class IContainer;
}

namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IBody;
    class IContact;
    class IHalfFacet;
    class IHalfVertex;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /** \brief Interface for a faceted wall.
        *
        * This is the interface for a faceted wall. A faceted wall is a wall that is composed of triangular 
        * facets. Each facet is composed of three edges and three vertices. A half-edge data structure is
        * used to efficiently store and retrieve adjacency information (e.g., facets adjacent to another facet, 
        * facets that share a vertex, etc.). This implementation requires that a FWall is a manifold, orientable
        * triangular mesh throughout. The position of a FWall is the average location of all vertices. 
        * As implemented, FWalls must exist within the model domain. When an FWall intersects a domain boundary
        * with reflect boundary conditions an exception occurs. When an FWall intersects a domain boundary with 
        * any other boundary condition the FWall translational and angular velocities are set to 0.
        * The vertices of an FWall can be moved and assigned a translational velocity. The user is responsible for
        * maintaining the manifold/orientable nature of the triangular mesh in these cases. When a vertex has 
        * a translational velocity then the FWall translational/angular velocities are set to 0.
        *
        * \ingroup wallsrc
        */

    class IFWall {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x4c90658e;
        /// Return an IThing interface for this object.
        virtual IThing *                getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *          getIThing() const=0;
        /// Return an IBody pointer. 
        virtual IBody *                 getIBody() = 0;
        /// Return a const IBody pointer.
        virtual const IBody *           getIBody() const = 0;
        /// Return the QString name.
        virtual QString                 getName() const=0;
        /// Return the next IFWall object in the global list, or 0 if it is the last set.
        virtual IFWall *                getNextIFWall()=0;
        /// Return the next IFWall object in the global list, or 0 if it is the last set.
        virtual const IFWall *          getNextIFWall() const=0;
        /// Return the first IHalfFacet associated with this FWall. 
        virtual IHalfFacet *            getFirstIHalfFacet()=0;
        /// Return the const first IHalfFacet associated with this FWall.
        virtual const IHalfFacet       *getFirstIHalfFacet() const=0;
        /// Return the DExtent3 extent of the faceted wall.
        virtual DExtent3                getExtent() const=0;
        /// Get the user defined cutoff angle for contact resolution - in radians
        virtual double                  getCutoffAng() const=0;
        /// Set the user defined cutoff angle for contact resolution - provided in radians
        virtual void                    setCutoffAng(const double &d) =0;
        /// Return the IContainer of facets associated with a wall
        virtual IContainer             *getFWallAsFacetContainer() =0;
        /// Return the IContainer of facets associated with a wall
        virtual const IContainer       *getFWallAsFacetContainer() const=0;
        /// Return the IContainer of vertices associated with a wall
        virtual IContainer             *getFWallAsVertexContainer() =0;
        /// Return the IContainer of vertices associated with a wall
        virtual const IContainer       *getFWallAsVertexContainer() const=0;


        /// Return the number of IHalfFacets.
        virtual int getNumberFacets() const=0;
        /// Return a list of IHalfFacets.
        virtual void getFacetList(FArray<IHalfFacet *> *ret)=0;
        /// Return a list of const IHalfFacets.
        virtual void getFacetList(FArray<const IHalfFacet *> *ret) const=0;
        /// Find the const IHalfFacet with ID \a id;
        virtual const IHalfFacet * findFacetWithID(quint64 id) const=0;
        /// Find the IHalfFacet with ID \a id.
        virtual IHalfFacet *       findFacetWithID(quint64 id) =0;
        /// Find the IHalfFacet closest to \a pos.
        virtual const IHalfFacet * findClosestFacet(const DVect &pos,const double &radius = limits<double>::max()) const=0;
        /// Find the const IHalfFacet closest to \a pos.
        virtual IHalfFacet * findClosestFacet(const DVect &pos,const double &radius = limits<double>::max())=0;
        /// Remove the IHalfFacet from the FWall
        virtual void removeIHalfFacet(IHalfFacet *fac) =0;
        /// Add a facet from vertices.
        virtual IHalfFacet * addFacetFromVertices(DVect verts[dim],QString &error,quint64 id=0,bool ignoreEnd=false,bool allowStar=false)=0;
        /// Cause a validate, updating convexity
        virtual void doValidate() = 0;

        /// Return a list of IHalfVertex objects.
        virtual void getVertexList(FArray<IHalfVertex *> *ret)=0;
        /// Return a list of const IHalfVertex object.
        virtual void getVertexList(FArray<const IHalfVertex *> *ret) const=0;
        /// Find the const IHalfVertex with ID \a id;
        virtual const IHalfVertex * findVertexWithID(quint64 id) const=0;
        /// Find the IHalfVertex with ID \a id.
        virtual IHalfVertex *       findVertexWithID(quint64 id) =0;
        /// Find the IHalfVertex closest to \a pos.
        virtual const IHalfVertex * findClosestVertex(const DVect &pos,const double &radius = limits<double>::max()) const=0;
        /// Find the const IHalfFacet closest to \a pos.
        virtual IHalfVertex * findClosestVertex(const DVect &pos,const double &radius = limits<double>::max())=0;

        /// Return the wall position. This is implementation dependent.
        virtual DVect            getPosition() const = 0;
        /// Return the center of rotation - the point about which the wall can rotate.
        virtual DVect            getCentRot() const = 0;
        /// Return the translational velocity of the wall.
        virtual DVect            getVelocity() const = 0;
        /// Return the angular velocity of the wall.
        virtual DAVect           getAngVelocity() const = 0;
        /// Return the accumulated displacement of the point corresponding to position.
        virtual DVect            getDisplacement() const = 0;
#ifdef TWOD
        /// Return the accumulated angular displacement in degrees.
        virtual double           getAngDisplacement() const = 0;
#endif
        /// Return the sum of all contact forces to which the wall does not react.
        virtual DVect            getForce() const = 0;
        virtual DVect            getContactForce() const = 0;
        /// Return the moment of all contacts with with wall.
        virtual DAVect           getMoment() const = 0;
        virtual DAVect           getContactMoment() const = 0;
        /// Return the current orientation
        virtual Quat             getOrient() const=0;
        /// Return a boolean indicating whether or not this is a closed wall
        virtual bool             getIsClosed() const=0;
        /// Return a bool indicating whether or not a point is inside a wall. If not closed then returns false
        virtual bool             getIsIn(const DVect &d) const=0;
        /// Retuen boolean indicating convex. Returns false if not closed 
        virtual bool             getIsConvex() const=0;

        /// Set the wall position to \a p.
        virtual void setPosition(const DVect &p) = 0;
        /// Set the wall center of rotation to \a p.
        virtual void setCentRot(const DVect &p) = 0;
        /// Set the wall translational velocity to \a p.
        virtual void setVelocity(const DVect &p) = 0;
        /// Set the wall angular velocity to \a p.
        virtual void setAngVelocity(const DAVect &p) = 0;
        /// Set the wall accumulated displacement to \a v.
        virtual void setDisplacement(const DVect &v) = 0;
#ifdef TWOD
        /// Set the wall accumulated angular displacement to \a v in degrees.
        virtual void setAngDisplacement(const double &v) = 0;
#endif
        /// Set the contact force.
        virtual void            setContactForce(const DVect &v)=0;
        /// Set the contact moment.
        virtual void            setContactMoment(const DAVect &v)=0;
        /// Set the orientation
        virtual void setOrient(const Quat &q)=0;
#ifdef THREED
        /// Get the Euler angles from the orientaion 
        virtual DVect   getEulerFromOrient() const=0;
        /// Set the orientation from euler angles
        virtual void setOrientFromEuler(const DVect3 &d)=0;
#endif
        /// Set the properties in \a prop to \a v for all PieceMechanicals that are in this wall.
        virtual bool  setProperty(const QString &name,const QVariant &v)=0;

        /// Rotate the wall about the axis \a axis through the rotation point \a rotp by the angle \a w (in radians).
        /// If orientation tracking is enabled then it is updated when the \a update boolean is set to true
        virtual void rotate(const DVect &rotp,const DVect &axis,const double &w,bool update=true,bool noThrow = false) = 0;


        /// Get the boolean indicating whether or not the vertices have velocities.
        virtual bool getVertsHaveVel() const = 0;
        /// Update the position of the wall centroid
        virtual void updateFWallPosition(IHalfVertex *v=0,DVect *prevPos=0)=0;

        /// Return the wall fragment index
        virtual int                          getFragmentIndex() const=0;
        /// Set the wall fragment index
        virtual void                         setFragmentIndex(int ind)=0;
        /// Return a list of IContacts.
        virtual void getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0)=0;
        /// Return a list of const IContacts.
        virtual void getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const=0;

        /// Get wall servo gain relaxation factor
        virtual double getServoGainFactor() const =0;
        /// Get wall servo gain 
        virtual double getServoGain()       const =0;
        /// Get wall servo cap velocity 
        virtual double getServoVmax()       const =0;
        /// Get wall servo force 
        virtual DVect  getServoForce()      const =0;
        /// Get wall servo activity status 
        virtual bool   getServoActive()     const =0;
        /// Get wall servo gain update interval 
        virtual int    getServoGainUpdate() const =0;
        /// Set servo gain relaxation factor
        virtual void   setServoGainFactor(const double &d) =0;
        /// Set servo gain 
        virtual void   setServoGain(const double &d)       =0;
        /// Set servo cap velocity 
        virtual void   setServoVmax(const double &d)       =0;
        /// Set servo force 
        virtual void   setServoForce(const DVect &f)       =0;
        /// Set servo active status 
        virtual void   setServoActive(bool b)              =0;
        /// Set servo gain update interval 
        virtual void   setServoGainUpdate(int i)           =0;
    };
} // namespace pfc
// EoF

