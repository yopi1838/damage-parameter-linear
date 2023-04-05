#pragma once
// ipieceextension.h

// Interface for functionality common to all piece extensions.

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IPieceExtension {
    public:
        // Piece extension attributes
        virtual bool          setAttribute(const QString &name,const QVariant &v)=0;
        virtual QVariant      getAttribute(const QString &name) const=0;
        virtual QStringList   getAllAttributes() const=0;
        virtual bool          isAttribute(const QString &name) const=0;

    };
} // namespace itascaxd

// EoF
