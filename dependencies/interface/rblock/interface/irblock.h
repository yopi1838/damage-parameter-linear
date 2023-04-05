#pragma once
#include "base/src/farray.h"

namespace itasca {
    class IThing;
    class IGroup;
    class IContainer;
}

namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IBodyMechanical;
    class IRigidBodyMechanical;
    class IBody;
    class IContact;
    class IHalfFacet;
    class IHalfVertex;
}

namespace rblock {
    using namespace itasca;
    using namespace itascaxd;

    class IRBlock {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x5595cd9e;
        /// Return an IThing interface for this object.
        virtual IThing *                getIThing()=0;
        /// Return a const IThing interface for this object.
        virtual const IThing *          getIThing() const=0;
        /// Return the next IRBlock object in the global list, or 0 if it is the last set.
        virtual IRBlock *               getNextIRBlock()=0;
        /// Return the next IRBlock object in the global list, or 0 if it is the last set.
        virtual const IRBlock *         getNextIRBlock() const=0;
        /// Return the DExtent3 extent.
        virtual DExtent3                getExtent() const=0;
        // Return the cell extent
        virtual const DExtent &getCellExtent() const=0;


        /// Get the list of vertices and facets. Nothing is done with the second array in 2D.
        virtual void                    getGeometry(FArray<DVect> *vert,FArray<int> *fac=0) const=0;
        /// Returns the ball position containing the block.
        virtual DVect                   getBallOffset() const=0;
        /// Returns the ball radius containing the block.
        virtual double                  getBallRadius() const=0;
        /// Returns the aspect ratio of the block.
        virtual double                  getAspectRatio() const=0;
        /// Get the vertices listed out in facet format 
        virtual void                    getFacetGeometry(FArray<DVect> *vert) const=0;
        /// Returns the 2 face indices of the edge and the edge index of the edge in the first face
        virtual void                    getEdgeInfo(FArray<IVect3> *info) const=0;
        /// Get the vertex of a facet
        virtual DVect                   getVertexOfFacet(int fac,int ver) const=0;
        /// Get the area of a facet
        virtual double                  getFacetArea(int fac) const = 0;
        /// Get the vertex
        virtual DVect                   getVertex(int ver) const=0;
        /// Get the closest point on a facet to a point in space
        virtual DVect                   getClosestPointOnFacet(const DVect &,int fac) const =0;
        /// Get the facet index with point closest to a position in space. If 2 facets are equally close then the 
        /// last one found is returned
        virtual int                     getFacetClosestToPoint(const DVect &) const =0;
        /// Get the facet normal
        virtual DVect                   getFacetNormal(int fac) const=0;
        /// Get the facet position
        virtual DVect                   getFacetPosition(int fac) const=0;
        /// Returns a boolean indicating whether a point in space is inside or outside of the rigid block 
        virtual bool                    getIsIn(const DVect &) const = 0;


        /// Return the number of facets.
        virtual int                     getNumberFacets() const=0;
        /// Return the number of vertices.
        virtual int                     getNumberVertices() const=0;
        /// Return the facets adjacent to this vertex
        virtual void                    getFacesAdjacentToVertex(quint16 v,FArray<quint16> *ret) const=0;
        /// Return the IPiece pointer.
        virtual const IPiece *          getIPiece() const = 0;
        virtual IPiece *                getIPiece() = 0;
        /// Return the IBody pointer.
        virtual const IBody *           getIBody() const = 0;
        virtual IBody *                 getIBody() = 0;
        /// Return the IBodyMechanical pointer.
        virtual const IBodyMechanical * getIBodyMechanical() const = 0;
        virtual IBodyMechanical *       getIBodyMechanical() = 0;
        /// Return the IRigidBodyMechanical pointer.
        virtual const IRigidBodyMechanical *    getIRigidBodyMechanical() const = 0;
        virtual IRigidBodyMechanical *          getIRigidBodyMechanical() = 0;
        /// Return the inertia tensor in principal axis form.
        virtual DAVect                    getMoi() const=0;
        /// Return the inertial (may be scaled) inertia tensor in principal axis form.
        virtual DAVect                    getInertialMoi() const=0;
        /// Return a component of the inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D
        virtual double                    getMoiComp(int) const = 0;
        /// Return a component of the inertial (may be scaled) inertia tensor. In 2D just the value is returned. In 3D the components are returned. 
        /// Admissible values are: 11,12,13,22,23,33 in 3D
        virtual double                    getInertialMoiComp(int) const = 0;

#ifdef THREED
        /// Return the inertia tensor. 
        virtual SymTensor                 getInertialMoiTensor() const = 0;
        virtual SymTensor                 getMoiTensor() const = 0;
#endif
        /// Set a component of the inertia tensor. Admissible values are: 11,12,13,22,23,33 in 3D
        virtual void                      setMoiComp(const double &,int) = 0;

        /// Set the principal values of the inertia tensor directly.
        virtual void                    setMoi(const DAVect &d)=0;

        /// When setting the MOI from FISH one needs make sure that it is not reset no matter what
        virtual void                        setMoiFix(bool b)=0;
        virtual bool                        isMoiFix()=0;
        virtual bool                        isMoiFix() const=0;

        /// Set the volume without scaling the rblock
        virtual void                        setVolumeNoScale(const double &d)=0;

        /// Return a list of IContacts.
        virtual void getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0)=0;
        /// Return a list of const IContacts.
        virtual void getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const=0;
        /// Return the number of contacts. 
        virtual quint64     getContactCount(bool active,const TType &type=0,bool domainToo = false) const=0;

        /// Return the current stress computed for this rblock
        virtual DMatrix<DIM,DIM>             getStress() const=0;

        /// Rotate the rblock about the axis \a axis through the rotation point \a rotp by the angle \a w (in radians).
        /// If orientation tracking is enabled then it is updated when the \a update boolean is set to true
        virtual bool rotate(const DVect &rotp,const DVect &axis,const double &w,bool update=true,bool noThrow = false) = 0;
        /// Scale the rblock to have this area or volume.
        virtual bool scale(const double &)=0;
        /// Get the rotation matrix with reference to the principal axis system
        virtual void getSurfaceRotMatrix(DMatrix<dim,dim> *) const=0;

        /// Return a boolean indicating the fixity condition.
        virtual bool            getFix(quint32 dof) const=0;
        /// Set the fixity condition.
        virtual void            setFix(quint32 dof,bool b)=0;
        /// Get the volume
        virtual double          getVolume() const=0;
        /// Return the local damping coefficient.
        virtual double          getLocalDamping() const=0;
        /// Set the local damping coefficient.
        virtual void            setLocalDamping(const double &d)=0;
        /// Return the density.    
        virtual double          getDensity() const=0;
        /// Set the density.
        virtual void            setDensity(const double &d)=0;
        /// Return the inertial mass.
        virtual double          getInertialMass() const=0;
        /// Return the mass.
        virtual double          getMass() const=0;
        /// Return the fragment index. 
        virtual int             getFragmentIndex() const=0;
        /// Set the fragment index.
        virtual void            setFragmentIndex(int ind)=0;
        /// Get the centroid
        virtual DVect           getCentroid() const=0;
        /// Set the centroid.
        virtual void            setCentroid(const DVect &p)=0;
        /// Get the displacement.
        virtual DVect           getDisplacement() const=0;
        /// Set the displacement.
        virtual void            setDisplacement(const DVect &v)=0;
#ifdef TWOD
        /// Get the angular displacement.
        virtual double            getAngDisplacement() const=0;
        /// Set the angular displacement.
        virtual void              setAngDisplacement(const double &v)=0;
#endif
        /// Get the velocity.
        virtual DVect             getVelocity() const=0;
        /// Set the velocity.
        virtual void              setVelocity(const DVect &p)=0;
        /// Get the angular velocity.
        virtual DAVect            getAngVelocity() const=0;
        /// Set the angular velocity. 
        virtual void              setAngVelocity(const DAVect &p)=0;
        /// Return the applied force.
        virtual DVect           getAppliedForce() const=0;
        /// Set the applied force.
        virtual void            setAppliedForce(const DVect &v)=0;
        /// Return the applied moment.
        virtual DAVect          getAppliedMoment() const=0;
        /// Set the applied moment.
        virtual void            setAppliedMoment(const DAVect &v)=0;
        /// Return the resultant of all contact forces.
        virtual DVect           getContactForce() const=0;
        /// Set the contact force.
        virtual void            setContactForce(const DVect &v)=0;
        /// Return the unbalanced force.
        virtual DVect           getUnbalancedForce() const=0;
        /// Return the unbalanced moment.
        virtual DAVect          getUnbalancedMoment() const=0;
        /// Return the resultant of all contact moments.
        virtual DAVect          getContactMoment() const=0;
        /// Set the contact moment.
        virtual void            setContactMoment(const DAVect &v)=0;

        /// Return the rblock position. This is implementation dependent.
        virtual DVect            getPosition() const = 0;
        /// Return the rounding 
        virtual double           getRounding() const = 0;
#ifdef THREED
        virtual DVect             getEulerFromOrient() const=0;
        virtual void              setOrientFromEuler(const DVect3 &d)=0;
#endif

        /// Support for face groups
        virtual QString           getFaceGroupName(uint side,const ISlotID &slot=ISlotID()) const=0;

    };
} // namespace rblock
// EoF

