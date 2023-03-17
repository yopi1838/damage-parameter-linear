//isurfacelist.h
#pragma once

/**
  * \file
  * \brief Interface to the global list of all surfaces, see command GEN SURFACE.
  */


namespace itasca
{
  class IContainer;
}

namespace zone
{
  class ISurface;

  /// \brief Interface to the global list of all surfaces, see command GEN SURFACE.
  /// \ingroup Zone
  class ISurfaceList
  {
  public:
    /// Returns a const pointer to the IContainer interface representing the global list of surfaces.
    virtual const IContainer  * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of surfaces.
    virtual IContainer        * getIContainer() =0;
    /// Return a const pointer to the first surface stored.\n
    /// ISurface::getNext() can be used to iterate on the list.
    virtual const ISurface    * getFirstSurface() const=0;
    /// Return a pointer to the first surface stored.\n
    /// ISurface::getNext() can be used to iterate on the list.
    virtual ISurface          * getFirstSurface()=0;
    /// Given a surface \b id, returns a const pointer to the surface object.
    virtual const ISurface    * findSurfaceWithID(quint64 id) const=0;
    /// Given a surface \b id, returns a pointer to the surface object.
    virtual ISurface          * findSurfaceWithID(quint64 id)=0;
    /// Returns the general surface type.
    virtual quint32             getSurfaceType() const=0;
  };
} // namespace zone
// EoF

