#pragma once
// isurface.h

/**
  * \file
  * \brief Interface to provide access to surfaces, see command GEN SURFACE.
  */

namespace itasca
{
  class IThing;
}

namespace zone
{
  /// \brief Interface to provide access to surfaces, see command GEN SURFACE.
  /// \ingroup Zone
  class ISurface
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a2c;
    /// returns a const IThing pointer
    virtual const IThing    *   getIThing() const=0;
    /// returns a IThing pointer
    virtual IThing          *   getIThing()=0;
    /// Returns a const pointer to the next surface object in the global list of surfaces, 
    /// or 0 for the last element of the list. See ISurfaceList::getFirstSurface()  
    virtual const ISurface  *   getNext() const=0;
    /// Returns a pointer to the next surface object in the global list of surfaces, 
    /// or 0 for the last element of the list. See ISurfaceList::getFirstSurface()  
    virtual ISurface        *   getNext()=0;
    /// returns the volume ID that a surface belongs to
    virtual quint64            getVolumeID() const=0;
    /// given a node number, return the node position
    virtual  DVect3             getNodePosition(quint32 no) const= 0;
    /// return the name of collection of triangles if there is a collection
    virtual  QString            getName() const = 0;
    /// returns the surface normal vector
    virtual  DVect3             getNormal() const = 0;
    /// given a edge number, return the join surface
    virtual  const ISurface *   getJoinSurface(quint32 no) const = 0;
  };
} // namespace zone
// EoF

