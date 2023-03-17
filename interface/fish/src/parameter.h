#pragma once

// FISH parameter - main storage of FISH values, able to hold any legal FISH type
// Availables types are:  boolean
//                        64 bit integer
//                        double precision floating point
//                        QString
//                        Pointer
//                        DVect2
//                        DVect3
//                        SymTensor
//                        List
//                        Map
//                        Struct
//                        Index
//                        Array
//                        Memory
//                        Matrix
//                        Iterator
//                        Reference

#include "index.h"
#include "matrix.h"
#include "pointer.h"
#include "reference.h"
#include "utility/src/accumulator.h"
#include "utility/src/tptr.h"
#include "shared/src/archive2.h"
#include "base/src/vect.h"

#include "../interface/iparameter.h"

namespace fish {
    using itasca::TPtr;
    using itasca::AccOnly;
    class Array;
    class Index;
    class Library;
    class LocalRuntime;
    class Memory;
    class Structure;
    class Symbol;

#pragma warning(push)
#pragma warning(disable:26495) // VS Code Analysys initializtion error on anonymous union members
    class Parameter : public IParameter {
    public:
        enum class Type { Bool=0, 
                          Int, 
                          Double, 
                          String, 
                          PointerOld,  // Old pointer value for save/restore compatibility
                          Vect2, 
                          Vect3, 
                          Tensor, 
                          List, 
                          Map, 
                          Struct, 
                          Index, 
                          Array, 
                          Memory,
                          Matrix,
                          Iterator,
                          Reference,
                          Compatibility, // Tag for compatibility with old parameters, the plugin type.
                          Accum,
                          Pointer,
                          LastMark }; // Last mark is never used

                    Parameter() : type_(Type::Int), int_(0) { }
                    Parameter(const Parameter &p) : type_(p.type_) { create(p); }
                    Parameter(Parameter &&p) noexcept : type_(p.type_) { createMove(std::move(p)); }
                    explicit Parameter(bool b) : type_(Type::Bool), bool_(b) { }
                    explicit Parameter(qint64 i) : type_(Type::Int), int_(i) { }
                    explicit Parameter(qint32 i) : type_(Type::Int), int_(i) { }
                    explicit Parameter(quint64 i) : type_(Type::Int), int_(i) { }
                    explicit Parameter(quint32 i) : type_(Type::Int), int_(i) { }
                    explicit Parameter(double d) : type_(Type::Double), double_(d) { }
                    explicit Parameter(const QString &s) : type_(Type::String), string_(s) {  }
                    explicit Parameter(QString &&s) noexcept : type_(Type::String), string_(std::move(s)) { }
                    explicit Parameter(const DAVect2 &v) : type_(Type::Double), double_(v.z()) { }
                    explicit Parameter(const DVect2 &v) : type_(Type::Vect2), vect2_(v) { }
                    explicit Parameter(const DVect3 &v) : type_(Type::Vect3), vect3_(v) { }
                    explicit Parameter(const SymTensor &s) : type_(Type::Tensor), tensor_(s) { }
                    explicit Parameter(const QVariant &v) : type_(Type::Int) { operator=(v); }
                    explicit Parameter(const Variant &v) : type_(Type::Int) { operator=(v); }
                    explicit Parameter(const IThing *t) : type_(Type::Pointer), pointer_(const_cast<IThing *>(t)) { }
       FISH_EXPORT  explicit Parameter(const List &l);
       FISH_EXPORT  explicit Parameter(List &&l) noexcept;
       FISH_EXPORT  explicit Parameter(const Map &m);
       FISH_EXPORT  explicit Parameter(Map &&m) noexcept;
                    explicit Parameter(const itasca::Mat &m) : type_(Type::Matrix), matrix_(m) { }
                    explicit Parameter(itasca::Mat &&m) noexcept : type_(Type::Matrix), matrix_(std::move(m)) {}
                    explicit Parameter(const Matrix &m) : type_(Type::Matrix), matrix_(m) { }
                    explicit Parameter(Matrix &&m) noexcept : type_(Type::Matrix), matrix_(std::move(m)) {}
        FISH_EXPORT explicit Parameter(const Array &a);
        FISH_EXPORT explicit Parameter(const Memory &m);
        FISH_EXPORT explicit Parameter(Memory &&m) noexcept;
        FISH_EXPORT explicit Parameter(const Iterator &i);
        FISH_EXPORT explicit Parameter(const Reference &r);
                    explicit Parameter(const AccOnly &r) : type_(Type::Accum), accum_(r) { }
                    virtual ~Parameter() { dest(); }
        FISH_EXPORT const Parameter &operator=(const Parameter &p);
        FISH_EXPORT Parameter &operator=(Parameter &&p) noexcept;
        FISH_EXPORT const Parameter &operator=(const IParameter &p);
                    bool operator==(const Parameter &p) const { if (type_!=p.type_) return false;  return eqValue(p); }
                    bool operator!=(const Parameter &p) const { return !operator==(p); }
                    bool operator<(const Parameter &p) const { if (type_!=p.type_) return type_ < p.type_;   return ltValue(p); }
                    bool eqWithTol(const Parameter &p) const { if (type_ != p.type_) return false;  return eqValueTol(p); }
                    bool neWithTol(const Parameter &p) const { return !eqWithTol(p);  }
                    bool ltWithTol(const Parameter &p) const { if (type_ != p.type_) return type_ < p.type_;   return ltValue(p); }

                    Type    type() const { return type_; }
                    QString name() const { return nameFromType(type()); }
                    TType   indexType() const { toIndex();  return index_.getType(); }

                    // IParameter overrides
                    virtual QString getName() const override { return name(); }
                    virtual bool getIsBool() const override { return isBool(); }
                    virtual bool getIsInt() const override { return isInt(); }
                    virtual bool getIsDouble() const override { return isDouble(); }
                    virtual bool getIsNumber() const override { return isNumber(); }
                    virtual bool getIsString() const override { return isString(); }
                    virtual bool getIsPoint() const override { return isPoint(); }
                    virtual bool getIsVect2() const override { return isVect2(); }
                    virtual bool getIsVect3() const override { return isVect3(); }
                    virtual bool getIsList() const override { return isList(); }
                    virtual bool getIsMap() const override { return isMap(); }
                    virtual bool      getBool() const override { return toBool(); }
                    virtual qint64    getInt(qint64 min=limits<qint64>::min(),qint64 max=limits<qint64>::max()) const override { return toInt(min,max); }
                    virtual double    getDouble(double min=-limits<double>::max(),double max=limits<double>::max()) const override { return toDouble(min,max); }
                    virtual QString   getString() const override { return toString(); }
                    virtual DVect2    getVect2() const override { return toVect2(); }
                    virtual DVect3    getVect3() const override { return toVect3(); }
                    virtual IThing *  getPoint() const override { return toPoint(); }
                    virtual SymTensor getTensor() const override { return toTensor(); }
                    virtual QVariant  getQVariant() const override { return toQVariant(); }
        FISH_EXPORT virtual List      getList() const override;
        FISH_EXPORT virtual Map       getMap() const override;

        FISH_EXPORT virtual double    safeToDouble(double min=-limits<double>::max(),double max=limits<double>::max()) const;
        FISH_EXPORT virtual DVect2    safeToVect2() const;
        FISH_EXPORT virtual DVect3    safeToVect3() const;
        FISH_EXPORT virtual SymTensor safeToTensor() const;
                    virtual QString   list() const { return build("%1 (%2)",toString(),name()); }
        FISH_EXPORT virtual double    getScalar(TreatAs type,Component comp,Quantity quant,bool stress) const;
        FISH_EXPORT virtual void      copy(const IParameter &p) { *this = *check_cast<Parameter>(&p); }
        FISH_EXPORT virtual void      fromString(const QString &s);

                    bool isBool() const { return type_==Type::Bool; }
                    bool isInt() const { return type_==Type::Int; }
                    bool isDouble() const { return type_==Type::Double; }
                    bool isNumber() const { return isInt() || isDouble(); }
                    bool isString() const { return type_==Type::String; }
                    bool isQString() const { return isString(); }
                    bool isPoint() const { return type_==Type::Pointer; }
                    bool isVect2() const { return type_==Type::Vect2; }
                    bool isVect3() const { return type_==Type::Vect3; }
                    bool isTensor() const { return type_==Type::Tensor; }
                    bool isList() const { return type_==Type::List; }
                    bool isMap() const { return type_==Type::Map; }
                    bool isStructure() const { return type_==Type::Struct; }
                    bool isIndex() const { return type_==Type::Index; }
                    bool isArray() const { return type_==Type::Array; }
                    bool isMemory() const { return type_==Type::Memory; }
                    bool isMatrix() const { return type_==Type::Matrix; }
                    bool isReference() const { return type_==Type::Reference; }
                    bool isAccum() const { return type_==Type::Accum; }
        template <class T> bool isPointer() const;
        template <typename T> bool is() const { static_assert(sizeof(T{})==0); return false; }

                    bool      toBool() const { if (isBool()) return fastToBool();  return convertToBool(); }
                    bool      toBoolean() const { return toBool(); }
                    qint64    toInt() const { if (isInt()) return fastToInt();  return convertToInt(); }
        FISH_EXPORT qint64    toInt(qint64 min,qint64 max=limits<qint64>::max()) const;
                    qint32    toInt32() const { return ::to<qint32>(toInt(limits<qint32>::min(),limits<qint32>::max())); }
                    qint32    toInt32(qint32 min,qint32 max=limits<qint32>::max()) const { return ::to<qint32>(toInt(min,max)); }
                    quint32   toUInt32() const { return ::to<qint32>(toInt(0,limits<quint32>::max())); }
                    quint32   toUInt32(quint32 min,quint32 max=limits<quint32>::max()) const { return ::to<qint32>(toInt(min,max)); }
                    quint64   toUInt64() const { return toInt(0,limits<qint64>::max()); }
                    quint64   toUInt64(quint64 min,quint64 max=limits<qint64>::max()) const { return toInt(min,max); }
                    double    toDouble() const { if (isDouble()) return fastToDouble();  return convertToDouble(); }
        FISH_EXPORT double    toDouble(double min,double max=limits<double>::max()) const;
                    QString   toString() const { if (isString()) return fastToString(); return convertToString(); }
                    QString   toQString() const { return toString(); }
                    IThing *  toPoint() const { if (isPoint()) return fastToPoint();  return convertToPoint(); }
                    DVect2    toVect2() const { if (isVect2()) return fastToVect2();  return convertToVect2(); }
                    DVect2    toDVect2() const { return toVect2(); }
                    DVect3    toVect3() const { if (isVect3()) return fastToVect3();  return convertToVect3(); }
                    DVect3    toDVect3() const { return toVect3(); }
                    SymTensor toTensor() const { if (isTensor()) return fastToTensor();  return convertToTensor(); }
        FISH_EXPORT List      toList() const;
        FISH_EXPORT Map       toMap() const;
        FISH_EXPORT Structure toStructure() const;
                    quint64   toIndex() const { if (isIndex()) return fastToIndex(); return convertToIndex();  }
        FISH_EXPORT Array     toArray() const;
        FISH_EXPORT Memory    toMemory() const;
                    Matrix    toMatrix() const { if (isMatrix()) return fastToMatrix(); return convertToMatrix(); }
                    Reference toReference() const { if (isReference()) return fastToReference();  return convertToReference(); }
                    template <typename T> T to() const { static_assert(sizeof(T{})==0); return T{}; }

        FISH_EXPORT QVariant   toQVariant() const;
        FISH_EXPORT Variant    toVariant() const;
        template <class T> inline T *toPointer() const;

                    const bool &        fastToBool() const { assert(isBool());  return bool_; }
                    bool &              fastToBool() { assert(isBool());  return bool_; }
                    const qint64 &      fastToInt() const { assert(isInt());  return int_; }
                    qint64 &            fastToInt() { assert(isInt());  return int_; }
                    const double &      fastToDouble() const { assert(isDouble());  return double_; }
                    double &            fastToDouble() { assert(isDouble());  return double_; }
                    const QString &     fastToString() const { assert(isString());  return string_; }
                    QString &           fastToString() { assert(isString());  return string_; }
                    IThing *            fastToPoint() const { assert(isPoint());  return const_cast<IThing *>(pointer_.get()); }
                    const DVect2 &      fastToVect2() const { assert(isVect2());  return vect2_; }
                    DVect2 &            fastToVect2() { assert(isVect2());  return vect2_; }
                    const DVect3 &      fastToVect3() const { assert(isVect3());  return vect3_; }
                    DVect3 &            fastToVect3() { assert(isVect3());  return vect3_; }
                    const SymTensor &   fastToTensor() const { assert(isTensor());  return tensor_; }
                    SymTensor &         fastToTensor() { assert(isTensor());  return tensor_; }
                    const List &        fastToList() const { assert(isList());  return buf<List>(); }
                    List &              fastToList() { assert(isList());  return buf<List>(); }
                    const Map &         fastToMap() const { assert(isMap());  return buf<Map>(); }
                    Map &               fastToMap() { assert(isMap());  return buf<Map>(); }
                    const Structure &   fastToStructure() const { assert(isStructure());  return buf<Structure>(); }
                    Structure &         fastToStructure() { assert(isStructure());  return buf<Structure>(); }
                    quint64             fastToIndex() const { assert(isIndex());  return index_.value(); }
                    const Array &       fastToArray() const { assert(isArray());  return buf<Array>(); }
                    Array &             fastToArray() { assert(isArray());  return buf<Array>(); }
                    const Memory &      fastToMemory() const { assert(isMemory());  return buf<Memory>(); }
                    Memory &            fastToMemory() { assert(isMemory());  return buf<Memory>(); }
                    const Matrix &      fastToMatrix() const { assert(isMatrix());  return matrix_; }
                    Matrix &            fastToMatrix() { assert(isMatrix());  return matrix_; }
                    Iterator &          fastToIterator() { assert(type_==Type::Iterator);  return buf<Iterator>(); }
                    Reference &         fastToReference() { assert(isReference());  return reference_; }
                    const Reference &   fastToReference() const { assert(isReference());  return reference_; }
                    AccOnly &           fastToAccum() { assert(isAccum());  return accum_; }
                    const AccOnly &     fastToAccum() const { assert(isAccum());  return accum_; }

        template <class T> inline T *fastToPointer() const { return fastToPoint()->convert<T>(); }
        template <typename T> const T &fastTo() const { static_assert(sizeof(T{})==0); return reinterpret_cast<const T &>(*this);  }
        template <typename T> T &fastTo() { static_assert(sizeof(T{})==0);  return reinterpret_cast<T &>(*this); }

                    void operator=(bool b)             { dest(Type::Bool);    bool_ = b; }
                    void operator=(qint64 i)           { dest(Type::Int);     int_ = i; } 
                    void operator=(qint32 i)           { dest(Type::Int);     int_ = i; } 
                    void operator=(quint64 i)          { dest(Type::Int);     int_ = i; } 
                    void operator=(quint32 i)          { dest(Type::Int);     int_ = i; } 
                    void operator=(double d)           { dest(Type::Double);  double_ = d; }
                    void operator=(const QString &s)   { if (dest(Type::String)) new(&string_) QString(s); else string_ = s; }
                    void operator=(QString &&s)        { if (dest(Type::String)) new(&string_) QString(std::move(s));  else string_ = std::move(s); }
                    void operator=(const IThing *p)    { if (dest(Type::Pointer)) new(&pointer_) Pointer(const_cast<IThing *>(p));  else pointer_ = const_cast<IThing *>(p); }
                    void operator=(std::nullptr_t)     { operator=((const IThing *)nullptr); }
                    void operator=(const DVect2 &v2)   { dest(Type::Vect2);   vect2_ = v2; }
                    void operator=(const DVect3 &v3)   { dest(Type::Vect3);   vect3_ = v3; }
                    void operator=(const DAVect2 &v2)  { dest(Type::Double);  double_ = v2.z(); }
                    void operator=(const DAVect3 &v3)  { dest(Type::Vect3);   vect3_ = v3; }
                    void operator=(const SymTensor &t) { dest(Type::Tensor);  tensor_ = t; }
        FISH_EXPORT void operator=(const List &l);
        FISH_EXPORT void operator=(List &&l);
        FISH_EXPORT void operator=(const Map &m);
        FISH_EXPORT void operator=(Map &&m);
        FISH_EXPORT void operator=(const Structure &s);
        FISH_EXPORT void operator=(Structure &&s);
                    void setIndex(qint64 i, TType type) { if (dest(Type::Index)) new(&index_) Index(i, type);  else { index_.value() = i; index_.setType(type); } }
        FISH_EXPORT void operator=(const Array &a);
        FISH_EXPORT void operator=(const Memory &m);
                    void operator=(const itasca::Mat &m) { if (dest(Type::Matrix)) new(&matrix_) Matrix(m);  else matrix_ = m; }
                    void operator=(itasca::Mat &&m)      { if (dest(Type::Matrix)) new(&matrix_) Matrix(std::move(m));  else matrix_ = std::move(m); }
                    void operator=(const Matrix &m) { if (dest(Type::Matrix)) new(&matrix_) Matrix(m);  else matrix_ = m; }
                    void operator=(Matrix &&m)      { if (dest(Type::Matrix)) new(&matrix_) Matrix(std::move(m)); else matrix_ = std::move(m); }
                    void operator=(const AccOnly &a) { if (dest(Type::Accum)) new(&accum_) AccOnly(a);  else accum_ = a; }
                    void setReference(Parameter *p,const QString &name,const QString &function) { if (dest(Type::Reference)) new (&reference_) Reference(p,name,function);  else reference_.setLocal(p,name,function); }
                    void setReference(Symbol *s) { if (dest(Type::Reference)) new (&reference_) Reference(s);  else reference_.setSymbol(s); }
                    void setReference(Library *l) { if (dest(Type::Reference)) new (&reference_) Reference(l);  else reference_.setLibrary(l); }
        FISH_EXPORT void operator=(const QVariant &v);
        FISH_EXPORT void operator=(const Variant &v);
                    void makeQVariant(const QVariant &v) { operator=(v); }

        FISH_EXPORT void conversionException(const QString &from,const QString &to) const;

        FISH_EXPORT QString encodeAsString() const;
        FISH_EXPORT void    decodeFromString(const QString &s);

        FISH_EXPORT void save(itasca::Archive2 &a) const;
        FISH_EXPORT bool restore(itasca::Archive2 &a,quint64 label);
        FISH_EXPORT void remap(itasca::Archive2 &a);
        FISH_EXPORT void fixDroppedPointer(quint64 *missed);
        //FISH_EXPORT void savePointerDetails() const;

        FISH_EXPORT static QString nameFromType(Type type);
                    static bool boolConvert() { return boolConvert_; }
                    static void boolConvert(bool b) { boolConvert_ = b; }
                    static int precision() { return precision_; }
                    static void precision(int i) { precision_ = i; }

    private:
        explicit Parameter(void *);
        void operator=(void *); // Here to prevent = to pointer to unknown class from resolving to operator=(bool).

        template <class T>
        T &buf() { return *reinterpret_cast<T *>(buffer_.data()); }
        template <class T>
        const T &buf() const { return *reinterpret_cast<const T *>(buffer_.data());  }
        template <class T>
        void bufCreate(const T &t) { new(&buf<T>()) T(t); }
        template <class T>
        void bufCreate(T &&t) { new(&buf<T>()) T(std::move(t)); }
        template <class T> 
        void bufAssign(const T &t) { buf<T>() = t;  }
        template <class T>
        void bufAssign(T &&t) { buf<T>() = std::move(t);  }

        FISH_EXPORT void    dest();
                    bool    dest(Type t) { if (type()==t) return false; dest(); type_ = t; return true; }
        FISH_EXPORT void    create(const Parameter &p);
        FISH_EXPORT void    createMove(Parameter &&p);
                    void    copy(const Parameter &p);
                    void    move(Parameter &&p);
        FISH_EXPORT QString cleanup(const char *name) const;
        FISH_EXPORT bool       convertToBool() const;
        FISH_EXPORT qint64     convertToInt() const;
        FISH_EXPORT double     convertToDouble() const;
        FISH_EXPORT QString    convertToString() const;
        FISH_EXPORT IThing *   convertToPoint() const;
        FISH_EXPORT DVect2     convertToVect2() const;
        FISH_EXPORT DVect3     convertToVect3() const;
        FISH_EXPORT SymTensor  convertToTensor() const;
        FISH_EXPORT List       convertToList() const;
        FISH_EXPORT Map        convertToMap() const;
        FISH_EXPORT Structure  convertToStructure() const;
        FISH_EXPORT qint64     convertToIndex() const;
        FISH_EXPORT Array      convertToArray() const;
        FISH_EXPORT Memory     convertToMemory() const;
        FISH_EXPORT Matrix     convertToMatrix() const;
        FISH_EXPORT Reference  convertToReference() const;
        FISH_EXPORT bool       eqValue(const Parameter &p) const;
        FISH_EXPORT bool       ltValue(const Parameter &p) const;
        FISH_EXPORT bool       eqValueTol(const Parameter& p) const;
        FISH_EXPORT bool       ltValueTol(const Parameter& p) const;

                    itasca::Archive2::Type getArchiveType() const;
                    void                   setType(Type t);
                    void                   saveValue(itasca::Archive2 &a) const;
                    void                   restoreValue(itasca::Archive2 &a);

                    Type           typeFromOldType(quint64 oldType) const;
                    void           restoreOldValue(itasca::Archive2 &a);

        FISH_EXPORT static bool boolConvert_;
        FISH_EXPORT static int  precision_;
        static constexpr uint32 bufSize_ = isDebugCompile ? 88 : 48;

        Type type_;
        union {
            bool                       bool_;
            int64                      int_;
            double                     double_;
            DVect2                     vect2_;
            DVect3                     vect3_;
            SymTensor                  tensor_;
            QString                    string_;
            AccOnly                    accum_;
            Pointer                    pointer_;
            Matrix                     matrix_;
            Reference                  reference_;
            Index                      index_;
            std::array<char, bufSize_> buffer_; // For types that use Parameter themselves.
        };
    };
#pragma warning(pop)

//#ifdef __LINUX
    template <> inline bool Parameter::is<bool>()      const { return isBool(); }
    template <> inline bool Parameter::is<qint64>()    const { return isInt(); }
    template <> inline bool Parameter::is<double>()    const { return isDouble(); }
    template <> inline bool Parameter::is<QString>()   const { return isString(); }
    template <> inline bool Parameter::is<IThing*>()  const { return isPoint(); }
    template <> inline bool Parameter::is<DVect2>()    const { return isVect2(); }
    template <> inline bool Parameter::is<DVect3>()    const { return isVect3(); }
    template <> inline bool Parameter::is<SymTensor>() const { return isTensor(); }
    template <> inline bool Parameter::is<List>()      const { return isList(); }
    template <> inline bool Parameter::is<Map>()       const { return isMap(); }
    template <> inline bool Parameter::is<Structure>() const { return isStructure(); }
    template <> inline bool Parameter::is<Array>()     const { return isArray(); }
    template <> inline bool Parameter::is<Memory>()    const { return isMemory(); }
    template <> inline bool Parameter::is<Matrix>()    const { return isMatrix(); }

    template <> inline bool      Parameter::to<bool>()      const { return toBool(); }
    template <> inline qint64    Parameter::to<qint64>()    const { return toInt(); }
    template <> inline qint32    Parameter::to<qint32>()    const { return (qint32)toInt(); }
    template <> inline quint64   Parameter::to<quint64>()   const { return (quint64)toInt(0); }
    template <> inline quint32   Parameter::to<quint32>()   const { return (quint32)toInt(0); }
    template <> inline double    Parameter::to<double>()    const { return toDouble(); }
    template <> inline QString   Parameter::to<QString>()   const { return toString(); }
    template <> inline IThing*   Parameter::to<IThing*>()  const { return toPoint(); }
    template <> inline DVect2    Parameter::to<DVect2>()    const { return toVect2(); }
    template <> inline DVect3    Parameter::to<DVect3>()    const { return toVect3(); }
    template <> inline SymTensor Parameter::to<SymTensor>() const { return toTensor(); }
    template <> List      Parameter::to<List>()      const;
    template <> Map       Parameter::to<Map>()       const;
    template <> Structure Parameter::to<Structure>() const;
    template <> Array     Parameter::to<Array>()     const;
    template <> Memory    Parameter::to<Memory>()    const;
    template <> inline Matrix    Parameter::to<Matrix>()    const { return toMatrix(); }

    template <> inline const bool& Parameter::fastTo<bool>()      const { return fastToBool(); }
    template <> inline const qint64& Parameter::fastTo<qint64>()    const { return fastToInt(); }
    template <> inline const double& Parameter::fastTo<double>()    const { return fastToDouble(); }
    template <> inline const QString& Parameter::fastTo<QString>()   const { return fastToString(); }
    template <> inline const DVect2& Parameter::fastTo<DVect2>()    const { return fastToVect2(); }
    template <> inline const DVect3& Parameter::fastTo<DVect3>()    const { return fastToVect3(); }
    template <> inline const SymTensor& Parameter::fastTo<SymTensor>() const { return fastToTensor(); }
    template <> inline const List& Parameter::fastTo<List>()      const { return fastToList(); }
    template <> inline const Map& Parameter::fastTo<Map>()       const { return fastToMap(); }
    template <> inline const Structure& Parameter::fastTo<Structure>() const { return fastToStructure(); }
    template <> inline const Array& Parameter::fastTo<Array>()     const { return fastToArray(); }
    template <> inline const Memory& Parameter::fastTo<Memory>()    const { return fastToMemory(); }
    template <> inline const Matrix& Parameter::fastTo<Matrix>()    const { return fastToMatrix(); }

    template <> inline bool& Parameter::fastTo<bool>() { return fastToBool(); }
    template <> inline qint64& Parameter::fastTo<qint64>() { return fastToInt(); }
    template <> inline double& Parameter::fastTo<double>() { return fastToDouble(); }
    template <> inline QString& Parameter::fastTo<QString>() { return fastToString(); }
    template <> inline DVect2& Parameter::fastTo<DVect2>() { return fastToVect2(); }
    template <> inline DVect3& Parameter::fastTo<DVect3>() { return fastToVect3(); }
    template <> inline SymTensor& Parameter::fastTo<SymTensor>() { return fastToTensor(); }
    template <> inline List& Parameter::fastTo<List>() { return fastToList(); }
    template <> inline Map& Parameter::fastTo<Map>() { return fastToMap(); }
    template <> inline Structure& Parameter::fastTo<Structure>() { return fastToStructure(); }
    template <> inline Array& Parameter::fastTo<Array>() { return fastToArray(); }
    template <> inline Memory& Parameter::fastTo<Memory>() { return fastToMemory(); }
    template <> inline Matrix& Parameter::fastTo<Matrix>() { return fastToMatrix(); }
//#endif

    template <class T> bool Parameter::isPointer() const {
        if (!isPoint()) return false;
        IThing *th = fastToPoint();
        if (!th) return false;
        const T *t = th->convert<T>();
        return (t ? true : false); 
    }

    template <class T> T *Parameter::toPointer() const {
        T *t = nullptr;
        IThing *th = toPoint();
        if (th)
            t = th->convert<T>();
        if (!t) {
            QString name;
            if (th) name = build("Pointer to %1 (%2)",th->getTypeName(),th->getID());
            else    name = build("Null Pointer");
            conversionException(name,cleanup(typeid(t).name()));  
        }
        return t;
    }
} // namespace fish
namespace archive {
    template <>
    class Traits<fish::Parameter> : public TraitsObject<fish::Parameter> {};
}

// EoF
