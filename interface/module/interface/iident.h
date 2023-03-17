#pragma once

#include "base/src/baseqt.h"

/**
    * \file
    * \brief Central class for information identifying the particular program
    * using the kernel library.
    */

namespace itasca {
    /// \brief Central class for information identifying the particular program using the kernel library.
    /// \ingroup module
    class IIdent {
    public:
        /// Possible code types
        enum class CodeType {
            Release=0,    /*!< Release code.  Available for purchase.*/
            Alpha=1,      /*!< Alpha code. In active development, may change at any time, no guaranteed of future file compatibility.*/
            Beta=2,       /*!< Beta code. Feature complete, being tested internally to Itasca.*/
            PreRelease=3, /*!< Pre-Release code. Feature complete, partially documented, being tested internally and externally by pre-purchasers*/
            NewVersionAvailable=4,     /*!< The next version has been made available.  This version is still supported for now.  Customers should upgrade. */
            Unsupported=5 /*!< This version is no longer officially supported. Upgrade to the latest version */
        };

        /// Returns the default file name (without extension) for files
        ///< from this program.  Like "flac3d".
        virtual QString getDefaultFileName() const=0;

        /// Returns a text string with full version information. Like "3.10 Beta 453".
        virtual QString getVersionString() const=0;

        /// QString to place persistant storage under. "flac3d400".
        virtual QString getRegistryString() const=0;

        /// This returns a string identifying what branch this code is under, if any
        virtual QString getBranch() const=0;

        /// For instance, the '3dec' in the extension '3decsav'.
        //virtual QString getExtensionPrefix() const=0;

        /// Returns the current code status - alpha, beta, or release.
        virtual CodeType getCodeType() const=0;

        /// Returns the current code major version number - like 500 for version 5.00
        virtual uint getCodeVersion() const=0;
    };
} // namespace itasca
// EoF
