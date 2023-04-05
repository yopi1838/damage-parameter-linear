#pragma once

namespace itasca {
    class ITextOutput;
    class IParse;

    /** \brief Interface describing ALL output support the kernel needs to send information to whatever interface is driving it.
    *
    * Centralized pipe for *all* output the kernel needs from outside the kernel process itself.
    * ingroup utility
    */
    class IKernelOutput {
    public:
        /// Enumeration specifying the meaning of the cyclingOutput method.
        /// In terms of the GUI cycling indicator:
        ///     Info is the line (or lines) of text above the header, with general information, also indicates start.
        ///     Header are the headers for each column of text updated during cycling.
        ///     Data is the first values in each column (current values).
        ///     Data2 is the second values in each column (limits).
        ///     Close indicates cycling ended.
        enum CycleOutputMode { InfoCycleOutputMode,  ///< Info strings passed, cycling started
                               HeaderCycleOutputMode, ///< Header strings passed, cycling ongoing
                               DataCycleOutputMode,   ///< Data strings passed, cycling ongoing
                               Data2CycleOutputMode,  ///< Second row of data strings passed.
                               CloseCycleOutputMode   ///< Cycling stopped, strings ignored
                             };
        /// Used to output helpful information messages. May throw a quit exception or interrupt exception.
        virtual void         information(const QString &s)=0;                                      
        /// Used to output warning messages, something might be wrong but it isn't an error that should stop 
        /// processing. May throw a quit exception or interrupt exception.
        virtual void         warning(const QString &s,uint level,int line,const QString &file)=0;  
        /// Used to output error messages - called from Kernel after catching exception. May throw a quit exception or interrupt exception.
        virtual void         error(const QString &s,int line,const QString &file,bool expected)=0;
        /// Progress indicator.  Initiate with a negative percentage and a valid message.\n
        /// Update with a percentage between 0 and 100 inclusive (message not used).\n
        /// Finish with a per over 100 (message not used).\n
        /// May throw a quit exception or interrupt exception.
        virtual void         progress(int per,const QString &message)=0;
        /// Displays wait <b>message</b>. If show is false, message is cleared from output device. Function does not block, but it may throw quit exception.
        virtual void         waitNotice(bool show,const QString &message)=0;                       
        /// Parses a GUI command. May throw any exception. Not supported in console interface.
        virtual void         interfaceGuiCommand(const itasca::IParse &par)=0;                             
        /// Parses a PLOT command. May throw any exception. Not supported in console interface.
        virtual void         interfacePlotCommand(const itasca::IParse &par)=0;                            
        /// Returns a pointer to ITextOutput.
        virtual ITextOutput *getITextOutput()=0;
        /// Indicates that output 'information' mode is on.
        virtual void         informationOn(bool b)=0;
        virtual bool         informationOn() const=0;
        /// Indicates that output 'warning' mode is on.
        virtual void         warningOn(bool b)=0;
        virtual bool         warningOn() const=0;
        /// Sets output global pagination ON/OFF.
        virtual void         globalPaginationOn(bool b)=0;
        /// Output information during cycling, according to the CycleOutputMode.  Any exception.
        /// The modes are defined in CycleOutputMode above.
        virtual void         cyclingOutput(uint mode,const QStringList &list)=0;
        virtual void         logEvent(const QString &str)=0;
        /// Add a help file entry in the menu. /a name is the name given to the menu item, /a file is 
        /// the help file name, and /a top denoted whether to add it to the top or bottom of the menu items.
        virtual QObject *    addHelpEntry(const QString &name,bool top)=0;
        /// Display the contents of a help file
        virtual void         displayHelpFile(QObject *parent,const QString &file, const QString &pagePath,quint32 command=0, const QString &windowName = "Mainwin")=0;
        /// Add an entry for opening files of a different type if a module is loaded
        //virtual void         addAlternateDataFilePrefix(const QString &pref)=0;

        virtual bool acceptRedirect() const=0;
        virtual void setRedirectOutput(IKernelOutput * ko)=0;
    };
} // /namespace itasca
// EOF
