#pragma once
/** \file ifishinput.h
* \brief This class provides FISH with input data.
* \defgroup fish FISH interface specification
*/
namespace itasca {
    class IGUIInput;
}

namespace fish {
    class IDebug;

    /// \brief This class provides FISH with input data.
    class IInput {
    public:
        /// Stops FISH processing and waits, providing an IFishDebug interface to give the function implementation a chance to debug the FISH code.
        /// This interface can be used to query variables, step through the function, etc.
        /// This function is intended to be the main interface to a FISH debugger implemented by the program controlling FISH.
        /// \param fishdebug A pointer to the IFishDebug interface that will be used to control the debug session.
        /// \param message   A message indicating why FISH processing was stopped and control passed to the debugger.
        virtual void        debug(IDebug *fishdebug,const QString &message)=0;
        /// Checks if the system supports FISH intrinsic plugins. Non-null string indicates error message.
        virtual QString     checkIntrinsicPlugin()=0; 
        /// Allows GUI to interrupt if necessary, will throw InterruptException(false) if indicated by the system.
        /// Quit command will throw an exception.
        virtual void        poll()=0;  
        /// Returns interface to graphics user interface input functionality. May return a null value. 
        virtual itasca::IGUIInput *getIGUIInput()=0; 
        /// Returns TRUE if command processing is in TEST mode.
        virtual bool        isTestMode()=0;
        /// Returns a single line of input from the current source.  Will automatically
        /// pop the stack and use the next source if the current one runs out.
        /// \param prompt The prompt to use on the command line if appropriate.
        /// \param interactive TRUE if the FISH input source is considered "interactive", i.e. the next line from a data file is not appropriate.
        virtual QString     getInput(const QString &prompt,bool interactive)=0;
        /// Checks for authorization to perform a security restricted activity.
        /// (Anything requiring a license to perform).
        //virtual bool        checkAuthorization()=0;
    };
} // namespace itasca
  // EOF
