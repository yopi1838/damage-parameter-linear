#pragma once
// ipiecefluid.h

// Interface for functionality common to all fluid pieces.
// They all define surface properties, used by fluid contacts.
// They also store a pointer to a mechanical piece

namespace itascaxd {
    class IPiece;
    class IPieceMechanical;

    class IPieceFluid {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4fe8e8a0;
        virtual const IPiece *           getIPiece() const=0;
        virtual IPiece *                 getIPiece()=0;
        virtual const IPieceMechanical * getIPieceMechanical() const=0;
        virtual IPieceMechanical *       getIPieceMechanical()=0;
    };
} // namespace itascaxd

// EoF
