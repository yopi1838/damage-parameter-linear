#pragma once
// iselprop.h

#include "iblockdef.h"

/**
 * \file
 *  \brief This is the interface for SelProp, a C++ wrapper for structural element properties.
*/

/*! \brief namespace Itasca
 *
 */
namespace itasca
{
  class IThing;
}

namespace block
{
  /// \brief This interface provides access to cable thing.
  ///  The fortran offset for the SelProp ID is $ktypcpp in the Fortran sel property data array
    /// This will undoubtedly cause much confusion since there is a material number that is set
    /// by the user, and also now an id that is set when this SelProp thing is added to the C++ array.

  class ISelProp
  {
  public:
      /// type used in IThing typing system
      inline static const TType type_ = 0x56783351;
      /// Returns a const IThing pointer
      virtual const itasca::IThing *      getIThing() const=0;
      /// Returns a IThing pointer
      virtual itasca::IThing *            getIThing()=0;
      /// Return node fortran offset. 
      virtual FInt getFortranOffset() const=0;
      /// Get material number
      virtual quint64 getMaterialNumber() const=0;
      /// Get elastic modulus
      virtual double getElasticModulus() const=0;
      /// Set elastic modulus
      virtual void setElasticModulus(double d)=0;
      /// Get thermal expansion coefficient
      virtual double getThExp() const=0;
      /// Set thermal expansion coefficient
      virtual void setThExp(double d)=0;
      /// Get density
      virtual double getDensity() const=0;
      /// Set density
      virtual void setDensity(double d)=0;

      /// beams and cables

      /// Get tensile yield capacity
      virtual double getTensileStrength() const=0;
      /// Set tensile yield capacity
      virtual void setTensileStrength(double d)=0;
      /// Get compressive yield strength
      virtual double getCompressiveStrength() const=0;
      /// Set compressive yield strength
      virtual void setCompressiveStrength(double d)=0;
      /// Get tensile rupture strain limit
      virtual double getTensileStrainLimit() const=0;
      /// Set tensile rupture strain limit
      virtual void setTensileStrainLimit(double d)=0;
      /// Get cross-sectional area
      virtual double getArea() const = 0;
      /// Set cable cross-sectional area
      virtual void setArea(double d) = 0;

      /// beams and liners

      /// Get normal stiffness (force/disp) of sel/rock interface
      virtual double getSelRockKn() const=0;
      /// Set normal stiffness (force/disp) of sel/rock interface
      virtual void setSelRockKn(double d)=0;
      /// Get shear stiffness (force/disp) of sel/rock interface
      virtual double getSelRockKs() const=0;
      /// Set shear stiffness (force/disp) of sel/rock interface
      virtual void setSelRockKs(double d)=0;
      /// Get tensile capacity (force) of sel/rock interface
      virtual double getSelRockTensileStrength() const=0;
      /// Set tensile capacity (force) of sel/rock interface
      virtual void setSelRockTensileStrength(double d)=0;
      /// Get cohesive capacity (force) of sel/rock interface
      virtual double getSelRockCohesion() const=0;
      /// Set cohesive capacity (force) of sel/rock interface
      virtual void setSelRockCohesion(double d)=0;
      /// Get friction coefficient of sel/rock interface
      virtual double getSelRockMu() const=0;
      /// Set friction coefficient of sel/rock interface
      virtual void setSelRockMu(double d)=0;

      /// cables

      /// Get cable grout stiffness
      virtual double getGroutStiffness() const=0;
      /// Set cable grout stiffness
      virtual void setGroutStiffness(double d)=0;
      /// Get cable grout cohesion
      virtual double getGroutCohesion() const=0;
      /// Set cable grout cohesion
      virtual void setGroutCohesion(double d)=0;
      /// Get cable grout friction coefficient
      virtual double getGroutMu() const=0;
      /// Set cable grout friction coefficient
      virtual void setGroutMu(double d)=0;
      /// Get hole perimeter
      virtual double getHolePerimeter() const=0;
      /// Set hole perimeter
      virtual void setHolePerimeter(double d)=0;
      /// Get dowel shear stiffness
      virtual double getDowelStiffness() const=0;
      /// Set dowel shear stiffness
      virtual void setDowelStiffness(double d)=0;
      /// Get dowel shear strength
      virtual double getDowelStrength() const=0;
      /// Set dowel shear strength
      virtual void setDowelStrength(double d)=0;
      /// Get dowel rupture shear strain
      virtual double getDowelStrainLimit() const=0;
      /// Set dowel rupture shear strain
      virtual void setDowelStrainLimit(double d)=0;

      /// liners

      /// Get liner thickness
      virtual double getLinerThickness() const=0;
      /// Set liner thickness
      virtual void setLinerThickness(double d)=0;
      /// Get liner Poisson's ratio
      virtual double getLinerPoissonsRatio() const=0;
      /// Set liner Poisson's ratio
      virtual void setLinerPoissonsRatio(double d)=0;

      /// beams
      
      /// Get beam bending inertia isotropic
      virtual double getMOI_iso() const=0;
      /// Set beam bending inertia isotropic
      virtual void setMOI_iso(double d)=0;
      /// Get beam bending inertia about s1
      virtual double getBeamI1() const=0;
      /// Set beam bending inertia about s1
      virtual void setBeamI1(double d)=0;
      /// Get beam bending inertia about s2
      virtual double getBeamI2() const=0;
      /// Set beam bending inertia about s2
      virtual void setBeamI2(double d)=0;
      /// Get beam torsional intertia
      virtual double getBeamJ() const=0;
      /// Set beam torsional intertia
      virtual void setBeamJ(double d)=0;
      /// Get beam s1 direction
      virtual DVect3 getBeamS1() const=0;
      /// Set beam s1 direction
      virtual void setBeamS1(const DVect3 & dv)=0;

   };

 }

