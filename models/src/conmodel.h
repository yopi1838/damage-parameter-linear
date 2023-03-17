#pragma once

#include "conmodelbase.h"
#include "state.h"
#include "convert.h"
#include <vector>
#include <iostream>

/**
* \file
* \brief The base class for constitutive model plug-ins.
* \defgroup models Constitutive Model interface specification
* \addtogroup models
* @{
*/

/// \brief The Constitutive Model interface library.
namespace models {

    struct State;

    /// \brief The base class for constitutive model plug-ins.
    /** All constitutive models (null, elastic, mohr-coulomb, etc) are derived from this class.\n
    *  Any custom constitutive model provided as a plug-in must be derived from this class.\n
    *  Pure virtual method must be provided by the user - some utility functions are provided.
    *  \note The file name of the DLL produced must be model|name|###|.dll (in release)
    *          and model|name|###|_debug.dll (in debug).\n
    *        Where |name| is the string returned by getName(), 
    *          and |###| is the Major version number (3 digits, right justified, filled with zeros).\n
    *        For instance modelelastic001.dll and modelelastic001_debug.dll
    */
    class CONMODEL_EXPORT ConstitutiveModel {
    public:
        /// Returns a string containing the name of the constitutive model as the
        ///   user will refer to it with the MODEL command.\n
        /// For example, "elastic" would be a valid string in C++.\n
        /// This must be a unique name, and is used for synchronizing Save/Restore.\n
        /// This name is also used in the file name of the DLL.
        /// \brief Must be unique, used to identify model in save/restore, on command line, filename.
        virtual String             getName() const=0; 
        /// There should be no reason for an implementation to override the default behavior of this function.
        virtual String             getPluginName() const { return getName(); }
        /// Returns a string containing the name of the constitutive model that is to be used on printout\n
        ///   (e.g., resulting from the LIST zone command).\n 
        /// The name may or may not be the same as that given by the getName() member function,\n
        ///   but note that the code may truncate long strings on printout.\n
        /// An example of a valid string is "Linearly elastic".
        /// \brief The full name of the model.
        virtual String             getFullName() const=0; 
        /// Returns true if this model represents a NULL material.  Only the NULL model should return true.
        virtual bool               isNull() const { return false; }
        /// Return TRUE if the model should be considered an "advanced" model, and so not displayed
        ///    on a list of the simpler material models available.
        virtual bool               isModelAdvanced() const { return false; }
        /// Return TRUE if the model should be considered "creep", in that it contains time dependent material behavior.
        virtual bool               isCreep() const { return false; }
        /// Return TRUE if the model should be considered "Liquefaction" model.
        virtual bool               isLiquefaction() const { return false; }
        /// The version number of the constitutive model should be returned.\n
        /// This may be used to deal with the case of restoring files containing objects of earlier versions
        ///   of the model, which perhaps omit certain variables.
        /// \brief Returns minor version of base implementation, override for actual model.
        virtual UInt               getMinorVersion() const; 
        /// The following string is a valid example: "shear, bulk".\n
        /// The given names will be those recognized by the PROPERTY command.\n
        /// Property names are delimited by commas, synonyms of the same property may be provided
        ///   within one entry, separated by spaces.
        /// \brief Returns a string containing the names of model properties.
        virtual String             getProperties() const=0; // comma delimited
        /// The names are used on printout and in plotting to identify user-defined internal states
        ///   of the model (e.g., plastic flow).\n
        /// The following string is a valid example: "yielding, tension".\n
        /// See the variable State::state_.\n
        /// State names are delimited by commas
        /// \brief Returns a string containing state names.
        virtual String             getStates() const=0;
        /// A value should be returned for the model property of sequence number n\n
        ///  (previously defined by a getProperties() call, with n=1 denoting the first property). 
        /// Return real/0.0 if property doesn't exist.
        /// \note Property indices start with 1!
        /// \brief Return the value of the property of that \a index (base 1).
        virtual Variant            getProperty(UInt index) const=0; 
        /// The value of val supplied by the call comes from a command of the form PROP name=val;
        ///   the supplied value of n is the sequence number (starting with 1) of the property 
        ///   name previously specified by means of a getProperties() call.\n
        /// The model object is required to store the supplied value in an appropriate private member variable.\n
        /// The third argument, restoreVersion, is currently only used to distinguish between calls made while
        ///   processing data files versus values set during a restore of a save file.  
        ///   It will be zero for command processing, and 1 in a restore.
        /// The base class implementation calls setValid(0), invalidating the object.                                                                
        /// \brief Sets the value of the property with \a index i (base 1).
        virtual void               setProperty(UInt i,const Variant &val,UInt restoreVersion=0) { i;  val;  restoreVersion; setValid(0); }
        /// Return TRUE if property index i should be considered read only, and not allowed to be set by the user.
        ///    Note that a setProperty() called directly will still be allowed (and should be skipped for this property)
        ///    , just that the places in the code (Property command, FISH, etc) will check this first.
        virtual bool               isPropertyReadOnly(UInt i) const { i;  return false; }
        /// Return TRUE if this property should be considered "Advanced".
        ///   It should have a default value and should not be necessary to specify for simpler applications of the model.
        virtual bool               isPropertyAdvanced(UInt i) const { i;  return false; }
        /// Allows data other than properties to be saved efficiently.
        /// Note that you are responsible for adding data to allow multiple version compatibility yourself.
        /// Also note that you should use BINARY output (os.write, is.read) not formatted IO <<.
        virtual void               save(std::ostream &o) const;
        /// Allows data other than properties to be restored efficiently.
        virtual void               restore(std::istream &i,UInt restoreVersion);
        /// A new object, of the same class as the current object, must be created, and a pointer to it of type ConstitutiveModel returned.
        /// This function is called whenever FLAC3D installs the model in a zone.    
        /// \brief Returns an instance this class.
        virtual ConstitutiveModel *clone() const=0;
        /// The model object must return a value for its best estimate of the maximum confined modulus.\n
        /// This is used by the code to compute the stable timestep or adjust inertial mass.\n
        /// For an isotropic elastic model, the confined modulus is K + 4G/3.
        /// \brief Return estimate of maximum confined modulus.
        virtual Double             getConfinedModulus() const=0;
        /// The model object must return a value for its best estimate of the current tangent shear-modulus.\n
        /// This is used by the code to determine coefficients for quiet boundaries in dynamic mode.    
        /// \brief Return estimate of tangent shear-modulus.
        virtual Double             getShearModulus() const=0;
        /// This is used by FLAC3D zone relaxation to query the current elastic bulk modulus of a zone.
        /// \brief Return estimate of tangent bulk-modulus.
        virtual Double             getBulkModulus() const=0;
        /// This member function should first call the base class copy function, and then copy all essential data from the model object
        ///   pointed to by cm (assumed to be of the same derived class as the current model).\n
        /// It is not necessary to copy data members that are recomputed when the initialize() function is called.\n
        /// Use dynamic cast<MyModel *>(cm) to upcast cm.
        /// \brief Copy property values and state from another constitutive model.
        virtual void               copy(const ConstitutiveModel *mod);
        /// Set true if an alternative mixing technqiue rather than the default one in the code is used.\n
        virtual bool               performMixing() const { return false; }
        /// User defined strain mixing technique.\n
        virtual void               mixStrain(UByte d,State *s,SymTensor *strains,double *volumes) { d; s; strains; volumes; }
        /// This function is called for each tetra in each zone at each cycle from within FLAC3D’s zone scan.\n
        /// The model must update the stress tensor from strain increments.\n
        /// The structure State \a s contains the current stress components and the computed 
        ///   strain increment components for the zone being processed.\n
        /// \a dim will be either 2 or 3, depending on the dimension of the calling program.
        /// The stress components already contain the rotation-correction terms when run() is called.\n
        /// The base class implementation will call initialize() if isValid() returns false.
        /// \brief Calculate stress increment given a strain increment.  
        virtual void               run(UByte d,State *s) { if (!isValid(d)) initialize(d,s); }
        /// User defined stress mixing technique.\n
        virtual void               mixStress(UByte d,State *s,SymTensor *stresses,double *volumes) { d; s; stresses; volumes; }
        /// This function is called once for each model object (i.e., for each full zone) when the 
        ///   SOLVE/Step/CYCLE command is given when the large-strain update is performed, and at the 
        ///   beginning of the run() method if isValid() returns false.\n
        /// The model object may perform initialization of its property or state variables, or it may do nothing.\n
        /// The dimensionality (e.g., this is 3 for FLAC3D ) is given as \a dim, and structure \a s contains current information
        ///   for the zone containing the model object.\n
        /// Note that strains may be undefined when initialize() is called.\n
        /// The average stress components for the full zone are available in the state structure; they should not be 
        ///   changed by the initialize() member function.\n
        /// The base class implementation calls setValid(dim).
        /// \brief Initializes the constitutive model in preparation for calls to run().
        virtual void               initialize(UByte d,State *s) { s; setValid(d); }
        /// Returns the ratio of the passed stress tensor to the yield strength of the zone properties.\n
        /// Implementing this function is optional.\n
        /// By convention, 10.0 is the maximum value returned.
        /// \brief Returns the ratio of the stress tensor to the current yield strength.
        virtual Double             getStressStrengthRatio(const SymTensor &) const { return 10.0; }
        /// This function is used to set scaled properties for the Safety Factor calculation.
        /// Implementing this function is optional.
        /// \arg f is the factor to scale.  Some properties may not scale linearly.
        /// \arg v is the list of properties (by index) that are requested to be scaled.
        /// \brief Scales failure property indices \a v by the factor \a f.
        virtual void               scaleProperties(const Double &f,const std::vector<UInt> &v) { f; v; throw std::logic_error("Does not support property scaling"); }
        /// Returns true if hysteretic damping is supported by run(). See State::hysteretic_damping_.
        virtual bool               supportsHystereticDamping() const { return false; }
        /// Returns true if strength-stress ratio calculations are supported via getStrengthStressRatio().    
        virtual bool               supportsStressStrengthRatio() const { return false; }
        /// Returns true if property scaling is supported for factor-of-safety calculations via scaleProperties().
        virtual bool               supportsPropertyScaling() const { return false; }
        /// Returns true if small strain effect is supported by run(). 
        ///   Currently this has not yet been implemented.
        virtual bool               supportsSmallStrainEffect() const { return false; }
        /// There should be no reason for an implementation to change the default behavior of this function.
        virtual void               destroy() { delete this; }

        /// The constitutive model constructor.  The model is invald, and canFail is set to true.
        ConstitutiveModel(unsigned short option=0);
        /// The constitutive model destructor.
        virtual ~ConstitutiveModel();
        /// This indicates the minor version of the constitutive model library routines.\n
        /// This will be updated to indicate bug fixes, and minor changes that do not affect binary compatibility.\n
        /// \brief Returns the minor version number of the constitutive model library.
        static UInt getLibraryMinorVersion();
        /// Indicates whether initializion is necessary - by dimension.
        bool isValid(UByte d) const { return valid_==d; }
        /// Sets the current valid state to dimension \a dim.
        void setValid(UByte d) { valid_ = d; }
        /// Indicates whether failure should be allowed.\n
        /// Constitutive models should honor this state, and not allow failure if it returns true.
        bool canFail() const { return canFail_; }
        /// Specifies whether or not "failure" is being allowed for this instanced of the constitutive model.
        void setIfCanFail(bool b) { canFail_ = b; }
        /// Used by Itasa only
        std::vector<unsigned short> gEOB() const;
        /// Used by Itasa only
        void                        sEOB(const std::vector<unsigned short> &reply);
        /// Used by Itasa only
        bool                        cEOB() const;

#ifndef DOXYGEN
        // Memory Allocation Customization
        // Changing this is very dangerous, as the calling program will make assumptions about
        // where memory has been allocated to and from.
        void *operator new(size_t size);
        void *operator new(size_t size,const char *name,const char *file,unsigned line);
        void  operator delete(void *v);
        void  operator delete(void *v,const char *name,const char *file,unsigned line);
        typedef void *(*NewFunction)(size_t,const char *,const char *,unsigned);
        typedef void (*DeleteFunction)(void *,const char *,const char *,unsigned);
        typedef std::pair<NewFunction,DeleteFunction> AllocFunctions;
        static AllocFunctions setAllocFunctions(AllocFunctions afunc);
        static AllocFunctions getDefaultAllocFunctions();
        static AllocFunctions getAllocFunctions() { return AllocFunctions(newFunc_,deleteFunc_); }
#endif

    private:
        static NewFunction    newFunc_;
        static DeleteFunction deleteFunc_;
        Byte           valid_ = 0;
        bool           canFail_ = true;
        unsigned short option_ = 0;

    protected:
        // ensure the same bits for all models
        static const unsigned long shear_now          = 0x001;
        static const unsigned long tension_now        = 0x002;
        static const unsigned long shear_past         = 0x004;
        static const unsigned long tension_past       = 0x008;
        static const unsigned long joint_shear_now    = 0x010;
        static const unsigned long joint_tension_now  = 0x020;
        static const unsigned long joint_shear_past   = 0x040;
        static const unsigned long joint_tension_past = 0x080;
        static const unsigned long volume_now         = 0x100;
        static const unsigned long volume_past        = 0x200;
        // shared frequently used constants
        static constexpr double pi = 3.14159265358979323846264338327950;
        static constexpr double degrad = 0.01745329251994329576923690768488;
        static constexpr double d1d3 = 0.33333333333333333333333333333333;
        static constexpr double d2d3 = 0.66666666666666666666666666666667;
        static constexpr double d4d3 = 1.33333333333333333333333333333333;
        static constexpr double rt6 = 2.44948974278317809819728407470589; // sqrt(6.0)
        //
        static double cos3t(const double& n11, const double& n22, const double& n33,
                            const double& n12, const double& n31, const double& n23, double *tensornorm=0);
        static double gfunc(const double &c, const double &cos3t, const double &expn=-0.25, double *B=0, double *C=0);
    };

#  define C_M ConstitutiveModel
} // namespace models

/// @}
// EoF
