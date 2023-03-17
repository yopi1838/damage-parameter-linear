#pragma once
// iinter.h

/**
  * \file
  * \brief Interface to FLAC3D's interface constitutive model.
  */

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace zone
{
  using namespace itasca;

  class IGp;
  class IIntNode;
  class IIntElem;

  /// \brief Class provides access to the an interface 
  /** An interface is used to keep track of nodes and elements
  */
  /// \ingroup Zone
  class IInterface
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a14;
    /// returns a const IThing pointer
    virtual const IThing *          getIThing() const=0;
    /// returns a IThing pointer
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer the next interface in the global interface list, or 0 to indicate the end of the list.\n
    /// See IInterfaceList::getFirstInterface();
    virtual const IInterface *      getNext() const=0;
    /// Returns a pointer the next interface in the global interface list, or 0 to indicate the end of the list.\n
    /// See IInterfaceList::getFirstInterface();
    virtual IInterface *            getNext()=0;
    /// Return a const pointer to the first interface node stored.\n
    /// IIntNode::getNext() can be used to iterate on the list.
    virtual const IIntNode *        getFirstINode() const=0;
    /// Return a pointer to the first interface node stored.\n
    /// IIntNode::getNext() can be used to iterate on the list.
    virtual IIntNode *              getFirstINode()=0;
    /// Return a const pointer to the first interface element stored.\n
    /// IIntElem::getNext() can be used to iterate on the list.
    virtual const IIntElem *        getFirstIntElement() const=0;
    /// Return a pointer to the first interface element stored.\n
    /// IIntElem::getNext() can be used to iterate on the list.
    virtual IIntElem *              getFirstIntElement()=0;
    /// Given a interface element \b id, returns a const pointer to the interface element, or 0 if \b id does not exist.
    virtual const IIntElem *        getElementWithID(quint64 id) const=0;
    /// Given a interface element \b id, returns a pointer to the interface element, or 0 if \b id does not exist.
    virtual IIntElem *              getElementWithID(quint64 id)=0;
    /// Given a interface node \b id, returns a const pointer to the interface node, or 0 if \b id does not exist.
    virtual const IIntNode *        getNodeWithID(quint64 Id) const=0;
    /// Given a interface node \b id, returns a pointer to the interface node, or 0 if \b id does not exist.
    virtual IIntNode *              getNodeWithID(quint64 Id)=0;
    /// Returns TRUE if the zone with id \a id is either attached to or a target of the interface.
    virtual bool                    isZoneOnInterface(quint64 id) const=0;
    /// Returns TRUE if the face with zone \a id and side \a face is either attached to or a target of the interface.
    virtual bool                    isFaceOnInterface(quint64 zone,quint32 face) const=0;
    /// Returns TRUE if the gp is attached to a face that is considered on the interface (via isFaceOnInterface).
    virtual bool                    isGpOnInterface(const IGp *gp) const=0;
    /// Returns the IContainer interface to the list of nodes associated with this interface
    virtual IContainer *            getNodeList()=0;
    /// Returns the IContainer interface to the list of nodes associated with this interface
    virtual const IContainer *      getNodeList() const=0;
    /// Returns the IContainer interface to the list of elements associated with this interface
    virtual IContainer *            getElemList()=0;
    /// Returns the IContainer interface to the list of elements associated with this interface
    virtual const IContainer *      getElemList() const=0;
  };

} // namespace zone
// EoF
