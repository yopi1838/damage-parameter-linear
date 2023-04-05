#pragma once

/**
  * \file
  * \brief Interface to cable structural elements 
  */

#include "isel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    /// \brief Interface to cable structural elements
    /** This interface inherits from ISEL.
    * \ingroup SEL
    */
    class ISELCable {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a0b;
        /// returns a const IThing pointer
        virtual const IThing    *getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing          *getIThing()=0;
        /// Returns a const pointer to object base class
        virtual const ISEL      *getISEL() const=0;
        /// Returns a pointer to object base class
        virtual ISEL            *getISEL()=0;
        /// Returns the compressive-yield force, see setPropYComp()
        virtual double           getPropYComp() const=0;
        /// Sets the compressive-yield force, see getPropYComp()
        virtual void             setPropYComp(const double &d)=0;
        /// Returns the tensile-yield force, see setPropYTens()
        virtual double           getPropYTens() const=0;
        /// Sets the tensile-yield force, see getPropYTens()
        virtual void             setPropYTens(const double &d)=0;
        /// Returns the grout stiffness, see setPropGrK()
        virtual double           getPropGrK() const=0;
        /// Sets the grout stiffness, see getPropGrK()
        virtual void             setPropGrK(const double &d)=0;
        /// Returns the grout cohesive strength, see setPropGrCoh()
        virtual double           getPropGrCoh() const=0;
        /// Sets the grout cohesive strength, see getPropGrCoh()
        virtual void             setPropGrCoh(const double &d)=0;
        /// Returns the grout friction angle in degrees, see setPropGrFric()
        virtual double           getPropGrFric() const=0;
        /// Sets the grout friction angle in degrees, see getPropGrFric()
        virtual void             setPropGrFric(const double &d)=0;
        /// Returns the grout exposed perimeter in meters, see setPropGrPer()
        virtual double           getPropGrPer() const=0;
        /// Sets the grout exposed perimeter in meters, see getPropGrPer()
        virtual void             setPropGrPer(const double &d)=0;
        /// Returns true if the large-strain sliding flag is activated, see setPropSlide()
        virtual bool             getPropSlide() const=0;
        /// Activates the large-strain sliding flag if parameter \b b is true
        virtual void             setPropSlide(bool b)=0;
        /// Returns the zone-in tolerance for large-strain sliding, see setPropSlideTol()
        virtual double           getPropSlideTol() const=0;
        /// Stes the zone-in tolerance for large-strain sliding, see getPropSlideTol()
        virtual void             setPropSlideTol(const double &d)=0;
        /// Returns the Young's modulus for the cable, see setPropE()
        virtual double           getPropE() const=0;
        /// Sets the Young's modulus for the cable, see getPropE()
        virtual void             setPropE(const double &e)=0;
        /// Returns the cross-sectional area for the cable, setPropXCArea()
        virtual double           getPropXCArea() const=0;
        /// Sets the cross-sectional area for the cable, getPropXCArea()
        virtual void             setPropXCArea(const double &d)=0;
        /// Returns the current yield state of the element. \n
        /// If \param tension is true, then it is the tensile yield state, otherwise it is the compressive yield state.
        virtual ISEL::YieldState getRespYield(bool tension) const=0;
        /// Returns the length of the cable
        virtual double           getLength() const=0;
        /// The surface area of the grout exposed to the surrounding material.\n
        /// The length of the element times the exposed grout perimiter.
        virtual double           getArea() const=0;
        /// Returns the volume of the cable
        virtual double           getVolume() const=0;
        /// Returns the average axial force on the cable (positive: tension; negative: compression)
        virtual double           getRespForce() const=0;
        /// Returns the average axial stress on the cable (positive: tension; negative: compression)
        virtual double           getRespStress() const=0;
        /// Returns the stress on grout (shear coupling spring) at \b end of cableSEL.\n
        /// Valid values for \b end are 0 or 1. 
        /// Positive or negative with respect to the average axial direction of the cable
        virtual double           getRespGrStress2(quint32 end) const=0;
        /// Returns the displacement on grout (shear coupling spring) at \b end of cableSEL.\n
        /// Valid values for \b end are 0 or 1. 
        /// Positive or negative with respect to the average axial direction of the cable
        virtual double           getRespGrDisp2(quint32 end) const=0;
        /// Returns the direction (unit vector in global system) in which the cable is being loaded at \b end of cableSEL.\n
        /// Valid values for \b end are 0 or 1. \n
        /// Positive or negative with respect to the average axial direction of the cable 
        virtual DVect            getRespGrStressDir2(quint32 end) const=0;
        /// Returns the slip state of the grout (shear coupling spring) at \b end of cableSEL.\n
        /// Valid values for \b end are 0 or 1. \n
        /// Valid return values are 0=never slipped, 1=now slipping, 2=slipped in the past 
        virtual ISEL::YieldState getRespGrYield2(quint32 end) const=0;
        /// Returns the confining stress acting on the grout at \b end (negative value is compression).\n
        /// Valid values for \b end are 0 or 1. \n
        /// Stress acts in plane perpendicular to cable axis.
        virtual double           getRespGrConfine2(quint32 end) const=0;
        virtual double           getAppliedTensionValue() const=0;
        virtual void             setAppliedTensionValue(double d)=0;
        virtual bool             getAppliedTensionState() const=0;
        virtual void             setAppliedTensionState(bool b)=0;
        virtual double           getPlasticTensileStrain() const=0;
        virtual double           getPlasticTensileFailureStrain() const = 0;
        virtual void             setPlasticTensileFailureStrain(double d) = 0;
        virtual bool             getPlasticTensileStrainFailure() const=0;
        virtual void             setPlasticTensileStrainFailure(bool b)=0;
    };
} // namespace sel
// EoF
