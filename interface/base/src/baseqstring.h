#pragma once
/** \file basestring.h
* \brief QString helper functions, plus some additions.
*
* \addtogroup Base Base interface specification 
* @{
*/

#include "basetoqt.h"
//#include <xfunctional>

/** \brief Converts from a base type to a QString, using the width and fill character specified.
*
*  A positive \a width indicates right-aligned text, a negative \a width indicates left-aligned text.\n
*  For floating point values, the format specifications are:
*    - 'e'  format as [-]9.9e[+|-]99
*    - 'E'  format as [-]9.9E[+|-]99
*    - 'f'  format as [-]9.9
*    - 'g'  use e or f format, whichever is most concise.
*    - 'G'  use E or f format, whichever is most concise.
*/
BASE_EXPORT QString toQString(const QString &s,int width=0,char fill=' ');
BASE_EXPORT QString toQString(const char *str,int width=0,char fill=' '); ///< \overload
BASE_EXPORT QString toQString(const qint8 &v,int width=0,char fill=' ');  ///< \overload
BASE_EXPORT QString toQString(const quint8 &v,int width=0,char fill=' ');  ///< \overload  
BASE_EXPORT QString toQString(const qint16 &v,int width=0,char fill=' ');    ///< \overload
BASE_EXPORT QString toQString(const quint16 &v,int width=0,char fill=' ');   ///< \overload
BASE_EXPORT QString toQString(const qint32 &v,int width=0,char fill=' ');    ///< \overload
BASE_EXPORT QString toQString(const quint32 &v,int width=0,char fill=' ');   ///< \overload
BASE_EXPORT QString toQString(const qint64 &v,int width=0,char fill=' ');    ///< \overload
BASE_EXPORT QString toQString(const quint64 &v,int width=0,char fill=' ');   ///< \overload
BASE_EXPORT QString toQString(const String &v,int width,char fill=' ');  ///< \overload
BASE_EXPORT QString toQString(const float & v,int width=0,char format='g',int precision=-1,char fill=' ');  ///< \overload
BASE_EXPORT QString toQString(const double &v,int width=0,char format='g',int precision=-1,char fill=' ');  ///< \overload
BASE_EXPORT QString toQString(const SymTensor &v,int width=0,char format='g',int precision=-1,char fill=' ');  ///< \overload
BASE_EXPORT QString toQString(const QVariant &v,int width=0,char fill=' ',char format='g',int precision=-1);   ///< \overload
/// Outputs a pointer in 0x0000 format.
BASE_EXPORT QString toQString(const void *v,int width=0,char fill=' ');
/// Outputs a bool, using the \a out string (comma delimited) to determine the true of false output value.
/// The default value of \a out is "on,off".
BASE_EXPORT QString toQString(bool b,const QString &out="on,off",int width=0,char fill=' ');


// Template version of toQString, that uses the same argument list
template <class T> 
QString toQStringT(const T &t,int width=0,char fill=' ',char format='g',int precision=-1) {
    format;  precision;
    return toQString(t,width,fill);
}
template <>
inline QString toQStringT(const float &t,int width,char fill,char format,int precision) {
    return toQString(t,width,format,precision,fill);
}
template <>
inline QString toQStringT(const double &t,int width,char fill,char format,int precision) {
    return toQString(t,width,format,precision,fill);
}
template <>
inline QString toQStringT(const QVariant &t,int width,char fill,char format,int precision) {
    return toQString(t,width,fill,format,precision);
}
template <>
inline QString toQStringT(const bool &b,int width,char fill,char format,int precision) {
    format;  precision;
    return toQString(b,"on,off",width,fill);
}

/// Converts a Vector2<T> to a string, using the save width, fmt, prec, and fill arguments as toQString(const double &,...).
/// \a width indicates the total width, which is output in the format (x,y).  
/// This means each component gets (w-3)/2 characters width.
template <class T> inline QString toQString(const Vector2<T> &v,int width=0,char fmt='g',int prec=-1,char fill=' ') {
    QString s("(%1,%2)");
    int w = (std::abs(width) - 3) / 2;
    if (width<0) w = -1;
    s = s.arg(toQStringT(v.x(),w,fill,fmt,prec));
    s = s.arg(toQStringT(v.y(),w,fill,fmt,prec));
    return s;
}
/// Converts a Vector3<T> to a string, using the save width, fmt, prec, and fill arguments as toQString(const double &,...).
/// \a width indicates the total width, which is output in the format (x,y,z).  
/// This means each component gets (w-4)/3 characters width.
template <class T> inline QString toQString(const Vector3<T> &v,int width=0,char fmt='g',int prec=-1,char fill=' ') {
    QString s("(%1,%2,%3)");
    int w = (std::abs(width) - 4) / 3;
    if (width<0) w = -1;
    s = s.arg(toQStringT(v.x(),w,fill,fmt,prec));
    s = s.arg(toQStringT(v.y(),w,fill,fmt,prec));
    s = s.arg(toQStringT(v.z(),w,fill,fmt,prec));
    return s;
}

/// Converts a AVector2<T> to a string, using the save width, fmt, prec, and fill arguments as toQString(const double &,...).
/// \a width indicates the total width, which is output in the format (x,y).  
/// This means each component gets (w-3)/2 characters width.
template <class T> inline QString toQString(const AVector2<T> &v,int width=0,char fmt='g',int prec=-1,char fill=' ') {
    QString s("(%1)");
    int w = (std::abs(width) - 3) / 2;
    if (width<0) w = -1;
    s = s.arg(toQStringT(v.z(),w,fill,fmt,prec));
    return s;
}
/// Converts a AVector3<T> to a string, using the save width, fmt, prec, and fill arguments as toQString(const double &,...).
/// \a width indicates the total width, which is output in the format (x,y,z).  
/// This means each component gets (w-4)/3 characters width.
template <class T> inline QString toQString(const AVector3<T> &v,int width=0,char fmt='g',int prec=-1,char fill=' ') {
    QString s("(%1,%2,%3)");
    int w = (std::abs(width) - 4) / 3;
    if (width<0) w = -1;
    s = s.arg(v.x(),w,fill,fmt,prec);
    s = s.arg(v.y(),w,fill,fmt,prec);
    s = s.arg(v.z(),w,fill,fmt,prec);
    return s;
}

/** Handy method that truncates the string to size (length-3) and
*  appends "..." to get a final size of length (does nothing if the initial
*  string size is less than parameter length).\n
*  \a length must be at least equal to 3
*/
BASE_EXPORT QString clip(const QString &in,int length);
/** \brief Performs "itasca match" of this string (as keyword) to token.
* 
*    Specifically, compares to length of token or keyword, whichever is shorter.\n
*    If token is longer than keyword no match.\n
*    If keyword contains the character '^', that character is disregarded for comparison purposes
*      and token must have at least as many characters as precede the '^' character.\n
*    If a token has a starting hyphen '-', then it is ignored for matching.
*    If a token has an internal hyphen 'one-two', then the hyphen must be present in keyword and
*      BOTH sides are checked using matching rules and any possible '^' character.
*/
BASE_EXPORT bool match(const QString &keyword,const QString &token);
BASE_EXPORT void matchSynonymsAllowed(bool b);
/** \brief Uses \%1, \%2, \%3,... as replacement tokens.
*
*  All other uses of % must use an extra --- i.e. "%%" parses to "%".\n
*  For example: build("This %1 a %2","is","test.") expands to "This is a test."\n
*  Basically, this is shorthand for: QString("").arg().arg().arg()...
*/
BASE_EXPORT QString build(const QString &s,const QVariant &v1=QVariant(),const QVariant &v2=QVariant(),
                          const QVariant &v3=QVariant(),const QVariant &v4=QVariant(),
                          const QVariant &v5=QVariant(),const QVariant &v6=QVariant());

/** \brief Converts number \a ul into a memory size string.
*
* Uses units of Bytes, KBytes, or MBytes depending on size.
*/
BASE_EXPORT QString toBytes(const quint64 &ul);

/// Reads a single QString line from a QDataStream (since QTextStream is bugged), uses '\\n' as an end-of-line marker.
BASE_EXPORT QString readLine(QDataStream &ds,qint64 maxlen=0);
/// Reads a single whitespace delimited word from the QDataStream (since QTextStream is bugged), using the given separator list.
BASE_EXPORT QString readWord(QDataStream &ds,qint64 maxlen=0,const QString &separators=" \t\r\n");
BASE_EXPORT QString cleanupTypename(const char *name);
// Converts the string This is a "test"  to "This ia a \"test\""
BASE_EXPORT QString enquoteString(const QString &s,QChar quote='"');


/** These functions check if the contents of the string can be converted into another type.\n
*  They will return TRUE if so, and fill the pointer with the converted value if given.\n
*  These are basically alternative syntax forms of the QString::toXXX functions.\n
*/
BASE_EXPORT bool isInt(const QString &in,int *i=0);
BASE_EXPORT bool isUInt(const QString &in,uint *u=0); ///< \overload
BASE_EXPORT bool isLong(const QString &in,qint64 *l=0); ///< \overload
BASE_EXPORT bool isULong(const QString &in,quint64 *ul=0); ///< \overload
BASE_EXPORT bool isDouble(const QString &in,double *d=0); ///< \overload
/** This functions checks to see if the string can be converted into a boolean value.\n
* The \a out string is used to match against pairs of values (true,false,...).  \n
* The default \a out string is "on,off,true,false,yes,no".
* The \a b argument is filled with the result.
* Returns true on a successful conversion, false otherwise.
*/
BASE_EXPORT bool isBool(const QString &in,bool *b=0,const QString &out="on,off,true,false,yes,no");
/** Converts from a QString to a bool, using the same rules as isBool().\n
* Returns the result of the conversion, while \a ok is filled with true on success, false otherwise.
*/
BASE_EXPORT bool toBool(const QString &in,bool *ok=0,const QString &out="on,off,true,false,yes,no");
/// converts a QString into a base string.
inline String toString(const QString &in) { return in.toStdWString(); }
/// returns a copy of \a in
inline String toString(const std::wstring &in) { return in; }
/// converts a string to a wide character string
inline String toString(const string &in) { return QString::fromUtf8(in.c_str()).toStdWString(); }
/// converts a wide string into a QString
inline QString toQString(const std::wstring &in) { return QString::fromStdWString(in); }
/// converts a string into a QString
inline QString toQString(const string &in) { return QString::fromUtf8(in.c_str()); }
// compares two Strings
inline int compare(const String &s1,const String &s2,bool case_sensitive=false) {
    if (case_sensitive) return s1.compare(s2);
#ifdef __LINUX
    else return wcscasecmp(s1.c_str(),s2.c_str());
#else
    else return _wcsicmp(s1.c_str(),s2.c_str());
#endif
}
// std::hash interface for QStrings!
//namespace std {
//    template <>
//    class hash<QString> {
//    public:
//        size_t operator()(const QString &s) const { return qHash(s); }
//    };
//}

// < and == functors for strings - to use in std containers when CI comparisons are wanted!
struct QStringCILess2 {
public:
    bool operator()(const QString &left, const QString &right) const { 
        return left.compare(right,Qt::CaseInsensitive) < 0;
    }
};
struct QStringLocalLess2 {
public:
    bool operator()(const QString &left, const QString &right) const { 
        return QString::localeAwareCompare(left,right) < 0;
    }
};
struct QStringCIEqual2 {
public:
    bool operator()(const QString &left, const QString &right) const { 
        return left.compare(right,Qt::CaseInsensitive) == 0;
    }
};
struct QStringCIHash2 {
public:
    uint operator()(const QString &in) const {
        uint h = 0;
        auto len = in.size();
        auto *p = in.constData();
        for (size_t i = 0; i<len; ++i) {
            h = 31 * h + p[i].toUpper().unicode();
        }
        return h;
    }
};


inline string tostring(const QString &s) {  return s.toStdString(); }

template <typename T> using QStringMap = std::map<QString, T, QStringCILess2>;

template <typename T> using QStringHashMap = std::unordered_map<QString, T, QStringCIHash2, QStringCIEqual2>;

using QStringSet = std::set<QString,QStringCILess2>;

using QStringHashSet = std::unordered_set<QString, QStringCIHash2, QStringCIEqual2>;

/// @}
// EoF
