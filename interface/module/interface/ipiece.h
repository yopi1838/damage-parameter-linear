#pragma once
// ipiece.h

// Interface for Piece

#include "dim.h"
#include "base/src/farray.h"

namespace itascaxd {
    using namespace itasca;
    class IContact;
    class IBody;

    /// \ingroup module
    class IPiece {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b8f;
        // Returns an IThing interface
        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0;

        // Returns a list of contacts connected to this piece.
        // If type is specified, restricts to contacts matching that type.
        // If p2 is specified, restricts to contacts with that piece.
        // NOTE: We will be able to do "MOVE COPY" semantics here in VS2010.
        virtual void getContactList(FArray<IContact *> *ret,const TType &type=0,const IPiece *p2=0)=0;
        virtual void getContactList(FArray<const IContact *> *ret,const TType &type=0,const IPiece *p2=0) const=0;

        // Return the count of contacts, active and inactive
        virtual quint64 getContactCount(bool active,const TType &type=0,bool domainToo = false) const = 0;

        // Returns TRUE if a matching contact exists to this piece.
        // If type is specified, contact must match that type.
        // If p2 is specified, contact must be with that piece.
        virtual bool hasContact(const TType &type=0,const IPiece *p2=0) const=0;

        // Get a pointer to the body to which this Piece belongs
        virtual IBody *      getIBody()=0;
        virtual const IBody *getIBody() const=0;

        // Get a pointer to the IThing used for interactions
        virtual IThing *          getIThingForInteraction()=0;
        virtual const IThing *    getIThingForInteraction() const=0;

        // Get next IPiece in the LOCAL list of Pieces associated with this Body
        virtual IPiece *      getNextPiece()=0;
        virtual const IPiece *getNextPiece() const=0;

        // Piece properties
        virtual bool              setProperty(const QString &name,const QVariant &v)=0;
        virtual QVariant          getProperty(const QString &name) const=0;
        virtual QStringList       getAllProperties() const=0;
        virtual bool              isProperty(const QString &name) const=0;

        // Piece attributes
        virtual bool          setAttribute(const QString &name,const QVariant &v)=0;
        virtual QVariant      getAttribute(const QString &name) const=0;
        virtual QStringList   getAllAttributes() const=0;
        virtual bool          isAttribute(const QString &name) const=0;

        // Get the ultimate derived type
        virtual TType getDerivedType() const=0;


    };

} // namespace itascaxd
// EoF
