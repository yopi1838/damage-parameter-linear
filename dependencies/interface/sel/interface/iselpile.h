#pragma once
// ISELPile.h

/**
  * \file
  * \brief Interface to pile structural elements
  */

#include "isel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    class ISELBeam;
    /// \brief Interface to pile structural elements
    /** This interface inherits from ISELBeam.
    * \ingroup SEL
    */
    class ISELPile {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a0d;
        /// returns a const IThing pointer
        virtual const IThing *    getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *          getIThing()=0;
        /// Returns a const pointer to object base class, ISEL
        virtual const ISEL *      getISEL() const=0;
        /// Returns a pointer to object base class, ISEL
        virtual ISEL *            getISEL()=0;
        /// Returns a const pointer to the class that it inherits from
        virtual const ISELBeam   *getISELBeam() const=0;
        /// Returns a pointer to the class that it inherits from
        virtual ISELBeam *        getISELBeam()=0;

        /// Returns the normal coupling spring cohesive strength, see setPropCSnCoh()
        virtual double            getPropCSnCoh()const=0;
        /// Returns the normal coupling spring friction angle, see setPropCSnFric()
        virtual double            getPropCSnFric()const=0;
        /// Returns \b true if normal coupling spring gap-use flag is \b on, otherwise returns false. See setPropCSnGap()
        virtual bool              getPropCSnGap()const=0;
        /// Returns the normal coupling spring stiffness, see setPropCSnK()
        virtual double            getPropCSnK()const=0;
        /// Returns the shear coupling spring cohesive strength, see setPropCSsCoh()
        virtual double            getPropCSsCoh()const=0;
        /// Returns the shear coupling spring friction angle, see setPropCSsFric()
        virtual double            getPropCSsFric()const=0;
        /// Returns the shear coupling spring stiffness, see setPropCSsK()
        virtual double            getPropCSsK()const=0;
        /// Returns the exposed perimeter, see setPropPer()
        virtual double            getPropPer()const=0;
        /// Returns \b true if the large-strain sliding flag is \b on, false otherwise. See setPropSlide()
        virtual bool              getPropSlide()const=0;
        /// Returns the large-strain sliding tolerance, see setPropSlideTol()
        virtual double            getPropSlideTol()const=0;

        /// Sets the normal coupling spring cohesive strength, see getPropCSnCoh()
        virtual void              setPropCSnCoh(const double &d)=0;
        /// Set the normal coupling spring friction angle, see getPropCSnFric()
        virtual void              setPropCSnFric(const double &d)=0;
        /// Sets the normal coupling spring gap-use flag, see setPropCSnGap()
        virtual void              setPropCSnGap(bool b)=0;
        /// Sets the normal coupling spring stiffness, see getPropCSnK()
        virtual void              setPropCSnK(const double &d)=0;
        /// Sets the shear coupling spring cohesive strength, see getPropCSsCoh()
        virtual void              setPropCSsCoh(const double &d)=0;
        /// Set the shear coupling spring friction angle, see getPropCSsFric()
        virtual void              setPropCSsFric(const double &d)=0;
        /// Sets the shear coupling spring stiffness, see getPropCSsK()
        virtual void              setPropCSsK(const double &d)=0;
        /// Sets the exposed perimeter, see getPropPer()
        virtual void              setPropPer(const double &d)=0;
        /// Sets the large-strain sliding flag, see getPropSlide()
        virtual void              setPropSlide(bool b)=0;
        /// Sets the large-strain sliding tolerance, see getPropSlideTol()
        virtual void              setPropSlideTol(const double &d)=0;

        /// Returns \b true if the shear direction incremental confining stress flag is \b on, see setPropCSsConfInc()
        virtual bool              getPropCSsConfInc() const=0;
        /// Returns the shear direction incremental confining stress factor table number, see setPropCSsConfTab()
        virtual QString           getPropCSsConfTab() const=0;
        /// Returns the shear direction cohesive strength table number, see setPropCSsCohTab()
        virtual QString           getPropCSsCohTab() const=0;
        /// Returns the shear direction friction angle table number, see setPropCSsFricTab()
        virtual QString           getPropCSsFricTab() const=0;
        /// Returns \b true is the rockbolt logic is \b on, see setPropRBolt()
        virtual bool              getPropRBolt() const=0;
        /// Returns the tensile failure strain (non-dimensional), see setPropTFStrain()
        virtual double            getPropTFStrain() const=0;
        /// Returns the axial tensile yield strength (force units) ,see setPropYield()
        virtual double            getPropYield() const=0;

        /// Sets the shear direction incremental confining stress flag, see getPropCSsConfInc()
        virtual void              setPropCSsConfInc(bool b) =0;
        /// Sets the shear direction incremental confining stress factor table number, see getPropCSsConfTab()
        virtual void              setPropCSsConfTab(QString s) =0;
        /// Sets the shear direction cohesive strength table number, see getPropCSsCohTab()
        virtual void              setPropCSsCohTab(QString s) =0;
        /// Sets the shear direction friction angle table number, see getPropCSsFricTab()
        virtual void              setPropCSsFricTab(QString s)=0;
        /// Sets the rockbolt logic flag, see getPropRBolt()
        virtual void              setPropRBolt(bool b)=0;
        /// Sets the tensile failure strain (non-dimensional), see getPropTFStrain()
        virtual void              setPropTFStrain(const double &d) =0;
        /// Sets the axial tensile yield strength (force units) ,see getPropYield()
        virtual void              setPropYield(const double &d) =0;
        virtual ISEL::YieldState  getRespYield(bool tension) const=0;
        virtual ISEL::YieldState  getRespMomentYield(bool bMy, quint32 end) const=0;

        // Response functions
        /// Returns the confining stress at \b end. Negative value indicates compression.\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Parameter \b bSig1 set to true will return the confining stress 11, and false the confining stress 22.
        virtual double            getRespCpConfine2(bool bSig1, quint32 end) const=0;
        /// Returns the displacement in coupling springs at \b end.\n
        /// Sign convension for shear spring is positive/negative with respect to average axial direcion of pile.\n
        /// Sign convension for normal spring is positive/negative, indicating separation/operlap.\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Parameter \b bNorm set to true indicates normal, and false indicates shear.
        virtual double            getRespCpDisp2(bool  bNorm, quint32 end) const=0;
        /// Returns the gap component, \b g, in normal coupling sprint at \b end.\n
        /// The gap is always tracked, but it only affects the behavior if the gap-use falg is on.\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Valid values for parameter \b dir are 0 (positive y-direction), 1 (negative y-direction), 
        ///    2(positive z-direction) and 3 (negative z-direction). 
        virtual double            getRespCpNGapVal2(quint32 end, quint32 dir) const=0;
        /// Returns the stress in coupling springs at \b end.\n
        /// Sign convension for shear spring is positive/negative with respect to average axial direcion of pile.\n
        /// Sign convension for normal spring is positive/negative, indicating separation/operlap.\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Parameter \b bNorm set to true indicates normal, and false indicates shear.
        virtual double            getRespCpStress2(bool  bNorm, quint32 end) const=0;
        /// Returns the direction (unit vector in global system) in which the pileSel is being loaded by coupling springs at \b end.\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Parameter \b bNorm set to true indicates normal, and false indicates shear.
        virtual DVect             getRespCpStressDir2(bool  bNorm, quint32 end) const=0;
        /// Returns the yield state in coupling springs at \b end.\n
        /// Return values are: 1 (never yielded), 2 (now yielding) and 3 (has yielded in past)\n
        /// Valid values for parameter \b end are 0 or 1.\n
        /// Parameter \b bNorm set to true indicates normal, and false indicates shear.
        virtual ISEL::YieldState  getRespCpYield2(bool  bNorm, quint32 end) const=0;

        virtual double getPlasticTensileStrain(quint32 node) const=0;
        virtual bool   getPlasticTensileStrainFailure() const=0;
    };
} // namespace sel
// EoF
