#pragma once
// ISELShell.h

/**
  * \file
  * \brief Interface to shell structural elements 
  */

#include "isel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    using namespace itasca;

    /// \brief Interface to shell structural elements
    /** This interface inherits from ISEL.
    * \ingroup SEL
    */
    class ISELShell {
    public:
        enum MaterialPropertyType { kIsotropicPropertyType=1, kOrthotropicPropertyType, kAnisotropicPropertyType };
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a04;
        virtual TType             getType() const { return type_; }
        /// returns a const IThing pointer
        virtual const IThing *    getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *          getIThing()=0;
        /// Returns a const pointer to object base class
        virtual const ISEL *      getISEL() const=0;
        /// Returns a pointer to object base class
        virtual ISEL *            getISEL()=0;
        /// Returns the shell surface area
        virtual double            getArea()const=0;
        /// Returns the keyword of the finite-element type
        virtual QString           getElementType() const=0; 
        /// Returns an isotropic material property\n.
        /// Valid values for parameter \b propNo are: 1(Young's modulus) and 2(Poisson's ratio)
        virtual double            getIsotropic(int propNo) const=0;
        /// Set an isotropic material property\n.
        /// Valid values for parameter \b propNo are: 1(Young's modulus) and 2(Poisson's ratio)
        virtual void              setIsotropic(const double &v, int propNo) =0;
        /// Returns material property type\n
        /// Return values are: 1 (isotropic), 2 (orthotropic), 3 (anisotropic)
        virtual MaterialPropertyType getMaterialPropertyType() const=0; 
        /// Returns a orthotropic material property. If the shell does not have orthotropic property, this function return 0.0\n.
        /// Valid values for parameter \b p(material-coefficient stiffness) are: 0 (C11), 1(C12) , 2 (C22) and 3(C33)\n
        /// Valid values for parameter \b t are: 1 (membrane) and 2 (bending)
        virtual double            getOrthotropic(int p, int t) const=0; 
        /// Returns the uniform pressure acting on shellSel in its local z-direction, see setPressure(). 
        virtual double            getPressure() const=0;
        /// Returns the shell thickness, see setThickness(). 
        virtual double            getThickness() const=0;
        /// Returns the shell volume (surface area time thickness). 
        virtual double            getVolume() const =0;        
        /// Returns the local axes system used to represent material properties.
        virtual Axes3D            getMaterialSys() const=0;
        /// Returns the angle (radians) between SEL local x-axis and material x'-axis, measured conterclockwise from SEL local x-axis. 
        virtual double            getBeta() const=0;
        /// Returns a anisotropic material property. If the shell does not have anisotropic property, this function return 0.0\n.
        /// Valid values for parameter \b p(material-coefficient stiffness) are: 0 (C11), 1(C12) , 2 (C13), 3(C22), 4(C23) and 5(C33)\n
        /// Valid values for parameter \b t are: 1 (membrane) and 2 (bending)
        virtual double            getAnisotropic(int p, int t) const=0; 
        /// Sets the uniform pressure acting on shellSel in its local z-direction, see getPressure(). 
        virtual void              setPressure(const double &p)=0;
        /// Sets the shell thickness, see getThickness(). 
        virtual void              setThickness(const double &t)=0;
        /// Returns the depth factor used in stress recovery.
        virtual double            getDepthFactor(uint index) const=0;
        /// Returns the principal stress at location \b locCode.\n
        /// Valid values for parameter \b locCode are: 0(centroid), 1(node-1) ,2 (node-2) and 3(node-3)
        virtual DVect3            getPrinStress(uint index,quint32 locCode) const=0; 
        /// Returns the stress resultant at location \b locCode.\n
        /// Valid values for parameter \b locCode are: 0(centroid), 1(node-1) ,2 (node-2) and 3(node-3)
        /// valid values for parameter \b component are: 0(Mx), 1(My), 2(Mxy), 3(Nx), 4(Ny), 5(Nxy), 6(Qx) and 7(qy)
        virtual double            getStressResultant2(uint index,quint32 locCode, quint32 component) const =0; 
        /// Indicates if the element is marked for stress resultant calculations.
        virtual bool              isMarked(uint index) const=0;
        /// Returns true if the surface system validity flag is valid and stress resultants are valid.\n
        virtual bool              isStressResultantValid(uint index) const=0;
        /// Returns true if the surface system validity flag is valid and stress values are valid at the current depth.\n
        virtual bool              isStressValid(uint index) const=0;
        /// Returns the symetric stress tensor at locacion \b locCode. Stresses are expressed in the global system.\n
        /// The stresses are recovered at the current depth factor.\n
        /// Valid values for parameter \b locCode are: 0(centroid), 1(node-1) ,2 (node-2) and 3(node-3)
        virtual SymTensor         getStress(uint index,quint32 locCode) const=0; 
        /// Returns membrane stress resultants Nx,Ny,Nxy (parameter \b memb) and bending stress 
        ///  resultants Mx,My,Mxy (parameter \b bend) for ONE ISOLATED element, in its local 
        ///  coordinate system: average stress resultants computed on each node, based on forces acting on it.
        virtual void              getElementStresses(DVect3 *memb,DVect3 *bend) const=0;
        /// Returns the yield state for geogrids and liners:
        /// 0 = "No Yield"
        /// 1 = "Yielding Now"
        /// 2 = "Yield in Past"
        //virtual int               getRYield(int,int) const = 0;
        // Gets the minimum or maximum stress for an elastic element by taking the 2D principal
        // stresses at the shell surfaces. Returns zero for a plastic element
        virtual double            getElasticStressBounds(const bool minimum) const = 0;
        virtual double            getElasticStressMax() const = 0;
        virtual double            getElasticStressMin() const = 0;

        /// Shell-type sels:
        /// Returns the stress in coupling springs at \b nd_idx and \b side.\n
        /// If \b norm is true, returns normal stress, else returns shear stress.\n
        /// Valid values for \b nd_idx are 0, 1 or 2.\n
        /// Valid values for \b side are 0 (side 1) or 1 (side 2).\n
        /// The computation is general and will work even if link assignments have been modified.
        /// Computes stress based on forces in all deformable directions of link at side.
        virtual double getCouplingStress(const ISELNode* node, bool norm, uint32 side) const = 0;
    };
} // namespace sel
// EoF

