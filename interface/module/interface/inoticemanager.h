#pragma once

#include "base/src/baseqt.h"

/**
    * \file
    * \brief Interface to the notification system.
    */

namespace itasca {
    /// \brief Interface to the notification system.

    /** This is a central place for change notifications to occur.
        * The idea is that the engine can quickly tag things as being changed,
        * but the notification to the GUI (or whoever) doesn't happen until later.
        * Then at distinct times (for efficiency) the GUI can be notified of all the changes at once.
        * This allows *every* zone (for instance) to notify the system that stresses have changed,
        *  without burdening the GUI (and engine) with 20,000 notification messages.
        * The basic idea is that tag() is fast, and notify() is slow.

        * This class is used by registering a notification type (identified by a string),
        *  which returns an ID uniquely identifying that notice.

        * To indicate that something has changed, call the tag() method with the ID returned
        *   from reserve().
        * The notify() method notifies all observers of all changes since the last notify() call.
        * \ingroup module
    */
    class INoticeManager {
    public:
        /// This returns an QObject which sends the signal notification(uint level,const quint64 &mask).
        ///< For instance, codes i = 0-63 is turned into level 0 with a mask of (1<<i).
        ///< This allows 64 notice indicators to be carried in a single notice message.
        ///< Note that if more than 64 notice types are registered, then 
        ///<   more than one sendNotice() method is called per notify().
        virtual const QObject *getQObject() const=0;

        /// Returns a unique ID given a name (like "table" or "zone stress").\n
        /// If name has already been registered an exception will be thrown.\n
        /// This ID should be stored somewhere for fast reference later.
        virtual uint reserve(const QString &name)=0;

        /// Releases an ID back to the Notice Mananger so it can be re-used.\n
        /// This is helpful to create notices for things that may later be deleted.\n
        /// This way the ID pool doesn't constantly increase, causing extra overhead.
        virtual bool release(uint u)=0;

        /// Checks to see if string name has already been reserved (given and ID number).
        virtual bool check(const QString &name)=0;

        /// Returns the ID number associated with a given name.\n
        /// Throws an exception if name has not been reserved().
        virtual uint find(const QString &name)=0;

        /// Tags ID u as having been changed, which will be sent out in the next notify() call.
        virtual void tag(uint u,bool modelChanged=true)=0;

        /// Notifies all observers of changes (in tag() calls) since the last notify() call.\n
        /// Emits signal notification(uint level,const quint &mask);
        virtual void notify()=0;

        /// Returns the Update Interval notice - sent to indicate that an update interval has occurred during cycling.
        virtual uint getUpdateInterval() const=0;

        /// Returns the notice code used to indicate that a FISH symbol has changed.  
        // MOO NOTE: Will probably want to move somewhere else later.
        virtual uint getFISHSymbolNotice() const=0;

        // signals
        /// This signal is emitted to notify that a given event has occurred. See notify()
        virtual void notification(QSet<uint> set)=0; 
        /// This signal is emitted when a cycle is completed. 
        virtual void cycleCompleted(const quint64 &step)=0;
    };
} // namespace itasca
// EoF
