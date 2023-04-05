#pragma once
// ivertexarray.h
#include "base/src/farray.h"

/**
 * \file
 * \brief container to store VertexThings.
 *
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IVertexThing;
  class VertexThing;
  class IGpData;


  /// \brief Container to store VertexThings.
  ///  ID of VertexThing is stored in offset $KVCPP in Fortran vertex linked list
  /// \ingroup THREEDEC
  class IVertexArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IVertexArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IVertexArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of grid points.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of grid points.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the vertex, IVertexThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual const IVertexThing *      findVertexWithID(quint64 id) const=0;
    /// Returns a pointer to the vertex, IVertexThing, with the ID \b id, or 0 if the vertex does not exist.
    virtual IVertexThing *            findVertexWithID(quint64 id)=0;
    /// Returns the vertex type identifier
    virtual quint32            getVertexType() const=0;
    /// Returns number of vertices in the container
    virtual quint64           getNumVertices() const=0;
    /// Return vertex nearest to input location
    virtual const IVertexThing *      findVertex(const DVect3 &loc, bool visible=false) =0;

    /// return array of vertices withing distance radius of inpult location.
    virtual FArray<IVertexThing*> findAllVertices(const DVect3& loc, double radius, bool visible = false)=0;

    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimBit() returns a bit index that can be used with the IVertexThing::getBit() and
    ///   IVertexThing::setBit() functions.
    /// This function is thread-safe (IVertexThing::getBit and IVertexThing::setBit are not).
    /// \sa releaseBit() IVertexThing::setBit() IVertexThing::getBit()
    /// \brief Claims a bit from the vertex utility bit field.
    virtual int                claimBit() const=0;
    /// \brief Releases a bit index previously obtained with claimBit().  Thread-safe.
    virtual void               releaseBit(int bit) const=0;
    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimValue() returns a value index that can be used with the IVertexThing::getValue() and
    ///   IVertexThing::setValue() functions.
    /// This function is thread-safe (IVertexThing::getValue and IVertexThing::setValue are not).
    /// \sa releaseBit() IVertexThing::setValue() IVertexThing::getValue()
    /// \brief Claims a bit from the zones utility value array.
    virtual int                claimValue() const=0;
    /// \brief Releases a value index back to the zones utility value array.  Thread-safe.
    virtual void               releaseValue(int bit) const=0;
    /// Returns a pointer to a new IGpData object. This pointer must be destroyed by the caller.
    virtual IGpData *          createIGpData(QObject *parent=0) const=0;
    /// Returns the tag for the vertex extra variable change notice
    virtual uint               getVertexExtraNotice() const=0;
    /// Returns the tag for the vertex displacement change notice
    virtual uint      getDisplacementNotice() const=0;
    /// Returns the tag for the vertex velocity change notice
    virtual uint      getVelocityNotice() const=0;
    /// Returns the tag for the vertex pore pressure change notice
    virtual uint      getPorePressureNotice() const=0;
    /// Returns the tag for the vertex temperature change notice
    virtual uint      getTemperatureNotice() const=0;
    /// Returns the tag for vertex geometry change notice
    virtual uint      getGeometryNotice() const=0;
    /// Returns the tag for vertex boundary condition change notice
    virtual uint      getBoundaryNotice() const=0;
  };

} // end namespace block
// EOF