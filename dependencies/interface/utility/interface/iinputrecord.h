#pragma once

#include "base/src/baseqt.h"

namespace itasca {
    class Archive2;

    /// The input record maintains a list of all commands used in the creation of the model.
    class IInputRecord {
    public:
        /// This struct holds file information stored in FileDataSet.
        struct FileData {
            QString   name_; ///< filePath relative to current directory *when added*
            QDateTime time_; ///< last modified time/date stamp *when added*
            qint64    size_; ///< size of the file *when added*
            QString   role_; ///< string used to indicate the file role, "save", "call", etc.
            quint32   order_; ///< order in which this file was added to the input record, starts with 1.
        };
        /// A list of files.
        using FileDataSet = QSet<FileData>;
        
        /// Return a QObject pointer to this object - useful for connecting Qt signals and slots.
        virtual QObject *      getQObject()=0;
        /// Return a const QObject pointer to this object.
        virtual const QObject *getQObject() const=0;
        
        // Get record, not from playback;
        /// Return the total number of lines in the input record.
        virtual quint64     getTotalInputLines() const=0;
        /// Retrieve input lines stored in the input record.
        /// \a start is the line number of the first line to retrieve.
        /// \a len is the number of lines to retrieve.
        virtual QStringList getInputLines(quint64 start=0,quint64 len=limits<quint64>::max()) const=0; 
        /// Return a list of the files used in the input record.
        virtual FileDataSet getInputFiles() const=0;
        ///// Read the model state from a save file stream in.
        ///// Return the input record (throgh out) and list of files used.
        //virtual FileDataSet readFromSaveFile(Archive2 &a,QStringList *linesw) const=0;
        ///// Read a model state (\a in) and return the input record (\a out) and list of files
        ///// used in the input record.
        //virtual FileDataSet extractFromStream(QDataStream &in,QTextStream &out) const=0;
        /// Return the estimated time (in milliseconds) required to perform an undo operation.
        /// \a marksToUndo is the number of actions to undo.
        virtual quint64     getTimeToPlayback(quint64 marksToUndo) const=0;
        virtual bool        canSmartUndo(quint64 marksToUndo) const=0;
        /// Return true if an undo operation is being performed or a call file is being executed.
        virtual bool        isPlaybackActive() const=0;
        /// Filters out all non-command lines and interrupted lines, moves FISH CREATE commands to the front.
        virtual QStringList filterInputForDataFile(const QStringList &input) const=0;
        
        /// Add a file to the input record.
        virtual void addFile(const QString &fullPath,const QString &role)=0;
        //
        /// Signal emitted when a line is added
        virtual void lineAdded(const QString &line)=0;
        /// Signal emitted when the last input line is modified
        virtual void inputLineModified(qint64 line,const QString &newLine)=0;
        virtual void inputLineRemoved(qint64 line)=0;
        /// Signal emitted when afile was added to the input record
        virtual void fileAdded(const IInputRecord::FileData &fd)=0;  // save, call, etc.
        /// Resets the input record data. The value of \b lines equal to-1 indicates reset to nothing, 
        /// otherwise indicates number of lines
        virtual void dataReset(qint64 lines)=0; 
        /// Signal emitted to indicate when playback starts or stops.
        /// TRUE indicates playback started, FALSE indicates playback stopped.
        virtual void playbackActiveChanged(bool on,double timeExpected)=0; 
    };

    inline uint32 qHash(const IInputRecord::FileData &fd) {
        return qHash(fd.name_+fd.role_);
    }

    inline bool operator==(const IInputRecord::FileData &fd1,const IInputRecord::FileData &fd2) {
        if (fd1.name_!=fd2.name_) return false;
        if (fd1.time_!=fd2.time_) return false;
        if (fd1.size_!=fd2.size_) return false;
        if (fd1.role_!=fd2.role_) return false;
        return true;
    }

} // namespace itascaxd
// EoF
