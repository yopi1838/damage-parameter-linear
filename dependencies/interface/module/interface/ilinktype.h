#pragma once
/// \file
/// \brief Interface for LinkType<> objects, POD that contain both the pointer to the next object *and* an index for reference within the next object.
/// \ingroup kernel

#include "module/interface/ilinktype.h"

namespace itascaxd {
    /// \brief Interface for LinkType<> objects, POD that contain both the pointer to the next object *and* an index for reference within the next object.
    /// A LinkType<> object is used to simplify linked-lists that need to refer both to the destination object and to a reference index within that destination object.
    /// The classic example is a list of all Zones connected to a GridPoint, each connection on the list needs to know the index of the GridPoint in the Zone following.
    /// \ingroup kernel
    template <class T>
    struct LinkType {
        /// default constructor
        LinkType() : pnt_(0), ind_(0) { }
        /// contructor takes a pointer /a t /a to the object T and a index /a i /a.
        LinkType(T *t,quint32 i) : pnt_(t),ind_(i) { }
        /// copy constructor
        template <class C> LinkType(const LinkType<C> &l) : pnt_(l.pnt_), ind_(l.ind_) { }
        /// copy operator
        template <class C> const LinkType<T> &operator=(const LinkType<C> &l) { pnt_=l.pnt_; ind_=l.ind_; return *this; }
        /// Equal to operator
        bool operator==(const LinkType &l) const { return pnt_==l.pnt_ && ind_==l.ind_; }
        /// Not equal to operator
        bool operator!=(const LinkType &l) const { return !operator==(l); }
        /// returns the T object pointer 
        T *      operator->() { return pnt_; }
        /// returns the T const object pointer 
        const T *operator->() const { return pnt_; }
        /// returns the object index
        quint32  ind() const { return ind_; }
        /// returns the T object pointer, see operator->()
        T *      pnt() { return pnt_; }
        /// returns the T const object pointer, see operator->()
        const T *pnt() const { return pnt_; }

        /// object pointer
        T *     pnt_;  
        /// reference index
        quint32 ind_;  
    };

    /// same as LynkType, but with a constant pointer object
    template <class T>
    struct CLinkType {
        /// default constructor
        CLinkType() : pnt_(0), ind_(0) { }
        /// contructor takes a const pointer /a t /a to the object T and a index /a i /a.
        CLinkType(const T *t,quint32 i) : pnt_(t),ind_(i) { }
        /// copy constructor
        CLinkType(const LinkType<T> &l) : pnt_(l.pnt_),ind_(l.ind_) { }
        /// copy constructor
        template <class C> CLinkType(const CLinkType<C> &l) : pnt_(l.pnt_), ind_(l.ind_) { }
        /// copy operator
        template <class C> const CLinkType<T> &operator=(const CLinkType<C> &l) { pnt_=l.pnt_; ind_=l.ind_; return *this; }
        /// copy operator
        const CLinkType<T> &operator=(const LinkType<T> &l) { pnt_=l.pnt_; ind_=l.ind_; return *this; }
        /// equal operator
        bool operator==(const CLinkType &l) const { return pnt_==l.pnt_ && ind_==l.ind_; }
        /// not equal operator
        bool operator!=(const CLinkType &l) const { return !operator==(l); }

        /// returns the T const object pointer 
        const T *operator->() const { return pnt_; }
        /// return the object index 
        quint32  ind() const { return ind_; }
        /// returns the T const object pointer, see operator->()
        const T *pnt() const { return pnt_; }

        /// object pointer
        const T *pnt_;  
        /// reference index
        quint32  ind_;  
    };
} // namespace itascaxd
// EoF
