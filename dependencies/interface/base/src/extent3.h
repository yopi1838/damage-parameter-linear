#pragma once
/** \file Extent3.h
* \brief Three dimensional extent aligned with cartesian axes.
* \addtogroup Base Base interface specification 
* @{
*/

#include "extent2.h"

/** \brief A Class representing a cartesian extent in 3D.

* Essentially a 3D version of Extent2.
* Some effort has been made to keep the API the same for Extent3 and Extent2,
*   to enable better 2D/3D compilation.
*/
template <class T> class Extent3 {
public:
    // Creators
    /// Default constructor, no data initialization
#ifdef _DEBUG
    Extent3() { tx1_ = tx2_ = ty1_ = ty2_ = tz1_ = tz2_ = initVal<T>(); }
#else
    Extent3() { }
#endif
    /// Explicit constructor, by providing all six cartesian extents.
    Extent3(const T &x1,const T &x2,const T &y1,const T &y2,const T &z1,const T &z2) 
        :tx1_(x1), tx2_(x2), ty1_(y1), ty2_(y2), tz1_(z1), tz2_(z2) { }
    /// Explicit constructor, by providing the lower-bound and upper-bound as two Vector3.
    Extent3(const Vector3<T> &v111,const Vector3<T> &v222) 
        :tx1_(v111.x()), tx2_(v222.x()), ty1_(v111.y()), ty2_(v222.y()), tz1_(v111.z()), tz2_(v222.z()) { }
    /// Copy constructor
    Extent3(const Extent3<T> &r) : tx1_(r.tx1_), tx2_(r.tx2_), ty1_(r.ty1_), ty2_(r.ty2_), tz1_(r.tz1_), tz2_(r.tz2_) { }
    /// Explicit contructor, from a Extent2.  The z extent is set to (0,1).
    explicit Extent3(const Extent2<T> &r) : tx1_(r.x1()), tx2_(r.x2()), ty1_(r.y1()), ty2_(r.y2()), tz1_(0), tz2_(1) { }
    /// Returns an Extent3 with maximum negative size.  Useful for calculating bounds.
    static constexpr Extent3<T> nothing() { Extent3<T> c(limits<T>::max(),-limits<T>::max(),
                                                         limits<T>::max(),-limits<T>::max(),
                                                         limits<T>::max(),-limits<T>::max());  return c; }
    // Accessors
    /// Returns the lower bound of the extent in the x-direction.
    const T &x1() const { return tx1_; }
    /// Returns the upper bound of the extent in the x-direction.
    const T &x2() const { return tx2_; }
    /// Returns the lower bound of the extent in the y-direction.
    const T &y1() const { return ty1_; }
    /// Returns the upper bound of the extent in the y-direction.
    const T &y2() const { return ty2_; }
    /// Returns the lower bound of the extent in the y-direction.
    const T &z1() const { return tz1_; }
    /// Returns the upper bound of the extent in the y-direction.
    const T &z2() const { return tz2_; }
    /// Returns the lower bound of degree-of-freedom dof
    const T &dof1(UInt u) const { 
        assert(u<3); 
        switch (u) {
        case 1:  return ty1_;
        case 2:  return tz1_;
        }
        return tx1_;
    }
    /// Returns the upper bound of degree-of-freedom dof
    const T &dof2(UInt u) const { 
        assert(u<3); 
        switch (u) {
        case 1:  return ty2_;
        case 2:  return tz2_;
        }
        return tx2_;
    }
    /// Returns the size of the extent in the x-direction.
    T width() const { return (tx2_-tx1_); }
    /// Returns the size of the extent in the y-direction.
    T height() const { return (ty2_-ty1_); }
    /// Returns the size of the extent in the z-direction.
    T depth() const { return (tz2_-tz1_); }
    /// Returns the geometric center of the extent as a Vector3.
    Vector3<T> centroid() const { Vector3<T> v((tx1_+tx2_)/2,(ty1_+ty2_)/2,(tz1_+tz2_)/2);  return v; }
    /// Returns the volume of the extent ( width() * height() * depth() ).
    T volume() const { return (width()*height()*depth()); }
    /// Returns the diagonal length of the extent, or the distance from the lower bound corner to the upper bound.
    T diagonal() const { return (size().mag()); }
    /// This function retrieves the coordinates of the 8 different vertices.
    ///   x,y,z are either zero or non-zero.
    Vector3<T> corner(UInt x,UInt y,UInt z) const {
        if (x) {
            if (y) { if (z) return c222(); else   return c221(); }
            else   { if (z) return c212(); else   return c211(); }
        } else {
            if (y) { if (z) return c122(); else   return c121(); }
            else   { if (z) return c112(); else   return c111(); }
        }
    }

    // Corner functions - returns the 8 corners of the region.
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c111() const { Vector3<T> out(tx1_,ty1_,tz1_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c121() const { Vector3<T> out(tx1_,ty2_,tz1_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c211() const { Vector3<T> out(tx2_,ty1_,tz1_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c221() const { Vector3<T> out(tx2_,ty2_,tz1_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c112() const { Vector3<T> out(tx1_,ty1_,tz2_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c122() const { Vector3<T> out(tx1_,ty2_,tz2_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c212() const { Vector3<T> out(tx2_,ty1_,tz2_); return out; }
    /// Returns one of the eith corners that characterize the extent.
    Vector3<T> c222() const { Vector3<T> out(tx2_,ty2_,tz2_); return out; }
    /// Returns the lower bounding corner of the extent, minimum value of x,y, and z.
    Vector3<T> lowerBound() const { return c111(); }
    /// Returns the upper bounding corner of the extent, maximum value of x,y, and z.
    Vector3<T> upperBound() const { return c222(); }
    /// Returns the size of the extent as a Vector3(width(),height(),depth()).
    Vector3<T> size() const { Vector3<T> out(width(),height(),depth());  return out; }

    // Comparison Operators
    ///  Returns true if the region is "inside out" in any of its three coordinates.
    bool isEmpty() const { return ( (tx1_>=tx2_) || (ty1_>=ty2_) || (tz1_>=tz2_) ); }
    ///  Returns true if the region is "inside out" with a tolerance in any of its three coordinates.
    bool tolIsEmpty(const double &tol = limits<double>::epsilon() * 100) const { return ( (tx1_ + tol >= tx2_) || (ty1_ + tol >= ty2_) || (tz1_ + tol >= tz2_) ); }
    /// Comparison operator
    bool operator==(const Extent3<T> &c) const { 
        return( (tx1_==c.tx1_)&&(tx2_==c.tx2_)&&(ty1_==c.ty1_)&&(ty2_==c.ty2_)&&(tz1_==c.tz1_)&&(tz2_==c.tz2_) ); 
    }
    /// Comparison operator
    bool operator!=(const Extent3<T> &r) const { return !operator==(r); }
    /// Comparison operator, based on volume.
    bool operator<(const Extent3<T> &c) const { return (volume() < c.volume()); }
    /// Comparison operator, based on volume.
    bool operator>(const Extent3<T> &c) const { return (volume() > c.volume()); }
    /// Returns true if Vector3 \a v is "inside" the extent, inclusive.
    bool isIn(const Vector3<T> &v) const { 
        if ( (v.x()>=tx1_) && (v.x()<=tx2_) && (v.y()>=ty1_) && (v.y()<=ty2_) && (v.z()>=tz1_) && (v.z()<=tz2_) ) return true;  
        return false; 
    }
    /// Returns true if the Extent3 c falls completely inside (inclusive) the extent.
    bool isIn(const Extent3<T> &c) const { 
        if ( (c.x1()>=tx1_) && (c.x2()<=tx2_) && (c.y1()>=ty1_) && (c.y2()<=ty2_) && (c.z1()>=tz1_) && (c.z2()<=tz2_) ) return true;  
        return false; 
    }
    /// Returns true if Vector3 \a v is "inside" the extent, inclusive with an added tolerance factor.
    bool tolIsIn(const Vector3<T> &v,const T &tol) const { 
        if ( (v.x()>=tx1_-tol) && (v.x()<=tx2_+tol) && (v.y()>=ty1_-tol) && (v.y()<=ty2_+tol) && (v.z()>=tz1_-tol) && (v.z()<=tz2_+tol) ) return true;  
        return false; 
    }
    /// Returns true if the Extent3 c falls completely inside (inclusive) the extent with an added tolerance factor.
    bool tolIsIn(const Extent3<T> &c,const T &tol) const { 
        if ( (c.x1()>=tx1_-tol) && (c.x2()<=tx2_+tol) && (c.y1()>=ty1_-tol) && (c.y2()<=ty2_+tol) && (c.z1()>=tz1_-tol) && (c.z2()<=tz2_+tol) ) return true;  
        return false; 
    }
    /// Returns true if the Extent3 C intersects (inclusive) this extent in any way.
    bool intersects(const Extent3<T> &c) const {
        if ((c.tx2_<tx1_) || (c.tx1_>tx2_) || 
            (c.ty2_<ty1_) || (c.ty1_>ty2_) ||
            (c.tz2_<tz1_) || (c.tz1_>tz2_)) return false;
        return true;
    }
    /// Returns true if the Extent3 C intersects (inclusive) this extent in any way with an added tolerance factor.
    bool tolIntersects(const Extent3<T> &c,const T &tol) const {
        if ((c.tx2_<tx1_-tol) || (c.tx1_>tx2_+tol) || 
            (c.ty2_<ty1_-tol) || (c.ty1_>ty2_+tol) ||
            (c.tz2_<tz1_-tol) || (c.tz1_>tz2_+tol)) return false;
        return true;
    }

    // Setters
    T &rx1() { return tx1_; } ///<  Reference access to the minimum x extent.
    T &rx2() { return tx2_; } ///<  Reference access to the maximum x extent.
    T &ry1() { return ty1_; } ///<  Reference access to the minimum y extent.
    T &ry2() { return ty2_; } ///<  Reference access to the maximum y extent.
    T &rz1() { return tz1_; } ///<  Reference access to the minimum z extent.
    T &rz2() { return tz2_; } ///<  Reference access to the maximum z extent.
    /// Reference access to lower bound of degree-of-freedom dof.
    T &rdof1(UInt u) { 
        assert(u<3); 
        switch (u) {
        case 1:  return ty1_;
        case 2:  return tz1_;
        }
        return tx1_;
    }
    /// Reference access to lower bound of degree-of-freedom dof.
    T &rdof2(UInt u) { 
        assert(u<3); 
        switch (u) {
        case 1:  return ty2_;
        case 2:  return tz2_;
        }
        return tx2_;
    }
    // Setting width, height, depth assumes LL corner (c111) stays constant.
    /// Sets the width of the extent by moving the maximum x extent, keeping the minimum constant.
    void width(const T &t) { tx2_ = tx1_ + t; }
    /// Sets the height of the extent by moving the maximum y extent, keeping the minimum constant.
    void height(const T &t) { ty2_ = ty1_ + t; }
    /// Sets the depth of the extent by moving the maximum z extent, keeping the minimum constant.
    void depth(const T &t) { tz2_ = tz1_ + t; }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c111(const Vector3<T> &v) { tx1_ = v.x(); ty1_ = v.y(); tz1_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c121(const Vector3<T> &v) { tx1_ = v.x(); ty2_ = v.y(); tz1_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c211(const Vector3<T> &v) { tx2_ = v.x(); ty1_ = v.y(); tz1_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c221(const Vector3<T> &v) { tx2_ = v.x(); ty2_ = v.y(); tz1_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c112(const Vector3<T> &v) { tx1_ = v.x(); ty1_ = v.y(); tz2_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c122(const Vector3<T> &v) { tx1_ = v.x(); ty2_ = v.y(); tz2_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c212(const Vector3<T> &v) { tx2_ = v.x(); ty1_ = v.y(); tz2_ = v.z(); }
    /// Sets one of the eight characteristic corners of the extent, keeping the other three extents constant.
    void c222(const Vector3<T> &v) { tx2_ = v.x(); ty2_ = v.y(); tz2_ = v.z(); }
    /// Sets the lower bound of the exetnt, keeping the upper bound constant.  Synonym to c111().
    void lowerBound(const Vector3<T> &v) { c111(v); }
    /// Sets the upper bound of the exetnt, keeping the lower bound constant.  Synonym to c222().
    void upperBound(const Vector3<T> &v) { c222(v); }
    /// Specifies the size of the extent by moving the upper bound, keeping the lower bound constant.
    void size(const Vector3<T> &v) { width(v.x());  height(v.y());  depth(v.z()); }
    /// Constrains the point \a v to fall inside the extent.  Any coordinate outside the extent is moved
    ///  to the minimum or maximum boundary.
    Vector3<T> bound(const Vector3<T> &v) const { return Vector3<T>(pBound(tx1_,v.x(),tx2_),pBound(ty1_,v.y(),ty2_),pBound(tz1_,v.z(),tz2_)); }

    // Manipulators - unary in place
    /// Adds \a v to all extent boundaries, effectively translating it.
    const Extent3<T> &operator+=(const Vector3<T> &v) { tx1_+=v.x(); tx2_+=v.x(); ty1_+=v.y(); ty2_+=v.y(); tz1_+=v.z(); tz2_+=v.z(); return *this;}
    /// Subtracts \a v from all extent boundaries, effectively translating it.
    const Extent3<T> &operator-=(const Vector3<T> &v) { tx1_-=v.x(); tx2_-=v.x(); ty1_-=v.y(); ty2_-=v.y(); tz1_-=v.z(); tz2_-=v.z(); return *this;}

    // binary operators 
    /// Returns the extent created ty translating this one \a v units.
    Extent3<T> operator+(const Vector3<T> &v) const { Extent3<T> out(tx1_+v.x(),tx2_+v.x(),ty1_+v.y(),ty2_+v.y(),tz1_+v.z(),tz2_+v.z());  return out; }
    /// Returns the extent created ty translating this one \a -v units.
    Extent3<T> operator-(const Vector3<T> &v) const { Extent3<T> out(tx1_-v.x(),tx2_-v.x(),ty1_-v.y(),ty2_-v.y(),tz1_-v.z(),tz2_-v.z());  return out; }
    /// Returns the extent formed by the intersections of the two extents.  May end up empty.
    Extent3<T> intersectedWith(const Extent3<T> &r) const {
        Extent3<T> out(std::max<T>(tx1_,r.tx1_),std::min<T>(tx2_,r.tx2_),
            std::max<T>(ty1_,r.ty1_),std::min<T>(ty2_,r.ty2_),
            std::max<T>(tz1_,r.tz1_),std::min<T>(tz2_,r.tz2_));  
        return out; 
    }
    /// Enlarges the extent to include both its original extent and \a r.
    const Extent3<T> &expandToInclude(const Extent3<T> &r) {
        tx1_ = std::min<T>(tx1_,r.tx1_);
        ty1_ = std::min<T>(ty1_,r.ty1_);
        tz1_ = std::min<T>(tz1_,r.tz1_);
        tx2_ = std::max<T>(tx2_,r.tx2_);
        ty2_ = std::max<T>(ty2_,r.ty2_);
        tz2_ = std::max<T>(tz2_,r.tz2_);
        return *this;
    }
    /// Enlarges the extent to contain \a v.
    const Extent3<T> &expandToInclude(const Vector3<T> &v) {
        tx1_ = std::min<T>(tx1_,v.x());
        ty1_ = std::min<T>(ty1_,v.y());
        tz1_ = std::min<T>(tz1_,v.z());
        tx2_ = std::max<T>(tx2_,v.x());
        ty2_ = std::max<T>(ty2_,v.y());
        tz2_ = std::max<T>(tz2_,v.z());
        return *this;
    }
    /// \overload
    const Extent3<T> &expandToInclude(const T &x,const T &y,const T &z) {
        return expandToInclude(Vector3<T>(x,y,z));
    }
    /// Returns an extent large enough to contain this one and \a r.
    Extent3<T> expandedToInclude(const Extent3<T> &r) const { 
        Extent3<T> out(*this);
        out.expandToInclude(r);
        return out; 
    }
    /// Returns an extent large enough to contain this one and the point \a v.
    Extent3<T> expandedToInclude(const Vector3<T> &v) const { 
        Extent3<T> out(*this);
        out.expandToInclude(v);
        return out; 
    }
    /// \overload
    Extent3<T> expandedToInclude(const T &x,const T &y,const T &z) const { 
        return expandedToInclude(Vector3<T>(x,y,z));
    }
    /// The extent is made bigger by 2*tol in all directions.
    /// In other words, each extent component is moved tol units away from the center.
    /// The centroid will remain constant.
    const Extent3<T> &expand(const T &tol) { tx1_ -= tol;  tx2_ += tol;  ty1_ -= tol;  ty2_ += tol;  tz1_ -= tol;  tz2_ += tol;  return *this; }
    /// Returns an extent that is bigger by 2*tol in all directions.
    /// In other words, each extent component is moved tol units away from the center.
    /// The centroid will remain constant.
    Extent3<T> expanded(const T &tol) const { Extent3<T> out(*this);  return out.expand(tol); }
    /// Returns an extent expanded by a factor of fact in all directions.
    /// The centroid will remain constant.
    Extent3<T> biggerBy(const T &fact) const { 
        auto s = size()*0.5;
        s += s*fact;
        auto c = centroid();
        Extent3<T> out(c-s,c+s);  
        return out; 
    }
    /// Centers this Extent2 about point \a v.
    const Extent3<T> &center(const Vector3<T> &v) {
        Vector3<T> trans = v - this->centroid();
        tx1_ += trans.x();
        tx2_ += trans.x();
        ty1_ += trans.y();
        ty2_ += trans.y();
        tz1_ += trans.z();
        tz2_ += trans.z();
        return *this;
    }
    /// Returns a Extent2 centered about point \a v.
    Extent3<T> center(const Vector3<T> &v) const {
        Extent3<T> out(*this);
        out.center(v);  
        return out; 
    }

private:
    T pBound(const T &min,const T &v,const T &max) const { return std::min(std::max(v,min),max); }
    T tx1_;
    T tx2_;
    T ty1_;
    T ty2_;
    T tz1_;
    T tz2_;
};


// Predefined types for standard template arguments.
typedef Extent3<Double> DExtent3; ///< \related Extent3
typedef Extent3<Float>  FExtent3; ///< \related Extent3
typedef Extent3<Int>    IExtent3; ///< \related Extent3
typedef Extent3<UInt>   UExtent3; ///< \related Extent3

/// Returns a Extent3 
template <class T> inline const Extent3<T> &toExtent3(const Extent3<T> &t) { return t; }
/// Returns a Extent3 coverted from a Extent2, the length of the third dimension is zero
template <class T> inline Extent3<T> toExtent3(const Extent2<T> &t) { return Extent3<T>(t.x1(),t.x2(),t.y1(),t.y2(),0.0,0.0); }

/// @}
// EoF
