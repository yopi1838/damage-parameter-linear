#pragma once
/** \file ifishdebug.h
* \brief Interface for FISH debugging.
*/

namespace fish {
    /** \brief Interface for FISH debugging.
    * \ingroup fish
    */
    class IParameter;

    class IDebug {
    public:
        virtual QString              getSource()=0;
        virtual qint64               getLineNumber()=0;
        virtual QPair<qint64,qint64> getColumnRange()=0;
        virtual QString              getFunction()=0;
        virtual qint64               getInstruction()=0;
        virtual void                 stepLine(bool into)=0;
        virtual void                 stepInstruction(bool into)=0;
        virtual void                 stepToLine(qint64 u)=0;
        virtual void                 stepToInstruction(qint64 u)=0;
        virtual void                 run()=0;
        virtual void                 setBreakAtInstruction(qint64 code,bool on)=0;
        virtual void                 setBreakAtFunction(const QString &function,bool on)=0;
        virtual void                 setBreakAtLine(qint64 line,bool on)=0;
        virtual void                 clearAllBreakpoints()=0;
        virtual QStringList          getLocalNames()=0;
        virtual Parameter            getLocalValue(qint64 index)=0;
        virtual QStringList          getCallStack()=0;
    };
} // namespace itasca
  // EOF
