#pragma once
/** \file farray.h
*  \brief An array class that attempts to minimize unnecessary heap access.
* \addtogroup Base Base interface specification 
* @{
*/


/**
* \brief An array class that attempts to minimize unnecessary heap access.
*
* T is the data type.\n
* S is the stack length.\n
* This class attempts to provide a speed-optimized array
*   for use in plot generation.\n
* It does this in two ways.\n
* First, it attempts to minimize heap churn by not deallocating memory.\n
* Second, it has a second template argument that indicates how much memory 
*   to create ON THE STACK. Only if the array size needs to exceed S, does 
*   it actually use memory from the heap.\n
*/
//#pragma warning(disable:4244)
//#pragma warning(disable:4267)
#ifdef __LINUX
#include "basetoqt.h"
#endif
template <class T,uint64 S=32> class FArray {
public:
    using size_type = uint64;          ///< Typedef to assist in STL compatibility
    using value_type = T;              ///< Typedef to assist in STL compatibility
    using iterator = T *;              ///< Typedef to assist in STL compatibility
    using const_iterator = const T *;  ///< Typedef to assist in STL compatibility

    /// Default constructor - the array size is zero.
    FArray() : begin_(reinterpret_cast<T *>(arr_.data())), end_(begin_), allocated_(S), size_(0) { }
    /** Constructs the array with size s, and each entry in the array has initial value t.
    *  \param s Initial size of the array.
    *  \param t The value each element is initialized with.
    */
    explicit FArray(size_type s,const T &t=T()) : begin_(reinterpret_cast<T *>(arr_.data())), end_(begin_), allocated_(S), size_(0) { checkAllocated(s); end_ = begin_ + s; for (T *b=begin_;b<end_;++b) new(b) T(t); size_ = s; }
    /// Copy constructor, valid for FArrays of the same data type but different stack lengths.
    template <uint64 S2> FArray(const FArray<T,S2> &f) : begin_(reinterpret_cast<T *>(arr_.data())), end_(begin_), allocated_(S), size_(0) { operator=(f); }
    /// Specialized copy constructor, for the special case of when the stack lengths are the same.
    FArray(const FArray<T,S> &f) : begin_(reinterpret_cast<T *>(arr_.data())), end_(begin_), allocated_(S), size_(0) { operator=(f); }
    FArray(std::initializer_list<T> l) : begin_(reinterpret_cast<T *>(arr_.data())), end_(begin_), allocated_(S), size_(0) { 
        for (auto it=l.begin();it!=l.end();++it)
            push_back(*it);
    }
    /// Destructor
    ~FArray() { reset(); }
    /// Assignment operator, valid for FArrays of the same data type but different stack lengths.
    template <uint64 S2> const FArray<T,S> &operator=(const FArray<T,S2> &f) { 
        clear(); 
        checkAllocated(f.size());
        end_ = begin_ + f.size(); 
        size_ = f.size();
        T *bl = begin_;
        for (auto br=f.begin();bl<end();++bl,++br) 
            new(bl) T(*br); 
        return *this; 
    }
    /// Specialized assignment operator, for the special case of when the stack lengths are the same.
    const FArray<T,S> &operator=(const FArray<T,S> &f) {
        clear();
        checkAllocated(f.size());
        end_ = begin_ + f.size();
        size_ = f.size_;
        T *bl=begin_;
        for (T *br=f.begin_;bl<end_;++bl,++br) 
            new(bl) T(*br); 
        return *this; 
    }

    // Accessors
    /// \return the current size of the array - NOT the stack length OR the size allocated internally.
    size_type size() const { return size_; }
    /// Returns the size of the array pre-allocated on the stack.
    size_type stackSize() const { return S; }
    /// \return the size allocated internally -- this will be S (the stack length) unless the 
    ///         array has been made large enough to force heap access.
    size_type allocated() const { return allocated_; }
    /// \return true if the array is of zero length.
    bool      empty() const { return end_ == begin_; }
    /// \return a pointer to the beginning of the array data.  An FARRAY guarantees that it's internal
    ///         data is contiguous.
    T *       data() { return begin_; }
    /// \return a const pointer to the beginning of the array data.  An FARRAY guarantees that it's internal
    ///         data is contiguous.
    const T * data() const { return begin_; }
    /// \return a reference to the first element in the array.  This is undefined if the array is of zero length.
    T &       front() { return *begin_; }
    /// \return a const reference to the first element in the array.  This is undefined if the array is of zero length.
    const T & front() const { return *begin_; }
    /// \return a reference to the last element in the array.  This is undefined if the array is of zero length.
    T &       back() { return *(end_-1); }
    /// \return a const reference to the last element in the array.  This is undefined if the array is of zero length.
    const T & back() const { return *(end_-1); }
    /// Searches the array to find the first element matching \a t.  
    /// Returns the index found, or limits<size_type>::max() if not found.
    /// This search is linear time.
    size_type find(const T &t) const {
        for (size_type i=0;i<size();++i) {
            if (at(i)==t) return i;
        }
        return limits<size_type>::max();
    }

    // Manipulators
    /// Adds a new element to the end of the array, increasing the array size by one.
    void push_back(const T &t) { checkAllocated(size()+1); new(end_) T(t); ++end_; ++size_; }
    /// To avoid creating a temporary object, can call this to get memory location of new lcoation at end of 
    ///   stack and then use it to do an in-place new.
    /// new(fa.emplace_back()) Fred(arg1,arg2);
    T *emplace_back() { checkAllocated(size()+1); ++size_; return end_++; }    
    /// emplace_n_back: make sure n new positions are allocated and return a pointer to the first of those positions;
    /// caller should create objects at those positions.
    T *emplace_n_back(uint64 n) { checkAllocated(size() + n);  T *ret = end_; end_ += n;  size_ += n;  return ret; }
    /// Removes the last element in the array.  The results are undefined if the array is of zero length.
    void pop_back() { assert(size()); --end_; end_->~T(); --size_; }
    /// \param i The new array length.
    /// \param t The value assigned to new array elements (if any).
    /// Changes the actual length (not the allocated length) of the array to i.
    /// May increase the allocated length, but will not decrease it.
    void resize(size_type i,const T &t=T()) { 
        if (size()==i) return;
        if (size()>i) {
            T *e = end_;
            end_ = begin_ + i;
            for (T *p=e-1;p>=end_;--p)
                p->~T();
            size_ = i;
            return;
        }
        size_type s = size();
        checkAllocated(i); 
        end_ = begin_ + i; 
        size_ = i;
        for (T *p=begin_+s;p<end_;++p)
            new(p) T(t);
    }
    /// Inserts a value into the array at position i (the new position it will occupy).
    /// If I is past the end of the array, adds to end.
    iterator insert(size_type i,const T &t) {
        i = std::min(size(),i);
        if (i+1>size()) push_back(t);
        else {
            push_back(back());
            for (iterator p=end_-2;p>begin_+i;--p)
                *p = *(p-1);
            *(begin_+i) = t;
        }
        return begin_ + i;
    }
    /// Adds a value to the array, first making certain it is big enough to hold it
    void put(size_type i,const T &t) {
        if (size()<=i) {
            resize(i);
            push_back(t);
        } else
            at(i) = t;
    }
    /// Appends the contents of one FArray onto another
    template <class C> void append(const C &in) { for (auto i=in.begin();i!=in.end();++i) push_back(*i); }
    /// Inserts a value intot he array at position i (the new position it will occupy).
    /// If I is past the end of the array, adds to end.
    iterator insert(iterator it,const T &t) {
        size_type i = it - begin_;
        i = std::min(size(),i);
        if (i+1>size()) push_back(t);
        else {
            push_back(back());
            for (iterator p=end_-2;p>begin_+i;--p)
                *p = *(p-1);
            *(begin_+i) = t;
        }
        return begin_ + i;
    }
    /// Removes a value from the array at position i
    /// If i is past the end of the array, does nothing.
    bool remove(size_type i) {
        if (i>=size()) return false;
        for (iterator it=begin_+i;it+1<end_;++it)
            *it = *(it+1);
        pop_back();
        return true;
    }
    /// Removes all values from the array matching t (operator==).
    /// Returns the number of objects removed
    size_type removeAll(const T &t) {
        size_type ret = 0;
        for (size_type i=0;i<size();) {
            if (t==at(i)) {
                remove(i);
                ++ret;
            } else
                ++i;
        }
        return ret;
    }
    /// Resets the array size to zero, destroying all current elements.
    /// The allocated length is not changed.
        void clear() {
        // if type T is not plain old data, call the destructor for each array element;
        // std::is_pod is deprecated in C++20; can be replaced with is_integral, is_floating_point etc.
        if (!std::is_pod<T>::value)
            for (T *b=begin_;b<end_;++b) 
                b->~T();
        end_ = begin_; 
        size_ = 0; 
        }
    /// Resets the array size to zero. No destructor is called for the array elements.
    /// The allocated length is not changed.
    void clear_no_destruct() { end_ = begin_; size_ = 0; }
    /// Resets the array size to zero, destroying all current elements.
    /// The allocated length is reset back to the stack length, and any heap 
    ///   memory is returned.
    void reset() { 
        clear(); 
        if (begin_!=reinterpret_cast<T *>(arr_.data())) 
            ::free(begin_); 
        end_ = begin_ = reinterpret_cast<T *>(arr_.data()); 
        allocated_ = S; 
    }
    /// Reduces the allocated length to the actual length of the array, or the
    ///   stack length, whichever is greater.  This and reset() are the only
    ///    means of reducing the allocated length of an existing FArray.
    void clip() { if (size()*2<allocated_ && size()>S) changeAllocated(size()); }

    // Member access functions
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    iterator       begin() { return begin_; }
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator begin() const { return begin_; }
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator constBegin() const { return begin_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    iterator       end() { return end_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator end() const { return end_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator constEnd() const { return end_; }
    /// \return A reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    T &            at(size_type i) { return begin_[i]; }
    /// \return A const reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    const T &      at(size_type i) const { return begin_[i]; }
    /// \return A reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    T &            operator[](size_type i) { return at(i); }
    /// \return A const reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    const T &      operator[](size_type i) const { return at(i); }
    /// \return A copy of the value at the ith location in the array, or t if i
    ///         is an invalid index.
    /// This version uses a signed integer type (32 bit) as an index.
    T              value(int i,const T &t=T()) const { if (i<0) return t;  if (to<size_type>(i)>=size()) return t;  return at(i); }
    /// \return A copy of the value at the ith location in the array, or t if i
    ///         is an invalid index.
    /// This version uses an unsigned 32 bit integer type as an index.
    T              value(quint32 i,const T &t=T()) const { if (to<size_type>(i)>=size()) return t;  return at(i); }
    /// \return A copy of the value at the ith location in the array, or t if i
    ///         is an invalid index.
    /// This version uses an unsigned 64 bit integer type as an index.
    T              value(uint64 i,const T &t=T()) const { if (to<size_type>(i)>=size()) return t;  return at(i); }
private:
    void checkAllocated(size_type target) {
        static const size_type max_increase = 1000000;
        if (target<=allocated_) return;
        size_type newsize = std::max<size_type>(allocated_,8);
        while (newsize<target)
            newsize += to<size_type>(std::min(max_increase,newsize));
        changeAllocated(newsize);
    }
    void changeAllocated(size_type newSize) {
        size_type old_size = size();
        assert(old_size<=newSize);
        T *newBegin = reinterpret_cast<T *>(arr_.data());
        if (newSize>S)
#ifdef _DEBUG
            newBegin = reinterpret_cast<T *>(itasca::memory::imalloc(newSize*sizeof(T),__FILE__,__LINE__));
#else
            newBegin = reinterpret_cast<T *>(std::malloc(newSize*sizeof(T)));
#endif
        T *tl = newBegin;
        for (T *tr=begin_;tr<end_;++tl,++tr) {
            new(tl) T(*tr);
            tr->~T();
        }
        if (begin_!=reinterpret_cast<T *>(arr_.data())) 
            std::free(begin_);
        begin_ = newBegin;
        end_ = begin_ + old_size;
        allocated_ = newSize;
    }
    union { // Note:  std::array<> used so you can see contents in debugger (which has been really irritating).
        T* start_; // Placed in this union so constructors don't get called on creation.
        std::array<T, S> arr_;
    };
    T* begin_ = arr_.data();
    T* end_ = &begin_;
    size_type allocated_ = S;
    size_type size_ = 0;
};
/// Template specialization for S=0 - indication no stack space at all should be made.
template <class T> class FArray<T,0> {
public:
    using size_type = uint64;     ///< Typedef to assist in STL compatibility
    using iterator = T *;        ///< Typedef to assist in STL compatibility
    using const_iterator = const T *;  ///< Typedef to assist in STL compatibility
    using value_type = T;

    /// Default constructor - the array size is zero.
    FArray() : begin_(0), end_(0), allocated_(0), size_(0) { }
    /** Constructs the array with size s, and each entry in the array has initial value t.
    *  \param s Initial size of the array.
    *  \param t The value each element is initialized with.
    */
    FArray(size_type s,const T &t=T()) : begin_(0), end_(begin_), allocated_(0), size_(0) { checkAllocated(s); end_ = begin_ + s; for (T *b=begin_;b<end_;++b) new(b) T(t); size_ = s; }
    /// Copy constructor, valid for FArrays of the same data type but different stack lengths.
    template <uint64 S2> FArray(const FArray<T,S2> &f) : begin_(0), end_(begin_), allocated_(0), size_(0) { operator=(f); }
    /// Specialized copy constructor, for the special case of when the stack lengths are the same.
    FArray(const FArray<T,0> &f) : begin_(0), end_(begin_), allocated_(0), size_(0) { operator=(f); }
    FArray(std::initializer_list<T> l) : begin_(0), end_(begin_), allocated_(0), size_(0) { 
        for (auto it=l.begin();it!=l.end();++it)
            push_back(*it);
    }
    /// Destructor
    ~FArray() { reset(); }
    /// Assignment operator, valid for FArrays of the same data type but different stack lengths.
    template <uint64 S2> const FArray<T,0> &operator=(const FArray<T,S2> &f) { 
        clear(); 
        checkAllocated(f.size());
        end_ = begin_ + f.size(); 
        size_ = f.size();
        T *bl=begin_;
        for (T *br=f.begin();bl<end();++bl,++br) 
            new(bl) T(*br); 
        return *this; 
    }
    /// Specialized assignment operator, for the special case of when the stack lengths are the same.
    const FArray<T,0> &operator=(const FArray<T,0> &f) {
        clear();
        checkAllocated(f.size());
        end_ = begin_ + f.size();
        size_ = f.size_;
        T *bl=begin_;
        for (T *br=f.begin_;bl<end_;++bl,++br) 
            new(bl) T(*br); 
        return *this; 
    }

    // Accessors
    /// \return the current size of the array - NOT the stack length OR the size allocated internally.
    const size_type &size() const { return size_; }
    /// \return the size allocated internally -- this will be S (the stack length) unless the 
    ///         array has been made large enough to force heap access.
    const size_type &allocated() const { return allocated_; }
    /// \return true if the array is of zero length.
    bool             empty() const { return end_ == begin_; }
    /// \return a pointer to the beginning of the array data.  An FARRAY guarantees that it's internal
    ///         data is contiguous.
    T *              data() { return begin_; }
    /// \return a const pointer to the beginning of the array data.  An FARRAY guarantees that it's internal
    ///         data is contiguous.
    const T *        data() const { return begin_; }
    /// \return a reference to the first element in the array.  This is undefined if the array is of zero length.
    T &              front() { return *begin_; }
    /// \return a const reference to the first element in the array.  This is undefined if the array is of zero length.
    const T &        front() const { return *begin_; }
    /// \return a reference to the last element in the array.  This is undefined if the array is of zero length.
    T &              back() { return *(end_-1); }
    /// \return a const reference to the last element in the array.  This is undefined if the array is of zero length.
    const T &        back() const { return *(end_-1); }

    // Manipulators
    /// Adds a new element to the end of the array, increasing the array size by one.
    void push_back(const T &t) { checkAllocated(size()+1); new(end_) T(t); ++end_; ++size_; }
    /// To avoid creating a temporary object, can call this to get memory location of new lcoation at end of 
    ///   stack and then use it to do an in-place new.
    /// new(fa.emplace_back()) Fred(arg1,arg2);
    T *emplace_back() { checkAllocated(size()+1); ++size_; return end_++; }
    /// emplace_n_back: make sure n new positions are allocated and return a pointer to the first of those positions;
    /// caller should create objects at those positions.
    T *emplace_n_back(uint64 n) { checkAllocated(size() + n);  T *ret = end_; end_ += n;  size_ += n;  return ret; }
    /// Removes the last element in the array.  The results are undefined if the array is of zero length.
    void pop_back() { assert(size()); --end_; end_->~T(); --size_; }
    /// \param i The new array length.
    /// \param t The value assigned to new array elements (if any).
    /// Changes the actual length (not the allocated length) of the array to i.
    /// May increase the allocated length, but will not decrease it.
    void resize(size_type i,const T &t=T()) { 
        if (size()==i) return;
        if (size()>i) {
            T *e = end_;
            end_ = begin_ + i;
            for (T *p=e-1;p>=end_;--p)
                p->~T();
            size_ = i;
            return;
        }
        auto s = size();
        checkAllocated(i); 
        end_ = begin_ + i; 
        size_ = i;
        for (T *p=begin_+s;p<end_;++p)
            new(p) T(t);
    }
    /// Inserts a value intot he array at position i (the new position it will occupy).
    /// If I is past the end of the array, adds to end.
    iterator insert(size_type i,const T &t) {
        i = std::min(size(),i);
        if (i+1>size()) push_back(t);
        else {
            push_back(back());
            for (iterator p=end_-2;p>begin_+i;--p)
                *p = *(p-1);
            *(begin_+i) = t;
        }
        return begin_ + i;
    }
    /// Inserts a value intot he array at position i (the new position it will occupy).
    /// If I is past the end of the array, adds to end.
    iterator insert(iterator it,const T &t) {
        size_type i = it - begin_;
        i = std::min(size(),i);
        if (i+1>size()) push_back(t);
        else {
            push_back(back());
            for (iterator p=end_-2;p>begin_+i;--p)
                *p = *(p-1);
            *(begin_+i) = t;
        }
        return begin_ + i;
    }
    /// Adds a value to the array, first making certain it is big enough to hold it
    void put(size_type i,const T &t) {
        if (size()<=i) {
            resize(i);
            push_back(t);
        } else
            at(i) = t;
    }
    /// Appends the contents of one FArray onto another
    template <class C> void append(const C &in) { for (auto i=in.begin();i!=in.end();++i) push_back(*i); }
    /// Removes a value from the array at position i
    /// If i is past the end of the array, does nothing.
    bool remove(size_type i) {
        if (i>=size()) return false;
        for (iterator it=begin_+i;it<end_;++it)
            *it = *(it+1);
        pop_back();
        return true;
    }
    /// Removes all values from the array matching t (operator==).
    /// Returns the number of objects removed
    size_type removeAll(const T &t) {
        size_type ret = 0;
        for (size_type i=0;i<size();) {
            if (t==at(i)) {
                remove(i);
                ++ret;
            } else
                ++i;
        }
        return ret;
    }
    /// Resets the array size to zero, destroying all current elements.
    /// The allocated length is not changed.
        void clear() {
        // if type T is not plain old data, call the destructor for each array element;
        // std::is_pod is deprecated in C++20; can be replaced with is_integral, is_floating_point etc.
        if (!std::is_pod<T>::value)
            for (T *b=begin_;b<end_;++b) 
                b->~T();
        end_ = begin_; 
        size_ = 0; 
        }
    /// Resets the array size to zero. No destructor is called for the array elements.
    /// The allocated length is not changed.
    void clear_no_destruct() { end_ = begin_; size_ = 0; }
    /// Resets the array size to zero, destroying all current elements.
    /// The allocated length is reset back to the stack length, and any heap 
    ///   memory is returned.
    void reset() { 
        clear(); 
        if (begin_) 
            std::free(begin_); 
        end_ = begin_ = 0; 
        allocated_ = 0; 
    }
    /// Reduces the allocated length to the actual length of the array, or the
    ///   stack length, whichever is greater.  This and reset() are the only
    ///    means of reducing the allocated length of an existing FArray.
    void clip() { if (size()*2<allocated_) changeAllocated(size()); }

    // Member access functions
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    iterator       begin() { return begin_; }
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator begin() const { return begin_; }
    /// \return An iterator representing the first element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator constBegin() const { return begin_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    iterator       end() { return end_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator end() const { return end_; }
    /// \return An iterator representing one past the last element in the array.  
    /// Implemented as a pointer.  STL compatible as a random-access iterator.
    const_iterator constEnd() const { return end_; }
    /// \return A reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    T &            at(size_type i) { return begin_[i]; }
    /// \return A const reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    const T &      at(size_type i) const { return begin_[i]; }
    /// \return A reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    T &            operator[](size_type i) { return at(i); }
    /// \return A const reference to the element at the ith location in the array.
    /// Undefined if the array length is less than or equal to i.
    /// No bounds checking is performed, even in a debug compile.
    const T &      operator[](size_type i) const { return at(i); }
    /// \return A copy of the value a the ith location in the array, or t if i
    ///         in an invalid index.
    /// This version uses a signed integer type (32 bit) as an index.
    T              value(int i,const T &t=T()) const { if (i<0) return t;  if (to<size_type>(i)>=size()) return t;  return at(i); }
    /// \return A copy of the value a the ith location in the array, or t if i
    ///         in an invalid index.
    /// This version uses an unsigned 32 bit integer type as an index.
    T              value(quint32 i,const T &t=T()) const { if (to<size_type>(i)>=size()) return t;  return at(i); }
    /// \return A copy of the value a the ith location in the array, or t if i
    ///         in an invalid index.
    /// This version uses an unsigned 64 bit integer type as an index.
    T              value(uint64 i,const T &t=T()) const { if (to<size_type>(i)>=size()) return t;  return at(i); }
private:
    void checkAllocated(size_type target) {
        if (target<=allocated_) return;
        size_type newsize = std::max<size_type>(allocated_,8);
        while (newsize<target)
            newsize += std::min<size_type>(1000000,newsize);
        changeAllocated(newsize);
    }
    void changeAllocated(size_type newSize) {
        size_type old_size = size();
        assert(old_size<=newSize);
        T *newBegin = 0;
        if (newSize)
#ifdef _DEBUG
            newBegin = reinterpret_cast<T *>(itasca::memory::imalloc(newSize*sizeof(T),__FILE__,__LINE__));
#else
            newBegin = reinterpret_cast<T *>(std::malloc(newSize*sizeof(T)));
#endif
        T *tl = newBegin;
        for (T *tr=begin_;tr<end_;++tl,++tr) {
            new(tl) T(*tr);
            tr->~T();
        }
        std::free(begin_);
        begin_ = newBegin;
        end_ = begin_ + old_size;
        allocated_ = newSize;
    }
    T *       begin_;
    T *       end_;
    size_type allocated_;
    size_type size_;
};
//#pragma warning(default:4244)
//#pragma warning(default:4267)


/// @}
// EoF
