#pragma once

#include "base/src/baseqt.h"

#include "dim.h"
#include "utility/interface/itextinput.h"

/**
    * \file
    * \brief Interface to the main command processing class.
    */

namespace itasca {
    class ITextOutput;

    /// \brief Interface to the main command processing class.
    /** This is an interface to the main command processing class.
        * Specifically, this class takes command string and tokenizes them,
        *   allowing them to be interpreted as commands.

        * The class IParse::IMark is used to indication a position in the token list.
        * This class is completely opaque, you must use member function of IParse to create
        *   and modify them.
        * In particular - *do not* do "*im1 = *im2;" where both im1 and im2 are pointers to IParse::IMark.
        * This will compile and execute, but not produce the results you want.
        * I was unable to get this to give a compile time error ;(.

        * An IParse object contains an internal position indicator that is used by default
        *   if one is not specified in the command.
        * All functions that accept an IMark assume (without checking) that the IMark
        *   was created by the same IParse it was being used in.

        * There are four broad categores of functions\n
        *    isXXX() (Check conversion functions) check to see if a token can be interpreted 
        *            in a certain way, and return an IParse::RetCode to indicate success or failure.
        *            If successful, they will fill a pointer to the object (if given)
        *            and increment the IParse::IMark position indicator.\n
        *    toXXX() (Automatic throw routines) directly converts to the indicated type and returns the value, 
        *            throwing an Exception if the token was unable to be interpreted.\n
        *    IParse::IMark manipulation functions create, check, and modify the position 
        *            of IParse::IMark objects.\n
        *    General utility functions.

        * Functions getTokenIndex, getLineNumber and getTokenExtent return information 
        * on the original location of the token on the input line. Note that if the token was created as 
        * the result of macro expansion, the information indicates the macro it came from.
        *
        * \ingroup module
    */
    class IParse {
    public:
        /// \brief Opaque position indicator class. Do *not* attempt to use directly for anything.
        class IMark {
        public:
          /// Destroys this mark.
            virtual void destroy()=0;
        };

        /// Possible return codes for isXXX() functions
        enum RetCode { 
            rcOK,     ///< token was interpreted successfully.
            rcMiss,   ///< no token was present at that position.
            rcBad,    ///< the token could not be interepreted as asked.
            rcRange,  ///< the token was interpreted correctly but it's value fell outside of a restricted range given.
            rcExtra   ///< an extra unused token was found at the end of the command line.
        };

        /// Virtual destructor
        virtual ~IParse() { }

        /// Returns TRUE if the input source for the command being processed was interactive.
        virtual bool interactiveSource() const=0;
        virtual void tagLastInputForSmartUndo(const QString &label,quint64 maxLevel,
                                              const QString &undoCommand)=0;

        /// Returns the placement of the token on the command (base 0), 
        ///    over all continuation lines.  The end IMark indicator (not valid) is given as the 
        ///    size of the current token list.
        virtual int    getTokenIndex(IMark *p=nullptr) const=0;
        /// Returns the continuation line (starting at 0) the token came from.\n
        /// The end mark indicator returns the line of the last valid token, 
        ///   or 0 if no valid tokens currently exist.
        virtual int    getLineNumber(IMark *p=nullptr) const=0;
        /// Gives the minimum and maximum character index of the token in the original string, 
        ///  on the appropriate line.  The end mark indicator returns one past the last valid 
        ///  token ans beginning and end, or (0,0) if no valid tokens currently exist.
        virtual IVect2 getTokenExtent(IMark *p=nullptr) const=0;

        /// Returns an ITextIO interface associated with this parser.
        virtual ITextOutput *getITextOutput()=0;
        /// Returns an ITextInput interface associated with this parser.
        virtual const ITextInput *getITextInput() const=0;
        /// Returns an ITextInput interface associated with this parser.
        virtual ITextInput *getITextInput()=0;

        /// Returns the original strings used to generate the set of command tokens.
        virtual const QList<InputLine> &getOriginalInput() const=0;

        /// A method to return a temporary IParse Object that must be deleted.
        virtual IParse * createTemporaryParse(const QString &)=0;

        /// Creates an IMark indicating the pos token (base 0), or end() if token does not exist.\n
        /// MUST BE DELETED BY CALLER
        virtual IMark *createMark(int pos=0) const=0; 

        /// Creates an IMark that indicates the same position as p, 
        ///  or the internal indicator if p is not given.\n
        /// MUST BE DELETED BY CALLER
        virtual IMark *createMarkCopy(IMark *p=nullptr) const=0; 

        /// Increments the provided or internal mark.
        virtual void increment(IMark *p=nullptr) const=0;
        /// Decrements the provided or internal mark.
        virtual void decrement(IMark *p=nullptr) const=0;
        /// Sets the internal position indicator to p.
        virtual void setMark(IMark *pNew,IMark *p=nullptr) const=0;

        /// Returns the token present at the given mark - does not increment p.\n
        /// Does NOT check that p represents a valid token!
        virtual QString getToken(IMark *p=nullptr) const=0;

        /// Checks to make sure that p represents a valid token.\n
        /// NOTE:  Assumes that p is an mark type aquired from this instance of Parse.
        ///        Assuming p is a valid token, this will returns false only when
        ///        an IMark has been incremented past the last token in the command.
        virtual bool isToken(IMark *p=nullptr) const=0;

        /// Throws an exception (or not) based on rc assuming it was the result of trying to process
        ///<   the token p.  If rc==rcOK does nothing.
        virtual void checkError(RetCode rc,IMark *p=nullptr) const=0;
        virtual bool checkError(bool b,IMark *p=nullptr) const=0;

        /// Adds a QString representing a token type to the help list. Multiple matching Strings are filtered.
        virtual void addHelp(const QString &s) const=0;  

        /// Returns TRUE if the token matches one of the help_tokens.
        virtual bool isHelp(IMark *p=nullptr) const=0;

         
        /// Checks if token is a string. If successful, it will fill the string pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isString(QString *ps=nullptr,IMark *p=nullptr,bool encodeBackslash=true) const=0;
        /// Checks if token is a boolean. If successful, it will fill the boolean pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isBool(bool *pb=nullptr,IMark *p=nullptr,bool implicitOn=false) const=0;
        /// Checks if token is a signed 32-bit integer. If successful, it will fill the int pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isInt(int *pi=nullptr,IMark *p=nullptr,int iMin=limits<int>::min(),
                              int iMax=limits<int>::max()) const=0;
        /// Checks if token is a signed 64-bit long. If successful, it will fill the long pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isLong(qint64 *pl=nullptr,IMark *p=nullptr,qint64 lMin=limits<qint64>::min(),
                               qint64 lMax=limits<qint64>::max()) const=0;
        /// Checks if token is a unsigned 32-bit integer. If successful, it will fill the uint pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isUInt(uint *pu=nullptr,IMark *p=nullptr,uint uMin=0,
                               uint uMax=limits<uint>::max()) const=0;
        /// Checks if token is a unsigned 64-bit long. If successful, it will fill the long pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isULong(quint64 *pu=nullptr,IMark *p=nullptr,quint64 uMin=0,
                                quint64 uMax=limits<quint64>::max()) const=0;
        virtual RetCode isName(QString *ps=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a double. If successful, it will fill the double pointer to the object (if given)
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isDouble(double *pd=nullptr,IMark *p=nullptr,
                                 double dMin=-limits<double>::max(),
                                 double dMax= limits<double>::max()) const=0;
        /// Given a string that contains a list of comma delimited keywords, checks if the token is a valid word in the list.
        ///  If successful, it will fill the uint pointer to the position of the keyword in the keyword list. The position
        ///  in the string list starts at 1.
        ///  and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        /// A leading '-' indicator means the option is hidden/deprecated.  The '-' isn't necessary 
        ///    normally and it won't show up in a help query.   In testing mode the '-' is necessary
        ///    for a match.
        /// For example:  "fred,-george,mary lemon" has three possible return positions (1-3).   
        ///               "george" is hidden/deprecated.
        ///               Either "mary" or "lemon" will match the third position.
        virtual RetCode isKeyword(const QString &key,uint *pPos=nullptr,IMark *p=nullptr) const=0;
        virtual RetCode isDVect2(DVect2 *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a DVect3. If successful, it will fill the DVetc3 pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator three steps.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isDVect3(DVect3 *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a DVect. If successful, it will fill the DVect pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator three steps.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isDVect(DVect *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a SymTensor. If successful, it will fill the SymTensor pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator 6 steps.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isSymTensor(SymTensor *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a DVect. If successful, it will fill the DVect pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator two or three steps, dependig if code is 2D or 3D.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        //virtual RetCode isDVect(DVect *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a ADVect. If successful, it will fill the ADVect pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator  two or three steps, dependig if code is 2D or 3D.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        //virtual RetCode isDAVect(DAVect *pv=nullptr,IMark *p=nullptr) const=0;
        /// Checks if token is a QVariant of the given QVariant::type. If successful, it will fill the QVariant pointer to the object (if given)
        ///    and increment the IParse::IMark position indicator.\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isVariant(int type,QVariant *pv=nullptr,IMark *p=nullptr,uint len=0) const=0;
        /// Checks if token is a QVariant. If successful, it will fill the QVariant pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator.\n
        /// Checks in the following order: DVect3,DVect2,double,int,bool,QString\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isVariant(QVariant *pv=nullptr,IMark *p=nullptr) const=0;


        /// Checks if token is a valid file path. If successful, it will fill the Qstring pointer to the object (if given)
        ///   and increment the IParse::IMark position indicator.\n
        /// def_ext can contain multiple extensions, delimited by spaces.\n
        /// Will always return an absolution file path.\n
        /// If openForRead is true, will check for all default extensions and in both input and current directory for a valid file,
        ///   and return the one found.\n
        /// "inputdir" and "localdir" can be specified after the file name to force the file to be in the input directory
        ///   or the current working directory, respectively.\n
        /// If no extension is given, will use def_ext to provide an extension (if present).\n
        /// Returns a RetCode indicating if it was able to interpret the token as desired.
        virtual RetCode isFilename(QString *ps=nullptr,const QString &def_ext=QString(),bool openForRead=false,IMark *p=nullptr) const=0;
         
        /// Checks if token is a string. If successful, returns a QString and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual QString  toString(IMark *p=nullptr) const=0;
        /// Checks if token is a boolean. If successful, returns a boolean and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual bool     toBool(IMark *p=nullptr,bool implicitOn=false) const=0;
        /// Checks if token is a 32-bit integer. If successful, returns a int and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual int      toInt(IMark *p=nullptr,int iMin=limits<int>::min(),
                                          int iMax=limits<int>::max()) const=0;
        /// Checks if token is a 64-bit integer. If successful, returns a 64-bit integer and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual qint64   toLong(IMark *p=nullptr,qint64 lMin=limits<qint64>::min(),
                                qint64 lMax=limits<qint64>::max()) const=0;
        /// Checks if token is a uint. If successful, returns a uint and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual uint     toUInt(IMark *p=nullptr,uint uMin=0,
                                uint uMax=limits<uint>::max()) const=0;
        /// Checks if token is a 64-bit unsigned integer. If successful, returns a quint64 and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual quint64  toULong(IMark *p=nullptr,quint64 uMin=0,
                                 quint64 uMax=limits<quint64>::max()) const=0;
        virtual QString toName(IMark *p=nullptr) const=0;
        /// Checks if token is a double. If successful, returns a double and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual double   toDouble(IMark *p=nullptr,double dMin=-limits<double>::max(),
                                  double dMax= limits<double>::max()) const=0;
        /// Given a string that contains a list of comma delimited keywords, checks if the token is a valid word in the list. 
        /// If successful, returns a position of the keyword in the list and increment the IParse::IMark position indicator.
        /// The position in the list starts at 1.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual uint     toKeyword(const QString &key,IMark *p=nullptr) const=0;
        /// Checks if token is a DVect. If successful, returns a DVect and increment the 
        /// IParse::IMark position indicator two steps.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual DVect   toDVect(IMark *p = 0) const = 0;
        /// Checks if token is a DVect2. If successful, returns a DVect2 and increment the 
        /// IParse::IMark position indicator two steps.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual DVect2   toDVect2(IMark *p=nullptr) const=0;
        /// Checks if token is a DVect3. If successful, returns a DVect3 and increment the 
        /// IParse::IMark position indicator three steps.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual DVect3   toDVect3(IMark *p=nullptr) const=0;
        /// Checks if token is a SymTensor. If successful, returns a SymTensor and increments the 
        /// IParse::IMark position indicator six steps.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual SymTensor toSymTensor(IMark *p=nullptr) const=0;
        /// Checks if token is a DVect. If successful, returns a DVect and increment the 
        /// IParse::IMark position indicator two or three steps, dependig if code is 2D or 3D.\n
        /// Throws an Exception if the token was unable to be interpreted
        //virtual DVect    toDVect(IMark *p=nullptr) const=0;
        /// Checks if token is a DAVect. If successful, returns a DAVect and increment the 
        /// IParse::IMark position indicator two or three steps, dependig if code is 2D or 3D.\n
        /// Throws an Exception if the token was unable to be interpreted
        //virtual DAVect   toDAVect(IMark *p=nullptr) const=0;
        /// Checks if token is a QVariant of the given QVariant::type. Returns a QVariant and increment the 
        /// IParse::IMark position indicator.\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual QVariant toVariant(int type,IMark *p=nullptr,uint len=0) const=0;
        /// Checks if token is a QVariant. Returns a QVariant and increment the 
        /// IParse::IMark position indicator.\n
        /// Checks in the following order: DVect3,DVect2,double,int,bool,QString\n
        /// Throws an Exception if the token was unable to be interpreted
        virtual QVariant toVariant(IMark *p=nullptr) const=0;
        /// Checks if token is a valid file path. Throws an Exception if the token was unable to be interpreted
        /// \see isFilename()
        virtual QString  toFilename(const QString &def_ext=QString(),bool openForRead=false,IMark *p=nullptr) const=0;
        /// Determines if c is a string delimiter token, indicating the *start* of a string token.\n
        /// Returns 0 if it is not, a positive integer is the index (base 1) of the delimiter character.\n
        /// This can be used to find a matching end delimiter using isStringCharEnd().\n
        /// Note that beginning and ending string delimiters do not have to be the same, but most commonly are.
        virtual int isStringCharStart(QChar c) const=0;
        /// Determines if c is a string delimiter token, indicating the *end* of a string token.\n
        /// Returns 0 if it is not, a positive integer is the index (base 1) of the delimiter character.\n
        /// This can be used to find a matching end delimiter using isStringCharBegin().\n
        /// Note that beginning and ending string delimiters do not have to be the same, but most commonly are.\n
        virtual int isStringCharEnd(QChar c) const=0;

        /// Returns the original skip list parsed from the input line.
        virtual QStringList getSkipList() const=0;
        /// Checks the skipped delimiter exists before the mark token in the skip string BEFORE the token marker.
        /// Returns TRUE if found, and REMOVES THAT CHARACTER from the skipList.
        virtual bool checkSkipPre(QStringList *skipList,QChar c,IMark *p=nullptr) const=0;
        /// Checks the skipped delimiter exists after the mark token in the skip string AFTER the token marker.
        /// Returns TRUE if found, and REMOVES THAT CHARACTER from the skipList.
        virtual bool checkSkipPost(QStringList *skipList,QChar c,IMark *p=nullptr) const=0;
        /// Adds a character to the end of the skip string BEFORE the current token.
        virtual void addSkipPre(QStringList *skipList,QChar c,IMark *p=nullptr) const=0;
        /// Adds a character to the start of the skip string AFTER the current token.
        virtual void addSkipPost(QStringList *skipList,QChar c,IMark *p=nullptr) const=0;
        /// Returns TRUE if a keyword match is found in any token at or after pStart.
        /// pStart=nullptr means the internal position indicator.
        /// Fill pFound (if non-zero) with the location found.
        virtual bool findKeyword(const QString &keyword,IMark *pFound,IMark *pStart=nullptr) const=0;
        /// Removes tokens from the current list after and including token m.
        /// The current position indicator is updated to be consistent.
        /// p (if provided) is updated to be consistent *as well*.
        virtual void stripTokens(IMark *mBegin,IMark *mEnd,IMark *p=nullptr)=0;
        /// Turne help on/off
        virtual bool activateHelp(bool b)=0;
        /// Return a boolean indicating whether or not a help token is in the token list.
        virtual bool hasHelpToken() const=0;
        virtual void destroy()=0;
    };

#ifdef THREED
    /// Thin wrapper for IParse::isDVect3, designed to make it easier to write code that is the same in 2D and 3D. 
    inline IParse::RetCode isDVect(const IParse &par,DVect3 *v=nullptr,IParse::IMark *m=nullptr) { return par.isDVect3(v,m); }
    /// Thin wrapper for IParse::isDVect3, designed to make it easier to write code that is the same in 2D and 3D.
    inline IParse::RetCode isDAvect(const IParse &par,DAVect3 *v=nullptr,IParse::IMark *m=nullptr) { return par.isDVect3(v,m); }
    /// Thin wrapper for IParse::toDVect3, designed to make it easier to write code that is the same in 2D and 3D.
    inline DVect3 toDVect(const IParse &par,IParse::IMark *m=nullptr) { return par.toDVect3(m); }
    /// Thin wrapper for IParse::toDVect3, designed to make it easier to write code that is the same in 2D and 3D.
    inline DAVect3 toDAVect(const IParse &par,IParse::IMark *m=nullptr) { return par.toDVect3(m); }
#endif
#ifdef TWOD
    /// Thin wrapper for IParse::isDVect2, designed to make it easier to write code that is the same in 2D and 3D.
    inline IParse::RetCode isDVect(const IParse &par,DVect2 *v=nullptr,IParse::IMark *m=nullptr) { return par.isDVect2(v,m); }
    /// Thin wrapper for IParse::isDVect2, designed to make it easier to write code that is the same in 2D and 3D.
    inline IParse::RetCode isDAvect(const IParse &par,DAVect2 *v=nullptr,IParse::IMark *m=nullptr) { double d; IParse::RetCode rc = par.isDouble(&d,m); if (rc==IParse::rcOK && v) *v = DAVect2(d);  return rc; }
    /// Thin wrapper for IParse::toDVect2, designed to make it easier to write code that is the same in 2D and 3D.
    inline DVect2 toDVect(const IParse &par,IParse::IMark *m=nullptr) { return par.toDVect2(m); }
    /// Thin wrapper for IParse::toDVect2, designed to make it easier to write code that is the same in 2D and 3D.
    inline DAVect2 toDAVect(const IParse &par,IParse::IMark *m=nullptr) { return DAVect2(par.toDouble(m)); }
#endif
} // namespace itascaxd
// EoF
