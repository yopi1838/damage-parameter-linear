#ifndef _ITEXTINPUT_H
#define _ITEXTINPUT_H

namespace itasca {
    /** \brief Provides a interface for getting text.
    *
    * Provides interface for getting.
    * \ingroup utilitysrc
    */
    struct InputLine {
        inline InputLine(const QString &t,const QString &s,uint l,bool us,bool in=false) 
            : text_(t), source_(s), line_(l), undoStart_(us), interactive_(in), newAllowed_(true),
              safeInterruptAllowed_(true), undone_(false) { }
        QString text_;        // Actual text line input.
        QString source_;      // Description of where the input came from, terminal, file, etc.
        uint    line_;        // Line number of where the input came from, if applicable.
        bool    undoStart_;   // TRUE if this line is a start of a block of 1 or more lines that 
                              //      can be undone.
        bool    interactive_; // TRUE if this line came from an interactive source.
        bool    newAllowed_;  // TRUE if a model reset is allowed while processing this input
        bool    safeInterruptAllowed_; // TRUE if the input source supports repeatable interrupting.
        bool    undone_; // TRUE if a command that has been undone with smart undo, or a smart 
                         //      undo command.
    };

    struct InputContext {
        inline InputContext() : wait_(true), undoAllowed_(true), continuation_(false) { }
        QString prompt_;       // Prompt to use for interactive sources.
        QString starting_;     // Starting contents (if any) for prompted input.
        bool    wait_;         // If TRUE will wait till input available, otherwise if none
                               //    available will return with a null string immediately.
        bool    undoAllowed_;  // If FALSE this input cannot be used as a starting mark for an UNDO 
                               //     operation, must skip over.
        bool    continuation_; // If TRUE this input is considered a continuation of a previous 
                               //     input, for the purposes of collecting input lines 
                               //     representing a single operation.
        bool    checkExpectedError_ = true; // Check each line of input for first '!' character, and tag input source if present.
    };

    class ITextInput {
    public:
        /// Returns a single line of input from the current source.  Will automatically
        /// pop the stack and use the next source if the current one runs out.
        /// If undoTag is false, the line will be marked to be skipped over when finding a position for undo.
        /// If no input is available, this function will return immediately with a NULL string, 
        ///     unless wait is true then it should block till a string can be returned.
        virtual InputLine getInput(const InputContext &context)=0; 
    };
} // /namespace itasca
#endif
// EOF

