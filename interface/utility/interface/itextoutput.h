#pragma once

namespace itasca {
  /** \brief Provides a interface for simple writing of text.
    *
    * Provides interface for simple writing of text.
    * \ingroup utilitysrc
    */
  class ITextOutput {
  public:

    /// Turn on the local pagination and set the description to \a name.
    /// \param name QString identifying pagination.
    virtual void       setLocalPaginationOn(const QString &name)=0; // no exception
    /// Turn off the local pagination.
    virtual void       setLocalPaginationOff()=0;       // no exception
    /// Turns on or off the header recording.
    virtual void       setRecordHeader(bool b)=0;       // no exception
    /// Sets header to the string \a s.
    virtual void       setHeader(const QString &s)=0;   // no exception
    /// Sends output directly to the output device. Throws exception on quit, pagination or interrupt.
    virtual void       sendOutput(const QString &s)=0;  // exception on quit, pagination, or interrupt.
    /// Retrieves the underlying device implementation. 
    virtual QIODevice *getDevice()=0;   // no exception, however sending output to the device 
                                        // may throw exception on quit, pagination, or interrupt.
  };
} // namespace itasca
// EOF
