#pragma once
// isubcontactarray.h

/**
 *  \file
 *  \brief container to store SubcontactThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class ISubcontactThing;

  /// \brief Container to store SubcontactThings.
  ///  ID of SubcontactThing is stored in offset $KCXCPP in Fortran subcontac linked list
  /// \ingroup THREEDEC
  class ISubcontactArray
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
    /// Returns a const pointer to the subcontact, ISubcontactThing, with the ID \b id, or 0 if the contact does not exist.
    virtual const ISubcontactThing *   findSubcontactWithID(quint64 id) const=0;
    /// Returns a pointer to the subcontact, ISubcontactThing, with the ID \b id, or 0 if the contact does not exist.
    virtual ISubcontactThing *         findSubcontactWithID(quint64 id)=0;
    ///// Return subcontact closest to input location \b loc.
    //virtual const SubcontactThing *  findSubcontact(const DVect &loc) const = 0;
    /// Returns the subcontact type identifier
    virtual quint32            getSubcontactType() const=0;
    /// Returns number of subcontacts in the container
    virtual quint64                getNumSubcontacts() const=0;
    /// Returns tag for group change notice
    virtual uint     getGroupNotice() const=0;
    /// Returns the tag for the subcontact extra variable change notice
    virtual uint      getSubcontactExtraNotice() const=0;
    /// Returns the tag for the subcontact model change notice
    virtual uint                getModelNotice() const=0;
    /// Returns the tag for pore pressure change notice
    virtual uint     getPorePressureNotice() const=0;
    /// Returns subcontact within tolerance of input subcontact.
    /// Default tolerance is 0.1 * atol
    virtual const ISubcontactThing* findPartner(const ISubcontactThing* cx, double tol = -1) const=0;
  };

} // end namespace block
// EOF