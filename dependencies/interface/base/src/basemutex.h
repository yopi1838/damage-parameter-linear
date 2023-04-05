#pragma once

#include "baseexception.h"


// MOO NOTE: This should probably not be used any more in new code, instead use std::mutex
//           The problems it addressed have largely been solved by better runtime checking
//           and debugger support.


/** \brief An automatic QMutex lock/unlock helper class.
*
* Similar to QMutexLocker, but adds some debugging aids.\n
* Specifically, file,line tracking of who is asking for the mutex.\n
* Also, allows a "timeout" error to be thrown to help detect race conditions.
* This class is not intended to be created directly, instead use the 
*   macros LOCK_MUTEX1, LOCK_MUTEX2, and LOCK_MUTEX3.
* The QMutexLocker class itself is disabled via use of the preprocessor, to
*   force coders to use this version.
*/
class IMutexLocker {
public:
    /// Lock the mutex on construction, default simple version.
    IMutexLocker(QMutex *mutex) : mutex_(mutex), locked_(true) {
        assert(mutex_);
        mutex_->lock();
    }
    /// Lock the mutex on constructor, but allows specification of timeout time, file name and line number.
    /// \param mutex A pointer to the mutex being claimed.
    /// \param time The timeout time in milliseconds. 
    /// \param line The line number of the source where IMutexLocker was created.
    /// \param file The file name of the source where IMutexLocker was created.
    IMutexLocker(QMutex *mutex,int time,int line,const char *file) : mutex_(mutex), locked_(false) { 
        assert(mutex_);
        if (!mutex_->tryLock(time))
            throw iexception("Mutex timeout in {} / {}.",line,file);
        locked_ = true;
    }
    /// Destructor, if locked unlocks the mutex.
    ~IMutexLocker() {  if (mutex_ && locked_) mutex_->unlock();  }
    /// Returns a pointer to the mutex claimed by this locker.
    QMutex *mutex() { return mutex_; }
    /// Explicitly unlocks the mutex before destruction.
    void unlock() { if (mutex_ && locked_) { mutex_->unlock();  locked_ = false; } }
    /// Explicitly relocks the mutex after destruction, if unlock() was called.
    void relock(int time=30000,int line=0,const char *file=0) { 
        if (mutex_ && !locked_)  { 
            if (!mutex_->tryLock(time))
                throw iexception("Mutex timeout in %1 / %2.",line,file);
            locked_ = true;
        }
    }
private:
    QMutex *mutex_;
    bool    locked_;
};

/// \brief Macro to use to claim a mutex, using debug systems.
///
/// The mutex locker is given a default name of "default_mutex_name", the timeout is 30 seconds.\n
/// \param mutexpnt A pointer to the mutex to claim.
#define LOCK_MUTEX1(mutexpnt)           IMutexLocker default_mutex_name(mutexpnt,30000,__LINE__,__FILE__)
/// \brief Macro to use to claim a mutex, using debug systems.
///
/// The timeout is 30 seconds.\n
/// \param name The name to give the mutex locker object on the stack.
/// \param mutexpnt A pointer to the mutex to claim.
#define LOCK_MUTEX2(name,mutexpnt)      IMutexLocker name(mutexpnt,30000,__LINE__,__FILE__)
/// \brief Macro to use to claim a mutex, using debug systems.
///
/// \param name The name to give the mutex locker object on the stack.
/// \param mutexpnt A pointer to the mutex to claim.
/// \param time The number of millisends to block before throwing an exception.
#define LOCK_MUTEX3(name,mutexpnt,time) IMutexLocker name(mutexpnt,time,__LINE__,__FILE__)
/// Macro created to prevent a QMutexLocker from being used instead of the LOCK_MUTEX? macros.
#define QMutexLocker ERROR_SHOULD_NOT_CREATE_DIRECTLY

// EoF
