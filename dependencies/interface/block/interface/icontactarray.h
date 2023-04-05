#pragma once
// icontactarray.h

/**
 * \file
 * \brief Container to store ContactThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IContactThing;

  /// \brief This interface provides access to contact thing array.
  /// \ingroup THREEDEC
  class IContactArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IContactArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IContactArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of contacts.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of contacts.
    virtual IContainer *            getIContainer() =0;
    /// Returns a const pointer to the contact, IContactThing, with the ID \b id, or 0 if the contact does not exist.
    virtual const IContactThing *   findContactWithID(quint64 id) const=0;
    /// Returns a pointer to the contact, IContactThing, with the ID \b id, or 0 if the contact does not exist.
    virtual IContactThing *         findContactWithID(quint64 id)=0;
    /// Returns the contact type identifier
    virtual quint32                 getContactType() const=0;
    /// Returns number of contacts in the container
    virtual quint64                getNumContacts() const=0;
    /// Return contact closest to input location
    virtual IContactThing *     findContact(const DVect3 &dv)=0;

    /// Returns the tag for the contact extra variable change notice
    virtual uint      getContactExtraNotice() const=0;
    /// Returns tag for group change notice
    virtual uint     getGroupNotice() const=0;
    /// Returns the tag for model change notice
    virtual uint     getModelNotice() const=0;
    /// Returns the tag for material change notice
    virtual uint     getMaterialNotice() const=0;
  };

} // end namespace block
// EOF