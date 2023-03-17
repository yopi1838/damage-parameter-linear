#pragma once
// irigidbodymechanical.h

// Interface for functionality of all rigid mechanical bodies.

namespace itascaxd {
    class IRigidBodyMechanical {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4eb6dfcd;

#ifdef TWOD
        virtual double            getAngDisplacement() const=0;
#endif
        virtual DVect             getCentroid() const=0;
        virtual DVect             getVelocity() const=0;
        virtual DAVect            getAngVelocity() const=0;
        virtual DVect             getDisplacement() const=0;
        virtual double            getVolume() const=0;
        virtual Quat              getOrient() const=0;

#ifdef TWOD
        virtual void              setAngDisplacement(const double &v)=0;
#endif
        virtual void              setCentroid(const DVect &p)=0;
        virtual void              setVelocity(const DVect &p)=0;
        virtual void              setAngVelocity(const DAVect &p)=0;
        virtual void              setDisplacement(const DVect &v)=0;
        virtual void              setOrient(const Quat &q)=0;
#ifdef THREED
        virtual DVect             getEulerFromOrient() const=0;
        virtual void              setOrientFromEuler(const DVect3 &d)=0;
        virtual void              setOrientFromAAngle(const DVect3 &d)=0;
#endif

    };

} // namespace itascaxd
// EoF
