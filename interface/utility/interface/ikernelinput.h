#pragma once

namespace itasca {
    class IGUIInput;
    class ITextInput;

    /** \brief Interface describing ALL input support the kernel needs to receive from whatever interface is driving it.
    *
    * Centralized pipe for *all* input the kernel needs from outside the kernel process itself.
    * This includes input from the user interface, as well as (eventually) input from the file system, 
    * from sockets, and interprocess communication.\n
    * This class is a strict singleton - so all input from outside should (theoretically) go through here first.
    * ingroup utility
    */
    class IKernelInput {
    public:
        ///  Returns a pointer to ITextInput, the standard default text input 
        virtual ITextInput *getITextInput()=0;

        // Kernel specific more detailed input
        /// Returns a key. This function blocks, and it may throw a 'quit' exception.
        virtual Qt::Key getKey()=0;                                           
        /// Prompt for user input. This function blocks, and it may throw a 'quit' exception.
        /// \param message the message that will be displayed to the user
        /// \param options list of messages
        /// \param keys list of keys allowed to be entered by the user
        /// \param ok OK return code    
        /// \param cancel CANCEL returns code
        virtual uint    prompt(const QString &message,const QString &options,const QString &keys,uint ok,uint cancel)=0;
        /// Returns the name of the UI input device, e.g. "FLAC3D Console". This functions does not throw exceptions.
        virtual QString getUIName() const=0;
        /// Returns true if system is in test mode. This functions does not throw exceptions.
        virtual bool    autoTestMode() const=0;
        /// Returns the name of the front page of the help file
        virtual QString getHelpFileFrontPage() const=0;
        /// Returns the name of the directory of the doc files which could be code dependent
        virtual QString getDocDirectory() const=0;
        /// Returns the name of the revision history page in the help file
        virtual QString getLatestRevisionAddress() const=0;

        // Interrupt functions
        /// Allows GUI to interrupt if necessary. Quit exception if suppressExeption is false 
        virtual void poll(bool suppressException=false,bool skipTimingTest=false)=0;
        /// Polls as well as checks if GUI has indicated kernel should stop immediately even if left in an invalid state.
        /// May throw quit exception, returns false (interrupt exception) 
        virtual bool hasBeenInterrupted()=0;                
        /// Poll as well as checks if GUI has indicated kernel should stop at the next safe location.
        /// May throw quit exception, returns true (interrupt exception)
        virtual bool hasBeenSafeInterrupted()=0;            

        /// GUI Input elements
        /// Returns a pointer IGUIInput, the interface to graphics user interface input functionality. It may return null.
        virtual IGUIInput *getIGUIInput()=0;  

        // Undo support
        /// Fills obj and slotsignal with the signal and slot to connect undoAvailable signals to (uses Qt:queuedConnection) - UNDO support
        /// Undo Avail signal has signature function(bool).   PlaybackStartSignal has signature function(bool,double);
        virtual void getUndoConnect(QObject **obj,const char **undoAvailSignal,const char **playbackStartSignal)=0; 
        /// Returns TRUE if the last string input from ITextInput::getInput() was marked as the start of an undo block.
        //virtual bool getUndoStartMark()=0; 

        /// Return the project directory
        virtual QString getProjectPath() const = 0;

        virtual bool acceptRedirect() const=0;
        virtual void setRedirectInput(IKernelInput * ki)=0;
    };
} // /namespace itasca
// EOF
