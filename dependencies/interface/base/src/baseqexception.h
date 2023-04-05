#pragma once

#include "basetoqt.h"

/** \brief Base exception class for all Itasca code.
*
* By convention an exception thrown derived from std::exeption with message "quit" means shut down.
*/
class Exception : public std::exception  {
public:
    /// Constructor, uses the build() format to create the string for convenience.
    /// Currently, the total message cannot be more than 1024 characters.
    BASE_EXPORT Exception(const QString &s_,const QVariant &v1=QVariant(),const QVariant &v2=QVariant(),
                                            const QVariant &v3=QVariant(),const QVariant &v4=QVariant(),
                                            const QVariant &v5=QVariant(),const QVariant &v6=QVariant());
    /// Destructor
    BASE_EXPORT ~Exception() throw() { }
    /// Returns the contents of the error message as a const char.
    BASE_EXPORT virtual const char *what() const throw() { return message_; }
    /// Allows the message carried by the exception to be replaced with a new one.
    BASE_EXPORT void setMessage(const QString &s);
private:
    static const int maxExceptionMessageLength_ = 2048;
    char message_[maxExceptionMessageLength_];
};

/// Exception thrown when processing is done
class QuitException : public Exception {
public:
    BASE_EXPORT QuitException() : Exception("quit") { }
};

/// Exception thrown when interruption occurs, e.g. user press ESC key.
class InterruptException : public Exception {
public:
    /// Constructor
    /// \param safe indicates that is safe to interrupt
    /// \param state see function state() 
    BASE_EXPORT InterruptException(bool safe,const QString &state=QString()) 
        : Exception("Processing interrupted by user."), safe_(safe), state_(state) { }

    /// returns TRUE if the interrupt was "Safe", in that the model was left in a repeatable and valid state.
    BASE_EXPORT bool    wasSafe() const { return safe_; }
    /// Only needed for a safe interrupt.  A state string to be used to indicate that the
    /// conditions required to duplicate the interupt have been met.  A case-sensitive match between
    /// this string (Stored in the record) and a string representing the current state indicates
    /// that an interrupt should be simulated.
    BASE_EXPORT QString state() const { return state_; }
private:
    bool    safe_;
    QString state_;
};

// EoF
