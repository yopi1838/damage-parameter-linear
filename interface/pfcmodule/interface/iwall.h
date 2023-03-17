#pragma once
/** \file iwall.h
    * \brief Interface for a generic wall.
    *
    * \addtogroup wallsrc
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
    class IContact;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    /** \brief Interface for a generic wall.
        * This is the interface for a generic wall. Specific wall types must derive from this.
        * A wall is meant to be some geometric object with which other model components (balls and clumps)
        * can interact but walls do not react. Thus they act to provide boundary conditions. 
        * This interface provides basic functionality for accessing the Body and Pieces associated
        * with a wall. The properties that can be accessed through this interface are the position,
        * rotation point, velocity, angular velocity, displacement, angular displacement, force, and 
        * moment. The meaning of the position is implementation dependent. All of these properties
        * can be set with the exception of the force and moment.
        * \ingroup wallsrc
        */
    class IWall {
    public:
        /// TType for type casting.
        inline static const TType type_ = 0x4cd817c3;

        /// Return an IThing interface for this object.
        virtual IThing *                getIThing() = 0;
        /// Return a const IThing interface for this object.
        virtual const IThing *          getIThing() const = 0;
        /// Return the next IWall object in the global list, or 0 if it is the last set.
        virtual IWall *                 getNext() = 0;
        /// Return the next const IWall object in the global list, or 0 if it is the last set.
        virtual const IWall *           getNext() const = 0;
        /// Return the first IPiece in this wall.
        virtual IPiece *                getFirstIPiece() = 0;
        /// Return the first const IPiece in this wall.
        virtual const IPiece *          getFirstIPiece() const = 0;
        /// Return the first IPieceMechanical in this wall.
        virtual IPieceMechanical *      getFirstIPieceMechanical() = 0;
        /// Return the first const IPieceMechanical in this wall.
        virtual const IPieceMechanical *getFirstIPieceMechanical() const = 0;
        /// Return an IBody pointer. 
        virtual IBody *                 getIBody() = 0;
        /// Return a const IBody pointer.
        virtual const IBody *           getIBody() const = 0;
        /// Boolean to denote whether or not this is a faceted wall
        virtual bool isFWall() const = 0;
        /// Return a unique QStringList for each wall
        //virtual QStringList getAllPieceProperties() const = 0;

        /// Return the wall position. This is implementation dependent.
        virtual DVect            getPosition() const = 0;
        /// Return the center of rotation about which a wall can rotate.
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
        /// Return the moment of all contacts with with wall.
        virtual DAVect           getMoment() const = 0;
        /// Return the current orientation
        virtual Quat             getOrient() const=0;

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
        /// Set the orientation
        virtual void setOrient(const Quat &q)=0;
#ifdef THREED
        /// Set the orientation from euler angles
        virtual void setOrientFromEuler(const DVect3 &d)=0;
#endif

        /// Rotate the wall about the axis \a axis through the rotation point \a rotp by the angle \a w (in radians).
        /// If orientation tracking is enabled then it is updated when the \a update boolean is set to true
        virtual void rotate(const DVect &rotp,const DVect &axis,const double &w,bool update=true,bool noThrow = false) = 0;

        /// Set the properties in \a prop to \a v for all PieceMechanicals that are in this wall.
        virtual bool  setProperty(const QString &name,const QVariant &v)=0;
        /// Return a list of IContacts.
        virtual void getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0)=0;
        /// Return a list of const IContacts.
        virtual void getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const=0;
        /// Return the number of contacts. 
        virtual quint64     getContactCount(bool active,const TType &type=0,bool domainToo = false) const=0;
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