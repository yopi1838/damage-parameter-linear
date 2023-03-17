#pragma once

/**
  * \file
  * \brief Particle tracking logic
  */

#include "base/src/base.h"
#include "base/src/farray.h"

namespace itasca
{
  class IThing;
  class IGroup;
}

namespace zone
{
  using namespace itasca;

  class ICParticle;

  /// This class represents a single particle in the tracking system.
  /// ICParticleList contains all such particles current being tracked.
  /// \brief A particle being tracked.
  /// \ingroup Zone
  class ICParticle
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x4c815a0e;
    /// Returns a const IThing pointer
    virtual const IThing *    getIThing() const=0;
    /// Returns a IThing pointer
    virtual IThing *          getIThing()=0;
    /// Returns a const pointer to the next IParticle object in the CParticle list, or 0 to indicate the end of the list. 
    /// See ICParticleList::getFirst()
    virtual const ICParticle *getNext() const=0;
    /// Returns the current position of the particle
    virtual DVect3            getLocation() const=0;

    /// This funcion sets the last position saved for the particle.
    /// \arg dTime is the accumulated fluid time associated with this position.
    /// \arg dvCrd is the position of the particle.
    /// \arg dvVel is the velocity of the particle.
    virtual void setLastPt(const double &dTime, const DVect3 &dvCrd, const DVect3 &dvVel) = 0;

    /// This funcion gets the last position saved for the particle.
    /// This could have been set by a FISH function, or by the tracking logic.
    /// \arg dTime is the accumulated fluid time associated with this position.
    /// \arg dvCrd is the position of the particle.
    /// \arg dvVel is the velocity of the particle.
    virtual void getLastPt(double *dTime, DVect3 *dvCrd, DVect3 *dvVel) const= 0;

    /// This funcion populates a given vector with the positions of the trajectory of 
    /// the particle in the fluid.
    virtual void getParticlePath(FArray<DVect3> *path) const= 0;

  };    

  /// FLAC3D can track the positions of particles moving with the fluid flow.\n
  /// These particles can be placed in space, and their positions are automatically
  ///   updated periodically during cycling to move with the fluid.\n
  /// This class is a list of all such particles being tracked.
  /// \brief The list of all particles being tracked.
  /// \ingroup Zone
  class ICParticleList
  {
  public:
    /// Returns the particle type (IThing getType() return).
    virtual quint32              getCParticleType() const=0;
    /// Returns a const pointer to the first IParticle object in the list, see ICParticle::getNext()
    virtual const ICParticle *   getFirst() const=0;
    /// Returns a pointer to the first IParticle object in the list, see ICParticle::getNext()
    virtual ICParticle *         getFirst()=0;
    /// Returns a const pointer to the IParticle object in the list whose ID matches the parameter \b id
    virtual const ICParticle *   findCParticleWithID(quint64 id) const=0;
    /// Returns a notification code (see INoticeManager) indicating particle geomtry has changed\n
    /// This could mean particles have been moved, or that particles have been created or destroyed.
    virtual uint                 getGeometryNotice() const=0;
    /// Returns a notification code (see INoticeManager) indicating that the group assignments of
    ///  particles has changed.
    virtual uint                 getGroupNotice() const=0;
  };          
} // namespace zone
// EoF
