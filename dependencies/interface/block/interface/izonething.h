#pragma once
// izonething.h

/**
 * \file
 * \brief This is the interface for ZoneThing, a class that holds extra data not in the fortran arrays.
*/

#include "iblockdef.h"
#include "models/src/igenericzone.h"

namespace itasca
{
  class IThing;
}

namespace block
{

  class IVertexThing;
  class IBlockThing;
  class IFaceThing;

  /// \brief This is the interface for ZoneThing, a class that holds extra data not in the fortran arrays.
  ///           This will be stored in a container and the ID will be stored in
  ///           offset $KZCPP in the fortran zone linked list.
  ///           This inherits from Thing so we can use CellSpace
  /// \ingroup THREEDEC

  class IZoneThing : public models::IGenericZone
  {
  public:
    /// offset to obtain pointer to zonething ID
    static const FInt cpp   = 23; // C++ extension from Fortran array

    /// Returns a const IThing pointer
    virtual const itasca::IThing *      getIThing() const=0;
    /// Returns a IThing pointer
    virtual itasca::IThing *            getIThing()=0;
    /// type used in IThing typing system
    inline static const TType type_ = 0x4ede2fe7;
    /// Return fortran offset. Necessary for gui queries
    virtual FInt getFortranOffset() const=0;
    /// Return the zone ID
    virtual quint64 getZoneID() const = 0;
    /// Returns the boolean stored in utility bit \a bit in the zone.
    /// \arg bit must be a bit index previouly returned from IZoneArray::claimBit().
    virtual bool                getBit(int bit) const=0;
    /// Sets the boolean stored in utility bit \a bit in the zone.
    /// This function is const, to indicate that setting bit values does not impact the calculation
    ///   logic in any way.
    /// \arg bit must be a bit index previouly returned from IZoneArray::claimBit().
    /// Note - this is const through some hack so that we can use const ZoneThings when getting data
    virtual void                setBit(int bit,bool b) const=0;
    /// Retrieves the value stored in the utility array at \a index.
    /// \arg index must be a value previously returned from IZoneArray::claimValue().
    virtual QVariant            getValue(int index) const=0;
    /// Sets the value stored in the utility array at \a index.
    /// This is const to indicate that these bit values do not modify the model in any way.
    /// \arg index must be a value previously returned from IZoneArray::claimValue().
     /// Note - this is const through some hack so that we can use const ZoneThings when getting data
    virtual void                setValue(int index,const QVariant &v) const=0;
    /// Returns the number of vertices used by the zone.
    virtual quint32              getNumVertices() const=0;
    /// Returns the number of faces used by the zone.
    virtual quint32              getNumFaces() const=0;
    /// Returns a const pointer to the vertex with index \b index.
    /// \param index starts at 0, and must be lower than the number of gridpoint used by the zone, see getNumVertices()
    virtual const IVertexThing * getVertexThing(qint32 index) const=0;
    /// Returns the ID of the vertex with index \b index.
    /// \param index starts at 0, and must be lower than the number of gridpoint used by the zone, see getNumVertices()
    virtual quint64             getVertexThingID(qint32 index) const=0;
    /// Returns the fortran offset of the vertex with index \b index.
    /// \param index starts at 0, and must be lower than the number of gridpoint used by the zone, see getNumVertices()
    virtual FInt                getVertex(qint32 index) const=0;
    /// Returns the number of gridpoints used by the face of index \b face.
    /// \param face starts at 0, and must be lower than 4.
    virtual quint32             getFaceSize(quint32 face) const=0;
    /// Returns a const pointer to the gridpoint on face \b face and vertex \b index.
    /// \param face starts at 0, and must be lower than 4.
    /// \param index starts at 0, and must be lower than 3 at the moment.
    virtual const IVertexThing  * getFaceVertexThing(quint32 face,quint32 index) const=0;
    /// Returns the index of the gridpoint on face \a face and vertex \a index.
    /// \param face starts at 0, and must be lower than 4.
    /// \param index starts at 0, and must be lower than 3.
    virtual quint32             getFaceVertexIndex(quint32 face,quint32 index) const=0;
    /// Returns the unit normal vector to face \b face.
    /// \param face starts at 0, and must be lower than 4.
    /// This function is necessary because getFaceThing may return null if zone face is not a block face
    virtual DVect3              getFaceNormal(quint32 face) const=0;
    /// Returns the location of the centroid of face \b face in the global coordinate system.
    /// \param face starts at 0, and must be lower than 4.
    /// This function is necessary because getFaceThing may return null if zone face is not a block face
    virtual DVect3              getFaceCentroid(quint32 face) const=0;
    /// Get FaceThing for face \b face
    /// \param face starts at 0, and must be lower than 4.
     /// Return 0 if not a block face
    virtual const IFaceThing*   getFaceThing(quint32 face) const=0;
    /// Get FaceThing ID for face \b face
    /// \param face starts at 0, and must be lower than 4.
    /// Return 0 if not a block face
    virtual quint64            getFaceThingID(quint32 face) const=0;
    /// Returns a const pointer to the zone, IZoneThing, across face \b face.
    /// \param face starts at 0, and must be lower than 4.
    virtual const IZoneThing *  getJoin(quint32 face) const=0;
    /// returns the zone density
    virtual double              getDensity() const=0;
    /// sets the density of the individual zone (not the material density)
    virtual void                setDensity(double d)=0;
    /// Returns the name of the mechanical constitutive model present in the zone.
    virtual QString             getModelName() const=0;
    /// Set constitutive modoel name
    virtual void                setModelName(const QString &mname)=0;
    /// Returns the code for the plastic state
    virtual int                 getState() const=0;
    /// Sets the code for the plastic state
    virtual void                setState(int s)=0;
    /// Returns string associated with state
    virtual QString             getStateString() const=0;
    /// Given a property name, returns the index of the property of the constitutive model assigned to this zone.
    /// \note Base 1, returns 0 if name is not found.
    virtual quint32             getPropertyIndex(const QString &sc) const=0;
    /// Given an index, returns the name of the property of the constitutive model assigned to this zone.
    /// \note Base 1, returns QString() if \a index \a is less then 1 or greater than the number of properties.
    virtual QString             getPropertyName(quint32 index) const=0;
    /// returns a list of all the valid properties of the current constitutive model assigned to this zone
    virtual QStringList         getPropertyNames() const=0;
    /// Returns value of zone property corresponding to \b index \n
    /// Returns 0 if \b index is not valid
    virtual QVariant            getProperty(uint index) const=0;
    /// Returns value of zone property corresponding to \b propName \n
    /// Returns 0 if \b propName is not recognized
    virtual QVariant            getProperty(const QString& propName) const=0;
    /// Set property \b sprop to \b value. Returns false if property not found
    virtual void               setProperty(const QString &sprop, QVariant val)=0;
    /// Returns the list of properties for the thermal model
    virtual const QStringList   getThermProperties() const=0;
    /// Given a thermal property name, returns the index of the property assigned to this zone.
    /// \note Base 1, returns 0 if name is not found.
    virtual quint32             getThermPropertyIndex(const QString &sc) const=0;
    /// Returns value of zone thermal property corresponding to \b index \n
    /// Returns 0 if \b index is not valid
    virtual QVariant            getThermProperty(uint index) const=0;
    /// Returns value of zone thermal property corresponding to \b propName \n
    /// Returns 0 if \b propName is not recognized
    virtual QVariant            getThermProperty(const QString& propName) const=0;
    /// Set thermal property \b sprop to \b value. Returns false if property not found
    virtual bool                setThermProperty(const QString& sprop, QVariant value)=0;
    /// Returns the list of properties for the matrix flow model
    virtual const QStringList   getFluidProperties() const=0;
    /// Given a matrix flow property name, returns the index of the property assigned to this zone.
    /// \note Base 1, returns 0 if name is not found.
    virtual quint32             getFluidPropertyIndex(const QString &sc) const=0;
    /// Returns value of zone matrix flow property corresponding to \b index \n
    /// Returns 0 if \b index is not valid
    virtual QVariant            getFluidProperty(uint index) const=0;
    /// Returns value of zone matrix flow property corresponding to \b propName \n
    /// Returns 0 if \b propName is not recognized
    virtual QVariant            getFluidProperty(const QString& propName) const=0;
    /// Set fluid property \b sprop to \b value.  Returns false if property not found
    virtual bool                setFluidProperty(const QString& sprop, QVariant value)=0;
    /// Returns value of spare offset
    virtual double              getSpare() const=0;
    /// Return zone stress
    virtual SymTensor           getStress(bool bEffective) const=0;
    /// Set the zone stress
    virtual void                setStress(const SymTensor &st)=0;
    /// Return zone pore pressure (= average of gridpoint pore pressures)
    virtual double              getPP() const=0;
    /// Return strain rate
    virtual SymTensor           getStrainRate() const=0;
    /// Return strain increment
    virtual SymTensor           getStrainIncrement() const=0;
    /// Return rotational strain rate
    virtual SymTensor           getRotStrainRate() const=0;
    /// Return rotational strain increment
    virtual SymTensor           getRotStrainIncrement() const=0;
    /// return fortran offset of host block
    virtual FInt                getBlock() const=0;
    /// Return host blockthing
    virtual IBlockThing*         getBlockThing() const=0;
    /// Return host blockthing id
    virtual quint64             getBlockThingID() const=0;
    /// return zone mass
    virtual double              getMass() const=0;
    /// return zone material number
    virtual int                 getMaterialNumber() const=0;
    ///// Returns greatest compressive pressure
    //virtual double              getMaxPressure() const=0;
    /// Returns total volumetric strain
    //virtual double              getVolumetricStrain() const=0;
    ///// Returns greatest compressive volumetric strain
    //virtual double              getMaxVolumetricStrain() const=0;
    /// Returns true if zone is in a block marked as excavated
    virtual bool                isExcavated() const=0;
    /// Returns true if zone is in a block that is removed
    virtual bool                isRemoved() const=0;
    /// Returns true if zone or host block is hidden
    virtual bool                getHidden() const=0;
    /// Returns zone volume
    virtual double              getZoneVolume() const=0;
    /// Returns overlay number (0-3).  Used in quad zoning
    virtual quint32             getOverlay() const=0;
    /// Get minimum edge length
    virtual double              getMinEdgeLength() const=0;
    /// Checks to ascertain if the tet volume will be adequate for FLAC3D
    virtual bool                checkGoodForFLAC3D() const=0;
    /// Get the fluid discharge vector
    virtual DVect3              getFluidDischarge() const=0;
    /// Get thermal flow vector
    virtual DVect3              getThermalFlux() const=0;
    /// Get xx component of stress
    virtual double              getSxx() const=0;
    /// Get yy component of stress
    virtual double              getSyy() const=0;
    /// Get zz component of stress
    virtual double              getSzz() const=0;
    /// Get xy component of stress (=yx)
    virtual double              getSxy() const=0;
    /// Get xz component of stress (=zx)
    virtual double              getSxz() const=0;
    /// Get yz component of stress (=zy)
    virtual double              getSyz() const=0;
    /// Set xx component of stress
    virtual void                setSxx(double s)=0;
    /// Set yy component of stress
    virtual void                setSyy(double s)=0;
    /// Set zz component of stress
    virtual void                setSzz(double s)=0;
    /// Set xy component of stress (=yx)
    virtual void                setSxy(double s)=0;
    /// Set xz component of stress (=zx)
    virtual void                setSxz(double s)=0;
    /// Set yz component of stress (=zy)
    virtual void                setSyz(double s)=0;
    /// Returns the Stress to Strength ratio based on the current constitutive model properties and stress state \b s.\n
    /// The Stress to Strength ratio is greater than or equal to 1.0. A value of 1.0 means that the zone is failing 
    /// If the zone does not currently have any assigned constitutive model, or if the loaded constitutive model does not support 
    ///   this operation, then a default value of 10 is returned.
    virtual double              getStressStrengthRatio(const SymTensor &s) const=0; 
    /// Returns the condition of zone geometry.
    virtual double              getZoneCondition(int i) const=0;
    /// Return the position of the gridpoint with deformation exaggerated by factor if active is true
    virtual DVect3 getDeformedPosition(bool active, double factor) const=0;
    /// Return Shear Strain increment
    virtual double              getSSI() const=0;
    /// Return Shear Strain rate increment
    virtual double              getSSR() const=0;
    /// Return Volumetric Strain increment
    virtual double              getVSI() const=0;
    /// Return Volumetric Strain rate increment
    virtual double              getVSR() const=0;
    /// Returns zone position
    virtual DVect3              getLocation() const = 0;
  };

} // end namespace block
// EOF