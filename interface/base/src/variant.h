#pragma once
/** \file variant.h
  * \brief A simpler (and slightly faster) version of a QVariant, added for interfaces that do not use Qt.
  * \addtogroup Base Base interface specification 
  * @{
  */

#include "avect.h"
#include "basestring.h"
#include "type_selector.h"
#include "vect.h"
#include <cassert>
#pragma warning (disable:809)

class QDataStream;

/** \brief A simpler (and slightly faster) version of a QVariant, added for interfaces that do not use Qt.
  *
  * This class implements a variant. Used when parameters need to be passed or returned but the type could vary.\n
  * These types could be integral, floating-point, strings, 2D or 3D vectors, etc.\n
  * This type is used in the property interface for constitutive models, and by FISH.\n
  * If QT is active, it is preferred you use the Qt QVariant class.\n
  * QVariant is more general, but this version should perform better at runtime.
  */ 
#pragma warning(push)
#pragma warning(disable:26812) // Non-class enum
#pragma warning(disable:26495) // Unitiailized value in a union
class Variant {
  template <class T> friend class VUserType; // need this so that VUserType can access dataSize_
private:
  class UserTypeBase {
  public:
    /// Default constructor, no data initialization.
    UserTypeBase() { }
    /// Deconstructor
    virtual ~UserTypeBase() { }
    /// Clone function
    virtual UserTypeBase *clone(char *data) const=0;
    /// Copy from base pointer
    virtual void copy(const UserTypeBase *base)=0;
  };

  #ifdef _WIN64
#  ifdef _DEBUG
  static const int dataSize_ = 80;
#  else
  static const int dataSize_ = 72;
#  endif
#else
#  ifdef _DEBUG
  static const int dataSize_ = 40;
#  else
  static const int dataSize_ = 36;
#  endif
#endif
public:
  /** \brief This is a helper class, designed to make it possible to allow user-defined types
   *    to be encoded in a Variant.\n 
   *  
   *  Inherits from type_selector to provide support for types larger than a variant.
   *  
   *  See the \link Variant::registerType() Variant::registerType() \endlink method.
   */
  template <typename T> class VUserType;

  /// Allows writing a Variant to a QDataStream.
  friend BASE_EXPORT QDataStream &operator<<(QDataStream &,const Variant &);

  /// Indicates the type of the variant.  18 types are predefined.
  enum Type { ByteType=1,     ///< signed 8 bit integer.
              UByteType,      ///< unsigned 8 bit integer.
              ShortType,      ///< signed 16 bit integer.
              UShortType,     ///< unsigned 16 bit integer.
              IntType,        ///< signed 32 bit integer.
              UIntType,       ///< unsigned 32 bit integer.
              LongType,       ///< signed 64 bit integer.
              ULongType,      ///< unsigned 64 bit integer.
              FloatType,      ///< 32 bit floating point value.
              DoubleType,     ///< 64 bit floating point value.
              VoidType,       ///< A void pointer.
              BoolType,       ///< A value of type bool.
              IVect2Type,     ///< A 2D vector of 32 bit signed integers.
              DVect2Type,     ///< A 2D vector of 64 bit floating point values.
              IVect3Type,     ///< A 3D vector of 32 bit signed integers.
              DVect3Type,     ///< A 3D vector of 64 bit floating point values.
              RawDataType,    ///< Raw binary data -- see the Variant::setToRawData() and Variant::getRawData() methods.
              StringType=100, ///< A String() class.
              UserType        ///< A user-defined type.  See the Variant::registerType() method.
            };
  /// Copy constructor
  BASE_EXPORT Variant(const Variant &mv) : type_(IntType) { operator=(mv); }
  /// Default constructor, also the Int conversion constructor.
  /// Note that a default Variant is an integer of value 0.
  BASE_EXPORT Variant(const Int &v=0) : type_(IntType), int_(v) { }
  /// Conversion contructor -- this function and the ones like it
  ///   allow automatic conversion of Variants from the built-in types.
  BASE_EXPORT Variant(const bool &v) : type_(BoolType), bool_(v) { }
  BASE_EXPORT Variant(const Byte &v) : type_(ByteType), byte_(v) { }  ///< \overload
  BASE_EXPORT Variant(const UByte &v) : type_(UByteType), ubyte_(v) { }   ///< \overload
  BASE_EXPORT Variant(const Short &v) : type_(ShortType), short_(v) { }  ///< \overload
  BASE_EXPORT Variant(const UShort &v) : type_(UShortType), ushort_(v) { }  ///< \overload
  BASE_EXPORT Variant(const UInt &v) : type_(UIntType), uint_(v) { }  ///< \overload
  BASE_EXPORT Variant(const Long &v) : type_(LongType), long_(v) { }  ///< \overload
  BASE_EXPORT Variant(const ULong &v) : type_(ULongType), ulong_(v) { }  ///< \overload
  BASE_EXPORT Variant(const Float &v) : type_(FloatType), float_(v) { }  ///< \overload
  BASE_EXPORT Variant(const Double &v) : type_(DoubleType), double_(v) { }  ///< \overload
  BASE_EXPORT Variant(void *v) : type_(VoidType), void_(v) { }  ///< \overload
  BASE_EXPORT Variant(const void *v) : type_(VoidType), void_(const_cast<void *>(v)) { }  ///< \overload
  BASE_EXPORT Variant(const IVect2 &v) : type_(IVect2Type) { conv<IVect2>() = v; }  ///< \overload
  BASE_EXPORT Variant(const DVect2 &v) : type_(DVect2Type) { conv<DVect2>() = v; }  ///< \overload
  BASE_EXPORT Variant(const IVect3 &v) : type_(IVect3Type) { conv<IVect3>() = v; }  ///< \overload
  BASE_EXPORT Variant(const DVect3 &v) : type_(DVect3Type) { conv<DVect3>() = v; }  ///< \overload
  BASE_EXPORT Variant(const String &v); ///< \overload
  BASE_EXPORT Variant(const Char *v); ///< \overload
  /// Destructor
  BASE_EXPORT ~Variant() { del(); }
  /// Equality operator
  BASE_EXPORT const Variant &operator=(const Variant &mv);

  /// The type of the Variant.  Note that this may be a user-defined value.
  BASE_EXPORT Type   getType() const { return type_; }
  /// A string indicating the Variant type.
  BASE_EXPORT String getTypeName() const { return getTypeName(getType()); }

  /// Attempts to convert the Variant type to the indicated type.\n
  /// A failed conversion will return 0, or whatever the default constructed value is in general.\n
  /// \param success If provided, is true if the conversion was successful and false otherwise.
  BASE_EXPORT bool   toBool(bool *success=0) const; 
  BASE_EXPORT Byte   toByte(bool *success=0) const;   ///< \copydoc toBool
  BASE_EXPORT UByte  toUByte(bool *success=0) const;  ///< \copydoc toBool
  BASE_EXPORT Short  toShort(bool *success=0) const;  ///< \copydoc toBool
  BASE_EXPORT UShort toUShort(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT Int    toInt(bool *success=0) const;    ///< \copydoc toBool
  BASE_EXPORT UInt   toUInt(bool *success=0) const;   ///< \copydoc toBool
  BASE_EXPORT Long   toLong(bool *success=0) const;   ///< \copydoc toBool
  BASE_EXPORT ULong  toULong(bool *success=0) const;  ///< \copydoc toBool
  BASE_EXPORT Float  toFloat(bool *success=0) const;  ///< \copydoc toBool
  BASE_EXPORT Double toDouble(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT String toString(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT void * toVoid(bool *success=0) const;   ///< \copydoc toBool
  BASE_EXPORT IVect2 toIVect2(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT DVect2 toDVect2(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT IVect3 toIVect3(bool *success=0) const; ///< \copydoc toBool
  BASE_EXPORT DVect3 toDVect3(bool *success=0) const; ///< \copydoc toBool

  /// Very fast converion of a Variant to a specific type.
  /// Used only where performance is critical, and where you are certain the Variant
  ///   contains a value of *exactly* that type.
  BASE_EXPORT const Int &   fastToInt() const { return int_; }
  BASE_EXPORT const bool &  fastToBool() const { return bool_; }                                      ///< \copydoc fastToInt
  BASE_EXPORT const Long &  fastToLong() const { return long_; }                                    ///< \copydoc fastToInt
  BASE_EXPORT const Double &fastToDouble() const { return double_; }                                ///< \copydoc fastToInt
  BASE_EXPORT const DVect2 &fastToDVect2() const { return conv<DVect2>(); }                         ///< \copydoc fastToInt 
  BASE_EXPORT const DVect3 &fastToDVect3() const { return conv<DVect3>(); }                         ///< \copydoc fastToInt
  BASE_EXPORT const String &fastToString() const;                                                   ///< \copydoc fastToInt

  /// Assignment operator for a specific type, converts the Variant to the same type and value.
  const Variant &operator=(const bool &v)       { del(); type_ = BoolType;   bool_ = v;   return *this; }
  const Variant &operator=(const Byte &v)       { del(); type_ = ByteType;   byte_ = v;   return *this; }  ///< \overload
  const Variant &operator=(const UByte &v)      { del(); type_ = UByteType;  ubyte_ = v;  return *this; }  ///< \overload
  const Variant &operator=(const Short &v)      { del(); type_ = ShortType;  short_ = v;  return *this; }  ///< \overload
  const Variant &operator=(const UShort &v)     { del(); type_ = UShortType; ushort_ = v; return *this; }  ///< \overload
  const Variant &operator=(const Int &v)        { del(); type_ = IntType;    int_ = v;    return *this; }  ///< \overload
  const Variant &operator=(const UInt &v)       { del(); type_ = UIntType;   uint_ = v;   return *this; }  ///< \overload
  const Variant &operator=(const Long &v)       { del(); type_ = LongType;   long_ = v;   return *this; }  ///< \overload
  const Variant &operator=(const ULong &v)      { del(); type_ = ULongType;  ulong_ = v;  return *this; }  ///< \overload
  const Variant &operator=(const Float &v)      { del(); type_ = FloatType;  float_ = v;  return *this; }  ///< \overload
  const Variant &operator=(const Double &v)     { del(); type_ = DoubleType; double_ = v; return *this; }  ///< \overload
  const Variant &operator=(void *v)             { del(); type_ = VoidType;   void_ = v;   return *this; }  ///< \overload
  const Variant &operator=(const void *v)       { del(); type_ = VoidType;   void_ = const_cast<void *>(v);   return *this; } ///< \overload
  const Variant &operator=(const IVect2 &v)     { del(); type_ = IVect2Type; conv<IVect2>() = v; return *this; }  ///< \overload
  const Variant &operator=(const DVect2 &v)     { del(); type_ = DVect2Type; conv<DVect2>() = v; return *this; }  ///< \overload
  const Variant &operator=(const DAVect2 &v)    { del(); type_ = DoubleType; double_ = v.z(); return *this; }  ///< \overload
  const Variant &operator=(const IVect3 &v)     { del(); type_ = IVect3Type; conv<IVect3>() = v; return *this; }  ///< \overload
  const Variant &operator=(const DVect3 &v)     { del(); type_ = DVect3Type; conv<DVect3>() = v; return *this; }  ///< \overload
  BASE_EXPORT const Variant &operator=(const String &v);  ///< \overload
  const Variant &operator=(const Char *v)       { return operator=(String(v)); }  ///< \overload

  /// Clears the current contents of the Variant, and indicates that this variant will now hold raw data installed in it's available memory.
  /// Use the getRawData() method to access a pointer to the start of this memory.
  /// \warning { Any attempt to store more than getRawDataSize() bytes will result in overwriting of random memory! }
  /// \sa getRawData() getRawDataSize()
  BASE_EXPORT void        setToRawData();
  /// Returns a pointer to the raw data used to store Variant contents.
  /// \warning If the variant is not currently setToRawData(), you could cause a fatal data corruption by overwring memory.
  /// \warning Any attempt to store more than getRawDataSize() bytes will result in overwriting of random memory!
  /// \sa setToRawData() getRawDataSize()
  char *      getRawData() { return data_; }
  const char *getRawData() const { return data_; }  ///< \copydoc getRawData()

  /// Converts the variant to contain a copy of a user-specified type T.
  /// The type must have been previously registered with registerType().
  /// \sa toUserType()  isUserType()  registerType()  moveToUserType()
  template <class T> void     setToUserType(const T &t);

  /// Moves the given value of a user-specified type T into the value. rvalue references wheee
  /// The type must have been previously registered with registerType().
  /// \sa toUserType()  setToUserType() isUserType()  registerType()
  template <class T> void     moveToUserType(T && t);

  /// Converts the type of the variant to this user type (if necessary), and returns
  ///   a reference to the type. This is for optimization purposes, to avoid temporary objects.
  template <class T> T& convertToUserType();

  /// Attempts to convert the Variant to a specific user type.
  /// In general, conversion from different user-types is not possible, so this will only succeed if the Variant
  ///   was set to exactly this type.
  /// Will return a default contructed value of type T if the conversion is not possible.
  /// \sa setToUserType() isUserType()  registerType()  moveToUserType()
  template <class T> T        toUserType() const;
  /// Very fast converion of a Variant to a specific user-defined type.
  /// Used only where performance is critical, and where you are certain the Variant
  ///   contains a value of *exactly* that type.  
  /// If the Variant is not of that type the return value is undefined.
  template <class T> T &      fastToUserType() { return conv<VUserType<T> >().value(); }
  template <class T> const T &fastToUserType() const { return conv<VUserType<T> >().value(); }  ///< \copydoc fastToUserType()
  /// Returns TRUE if the Variant is of the user-defined type specified by T.
  /// \code if (v.isUserType<MyType>()) {  <do something> } \endcode
  /// The type must have been registered using registerType().
  /// \sa setToUserType() isUserType() registerType() moveToUserType()
  template <class T> bool     isUserType() const { return getType()==VUserType<T>::typeNumber_; }
  /// Returns the name associated with a given type value, of "UnKnown" if the type is not valid.
  static BASE_EXPORT String  getTypeName(Type type);
  /// Returns the size of the raw data available for Variant storage, in bytes.
  /// \sa setToRawData()  getRawData()
  static BASE_EXPORT int     getRawDataSize() { return dataSize_; }
  /// Returns true if the type value has already been claimed.
  /// This function can be used to find an unused valid type when calling registerType().
  static BASE_EXPORT bool    getUserTypeClaimed(int type);
  /** \brief This method allows user-defined types to be encoded in a variant.  
    *
    * Two steps must be followed.    
    * First, a static instance of a VUserType<T>::typeNumber_ must be created and initialized to zero.
    * Second, this static method must be called with a type number and a string used to indicate the type.
    * \returns false if the type number in invalid or is already in use.\n
    * Here is an example:
    * \code  
    *   qint32 VUserType<MyType>::typeNumber_(0);
    *   void myInitializationRoutine()
    *   {
    *     Variant::registerType<MyType>(Variant::UserType+20,"My Type");
    *   }
    * \endcode
    */
  template <class T> static bool registerType(int type,const String &name);
  
private:
  static const int version_;
  bool isSpecialType() const { return type_ >= StringType; }

  static BASE_EXPORT bool claimUserType(int type,const String &name);

  template <class T> T &conv() { return *reinterpret_cast<T *>(data_); }
  template <class T> const T &conv() const { return *reinterpret_cast<const T *>(data_); }
  void del() { if (isSpecialType()) conv<UserTypeBase>().~UserTypeBase(); }

  Type type_;
  union {
    Byte   byte_;
    UByte  ubyte_;
    Short  short_;
    UShort ushort_;
    Int    int_;
    UInt   uint_;
    Long   long_;
    ULong  ulong_;
    Float  float_;
    Double double_;
    void * void_;
    bool   bool_;
    char   data_[dataSize_];
  };
};
#pragma warning(pop)

/// Template specialization for void pointers; this should probably never be used, except to determine the size of VUserType for type_selector.
template <> class Variant::VUserType<void *> : public Variant::UserTypeBase {
public:
  /// Default constructor
  VUserType() : t_(nullptr) { }
  VUserType(void * const t) : t_(t) { }
  /// Destructor
  virtual ~VUserType() { }
  /// in-place clone
  virtual VUserType<void *> *clone(char *data) const { return new(data) VUserType<void *>(t_); }
  /// copy from base pointer, uses reinterpret_cast to upcast (careful!).
  virtual void copy(const UserTypeBase *base)
  {
    const VUserType<void *> *p = reinterpret_cast<const VUserType<void *> *>(base);
    if (!p) return;
    t_ = p->t_;
  }

  static bool isValue () { return true; } ///< Checks if the user type holds the literal value of T, or a pointer to it.
  void * t_;  ///< The actual value being stored.
  static int typeNumber_; ///< The typeNumber_ used to uniquely identify the type.
};

template <typename T> class Variant::VUserType : public Variant::UserTypeBase, public itasca::conditional_container<sizeof(T) + sizeof(Variant::VUserType<void *>) - sizeof(void *) <= Variant::dataSize_, T>
{
public:
  /// Default constructor
  VUserType(const T &t=T()) : itasca::conditional_container<sizeof(T) + sizeof(VUserType<void *>) - sizeof(void *) <= Variant::dataSize_, T>(t) {};
  /// Move constructor
  VUserType(T && t) : itasca::conditional_container<sizeof(T) + sizeof(VUserType<void *>) - sizeof(void *) <= Variant::dataSize_, T>(std::move(t)) {};
  /// Destructor
  virtual ~VUserType() {}
  /// in-place clone
  virtual VUserType<T> *clone(char *data) const { return new(data) VUserType<T>(this->value()); }
  /// copy from base pointer, uses reinterpret_cast to upcast (careful!).
  virtual void copy(const UserTypeBase *base)
  {
    const VUserType<T> *p = reinterpret_cast<const VUserType<T> *>(base);
    if (!p) return;
    operator=(*p);
  }

  static int typeNumber_; ///< The typeNumber_ used to uniquely identify the type.
};


template <class T> inline void Variant::setToUserType(const T &t)
{
  if (getType()!=VUserType<T>::typeNumber_)
  {
    del();
    new(data_) VUserType<T>(t);
    type_ = static_cast<Type>(VUserType<T>::typeNumber_);
    return;
  }
  conv<VUserType<T>>() = t;
}

template <class T> inline void Variant::moveToUserType(T && t) {
  if ( getType() != VUserType<T>::typeNumber_ ) {
    del();
    new(data_) VUserType<T>(std::move(t));
    type_ = static_cast<Type>(VUserType<T>::typeNumber_);
    return;
  }
  conv<VUserType<T>>() = std::move(t);
}

template <class T> inline T Variant::toUserType() const {
  if (getType()!=VUserType<T>::typeNumber_)
    return T();
  return conv<VUserType<T>>().VUserType<T>::value();
}

template <class T> T& Variant::convertToUserType() {
    if (getType() != VUserType<T>::typeNumber_) {
        del();
        new(data_) VUserType<T>();
        type_ = static_cast<Type>(VUserType<T>::typeNumber_);
    }
    VUserType<T>& v = conv<VUserType<T>>();
    return v.value();
}

template <class T> bool Variant::registerType(int typeNumber,const String &name) {
  //if ( sizeof(VUserType<T>) > dataSize_ ) { throw std::exception("ERROR: Type too large for variant."); }
  static_assert(sizeof(VUserType<T>) <= dataSize_, "ERROR: Type too large for variant.");
  if (!claimUserType(typeNumber,name)) return false;
  VUserType<T>::typeNumber_ = typeNumber;
  return true;
}
/// @}

// EOF
