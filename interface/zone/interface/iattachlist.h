//iattachlist.h
#pragma once

#include "iattach.h"

/**
  * \file
  * \brief Interface for accessing the global list of attachments
  */
namespace itasca {
    class IContainer;
    class IThing;
}

namespace zone {
    /// \brief Interface for accessing the global list of attachments, IAttachPoint
    /// \ingroup Zone
    class IAttachList {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4e5d11ef;

        /// returns a const IThing pointer
        virtual const IThing *            getIThing() const = 0;
        /// returns a IThing pointer
        virtual IThing *                  getIThing() = 0;
        /// Returns a const pointer to the IContainer interface representing the global list of attachments.
        virtual const IContainer *        getIContainer() const = 0;
        /// Returns a pointer to the IContainer interface representing the global list of attachments.
        virtual IContainer *              getIContainer() = 0;
        /// Returns the attachment type for the TypeEntry system
        virtual quint32                   getAttachType() const = 0;
        /// Returns a const pointer to the first attachment, IAttachPoint, in the list, or 0 if list is empty.
        /// Use in conjunction with IAttachPoint::getNext();
        virtual const IAttachPoint *      getFirstAttach() const = 0;
        /// Returns a pointer to the first attachment, IAttachPoint, in the list, or 0 if list is empty.\n
        /// Use in conjunction with IAttachPoint::getNext();
        virtual IAttachPoint *            getFirstAttach() = 0;
        /// Returns a pointer to the attachment, IAttachPoint, that matches \b id.\n
        virtual const IAttachPoint *      findAttachWithID(quint64 id) const = 0;
        /// Returns the notification code for attachments
        virtual uint                      getAttachPointNotice() const = 0;
        /// Creates an attachment
        /// \param type is the attachment type (gridpoint, edge or face)
        /// \param slave is the the slave gridpoint
        /// \param it1,it2,faceNo For attachment type gridpoint <b>it1</b> is the master gridpoint. Parameters <b>it2</b> and <b>faceNo</b> are not used.
        /// For type edge, <b>it1</b> and <b>it2</b> are the gridpoint pointers that form of the edge. Parameter <b>faceNo</b> is not used.
        /// For type face, <b>it1</b> is the zone pointer, <b>it2</b> is not used. <b>faceNo</b> is the face number, valid values are 0 to 5.
        /// \param snap see ATTACH command reference for behavior.
        virtual IAttachPoint            * createAttach(IAttachPoint::ClassType type,IGp *slave,IThing *it1,IThing *it2, quint32 faceNo, bool snap) = 0;
    };
} // namespace zone
// EoF
