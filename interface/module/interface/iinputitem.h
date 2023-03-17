#pragma once

#include "utility/interface/itextinput.h"

namespace itascaxd {
    using itasca::InputLine;
    using itasca::InputContext;

    class IInputItem {
    public:
        // Informational
        //   Type identifier constant.
        virtual const char *getType() const=0;
        //   Return TRUE if the input source represents an interactive input
        //     (Keyboard, GUI, etc.) 
        //   This activates things like pagination and prompts.
        virtual bool isInteractive() const=0;
        //   Return a description of the input source.
        virtual QString getName() const=0;
        //   Returns the number of lines input from this source.
        virtual int getLineNumber() const=0;
        //   Should return TRUE if one can execute a NEW statement while reading from this source.
        //   FISH command/end_command blocks, for instance, do not allow a NEW.
        virtual bool isNewAllowed() const=0;
        //   Returns TRUE if this is an input type than can be interrupted safely (and recover in a repeatable manner)
        virtual bool canSafeInterrupt() const=0;
        //   Sets the suppress status of the input item.
        //   The idea is that echoing of command processing is suspended while reading input from this source.
        //   echo_at_start is the echo state when called, which is what the echo state should be restored
        //   to when done processing this input source.
        virtual void suppressEcho(bool echo_at_start)=0;
        //   Return TRUE if echo during input processing should be suspended for this input source.
        virtual bool isEchoSuppressed() const=0;
        //   Returns the state of echo when suppressEcho() was specfied, assuming it was specified.
        virtual bool getEchoAtStart() const=0;
        //   Returns TRUE if this input item should be kept on the stack in the event of an error.
        virtual bool getKeepOnError() const=0;
        //      If not null, this will be presented at the command line instead of the prompt supplied with input.
        virtual QString getPromptOverride() const=0;
        //              Returns the path associated with input from this item, or NULL if no path is.
        virtual QString getPath() const=0;
        //              Returns the string that will be used by the CALL command to open this input item *at it's current location*.
        virtual QString getOpenString(QFileInfoList *list=nullptr) const=0;
        //              Add all files associated with this input item to the list of files associated with the model state.
        virtual void    addFiles() const=0;
        virtual bool    supportsInterruptState() const=0;
        virtual bool    getErrorExpected() const =0;
        virtual void    setErrorExpected(bool b) =0;

        // Communication
        //   Note that Open and Close will be called if the source calls another input source.
        //   Like a data file calling a new data file.  So the InputItem should be prepared
        //     to respond to a close() followed by an open() by continuing input from where
        //     it left off.
        virtual bool open()=0;
        virtual bool close()=0;
        //   Returns a single String.  Prompt is for interactive input, and start
        //     represents the initial String (also for interactive input).
        virtual InputLine input(const InputContext &context)=0;
        //   Return TRUE to indicate that there is nothing left in the item to read.
        virtual bool empty() const=0;

        virtual void destroy()=0;
    };
} // namespace itascaxd
// EoF
