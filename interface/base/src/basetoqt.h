#pragma once
/** \file basetoqt.h
* \brief Combines base interface with Qt -- allows Qt to interact with other Base types transparently.
* \addtogroup Base Base interface specification 
* @{
*/

#include "base.h"
#include "avect.h"

#ifdef _WIN32
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#endif
#ifdef __INTEL_COMPILER
#  pragma warning(disable:82)
#  pragma warning(disable:111)
#  pragma warning(disable:327)
#  pragma warning(disable:1879)
#  if __INTEL_COMPILER>=1210
#    pragma warning(disable:3199)
#    pragma warning(disable:3280)
#    pragma warning(disable:1292)
#  endif
#  ifdef VS2017
#    pragma warning(disable:4459)
#    pragma warning(disable:1744)
#    pragma warning(disable:3280)
#  endif
#else
#    pragma warning(disable:4251)
#    pragma warning(disable:4244)
#endif
#include <QtCore>
#undef rad1
#undef rad2
#undef small
#undef near
#undef far
#undef grp1
#undef grp2

#if QT_VERSION > 0x050000
#  define QT5
#endif

#ifndef DOXYGEN
// The following are commented since they should not be used with Qt anyway 
//Q_DECLARE_METATYPE(quint64);
Q_DECLARE_METATYPE(String);
Q_DECLARE_METATYPE(Variant);
Q_DECLARE_METATYPE(IVect2);
Q_DECLARE_METATYPE(FVect2);
Q_DECLARE_METATYPE(DVect2);
Q_DECLARE_METATYPE(UVect2);
Q_DECLARE_METATYPE(DAVect2);
Q_DECLARE_METATYPE(FAVect2);
Q_DECLARE_METATYPE(IAVect2);
Q_DECLARE_METATYPE(UAVect2);
Q_DECLARE_METATYPE(IVect3);
Q_DECLARE_METATYPE(FVect3);
Q_DECLARE_METATYPE(DVect3);
Q_DECLARE_METATYPE(UVect3);
Q_DECLARE_METATYPE(DAVect3);
Q_DECLARE_METATYPE(FAVect3);
Q_DECLARE_METATYPE(IAVect3);
Q_DECLARE_METATYPE(UAVect3);
Q_DECLARE_METATYPE(Orientation2);
Q_DECLARE_METATYPE(Orientation3);
Q_DECLARE_METATYPE(Axes2D);
Q_DECLARE_METATYPE(Axes3D);
Q_DECLARE_METATYPE(Quat2);
Q_DECLARE_METATYPE(Quat3);
Q_DECLARE_METATYPE(SymTensor);
#endif
#pragma warning(pop)

// Ensure always utf-8
class ItascaTextStream : public QTextStream {
public:
    BASE_EXPORT ItascaTextStream() : QTextStream() {
        setCodec("UTF-8");
    }

    BASE_EXPORT ItascaTextStream(const QByteArray &array, QIODevice::OpenMode openMode = QIODevice::ReadOnly) : QTextStream(array,openMode) {
        setCodec("UTF-8");
    }

    BASE_EXPORT ItascaTextStream(QByteArray *array, QIODevice::OpenMode openMode = QIODevice::ReadWrite) : QTextStream(array,openMode) {
        setCodec("UTF-8");
    }

    BASE_EXPORT ItascaTextStream(QString *string, QIODevice::OpenMode openMode = QIODevice::ReadWrite) : QTextStream(string,openMode) {
        setCodec("UTF-8");
    }

    BASE_EXPORT ItascaTextStream(FILE *fileHandle, QIODevice::OpenMode openMode = QIODevice::ReadWrite) : QTextStream(fileHandle,openMode) {
        setCodec("UTF-8");
    }

    BASE_EXPORT ItascaTextStream(QIODevice *device) : QTextStream(device) {
        setCodec("UTF-8");
    }
};

/// Initialization routine, called automatically by system.
/// This initializes various Base types with the QVariant system.
/// It also adds the executable directory to the list of paths searched for libraries.
BASE_EXPORT void initializeQtToBase();

// QDataStream functions
/// Allows a Base type to be written to, or read from, a QDataStream.
template <class T> inline QDataStream &operator<<(QDataStream &ds,const Vector2<T> &v) {
    ds << v.x() << v.y();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,Vector2<T> &v) {
    ds >> v.rx() >> v.ry();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator<<(QDataStream &ds,const Vector3<T> &v) {
    ds << v.x() << v.y() << v.z();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,Vector3<T> &v) {
    ds >> v.rx() >> v.ry() >> v.rz();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator<<(QDataStream &ds,const AVector2<T> &av) {
    ds << av.z();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,AVector2<T> &av) {
    ds >> av.rz();
    return ds;
}

/// \overload
template <class T> inline QDataStream &operator<<(QDataStream &ds,const AVector3<T> &av) {
    ds << av.x() << av.y() << av.z();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,AVector3<T> &av) {
    ds >> av.rx() >> av.ry() >> av.rz();
    return ds;
}

// Streaming Extent2 and Extent3
/// \overload
template <class T> inline QDataStream &operator<<(QDataStream &ds,const Extent2<T> &v) {
    ds << v.x1() << v.x2() << v.y1() << v.y2();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,Extent2<T> &v) {
    ds >> v.rx1() >> v.rx2() >> v.ry1() >> v.ry2();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator<<(QDataStream &ds,const Extent3<T> &v) {
    ds << v.x1() << v.x2() << v.y1() << v.y2() << v.z1() << v.z2();
    return ds;
}
/// \overload
template <class T> inline QDataStream &operator>>(QDataStream &ds,Extent3<T> &v) {
    ds >> v.rx1() >> v.rx2() >> v.ry1() >> v.ry2() >> v.rz1() >> v.rz2();
    return ds;
}

// Data Streaming Orientation, Axes, Variant, string, and SymTensor classes.
/// \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Orientation2 &o);
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Orientation2 &o);        ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Orientation3 &o);  ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Orientation3 &o);        ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Axes2D &v);        ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Axes2D &v);              ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Axes3D &v);        ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Axes3D &v);              ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const CAxes2D &v);        ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,CAxes2D &v);              ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const CAxes3D &v);        ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,CAxes3D &v);              ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Variant &v);       ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Variant &v);             ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const std::wstring &v);  ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,std::wstring &v);        ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const string &v);  ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,string &v);        ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const SymTensor &s);     ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,SymTensor &v);           ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Quat2 &o);    ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Quat2 &o);          ///< \overload
BASE_EXPORT QDataStream &operator<<(QDataStream &ds,const Quat3 &o);    ///< \overload
BASE_EXPORT QDataStream &operator>>(QDataStream &ds,Quat3 &o);          ///< \overload

// QTextStream functions
/// Writes a Vector2 to formatted text.  The current qSetFieldWidth is used for the entire vector.
/// Format is "(x,y)".  This means each component gets a field width of (w-3)/2.
template <class T> inline QTextStream &operator<<(QTextStream &ds,const Vector2<T> &v) {
    int w = ds.fieldWidth();
    w = std::max(0,w-3);
    w /= 2;
    ds << qSetFieldWidth(0) << "(" << qSetFieldWidth(w) << v.x();
    ds << qSetFieldWidth(0) << "," << qSetFieldWidth(w) << v.y();
    ds << qSetFieldWidth(0) << ")";
    return ds;
}
/// Writes a Vector3 to formatted text.  The current qSetFieldWidth is used for the entire vector.
/// Format is "(x,y,z)".  This means each component gets a field width of (w-4)/3.
template <class T> inline QTextStream &operator<<(QTextStream &ds,const Vector3<T> &v) {
    int w = ds.fieldWidth();
    w = std::max(0,w-4);
    w /= 3;
    ds << qSetFieldWidth(0) << "(" << qSetFieldWidth(w) << v.x();
    ds << qSetFieldWidth(0) << "," << qSetFieldWidth(w) << v.y();
    ds << qSetFieldWidth(0) << "," << qSetFieldWidth(w) << v.z();
    ds << qSetFieldWidth(0) << ")";
    return ds;
}
/// Writes an AVector2 to formatted text, as a scalar basically.
template <class T> inline QTextStream &operator<<(QTextStream &ds,const AVector2<T> &v) {
    ds << v.z();
    return ds;
}

// Conversion to Qt Core 2D vector types
/// Convert a Vector2 to a QPoint
template <class T> inline QPoint toQPoint(const Vector2<T> &v) {
    QPoint ret(to<int>(v.x()),to<int>(v.y()));
    return ret;
}
/// Convert a Vector2 to a QPointF
template <class T> inline QPointF toQPointF(const Vector2<T> &v) {
    QPointF ret(to<qreal>(v.x()),to<qreal>(v.y()));
    return ret;
}
/// Convert a QPoint to a DVect2
inline DVect2 toDVect2(const QPoint &p) {
    DVect2 dv2(to<Double>(p.x()),to<Double>(p.y()));
    return dv2;
}
/// Convert a QSize to a DVect2
inline DVect2 toDVect2(const QSize &p) {
    DVect2 dv2(to<Double>(p.width()),to<Double>(p.height()));
    return dv2;
}
/// Convert a QPoint to an FVect2
inline FVect2 toFVect2(const QPoint &p) {
    FVect2 fv2(to<Float>(p.x()),to<Float>(p.y()));
    return fv2;
}
/// Convert a QPoint to an IVect2
inline IVect2 toIVect2(const QPoint &p) {
    IVect2 iv2(to<Int>(p.x()),to<Int>(p.y()));
    return iv2;
}
/// Convert a QPoint to a UVect2
inline UVect2 toUVect2(const QPoint &p) {
    UVect2 uv2(to<UInt>(p.x()),to<UInt>(p.y()));
    return uv2;
}
/// Convert a QPointF to an DVect2
inline DVect2 toDVect2(const QPointF &p) {
    DVect2 dv2(to<Double>(p.x()),to<Double>(p.y()));
    return dv2;
}
/// Convert a QPointF to an FVect2
inline FVect2 toFVect2(const QPointF &p) {
    FVect2 fv2(to<Float>(p.x()),to<Float>(p.y()));
    return fv2;
}
/// Convert a QPointF to an IVect2
inline IVect2 toIVect2(const QPointF &p) {
    IVect2 iv2(to<Int>(p.x()),to<Int>(p.y()));
    return iv2;
}
/// Convert a QPointF to an UVect2
inline UVect2 toUVect2(const QPointF &p) {
    UVect2 uv2(to<UInt>(p.x()),to<UInt>(p.y()));
    return uv2;
}

/// returns true if Variants have the same type
inline bool checkQVariantTypes(const QVariant &v1, const QVariant &v2)
{
    int type1 = static_cast<int>(v1.type());
    int type2 = static_cast<int>(v2.type());
    if (type1==QVariant::UserType)
    {
        type1 = v1.userType();
        if (type2==QVariant::UserType)
            type2 = v2.userType();
        else
            return false;
    }
    return type1 == type2;
}

// Converts from QVariant to Variant and vice versa.
/// Convert a Variant to a QVariant.  This works for any type, technically.
/// For user-defined types, raw data, etc. the entire Variant is stored in the QVariant.
BASE_EXPORT QVariant toQVariant(const Variant &v);
/// Convert a QVariant to a Variant.  
/// This does not work for user-defined types.
/// If the QVariant type is not allowed by the Variant, an exception is thrown.
BASE_EXPORT Variant  toVariant(const QVariant &v);

/// Attempts to determine if the directory represented by dir is write protected for any reason.
/// A directory is considered "protected" for one of three reasons:
///   a) It is read-only, or an attempt to create and write to a file in that location fails.
///   b) It contains "Program Files" at the start of the absolute directory path.
///   c) It matches the installation path stored in the registry at 
///        HKEY_LOCAL_MACHINE\\Software\\Itasca\\"registryString" in the entry "installdir".
///      Note that a 32 bit process running under 64 bit windows is redirected to
///        HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\Itasca\\"registryString"
BASE_EXPORT bool isDirectoryProtected(const QDir &dir,const QString &registryString);

/// Attempts to read the registry to find a variable set during installation indicating the install directory.
/// This variable is named "installdir32" for a 32 bit exe, and "installdir64" for a 64 bit exe.
/// The directory will be:
///        HKEY_LOCAL_MACHINE\\Software\\Itasca\\"registryString" in the entry "installdir".
/// Note that a 32 bit process running under 64 bit windows is redirected to:
///        HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\Itasca\\"registryString"
BASE_EXPORT QString getInstallDirectory(const QString &registryString);

/// Converts a filePath string from being relative to a previous directory, to being relative to a new directory.
/// If the filePath is absolute, and the new directory references the same base system (drive letter or network path)
///   it might *become* a relative path.
/// This should work even if the relative file path is impossible (like a filePath of "../../../../../fred.dat" with a previousDir of "C:/one/").
/// If the new directory is not in the same system as the old directory, then filePath might *become* absolute.   
///    In that case, if the relative file path was impossible, the closest approximation is used (in the abolve example, "C:/fred.dat") 
///      and tree information might be lost.
BASE_EXPORT QString convertFilePath(const QString &filePath,const QDir &previousDir,const QDir &newDir);

/// Reentrant versions of setCurrentDirectory and QDir::getCurrent.
/// Those functions are not re-entrant (even though they are documented as such).
BASE_EXPORT bool setCurrentDirectory(const QString &dir);

/// Reentrant versions of setCurrentDirectory and QDir::getCurrent.
/// Those functions are not re-entrant (even though they are documented as such).
BASE_EXPORT QString getCurrentDirectory();

/// Reentrant versions of setCurrentDirectory and QDir::getCurrent.
/// Those functions are not re-entrant (even though they are documented as such).
BASE_EXPORT QDir getCurrentDirectoryObject();
/// @}

// EOF
