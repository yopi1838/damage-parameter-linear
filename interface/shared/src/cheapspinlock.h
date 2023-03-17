#pragma once
// cheapspinlock.h
/** \file cheapspinlock.h
* \brief Utilities for thread management.  Simple quick mutex.
*/

#include "exportshared.h"
#include "base/src/spinlock.h"
#include <mutex>

/** \brief An inexpensive spin lock mechanism for management of shared resources in a multithreaded enviornment.  Simple quick mutex.
*
* Prefer use of spinlock which uses std library semantics
*
* \ingroup utilitysrc
*/ 
class SHARED_EXPORT CheapSpinLock : public spinlock {
public:
    inline void acquire() { lock(); }
    inline void release() { unlock(); }
};

//class CheapSpinLock : public std::mutex {
//public:
//    /// Acquire a lock using the test-and-set paradigm. 
//    inline void acquire() { lock(); }
//    inline void release() { unlock(); }
//};

/** \brief A helper class holding a CheapSpinLock object.
* \ingroup utilitysrc
*/ 
class CheapLocker : public std::lock_guard<CheapSpinLock> {
public:
    /// Constructor taking a reference to a CheapSpinLock object.  A lock is acquired automatically
    /// upon creation.
    /// \param lock Reference to a CheapSpinLock.
    CheapLocker(CheapSpinLock &lock) : std::lock_guard<CheapSpinLock>(lock) {  }

    /// Constructor taking a pointer to a CheapSpinLock object.  A lock is acquired automatically
    /// upon creation.
    /// \param lock Pointer to a CheapSpinLock.
    CheapLocker(CheapSpinLock *lock) : std::lock_guard<CheapSpinLock>(*lock) {  }
private:
    CheapLocker(const CheapLocker &) = delete;
    void operator=(const CheapLocker &) = delete;
};

class CheapBreakLock : public reverse_lock<CheapSpinLock> {
public:
    CheapBreakLock(CheapSpinLock &lock) : reverse_lock<CheapSpinLock>(lock) {  }
    CheapBreakLock(CheapSpinLock *lock) : reverse_lock<CheapSpinLock>(*lock) {  }
private:
    CheapBreakLock(const CheapBreakLock &) = delete;
    void operator==(const CheapBreakLock &) = delete;
};

/** \brief A helper class holding a CheapSpinLock object.
* \ingroup utilitysrc
*/ 
class CheapLockerConditional : public conditional_lock<CheapSpinLock> {
public:
    /// Constructor taking a reference to a CheapSpinLock object.  A lock is acquired automatically
    /// upon creation.
    /// \param lock Reference to a CheapSpinLock.
    CheapLockerConditional(CheapSpinLock &lock, bool use) : conditional_lock<CheapSpinLock>(lock, use) {}

    /// Constructor taking a pointer to a CheapSpinLock object.  A lock is acquired automatically
    /// upon creation.
    /// \param lock Pointer to a CheapSpinLock.
    CheapLockerConditional(CheapSpinLock *lock,bool use) : conditional_lock<CheapSpinLock>(*lock, use) {}
private:
    CheapLockerConditional(const CheapLockerConditional &) = delete;
    void operator=(const CheapLockerConditional &) = delete;
};


// EoF
