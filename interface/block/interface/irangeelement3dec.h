#pragma once
// irangeelement3dec.h

/**
  * \file
  * \brief This interface provides access to filters.
  */


#include "module/interface/irangeelement.h"

namespace block
{
  using namespace itascaxd;

  /// \brief This interface provides access to orientation filters.
  /// \ingroup THREEDEC
  class IRangeElementOrientation // name = "orientation"
  {
  public:
    /// type code
    inline static const TType type_ = 0x4c8f35be;
    /// orientation notice code
    static const UInt orientationNotice_ = 10;
    /// dip tolerance change code
    static const UInt tol1Notice_ = 11;
    /// dd tolerance
    static const UInt tol2Notice_ = 12;

    /// returns orientation object
    virtual Orientation3   getOrientation() const=0;
    /// returns normal tolerance
    virtual Double         getNormalTolerance() const=0;
    /// returns dip tolerance
    virtual Double         getDipTolerance() const=0;
    ///eturns dip direction tolerance
    virtual Double         getDipDirectionTolerance() const=0;
    /// returns pointer to filter element
    virtual IRangeElement *getIRangeElement()=0;
    /// sets orientation filter element
    virtual void           setOrientation(const Orientation3  &o)=0;
    /// sets orientation filter normal tolerance
    virtual void           setNormalTolerance(const Double &d)=0;
    /// sets orientation filter dip tolerance
    virtual void           setDipTolerance(const Double &d)=0;
    /// sets orientation filter dip direction tolerance
    virtual void           setDipDirectionTolerance(const Double &d)=0;

    /// signals
    /// signal orientation changed
    /// \param o orientation
    virtual void orientationChanged(const Orientation3 &o)=0;
    /// signal dip tolerance changed
    /// \param d tolerance
    virtual void tolerance1Changed(const double &d)=0;
    /// signal dd tolerance changed
    /// \param d tolerance
    virtual void tolerance2Changed(const double &d)=0;
  };

  //
  //IRangeElementTwoInt
  //
  /// \brief IRangeElement for RangeElementMaterialIntersection
  /// \ingroup THREEDEC
  class IRangeElementTwoInt // name = "intintersection"
  {
  public:
    /// type code
    inline static const TType type_ = 0x4c8fe5bf;
    /// returns first material number specifed
    virtual Int getFirst() const=0;
    /// returns second material number specified
    virtual Int getSecond() const=0;
    /// get filter element pointer
    virtual IRangeElement *getIRangeElement()=0;
    /// set first material number
    /// \param i material number
    virtual void setFirst(Int i)=0;
    /// set second material number
    /// \param i material number
    virtual void setSecond(Int i)=0;
    /// signal
    /// \param i material number
    virtual void firstChanged(int i)=0;
    /// signal
    /// \param i material number
    virtual void secondChanged(int i)=0;

  };


  class IRangeElementSurface
  {
  public:
    /// The type identification number for this class, for use in convert_cast() and convert_getcast().
    inline static const TType type_ = 0x52cac3ba;
  };

} // namespace block
// EoF