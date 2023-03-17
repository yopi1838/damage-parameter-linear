#pragma once

/**
* \file
* \brief Interface to beam structural elements
*/

#include "isel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    /// \brief Interface to beam structural elements
    /** This interface inherits from ISEL and it is inherited by IPileSEL.
    * \ingroup SEL
    */
    class ISELBeam {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a09;
        /// Returns a const IThing pointer
        virtual const IThing  *getIThing() const=0;
        /// Returns a IThing pointer
        virtual IThing *       getIThing()=0;
        /// Returns a const pointer to object base class, ISEL
        virtual const ISEL    *getISEL() const=0;
        /// Returns a pointer to object base class, ISEL
        virtual ISEL *         getISEL()=0;
        /// Returns the Young's modulus for the beam, see setPropE()
        virtual double         getPropE()const=0;
        /// Returns the Poisson's ratio for the beam, see setPropNu()
        virtual double         getPropNu()const=0;
        /// Returns the cross-sectional area for the beam, setPropXCArea()
        virtual double         getPropXCArea()const=0;
        /// Returns the polar moment of inertia for the beam, see setPropXCPolMom()
        virtual double         getPropXCPolMom()const=0;
        /// Returns the second moment with respect to the beamSel y-axis, setPropXCIy()
        virtual double         getPropXCIy()const=0;
        /// Returns the second moment with respect to the beamSel z-axis, see setPropXCIz()
        virtual double         getPropXCIz()const=0;
        /// Returns the moments of inertia was a vector (J,Y,Z)
        virtual DVect3         getMOI() const { return DVect3(getPropXCPolMom(),getPropXCIy(),getPropXCIz()); }
        /// Returns the Y-Axis of the local system.
        virtual DVect3         getPropLsys()const=0;
        /// Returns the plastic moment capacity for the beam, see setPropPMom()
        virtual double         getPropPlasticMoment() const=0;
        virtual double         getPropPlasticMomentY() const=0;
        virtual double         getPropPlasticMomentZ() const=0;
        /// Returns the applied pressure (positive in y'-direction), see setAppYPress()
        virtual double         getAppYPress()const=0;
        /// Returns the applied pressure (positive in z'-direction), see setAppZPress()
        virtual double         getAppZPress()const=0;
        /// Returns the length of the beam
        virtual double         getLength()const=0;
        /// Returns the beam area, 
        virtual double         getArea()const=0;
        /// Returns the volume of the beam
        virtual double         getVolume()const=0;
        /// Returns one of the components of the response force at one of the ends of a BeamSEL\b
        /// Valid values for \b dof are 0,1 or 2
        /// Valid values for \b end are 0 or 1
        virtual double         getRespForce2(quint32 dof, quint32 end) const=0;
        /// Returns the response force at one of the ends of a BeamSEL\b
        /// Valid values for \b end are 0 or 1
        virtual DVect3         getRespForce2(quint32 end) const=0;
        /// Returns the moment at one of the ends of a BeamSEL\b
        /// Valid values for \b end are 0 or 1
        virtual DVect3         getRespMoment2(quint32 end) const=0;
        /// Returns the structural element density, see ISEL::setDensity()
        virtual double         getDensity()const=0;
        /// Returns the beam thermal expansion coefficient, see ISEL::setThExp()
        virtual double         getThExp()const=0;
        /// Returns the beam Timoshenko factor, which is teh inverse of shear coefficient 
        virtual double        getFactorTimo() const = 0;

        /// Sets the applied pressure (positive in y'-direction), see getAppYPress()
        virtual void           setAppYPress(const double &d)=0;
        /// Sets the applied pressure (positive in z'-direction), see getAppZPress()
        virtual void           setAppZPress(const double &d)=0;
        /// Sets the polar moment of inertia for the beam, see getPropXCPolMom()
        virtual void           setPropXCPolMom(const double& pmom)=0;
        /// Sets the Young's modulus for the beam, see getPropE()
        virtual void           setPropE(const double &e)=0;
        /// Sets the Poisson's ratio for the beam, see getPropNu()
        virtual void           setPropNu(const double &e)=0;
        /// Sets the cross-sectional area for the beam, getPropXCArea()
        virtual void           setPropXCArea(const double &d)=0;
        /// Sets the second moment with respect to the beamSel y-axis, getPropXCIy()
        virtual void           setPropXCIy(const double &d)=0;
        /// Sets the second moment with respect to the beamSel z-axis, getPropXCIz()
        virtual void           setPropXCIz(const double &d)=0;
        /// Sets the moments of inertia as a vector  (J,Y,Z)
        virtual void           setMOI(const DVect3 &v) { setPropXCPolMom(v.x());  setPropXCIy(v.y());  setPropXCIz(v.z()); }
        /// Sets the Y-Direction of the local element system.
        virtual void           setPropLsys(const DVect3& d)=0;
        /// Sets the plastic moment capacity for the beam, see getPropPMom()
        virtual void           setPropPlasticMoment(const double &d)=0;
        virtual void           setPropPlasticMomentY(const double &d)=0;
        virtual void           setPropPlasticMomentZ(const double &d)=0;
        /// Sets beam Timoshenko factor.
        virtual void           setFactorTimo(double d) = 0;
    };
} // namespace sel
// EoF
