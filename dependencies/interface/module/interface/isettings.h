#pragma once

#include "base/src/baseqt.h"

/**
    * \file
    * \brief Interface for accessing kernel settings.
    */

namespace itasca {
    /// \brief Interface for accessing kernel settings.
    /** These are global values that affect the program.
        * \ingroup module
    */
    class ISettings {
    public:
        /// Returns a pointer to a QObject representing this object.
        virtual const QObject *getQObject() const=0;

        /// Returns TRUE if string comparisons are made case sensitive or not.\n
        /// This includes macros, command matching, FISH, etc.
        //virtual bool getCaseSensitivity() const=0;

        /// Returns TRUE if text output is being sent to a log file as well.
        virtual bool getLog() const=0;

        /// Returns the name of the log file (whether logging is on or not).
        virtual QString getLogFileName() const=0;

        /// Returns TRUE if macros and fish symbols require a prefix character to be
        ///<   interpreted by the command processor.  Default is true.
        virtual bool getSafeConversion() const=0;

        /// Returns TRUE if command input is being printed out when received.
        virtual bool getEcho() const=0;

        /// Returns the customer title (1) set by the user and stored persistantly. \see getCustomerTitle2()
        virtual QString getCustomerTitle1() const=0;
        /// Returns the customer title (2) set by the user and stored persistantly. \see getCustomerTitle1()
        virtual QString getCustomerTitle2() const=0;

        /// Returns the Job title associated with the current model.
        virtual QString getJobTitle() const=0;

        /// Returns TRUE if a Left Handed coordinate system is being used.
        virtual bool getLHS() const=0;

        /// Returns the current license terms.\n
        // e.g.: (null) = Full or Non-Cycling license, "Demonstration" License, "Academic" License, etc.
        virtual QString getLicense() const=0;

        /// True if any cycling was ever done under an academic license.
        virtual bool getAcademicModel() const=0;

        /// True if any cycling was ever done under a demo license.
        virtual bool getDemoModel() const=0;

        /// Returns the frequency of notices during cycling.
        virtual uint getUpdateInterval() const=0;

        /// True if the model state has been tagged as read only.
        virtual bool getReadOnly() const=0;

        /// True if the model state has been tagged as being unrepeatable, indicating that
        ///   rerunning the model from an input record will not duplicate the model state.
        virtual bool getUnrepeatable() const=0;

        // True if the current model state was read from a results file.
        //    Cycling (and maybe other things) is disabled in this state.
        virtual bool getResultsState() const=0;

        virtual bool   getAutoModelSave() const=0;
        virtual double getAutoModelSaveInterval() const=0;
        virtual QString getAutoModelSaveDirectory() const=0;

        /// Sets the frequency of notices during cycling.
        virtual void setUpdateInterval(uint u)=0;
        /// Sets the log file on or off
        virtual void setLog(bool b,QIODevice::OpenMode=QIODevice::Append)=0;
        /// Sets the current log file
        virtual void setLogFileName(const QString &s,QIODevice::OpenMode=QIODevice::Append)=0;

        virtual void setAutoModelSave(bool b)=0;
        virtual void setAutoModelSaveInterval(double d)=0;
        virtual void setAutoModelSaveDirectory(const QString &s)=0;

        // signals
        /// This signal is emitted when the case sensitivity state has changed. See getCaseSensitivity()
        //virtual void caseSensitivityChanged(bool b)=0;
        /// This signal is emitted when the log state has changed. See getLog().
        virtual void logChanged(bool b)=0;
        /// This signal is emitted when the log file name has changed. See getLogFileName()
        virtual void logFileNameChanged(const QString &name)=0;
        /// This signal is emitted when the safe conversion state has changed. See getSafeConversion().
        virtual void safeConversionChanged(bool b)=0;
        /// This signal is emitted when the echo state has changed. See getEcho().
        virtual void echoChanged(bool b)=0;
        /// This signal is emitted when the customet title 1 has changed. See getCustomerTitle1().
        virtual void customerTitle1Changed(const QString &)=0;
        /// This signal is emitted when the customet title 2 has changed. See getCustomerTitle2().
        virtual void customerTitle2Changed(const QString &)=0;
        /// This signal is emitted when the job title has changed. See getJobTitle().
        virtual void jobTitleChanged(const QString &)=0;
        /// This signal is emitted when the Left Hand coordinate System state has changed. See getLHS().
        virtual void LHSChanged(bool b)=0;
        /// This signal is emitted when the license has changed. See getLicense().
        virtual void licenseChanged(const QString &s)=0;
        /// This signal is emitted when the academic model state has changed. See getAcademicModel(). 
        virtual void academicModelChanged(bool b)=0;
        /// This signal is emitted when the demo model state has changed. See getDemoModel()
        virtual void demoModelChanged(bool b)=0;
        /// This signal is emitted when the update interval has changed. See getUpdateInterval(), setUpdateInterval
        virtual void updateIntervalChanged(uint u)=0;
        /// This signal is emitted when the unrepeatable state of the model has changed.  See getUnrepeatable.
        virtual void unrepeatableChanged(bool b)=0;

        virtual void autoModelSaveChanged(bool b)=0;
        virtual void autoModelSaveIntervalChanged(double d)=0;
        virtual void autoModelSaveDirectoryChanged(const QString &s)=0;
    };
} // namespace itasca
// EoF
