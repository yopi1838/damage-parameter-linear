#pragma once
/** \file rect.h
* \brief 2D cartesion region in space.
* \addtogroup Base Base interface specification 
* @{
*/

#include "vect.h"

/** \brief 2D cartesian region in space.
*/
template <class T> class Extent2 {
public:
    // Creators
    /// Default constructor, no data initialization.
#ifdef _DEBUG
    Extent2() { tx1_ = tx2_ = ty1_ = ty2_ = initVal<T>(); }
#else
    Extent2() {}
#endif
    /// Explicit constructor, given the x and y extents.
    Extent2(const T &x1,const T &x2,const T &y1,const T &y2) : tx1_(x1), tx2_(x2), ty1_(y1), ty2_(y2) { }
    /// Explicit constructor, given the lower and upper bounds as two Vector2.
    Extent2(const Vector2<T> &v11,const Vector2<T> &v22) : tx1_(v11.x()), tx2_(v22.x()), ty1_(v11.y()), ty2_(v22.y()) { }
    /// Copy constructor
    Extent2(const Extent2<T> &r) : tx1_(r.tx1_), tx2_(r.tx2_), ty1_(r.ty1_), ty2_(r.ty2_) { }
    /// Static function returning object with maximum negative size, useful for calculating bounds.
    static constexpr Extent2<T> nothing() { Extent2<T> r(limits<T>::max(),-limits<T>::max(),
                                                         limits<T>::max(),-limits<T>::max());  return r; }
    // Accessors
    /// Returns the lower x-bound.
    const T &x1() const { return tx1_; }
    /// Returns the upper x-bound.
    const T &x2() const { return tx2_; }
    /// Returns the lower y-bound.
    const T &y1() const { return ty1_; }
    /// Returns the upper y-bound.
    const T &y2() const { return ty2_; }
    /// Returns the lower bound of degree-of-freedom dof
    const T &dof1(UInt u) const { 
        assert(u<2);
        if (u)
            return ty1_;
        return tx1_;
    }
    /// Returns the upper bound of degree-of-freedom dof
    const T &dof2(UInt u) const { 
        assert(u<2); 
        if (u) 
            return ty2_;
        return tx2_;
    }
    /// Returns the size of the x-extent (x2-x1)
    T width() const { return (tx2_-tx1_); }
    /// Return sthe size of the y-extent (y2-y1)
    T height() const { return (ty2_-ty1_); }
    /// Returns the centroid of the Extent2 as a Vector2.
    Vector2<T> centroid() const { Vector2<T> out((tx1_+tx2_)/2,(ty1_+ty2_)/2);  return out; }
    /// Returns one of the four characteristic corners of the 2D extent.
    Vector2<T> c11() const { Vector2<T> out(tx1_,ty1_); return out; }
    /// Returns one of the four characteristic corners of the 2D extent.
    Vector2<T> c12() const { Vector2<T> out(tx1_,ty2_); return out; }
    /// Returns one of the four characteristic corners of the 2D extent.
    Vector2<T> c21() const { Vector2<T> out(tx2_,ty1_); return out; }
    /// Returns one of the four characteristic corners of the 2D extent.
    Vector2<T> c22() const { Vector2<T> out(tx2_,ty2_); return out; }
    /// Returns the lower bound of the Extent2 (minimum x and y corner).
    Vector2<T> lowerBound() const { return c11(); }
    /// Returns the upper bound of the Extent2 (maximum x and y corner).
    Vector2<T> upperBound() const { return c22(); }
    /// Returns the size of the Extent2, the width() and height() encoded as a Vector2.
    Vector2<T> size() const { Vector2<T> out(width(),height());  return out; }
    /// Returns the width()*height() - could be negative.
    T area() const { return (width()*height()); }
    /// Returns the volume of the extent assuming unit depth ( width() * height()).
    T volume() const { return area(); }
    /// Returns the length of the diagonal from the lower bound to the upper bound.
    T diagonal() const { return size().mag(); }
    /// Returns true if the area of the Extent2 is <= 0.
    bool isEmpty() const { return( (tx1_>=tx2_) || (ty1_>=ty2_) ); }
    /// Returns true if the area of the Extent2 is <= 0 with a tolerance.
    bool tolIsEmpty(const double &tol = limits<double>::epsilon() * 100) const { return ( (tx1_ + tol >= tx2_) || (ty1_ + tol >= ty2_) ); }

    // Comparison operators
    /// Comparison operator, no tolerance applied.
    bool operator==(const Extent2<T> &r) const { return ( (tx1_==r.tx1_)&&(tx2_==r.tx2_)&&(ty1_==r.ty1_)&&(ty2_==r.ty2_) ); }
    /// Comparison operator, no tolerance applied.
    bool operator!=(const Extent2<T> &r) const { return !operator==(r); }
    /// Comparison operator, using area() as a metric.
    bool operator<(const Extent2<T> &r) const { return (area() < r.area()); }
    /// Comparison operator, using area() as a metric.
    bool operator>(const Extent2<T> &r) const { return (area() > r.area()); }
    /// Returns true if point \a v is inside (inclusive) the Extent2.
    bool isIn(const Vector2<T> &v) const { return ((v.x()>=tx1_)&&(v.x()<=tx2_)&&(v.y()>=ty1_)&&(v.y()<=ty2_)); }
    /// Returns true Extent2 \a v is inside (inclusive) the Extent2.
    bool isIn(const Extent2<T> &r) const { 
        if ( (r.tx1_>=tx1_) && (r.tx2_<=tx2_) && (r.ty1_>=ty1_) && (r.ty2_<=ty2_) ) return true;
        return false;
    }
    /// Returns true if point \a v is inside (inclusive) the Extent2 with an added tolerance factor.
    bool tolIsIn(const Vector2<T> &v,const T &tol) const { 
        return ((v.x()>=tx1_-tol)&&(v.x()<=tx2_+tol)&&(v.y()>=ty1_-tol)&&(v.y()<=ty2_+tol)); 
    }
    /// Returns true Extent2 \a v is inside (inclusive) the Extent2 with an added tolerance factor.
    bool tolIsIn(const Extent2<T> &r,const T &tol) const { 
        if ( (r.tx1_>=tx1_-tol) && (r.tx2_<=tx2_+tol) && (r.ty1_>=ty1_-tol) && (r.ty2_<=ty2_+tol) ) return true;
        return false;
    }
    /// Returns true if Extent2 \a r intersects (inclusive) the Extent2.
    bool intersects(const Extent2<T> &r) const {
        if ((r.tx2_<tx1_) || (r.tx1_>tx2_) || 
            (r.ty2_<ty1_) || (r.ty1_>ty2_)) return false;
        return true;
    }
    /// Returns true if Extent2 \a r intersects (inclusive) the Extent2 with an added tolerance factor.
    bool tolIntersects(const Extent2<T> &r,const T &tol) const {
        if ((r.tx2_<tx1_-tol) || (r.tx1_>tx2_+tol) || 
            (r.ty2_<ty1_-tol) || (r.ty1_>ty2_+tol)) return false;
        return true;
    }

    // Setters
    /// Access reference to the minimum x extent.
    T &rx1() { return tx1_; }
    /// Access reference to the maximum x extent.
    T &rx2() { return tx2_; }
    /// Access reference to the minimum y extent.
    T &ry1() { return ty1_; }
    /// Access reference to the maximum y extent.
    T &ry2() { return ty2_; }
    /// Reference access to lower bound of degree-of-freedom dof.
    T &rdof1(UInt u) { 
        assert(u<2);  
        if (u)
            return ty1_;
        return tx1_;
    }
    /// Reference access to lower bound of degree-of-freedom dof.
    T &rdof2(UInt u) { 
        assert(u<2);
        if (u)
            return ty2_;
        return tx2_;
    }
    //   Changing width or height assumes LL corner (c11) stays constant.
    /// Sets the width of the Extent2 by moving the maximum x extent, leaving the minimum alone.
    void width(const T &t) { tx2_ = tx1_ + t; }
    /// Sets the height of the Extent2 by moving the maximum y extent, leaving the minimum alone.
    void height(const T &t) { ty2_ = ty1_ + t; }
    /// Sets one of the four characteristic corners of the extent, leaving the other two values alone.
    void c11(const Vector2<T> &v) { rx1() = v.x();  ry1() = v.y();  }
    /// Sets one of the four characteristic corners of the extent, leaving the other two values alone.
    void c12(const Vector2<T> &v) { rx1() = v.x();  ry2() = v.y();  }
    /// Sets one of the four characteristic corners of the extent, leaving the other two values alone.
    void c21(const Vector2<T> &v) { rx2() = v.x();  ry1() = v.y();  }
    /// Sets one of the four characteristic corners of the extent, leaving the other two values alone.
    void c22(const Vector2<T> &v) { rx2() = v.x();  ry2() = v.y();  }
    /// Sets the lower bound of the Extent2, leaving the upper bound alone.
    void lowerBound(const Vector2<T> &v) { c11(v); }
    /// Sets the upper bound of the Extent2, leaving the lower bound alone.
    void upperBound(const Vector2<T> &v) { c22(v); }
    /// Sets the size of the Extent2 by moving the maximum x and y extents, leaving the minimum alone.
    void size(const Vector2<T> &v) { width(v.x());  height(v.y()); }
    /// Forces the point \a v to fall within the Extent2, by clamping the x and y values to fall within it's extent.
    Vector2<T> bound(const Vector2<T> &v) const { return Vector2<T>(pbound(tx1_,v.x(),tx2_),pbound(ty1_,v.y(),ty2_)); }

    // Manipulators - unary in place
    /// In place addition operator, offsets the exent by \a v.
    const Extent2<T> &operator+=(const Vector2<T> &v) { tx1_+=v.x(); tx2_+=v.x(); ty1_+=v.y(); ty2_+=v.y(); return *this; }
    /// In place subtraction operator, offsets the exent by \a -v.
    const Extent2<T> &operator-=(const Vector2<T> &v) { tx1_-=v.x(); tx2_-=v.x(); ty1_-=v.y(); ty2_-=v.y(); return *this; }

    // Binary operators
    /// Binary addition operator, returns a Extent2 offset by \a v.
    Extent2<T> operator+(const Vector2<T> &v) const { Extent2<T> out(tx1_+v.x(),tx2_+v.x(),ty1_+v.y(),ty2_+v.y());  return out; }
    /// Binary subtraction operator, returns a Extent2 offset by \a -v.
    Extent2<T> operator-(const Vector2<T> &v) const { Extent2<T> out(tx1_-v.x(),tx2_-v.x(),ty1_-v.y(),ty2_-v.y());  return out; }
    /// Returns a Extent2 representing the intersection (inclusive) of this with \a r.  
    /// The returned triangle may be "empty" (have zero or negative area).
    Extent2<T> intersectedWith(const Extent2<T> &r) const { 
        Extent2<T> out(std::max<T>(tx1_,r.tx1_),std::min<T>(tx2_,r.tx2_),std::max<T>(ty1_,r.ty1_),std::min<T>(ty2_,r.ty2_));  
        return out; 
    }
    /// Expands the extent of this Extent2 as necessary to completely include \a r.
    const Extent2<T> &expandToInclude(const Extent2<T> &r) {
        tx1_ = std::min<T>(tx1_,r.tx1_);
        tx2_ = std::max<T>(tx2_,r.tx2_);
        ty1_ = std::min<T>(ty1_,r.ty1_);
        ty2_ = std::max<T>(ty2_,r.ty2_);
        return *this;
    }
    /// Expands the extent of this Extent2 as necessary to completely include point \a v.
    const Extent2<T> &expandToInclude(const Vector2<T> &v) {
        tx1_ = std::min<T>(tx1_,v.x());
        tx2_ = std::max<T>(tx2_,v.x());
        ty1_ = std::min<T>(ty1_,v.y());
        ty2_ = std::max<T>(ty2_,v.y());
        return *this;
    }
    /// Returns a Extent2 expanded to completely include \a r.
    Extent2<T> expandedToInclude(const Extent2<T> &r) const { 
        Extent2<T> out(*this);
        out.expandToInclude(r);
        return out; 
    }
    /// Returns a Extent2 expanded to include point \a v..
    Extent2<T> expandedToInclude(const Vector2<T> &v) const {
        Extent2<T> out(*this);
        out.expandToInclude(v);  
        return out; 
    }
    /// Expands this Extent2 by 2*tol in all directions.
    /// The centroid will remain the same.
    const Extent2<T> &expand(const T &tol) { tx1_ -= tol;  tx2_ += tol;  ty1_ -= tol;  ty2_ += tol;  return *this; }
    /// Returns Extent2 bigger by 2*tol in all directions.
    /// The centroid will remain the same.
    Extent2<T> expanded(const T &tol) const { Extent2<T> out(*this);  return out.expand(tol); }
    /// Centers this Extent2 about point \a v.
    const Extent2<T> &center(const Vector2<T> &v) {
        Vector2<T> trans = v - this->centroid();
        tx1_ += trans.x();
        tx2_ += trans.x();
        ty1_ += trans.y();
        ty2_ += trans.y();
        return *this;
    }
    /// Returns a Extent2 centered about point \a v.
    Extent2<T> center(const Vector2<T> &v) const {
        Extent2<T> out(*this);
        out.center(v);  
        return out; 
    }
    /// Returns an extent expanded by a factor of fact in all directions.
    /// The centroid will remain constant.
    Extent2<T> biggerBy(const T &fact) const { 
        auto s = size()*0.5;
        s += s*fact;
        auto c = centroid();
        Extent2<T> out(c-s,c+s);  
        return out; 
    }

private:
    T pbound(const T &min,const T &v,const T &max) const { return std::min(std::max(v,min),max); }
    T tx1_;
    T tx2_;
    T ty1_;
    T ty2_;
};

typedef Extent2<Double> DExtent2; ///< \relates Extent2
typedef Extent2<Float>  FExtent2; ///< \relates Extent2
typedef Extent2<Int>    IExtent2; ///< \relates Extent2
typedef Extent2<UInt>   UExtent2; ///< \relates Extent2

/// @}
// EoF
