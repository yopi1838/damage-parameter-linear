#pragma once
// ifaceaarray.h

/**
 *  \file
 *  \brief Container to store block faces POST-zoning or triangulation
 *
 */

#include "iblockdef.h"

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IFaceThing;
  class IFaceData;

  /// \brief Container to store FaceThings (zoned or triangulated block faces).
  ///  ID of FaceThing is stored in offset $KFCPP in Fortran block linked list
  /// \ingroup THREEDEC
  class IFaceArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IFaceArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IFaceArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of faces.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of faces.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the face, IFace, with the ID \b id, or 0 if the face does not exist.
    virtual const IFaceThing *      findFaceWithID(quint64 id) const=0;
    /// Returns a pointer to the face, IFace, with the ID \b id, or 0 if the face does not exist.
    virtual IFaceThing *            findFaceWithID(quint64 id)=0;
    /// Returns the face type identifier
    virtual quint32            getFaceType() const=0;
    /// Returns the tag for the face extra variable change notice
    virtual uint               getFaceExtraNotice() const=0;
    /// Returns the tag for the face group change notice
    virtual uint               getGroupNotice() const=0;

    virtual IFaceData* createIFaceData(QObject* parent = 0) const = 0;
  };

} // end namespace block
// EOF