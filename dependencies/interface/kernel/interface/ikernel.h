#pragma once
#include "base/src/baseqt.h"
#include "utility/interface/iinputrecord.h"
#include "utility/interface/isignalbase.h"
#include "utility/interface/itextinput.h"
#include "security/interface/isecurity.h"

/**
  * \file
  * \brief This interface provides an entry point for command processing.
  * \addtogroup kernel
  * @{
  */
namespace security {
    class ISC; //ISecurityCommunication;
    struct LicenseLocation;
}

namespace itascaxd {
    class IInputItem;
    class IRange;
}

namespace itasca {
    class Archive2;
    class IKernelInput;
    class IKernelOutput;
    class IThing;
    class IContainer;

    /// \brief This Interface provides an entry point for command processing.
    class IKernel {
    public:
        /// Defines the Signal2<> type used for signaling to FISH that the model will be reset. It will
        /// be given with null arguments and all FISH pointers will be cleared. Needs to be hooked up  
        /// via the registerSignalOnThingDelete methed.
        typedef Signal2<const IThing *,const IContainer *> signal_type;

        /// Returns the QObject associated with the kernel, for signal/slot connections
        virtual const QObject *getQObject() const=0;

        /// This must be called before the kernel object (or the code in general) can be used.
        /// \b its and \b iti are the main text communication channels.
        /// \b comm is used for supplementary things - like getKey(), information(), etc.
        /// \b poll is called during all lengthy computations to give a single-threaded GUI
        ///   the chance to respond to user input.
        virtual bool initialize(IKernelInput *input,IKernelOutput *output)=0;

        /// This is called to indicate that the GUI is shutting down - no calls to input or output should occurr.
        virtual bool disconnectFromInterface()=0;

        /// This function should be called after initialize but before processing commands.
        /// It checks the current security setting.
        /// Returns TRUE if program should shut down.
        /// \b Note - might want to modify argument in some encrypted way to verify call made correctly.
        virtual bool security(security::ISC *scomm)=0;

        /// Process command line arguments prior to restoring projects
        /// Neccesary to assing memory in fortran codes
        virtual bool preCommandLineArguments(QStringList * args)=0;

        /// This processes a list of command line arguments.  \b args may have been edited by the GUI.
        /// \b args does NOT include the traditional first argument, the command that started the process.
        /// Returns \b TRUE if the program should shut down.
        /// Should be called *after* initialize and security.
        virtual bool commandLineArguments(const QStringList &args)=0;
        virtual QList<InputLine> fromCommandLineArguments(const QStringList &args)=0;

        /// Send contents of \b ii as commands, takes ownership of ii.
        /// Returns the error message if an error resulted, or QString() if none occurred.
        /// Command processing is done immediately, unless called recursively because commands are
        ///    already being processed.  In this case the commands are added to the lowest item
        ///    on the stack and will therefore be processed after the stack is done and has
        ///    exhausted prior commands.
        /// Will catch and handle exceptions thrown during command processing.
        virtual QString processCommands(itascaxd::IInputItem *ii,const QString &inputPath=QString(),
                                        bool recursive=false,bool suppress=false)=0;

        /// Send contents of \b sl as commands.   
        /// Command processing is done immediately, unless called recursively because commands are
        ///    already being processed.  In this case the commands are added to the lowest item
        ///    on the stack and will therefore be processed after the stack is done and has
        ///    exhausted prior commands.
        /// Returns the error message if an error resulted, or QString() if none occurred.
        /// \b source and \b line indicate the source of the commands in \b sl, and the line
        ///        number of the first line in \b sl from the original source.
        /// \b inputPath indicates the absolute pathname of the source file name.
        /// Will catch and handle exceptions thrown during command processing.
        virtual QString processCommands(const QList<InputLine> &sl,
                                        const QString &inputPath=QString(),bool recursive=false)=0;

        /// Send contents of \b s as a command.  
        /// Command processing is done immediately, unless called recursively because commands are
        ///    already being processed.  In this case the commands are added to the lowest item
        ///    on the stack and will therefore be processed after the stack is done and has
        ///    exhausted prior commands.
        /// Returns the error message if an error resulted, or QString() if none occurred.
        /// \b source and \b line indicate the source of the commands in \b sl, and the line
        ///        number of the first line in \b sl from the original source.
        /// \b inputPath indicates the absolute pathname of the source file name.
        /// Will catch and handle exceptions thrown during command processing.
        virtual QString processCommand(const InputLine &s,const QString &inputPath=QString(),
                                       bool recursive=false)=0;

        /// Return the test mode flag
        virtual bool getAutoTestMode() const=0;

        /// Returns all directories current part of the testing system.
        virtual QStringList            getTestDirectories() const=0;

        /// Returns a pointer to a IKernelOutput object.
        virtual IKernelOutput *getIKernelOutput()=0;

        /// Add cmd-doc definitions from a StringList
        virtual void addCommandDocumentation(const QString &referencename,const QStringList &list)=0; 

        /// Add inline help commands
        virtual void addSphinxCommands(const QStringList &)=0;

        /// Add inline help FISH
        virtual void addSphinxFish(const QStringList &)=0;

        /// Tests \a line as a line from a data file for validity, using internal doc data.
        /// Returns -1 if OK, otherwise returns the number of the first bad token in line.
        /// \a errorinfo may be filled with a specific error message.
        virtual int testValidCommand(const QString &line, QString *errorinfo=0) const=0;

        /// Tests the context of \a text as multiple lines of text, separated by '\n'.
        /// Returns the number of errors encountered (0 if none).
        /// \a errorinfo may be filled with a specific error message.
        /// \a errorlines is filled with the character positions where errors were found in each line of text, -1 indicating no error. (??)
        virtual int testValidCommandText(const QString &text, QString *errorinfo=0, QList<int> *errorlines=0) const=0;

        /// Given a line of proposed input to the command processor, return the full expanded command and first keyword from the first two tokens.
        /// Will return null in either if there is no match, or if there is no valid keyword or token.
        virtual QPair<QString,QString> expandCommandKeyword(const QString &line,QString &fileName,int cursor=-1) const=0;
        /// Given a line of proposed input to the command processor, returns information
        /// regarding the signature of the most recent command/FISH function referenced.
        virtual QStringList getCommandInformation(const QString &line,int cursor=-1,bool thinkFish=false) const=0;

        /// Get the name of the last save file
        virtual QString getSaveFileName() const=0;

        ///  Model changed state.
        virtual bool modelChanged() const=0;

        ///  Project changed state.
        virtual bool projectChanged() const=0;

        /// Project archive
        virtual void saveProject(Archive2 &a) const=0;
        virtual bool restoreProject(Archive2 &a,quint64 label)=0;
        virtual void remapProject(Archive2 &a)=0;

        /// Checks the input stream \a input, which is assumed to be the start of a valid save file.
        /// It checks for the existence of an ASCII format save file header, and returns FALSE if one is not found.
        /// If one *is* found, it returns the contents of the header in \a header.
        virtual bool readSaveFileInfo(QIODevice *dev,IInputRecord::FileDataSet *set,QStringList *lines) const=0;

        ///  Compress a list of save files. Two different SIGNALS are emitted
        ///  during compression of a list of files. compressingFileIndex(int) gives
        ///  the index of the file which is currently being processed, this is
        ///  an integer between 0 and files.size()-1.
        ///  compressingFileProgress(int) give the progress of the file which
        ///  is currently being compressed, this is an integer between 0 and
        ///  100. Throws an exception if there is an error.
        virtual void compressFileList(const QStringList &files)=0;
        virtual void decompressFileList(const QStringList &files)=0;
        /// Return true if the given file has been compressed.
        virtual bool fileIsCompressed(const QString &filename) const=0;

        // RESULT FILE SUPPORT
        struct ResultsDesc {
            QString name_;
            QString command_;
            bool    state_ = false;
            QString category_;
        };
        virtual std::vector<ResultsDesc>   getResultsState() const=0;
        // TRUE if results files will be generated automatically during cycling.
        virtual bool                       getResultsIntervalOn() const=0;
        // non-zero if results files will be generated based on a real-time interval.
        virtual quint64                    getResultsIntervalClock() const=0;
        // The name of the process used to determine the model time -- from the list of
        //    processes available to the current model.
        virtual QString                    getResultsIntervalProcess() const=0;
        // non-zero if results files will be generated based on the number of cycles/steps.
        virtual quint64                    getResultsIntervalStep() const=0;
        // non-zero if results files will be generated based on accumulated model time
        //    in the process returned by getResultsIntervalProcess().
        virtual double                     getResultsIntervalTime() const=0;
        // Returns the prefix used for automatically generated results files.
        virtual QString                    getResultsIntervalPrefix() const=0;
        // Returns the current index used for automatically generated results files.
        // This is appended to the prefix and incremented automatically.
        virtual quint32                    getResultsIntervalIndex() const=0;

        // Signals/Slots
        /// This SIGNAL is emitted when a new InputItem is pushed onto the input stack (a data file, FISH command/endcommand, etc).
        /// \arg type indicates the type of the new input.  Possible values include "keyboard", "Stringlist", and "file".
        /// \arg name is an identifying name of the input source.
        /// \arg start indicates this is the start of a new call tree.
        virtual void pushInputItem(const char *type,const QString &name,bool start)=0;

        /// This SIGNAL is emitted when an inputitem is popped off the input stack.
        /// \arg type indicates the type of the new input.  Possible values include "keyboard", "Stringlist", and "file".
        /// \arg name is an identifying name of the input source.
        virtual void popInputItem(const char *type,const QString &name)=0;

        /// This SIGNAL is emitted when the model state has been saved to a file.
        /// \arg name is the absolute path and file name of the save file created or overwritten.
        //virtual void saveModelState(const QString &name)=0;

        /// This SIGNAL is emiited when a model is restored from a saved state.
        /// \arg name is the absolute path and file name of the save file created or overwritten.
        virtual void restoreModelState(const QString &name)=0;

        /// This SIGNAL is emitted when the model state is reset, (a NEW command, for example).
        virtual void resetModelState()=0;

        virtual void exportResultFile(const QString &name)=0;
        virtual void importResultFile(const QString &name)=0;

        /// This SIGNAL Indicates that a file is being processed by the kernel.
        /// \arg fullPathName indicates the full file and path name begin processed.
        /// \arg type indicates the type of file being processed.  At present this will be "Grid", indicating an import grid file.
        virtual void startProcessingFile(const QString &fullPathName,const QString &type)=0;

        /// This SIGNAL Indicates that a file has finished being processed by the kernel, after a call to startProcessingFile().
        /// \arg fullPathName indicates the full file and path name begin processed.
        /// \arg type indicates the type of file that was processed.  At present this will be "Grid", indicating an import grid file.
        virtual void stopProcessingFile(const QString &fullPathName,const QString &type)=0;

        /// This SIGNAL indicates which file is being compressed during a call to compressFileList.
        /// The value is an integer between 0 and files.size()-1.
        virtual void compressingFileIndex(int index)=0;        
        /// This SIGNAL indicates the compression progressof the file presently being compressed during a call to compressFileList. 
        /// The value is an integer between 0 and 100.
        virtual void compressingFileProgress(int progress)=0;

        /// These are commands to drive the engine from a module: BE CAREFUL!
        /// Should clear the entire model and ready for the creation of a new one.  Called by NEW.
        virtual void resetModel(bool quiet=false)=0; 

        ///   Clears PROJECT level memory (filter, persistant FISH, etc).  Called by NEW PROJECT.
        virtual void resetProject(bool quiet=false)=0;

        ///   Asks the model the "clean" itself - doing whatever calculations are necessary to make itself valid for 
        ///    cycling after whatever changes were made.
        virtual void cleanModel(bool all=false)=0;

        ///   Call a calm method defined in each module. This "calms" the model in a pre-defined and 
        ///    module specific way. For mechanical rigid bodies, all dofs of velocity are set to 0.
        virtual void calmModel()=0;

        ///   Call a filter method in each module.
        virtual void filterModel(const itascaxd::IRange &)=0;

        ///   Return the signal indicating that the model will be reset, i.e., it is emitted before a reset.
        virtual signal_type *getResetSignal() const=0;

        ///   Return the signal indicating that the model will be saved, i.e., it is emitted before a save.
        virtual signal_type *getSaveSignal() const=0;

        ///   Return the signal indicating that the model has been restored. i.e., it is emitted after a restore.
        virtual signal_type *getRestoredSignal() const=0;

        ///    Saves to filename name.  Checks for file space and opens data stream. 
        ///    Assigns indices to containers.
        virtual void    save(QIODevice *dev,bool readOnly,bool text)=0;

        ///   Restores file from filename name.  Clears current model and opens data stream.
        ///   This also loads the header (calls restoreHeader) and checks for save file validity.
        ///   This function also calls remap() and clears containers of index arrays.
        virtual QStringList restore(QIODevice *dev,ItascaTextStream *out=0)=0;

        virtual void logEvent(const QString &str)=0;

        ///   Get the boolean indicating cycling mode
        virtual bool getInCycle() const=0;

        virtual QStringList getAllTypeKeywords() const=0;

        ///   This SIGNAL is emitted if the engine has declared the code needs to shut down.
        ///   Due to a QUIT or a STOP, or a catastrophic error, etc.
        virtual void abort()=0;

        /// Load the MPI module
        virtual void loadMPI()=0;
    };
}
/// @}
// EoF
