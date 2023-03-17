#pragma once

#include "basestring.h"

/** \brief Base exception class for all Itasca code.
*
* By convention an exception thrown derived from std::exeption with message "quit" means shut down.
*/
class iexception : public std::exception  {
public:
    template <typename... Args>
    iexception(const string &s, Args&&... args) { message_ = fmt::format(s, args...); }

    /// Returns the contents of the error message as a const char.
    BASE_EXPORT virtual const char *what() const throw() { return message_.data(); }

    /// Allows the message carried by the exception to be replaced with a new one.
    void setMessage(const string &s) { message_ = s; }

private:
    string message_; 
};

// EoF
