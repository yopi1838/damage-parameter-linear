#pragma once
// ipiecemechanical.h

// Interface for functionality common to all mechanical pieces.
// They all define surface properties, used by mechanical surface contacts.
// They also all need to define a velocity.

namespace itascaxd {
    class IPiece;

    class IPieceMechanical {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b91;
        virtual const IPiece *    getIPiece() const=0;
        virtual IPiece *          getIPiece()=0;
        virtual DVect             getVelocity() const=0;
        virtual DAVect            getAngVelocity() const=0;
        virtual DVect             getPosition() const=0;
        virtual DMatrix<dim,dim>  getUnscaledStress(const DVect &) const=0;
        virtual bool              isBonded(const TType &type=0,const IPiece *p2=0) const = 0;
    };
} // namespace itascaxd

// EoF
