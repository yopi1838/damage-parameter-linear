#pragma once

namespace itasca
{
  /**
    \brief Class used to select between two types based on a given condition.
    \author JT Davies

    \param condition Either true or false; determines which specification to use.
    \param if_true Type to be stored if true.
    \param if_false Type to be stored if false.
      
    This class uses template metaprogramming to act as an "if" statement to determine whether to use one type or another.
    If compile-time values such as <code>sizeof()</code>, or simply constants, are solely involved in <code>condition</code>, this can be evaluated at compile time.
  */
  template<bool condition, typename if_true, typename if_false>
  struct type_selector
  {
    static const bool result;
  };
 
  /**
    Specifies the "true" case for type_selector.

    \param if_true Type to be returned.
    \param if_false Required for specification, but useless in this case.
  */
  template<typename if_true, typename if_false>
  struct type_selector<true, if_true, if_false>
  {
    /// The actual "return value" of the type_selector.
    typedef if_true type;
    /// Can by used by checks to determine the result of the type_selector.
    static const bool result = true;
  };

  /**
    Specifies the "false" case for type_selector.

    \param if_true Required for specification, but useless in this case.
    \param if_false Type to be returned.
  */
  template<typename if_true, typename if_false>
  class type_selector<false, if_true, if_false>
  {
    /// The actual "return value" of the type_selector.
    typedef if_false type;
    /// Can be used by checks to determine the result of the type_selector.
    static const bool result = false;
  };

  /**
    \brief A container, which either stores its value as a value or a pointer depending on the condition.
    
    This is a more specific version of the same trick used by type_selector, which abstracts away whether a type is a pointer or a value.
    Primary user is VUserType<T>, where it enables types larger than the data buffer by automatically storing them as a pointer.
  */
  template<bool condition, typename T>
  class conditional_container: public type_selector<condition, T, T *> {};

  template<typename T>
  class conditional_container<true, T>
  {
  public:
    conditional_container() { }
    conditional_container(const T &t) : value_(t) {}

    conditional_container(const conditional_container<true, T> & other) : value_(other.value_) {}
    conditional_container(const conditional_container<false, T> & other); // needs to be defined after false

    void operator= (const T & t) { value_ = t; }
    void operator= (T && t) { value_ = std::move(t); }

    void operator= (const conditional_container<true, T> & other) { value_ = other.value(); }
    void operator= (const conditional_container<false, T> & other); // see above

    T & value () { return value_; }
    const T & value () const { return value_; }

    static inline bool isValue () { return true; }
  private:
    T value_;
  };

  template<typename T>
  class conditional_container<false, T>
  {
  public:
    conditional_container() { value_ = new T; }
    conditional_container(const T & t) : value_(nullptr) { value_ = new T(t); }
    conditional_container(T && t) : value_(nullptr) { value_ = new T(std::move(t)); }

    conditional_container(const conditional_container<true, T> & other) { value_ = new T(other.value()); }
    conditional_container(const conditional_container<false, T> & other)  { value_ = new T(other.value()); }
    conditional_container(conditional_container<false, T> && other)
    {
      value_ = other.value_;
      other.value_ = nullptr;
    }

    ~conditional_container() { if ( value_ != nullptr ) { delete value_; } }

    void operator= (const T & t) { *value_ = t; }
    void operator= (T && t) { *value_ = std::move(t); }

    void operator= (const conditional_container<true, T> & other) { *value_ = other.value(); }
    void operator= (const conditional_container<false, T> & other) { *value_ = other.value(); }
    void operator= (conditional_container<false, T> && other)
    {
      T * temp = value_;
      value_ = other.value_;
      other.value_ = temp;
    }

    T & value () { return *value_; }
    const T & value () const { return *value_; }

    static inline bool isValue () { return false; }
  private:
    T * value_;
  };

  template<typename T>
  conditional_container<true, T>::conditional_container (const conditional_container<false, T> & other) { value_ = other.conditional_container<false, T>::value(); }

  template<typename T>
  void conditional_container<true, T>::operator= (const conditional_container<false, T> & other) { value_ = other.conditional_container<false, T>::value(); }
}