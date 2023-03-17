#pragma once
/**
    * \file
    * \brief Interface to module global settings.
    */

#include "dim.h"

namespace itascaxd {
    class IContainerContact;
    class IContact;

    /// \brief Interface to modele global settings.
    /** This class allow to set and access global settings for the program.
      *  \ingroup module
      */

    class IGlobals {
    public:
        /// Returns an IThing interface for this object
        virtual const QObject *getQObject() const=0;
        /// Returns the timestep
        virtual double  getTimestep() const=0;
        /// Returns the total number of cycles
        virtual quint64 getTotalCycles() const=0;
        /// Returns the gravitational-acceleration vector 
        virtual DVect   getGravity() const=0;
        /// Returns the target number of threads to be used by default in multithreaded computations
        virtual uint    getThreads() const=0;
        /// Returns the boolean indicating whether or not orientation tracking is enabled.
        virtual bool    getOrientTracking() const=0;
        /// Returns the boolean indicating whether or not deterministic mode is enabled.
        virtual bool    getDeterministic() const=0;

        virtual double  getFactorOfSafetyTol() const=0;
        /// Last calculated mechanical factor-of-safety.
        virtual double  getFactorOfSafety() const=0;
        /// Whether determinism defaults to on or off on model reset.
        virtual bool    getDeterministicDefault() const=0;
        /// Whether new save files should be compressed
        virtual bool    getCompressFiles() const=0;
        /// Whether plugins are saved with the project.
        virtual bool    getSavePlugins() const=0;
        /// Whether help is shown in defaul browser
        virtual bool    getShowInBrowser() const=0;
        /// Get the precision for printing
        virtual uint    getPrecisionDefault() const = 0;
        /// Get the precision for printing
        virtual uint    getPrecision() const = 0;
        /// Get the dimensionality of the code
        virtual uint    getDimension() const = 0;
        /// Get the contact detection state
        virtual bool    getDetectionState() const = 0;
        // Get the large strain state
        virtual bool    getLargeStrain() const=0;
        virtual bool    getUseDeterminism() const=0;


        // SLOTS
        /// Sets the gravitational-acceleration vector 
        virtual void setGravity(const DVect &v)=0;
        /// Sets the target number of processors to be used by default in multithreaded computations
        virtual void setThreads(uint i)=0; 
        /// Sets the orientation tracking state.
        virtual void setOrientTracking(bool b)=0;
        /// Sets the deterministic mode state.
        virtual void setDeterministic(bool b)=0;
        /// Sets whether determinism os on or off on model reset.
        virtual void setDeterministicDefault(bool b)=0;
        /// Sets whether new save files get compressed.
        virtual void setCompressFiles(bool b)=0;
        /// Sets whether saving plugins with the project is on or off.
        virtual void setSavePlugins(bool b)=0;
        /// Sets whether help is shown in browser.
        virtual void setShowInBrowser(bool b)=0;
        /// Set the default precision for printing
        virtual void setPrecisionDefault(int i)=0;
        /// Set the precision for printing
        virtual void setPrecision(int i)=0;
        /// Set the contact detection state
        virtual void setDetectionState(bool)= 0;
        // Set the large strain state
        virtual void setLargeStrain(bool)=0;

        // SIGNALS
        /// Signal emitted when the timestep has changed
        virtual void timestepChanged(const double &d)=0;
        /// Signal emitted when the gravity vector changed
        virtual void gravityChanged(const DVect &v)=0;
        /// Signal emitted when the target number of threads to be used by default in multithreaded computations
        virtual void threadsChanged(uint)=0;
        virtual void factorOfSafetyTolChanged(double d)=0;
        virtual void factorOfSafetyChanged(double d)=0;
        /// Signal emitted when the deterministic mode has changed
        virtual void determinismChanged(bool)=0;
        /// Signal emitted when somebody changes default deterministic setting on model reset
        virtual void deterministicDefaultChanged(bool)=0;
        /// Signal emitted when somebody changes the "compress new save files and result files" setting
        virtual void compressFilesChanged(bool)=0;
        /// Signal emitted when somebody changes save plugins on a project save
        virtual void savePluginsChanged(bool)=0;
        /// Signal emitted when somebody changes the "show help in default web browser" setting
        virtual void showInBrowserChanged(bool)=0;
        /// Signal emitted when the default precision is changed
        virtual void precisionDefaultChanged(uint)=0;
        /// Signal emitted when the precision is changed
        virtual void precisionChanged(uint)=0;
        /// Signal emitted when the contact detection state is changed
        virtual void detectionStateChanged(bool b)=0;
        /// Signal emitted when the large strain state is changed
        virtual void largeStrainChanged(bool b)=0;

    };
} // namespace itascaxd
// EOF
