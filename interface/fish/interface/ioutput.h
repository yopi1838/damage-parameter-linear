#pragma once

/** \file ifishoutput.h
* \brief This class provides FISH with output data.
* \defgroup fish FISH interface specification
*/

namespace itasca {
    class ITextOutput;
    struct InputLine;
}

namespace fish {
    /// \brief This class provides FISH with output data.
    class IOutput {
    public:

        /// Send contents of \b sl as commands.   Returns \b error message (if any) instead of throwing exception.
        virtual QString              sendCommand(const std::vector<itasca::InputLine> &inputLines,bool suppress)=0;
        /// Return a pointer to the ITextOutput object to which this refers.
        virtual itasca::ITextOutput *getITextOutput()=0;
        virtual QDir                 currentInputDirectory() const=0;
        virtual QString              projectPath() const=0;
    };
} // namespace fish
  // EOF
