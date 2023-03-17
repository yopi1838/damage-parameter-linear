#pragma once

#include "jmodelbase.h"
#include <vector>
#include <iostream>

/**
* \file
* \brief The base class for joint constitutive model plug-ins.
* \defgroup jmodels Joint Model interface specification
* \addtogroup jmodels
* @{
*/

/// \brief The Joint Constitutive Model interface library.
namespace jmodels
{
  struct State;

  /// \brief The base class for joint constitutive model plug-ins.
    /** All joint constitutive models (elastic, mohr-coulomb, etc) are derived from this class.\n
    *  Any custom joint constitutive model provided as a plug-in must be derived from this class.\n
    *  Pure virtual methods must be provided by the user - some utility functions are provided.
    *  \note The file name of the DLL produced must be jmodel|name|###|.dll (in release)
    *          and model|name|###|_debug.dll (in debug).\n
    *        Where |name| is the string returned by getName(),
    *          and |###| is the Major version number (3 digits, right justified, filled with zeros).\n
    *        For instance jmodelelastic001.dll and jmodelelastic001_debug.dll
    */
  class JMODEL_EXPORT JointModel
  {
  public:
      /// Returns a string containing the name of the joint constitutive model as the
        ///   user will refer to it with the JMODEL command.\n
        /// For example, "elastic" would be a valid string in C++.\n
        /// This must be a unique name, and is used for synchronizing Save/Restore.\n
        /// This name is also used in the file name of the DLL.
        /// \brief Must be unique, used to identify model in save/restore, on command line, filename.
    virtual String      getName() const=0; 
    /// There should be no reason for an implementation to override the default behavior of this function.
    virtual String      getPluginName() const { return getName(); }
    /// Returns a string containing the name of the joint constitutive model that is to be used on printout\n
        ///   (e.g., resulting from the BLOCK CONTACT LIST command).\n 
        /// The name may or may not be the same as that given by the getName() member function,\n
        ///   but note that the code may truncate long strings on printout.\n
        /// An example of a valid string is "Linearly elastic".
        /// \brief The full name of the model.
    virtual String      getFullName() const=0; // Full name of model.
    /// The version number of the constitutive model should be returned.\n
        /// This may be used to deal with the case of restoring files containing objects of earlier versions
        ///   of the model, which perhaps omit certain variables.
        /// \brief Returns minor version of base implementation, override for actual model.
    virtual UInt        getMinorVersion() const; // Returns minor version of base implementation, override for actual model.
    /// The following string is a valid example: "shear, bulk".\n
        /// The given names will be those recognized by the BLOCK CONTACT PROPERTY command.\n
        /// Property names are delimited by commas, synonyms of the same property may be provided
        ///   within one entry, separated by spaces.
        /// \brief Returns a string containing the names of model properties.
    virtual String      getProperties() const=0; // comma delimited
    /// The names are used on printout and in plotting to identify user-defined internal states
        ///   of the model (e.g., shear-n).\n
        /// The following string is a valid example: "yielding, tension".\n
        /// See the variable State::state_.\n
        /// State names are delimited by commas
        /// \brief Returns a string containing state names.
    virtual String      getStates() const=0; // comma delimited
    /// A value should be returned for the model property of sequence number n\n
        ///  (previously defined by a getProperties() call, with n=1 denoting the first property). 
        /// Return real/0.0 if property doesn't exist.
        /// \note Property indices start with 1!
        /// \brief Return the value of the property of that \a index (base 1).
    virtual Variant     getProperty(UInt index) const=0; // Return real/0.0 if property doesn't exist.
    /// The value of val supplied by the call comes from a command of the form BLOCK CONTACT PROP name=val;
        ///   the supplied value of n is the sequence number (starting with 1) of the property 
        ///   name previously specified by means of a getProperties() call.\n
        /// The model object is required to store the supplied value in an appropriate private member variable.\n
        /// The third argument, restoreVersion, is currently only used to distinguish between calls made while
        ///   processing data files versus values set during a restore of a save file.  
        ///   It will be zero for command processing, and 1 in a restore.
        /// The base class implementation calls setValid(0), invalidating the object.                                                                
        /// \brief Sets the value of the property with \a index i (base 1).
    virtual void        setProperty(UInt index,const Variant &p,UInt restoreVersion=0); // Calls setValid(0).  Return true if error.
    /// Allows data other than properties to be saved efficiently.
        /// Note that you are responsible for adding data to allow multiple version compatibility yourself.
        /// Also note that you should use BINARY output (os.write, is.read) not formatted IO <<.
    virtual void        save(std::ostream &o) const;
    /// Allows data other than properties to be restored efficiently.
    virtual void        restore(std::istream &i,UInt restoreVersion);
    /// A new object, of the same class as the current object, must be created, and a pointer to it of type JointModel returned.
        /// This function is called whenever 3DEC installs the model in a subcontact.    
        /// \brief Returns an instance this class.
    virtual JointModel *clone() const=0;
    /// The model object must return a value for its best estimate of the maximum normal stiffness.\n
        /// This is used by the code to compute the stable timestep or adjust inertial mass.\n
    virtual Double      getMaxNormalStiffness() const=0;
    /// The model object must return a value for its best estimate of the shear stiffness.\n
        /// This is used by the code to compute the stable timestep or adjust inertial mass.\n
    virtual Double      getMaxShearStiffness() const=0;
    /// This member function should first call the base class copy function, and then copy all essential data from the model object
        ///   pointed to by cm (assumed to be of the same derived class as the current model).\n
        /// It is not necessary to copy data members that are recomputed when the initialize() function is called.\n
        /// Use dynamic cast<MyModel *>(cm) to upcast cm.
        /// \brief Copy property values and state from another constitutive model.
    virtual void        copy(const JointModel *mod);
    /// This function is called for each subcontact at each cycle.\n
        /// The model must update the normal and shear forces from displacement increments.\n
        /// The structure State \a s contains the current force components and the computed 
        ///   displacement components for the subcontact being processed.\n
        /// \a dim will be either 2 or 3, depending on the dimension of the calling program.
        /// The base class implementation will call initialize() if isValid() returns false.
        /// \brief Calculate force increments given a displacement increments.  
    virtual void        run(UByte dim,State *s); 
    /// This function is called once for each model object (i.e., for each subcontact) when the 
        ///   SOLVE/Step/CYCLE command is given when the large-strain update is performed, and at the 
        ///   beginning of the run() method if isValid() returns false.\n
        /// The model object may perform initialization of its property or state variables, or it may do nothing.\n
        /// The dimensionality (e.g., this is 3 for 3DEC ) is given as \a dim, and structure \a s contains current information
        ///   for the subcontact containing the model object.\n
        /// Note that displacements may be undefined when initialize() is called.\n
        /// The average force components for the full zone are available in the state structure; they should not be 
        ///   changed by the initialize() member function.\n
        /// The base class implementation calls setValid(dim).
        /// \brief Initializes the constitutive model in preparation for calls to run().
    virtual void        initialize(UByte dim,State *s); 
    /// Returns the ratio of the passed shear forces to the shear yield strength of the subcontact properties.\n
        /// Implementing this function is optional.\n
        /// By convention, 10.0 is the maximum value returned.
        /// \brief Returns the ratio of the shear force to the current yield strength.
    virtual Double      getStressStrengthRatio(const State &) const { return(10.0); }
    /// This function is used to set scaled properties for the Safety Factor calculation.
    /// Implementing this function is optional.
    /// \arg f is the factor to scale.  Some properties may not scale linearly.
    /// \arg v is the list of properties (by index) that are requested to be scaled.
    /// \brief Scales failure property indices \a v by the factor \a f.
    virtual void        scaleProperties(const Double &,const std::vector<UInt> &) { std::logic_error("Does not support property scaling"); }
    /// Returns true if strength-stress ratio calculations are supported via getStrengthStressRatio().    
    virtual bool        supportsStressStrengthRatio() const { return(false); }
    /// Returns true if property scaling is supported for factor-of-safety calculations via scaleProperties().
    virtual bool        supportsPropertyScaling() const { return(false); }
    /// There should be no reason for an implementation to change the default behavior of this function.
    virtual void        destroy() { delete this; }
    ///  Return true if subcontact is sliding.
    virtual bool        isSliding(const State &) { return false; }
    ///  Return true if subcontact is bonced.  Used in fragment calculations.
    virtual bool        isBonded(const State &) { return true; }

    /// Default implementation 4 energies should be defined for each model (at least) 
    enum EnergyKeys { kwEStrainShear = 1, kwEStrainCompression, kwEStrainTension, kwESlip };
    virtual String      getEnergies() const { return L"energy-strain-shear,energy-strain-compression,energy-strain-tension,energy-slip"; }
    virtual double      getEnergy(UInt) const { return 0.0; }                   // Base 1
    virtual bool        getEnergyAccumulate(UInt) const { return false; }       // Base 1
    virtual void        setEnergy(UInt, const double&) {}                       // Base 1    
    virtual void        activateEnergy() {}
    virtual bool        getEnergyActivated() const { return false; }  // Returns a boolean indicating if energies have been instanciated

    
    /// The joint constitutive model constructor.  The model is invald, and canFail is set to true.
    JointModel();
    /// The joint constitutive model destructor.
    virtual ~JointModel();
    /// Major version changes when the interface changes - which is defined here.
    static UInt getMajorVersion();
    /// This indicates the minor version of the constitutive model library routines.\n
        /// This will be updated to indicate bug fixes, and minor changes that do not affect binary compatibility.\n
        /// \brief Returns the minor version number of the constitutive model library.
    static UInt getLibraryMinorVersion();
    /// Indicates whether initializion is necessary - by dimension
    bool isValid(UByte dimVal) const { return(valid_==dimVal); }
    /// Sets the current valid state to dimension \a dim.
    void setValid(UByte dimVal) { valid_ = dimVal; }
    /// Indicates whether failure should be allowed
    /// /// Constitutive models should honor this state, and not allow failure if it returns true.
    bool canFail() const { return(can_fail_); }
    /// Specifies whether or not "failure" is being allowed for this instanced of the constitutive model.
    void setIfCanFail(bool b) { can_fail_ = b; }
    /// Indicates whether the model was loaded as a plugin (defaults to false).
    bool getPlugIn() const { return plugin_; }
    void setPlugIn(bool b) { plugin_ = b; }

  private:
    UByte valid_;
    bool  can_fail_;
    bool  plugin_;
  };
} // namespace jmodels

// EoF
