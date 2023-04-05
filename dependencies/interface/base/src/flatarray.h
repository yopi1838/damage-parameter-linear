#pragma once

template <class T> class FlatArray {
public:
    typedef quint64  size_type;       ///< Typedef to assist in STL compatibility
    typedef T *      iterator;        ///< Typedef to assist in STL compatibility
    typedef const T *const_iterator;  ///< Typedef to assist in STL compatibility

                                      /// Default constructor - the array size is zero.
    FlatArray() { }
    FlatArray(size_type s,double fac=1.3) : defaultStorage_(s), multFac_(fac) { checkAllocated(s); }
    FlatArray(const FlatArray<T> &f) { operator=(f); }
    FlatArray(std::initializer_list<T> l) { 
        for (auto it=l.begin();it!=l.end();++it)
            push_back(*it);
    }
    ~FlatArray() { defaultStorage_=0; reset(); }
    const FlatArray<T> &operator=(const FlatArray<T> &f) { 
        clear(); 
        defaultStorage_ = f.defaultStorage_;
        checkAllocated(f.size());
        end_ = begin_ + f.size(); 
        size_ = f.size();
        T *bl=begin_;
        for (T *br=f.begin();bl<end();++bl,++br) 
            new(bl) T(*br); 
        return *this; 
    }
    const size_type &size() const { return size_; }
    const size_type &allocated() const { return allocated_; }
    bool             empty() const { return end_ == begin_; }
    T *              data() { return begin_; }
    const T *        data() const { return begin_; }
    T &              front() { return *begin_; }
    const T &        front() const { return *begin_; }
    T &              back() { return *(end_-1); }
    const T &        back() const { return *(end_-1); }

    void push_back(const T &t) { checkAllocated(size()+1); new(end_) T(t); ++end_; ++size_; }
    T *emplace_back() { checkAllocated(size()+1); ++size_; return end_++; }
    void pop_back() { assert(size()); --end_; end_->~T(); --size_; }
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
    size_type removeReplaceLast(size_type i) {
        if (i>=size()) return -1;
        auto last = end_-1;
        iterator it = &begin_[i];
        (*it) = *last;
        pop_back();
        return size();
    }
    size_type removeReplaceLastClip(size_type i) {
        if (i>=size()) return -1;
        auto last = end_-1;
        iterator it = &begin_[i];
        (*it) = *last;
        pop_back();
        clip();
        return size();
    }

    bool remove(size_type i) {
        if (i>=size()) return false;
        for (iterator it=begin_+i;it<end_;++it)
            *it = *(it+1);
        pop_back();
        return true;
    }

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

    void clear() {
        if (!std::is_pod<T>::value)
            for (T *b=begin_;b<end_;++b) 
                b->~T();
        end_ = begin_; 
        size_ = 0; 
    }

    void reset() { 
        clear(); 
        if (defaultStorage_ != allocated_) {
            if (begin_) 
                std::free(begin_); 
            end_ = begin_ = 0; 
            allocated_ = 0; 
            if (defaultStorage_)
                changeAllocated(defaultStorage_);
        }
    }

    void clip() { if (size()*multFac_<allocated_) changeAllocated(size()*multFac_); }
    iterator       begin() { return begin_; }
    const_iterator begin() const { return begin_; }
    const_iterator constBegin() const { return begin_; }
    iterator       end() { return end_; }
    const_iterator end() const { return end_; }
    const_iterator constEnd() const { return end_; }
    T &            at(size_type i) { return begin_[i]; }
    const T &      at(size_type i) const { return begin_[i]; }
    T &            operator[](size_type i) { return at(i); }
    const T &      operator[](size_type i) const { return at(i); }
private:
    void checkAllocated(size_type target) {
        if (target<=allocated_) return;
        // How many should be allocated? 
        size_type oldSize = std::max<size_type>(allocated_,defaultStorage_);
        size_type newsize = multFac_*std::max<size_type>(target,oldSize);
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
    T *       begin_ = 0;
    T *       end_= 0;
    size_type allocated_ = 0;
    size_type size_ = 0;
    size_type defaultStorage_ = 1000;
    double    multFac_ = 1.3;
};

template <class T> class FlatArrayVec : public std::vector<T> {
public:
    typedef quint64  size_type;       
    FlatArrayVec() { }
    FlatArrayVec(size_type s,double =1.3) : defaultStorage_(s) { this->reserve(defaultStorage_); }
    FlatArrayVec(const FlatArrayVec<T> &f) { operator=(f); }
    FlatArrayVec(std::initializer_list<T> l) { 
        for (auto it=l.begin();it!=l.end();++it)
            push_back(*it);
    }
    ~FlatArrayVec() { defaultStorage_=0; reset(); }
    const FlatArrayVec<T> &operator=(const FlatArrayVec<T> &f) { 
        defaultStorage_ = f.defaultStorage_;
        this->clear();
        reserve(f.capacity());
        assign(f.begin(),f.end()); 
        return *this; 
    }
    const size_type &allocated() const { return this->capacity(); }
    size_type removeReplaceLast(size_type i) {
        if (i>=this->size()) return -1;
        std::swap(this->at(i),this->back());
        this->pop_back();
        return this->size();
    }
    size_type removeReplaceLastClip(size_type i) {
        size_type ret = removeReplaceLast(i);
        this->shrink_to_fit();
        return ret;
    }

    bool remove(size_type i) {
        if (i>=this->size()) return false;
        this->erase(i);
        return true;
    }
    size_type removeAll(const T &t) {
        size_type ret = 0;
        for (size_type i=0;i<this->size();) {
            if (t==this->at(i)) {
                remove(i);
                ++ret;
            } else
                ++i;
        }
        return ret;
    }
    void reset() { 
        this->clear(); 
        if (defaultStorage_ != this->capacity()) {
            this->resize(defaultStorage_);
            this->shrink_to_fit();
            this->clear();
        }
    }
    void clip() { this->shrink_to_fit(); }
private:
    size_type defaultStorage_ = 1000;
};
// EoF
