#pragma once

#include "extendedprecision.h"
#include "lowest.h"
#include "shared/src/archive2.h"

namespace itasca {
    class Archive2;

    // Notes:
    // Accumulators are used to provide order-independent results of adding a number of values 
    //      together, to 64 bits of precision.
    // This implementation uses ExtendedPrecision arithmetic to accumulate values to 178 bits of precision.

    // This class provides a convenient accumulator wr
    class UTILITY_EXPORT AccOnly : public ExtendedPrecision {
    public:
        AccOnly() { }
        AccOnly(const AccOnly &a) : ExtendedPrecision(a) { }
        AccOnly &operator=(const AccOnly &a) { ExtendedPrecision::operator=(a);  return *this; }
        explicit AccOnly(double d) : ExtendedPrecision(d) {  }
        const AccOnly &fromDouble(double d) { ExtendedPrecision::fromDouble(d);  return *this; }
        // Accumulation of a multiplied value
        void accumulateExMultiply(const AccOnly &a,double m) { ExtendedPrecision e2(a); e2.multiply(m); accumulateEx(e2); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    };

    class UTILITY_EXPORT AccVect2 {
    public:
        AccVect2() { }
        AccVect2(const AccVect2 &a) : x_(a.x_), y_(a.y_) { }
        AccVect2 & operator=(const AccVect2 &c) { 
            x_ = c.x_;
            y_ = c.y_;
            return *this;
        }
        explicit AccVect2(const DVect2 &v) { fromDVect(v); }
        explicit AccVect2(double d) : x_(d), y_(d) { }
        void reset() { x_.reset(); y_.reset(); } 
        const AccVect2 &fromDVect(const DVect2 &v) { x_.fromDouble(v.x());  y_.fromDouble(v.y()); return *this; }
        void accumulate(const DVect2 &v) { x_.accumulate(v.x()); y_.accumulate(v.y()); }
        void accumulateEx(const AccVect2 &v) { x_.accumulateEx(v.x_); y_.accumulateEx(v.y_); }
        void multiply(double d) { x_.multiply(d); y_.multiply(d); }
        void accumulateExMultiply(AccVect2 d,double m) { x_.accumulateExMultiply(d.x(),m); y_.accumulateExMultiply(d.y(),m); }
        DVect2 toDVect() const { return DVect2(x_.toDouble(),y_.toDouble()); }
        // In case you need to query details, should never really be used except for debugging/validation.
        const AccOnly &x() const { return x_; }
        const AccOnly &y() const { return y_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    private:
        AccOnly x_;
        AccOnly y_;
    };
    
    class UTILITY_EXPORT AccVect3 {
    public:
        AccVect3() { }
        AccVect3(const AccVect3 &a) : x_(a.x_), y_(a.y_), z_(a.z_) { }
        explicit AccVect3(double d) : x_(d), y_(d), z_(d) { }
        AccVect3 & operator=(const AccVect3 &c) { 
            x_ = c.x_;
            y_ = c.y_;
            z_ = c.z_;
            return *this;
        }
        explicit AccVect3(const DVect3 &v) { fromDVect(v); }
        void reset() { x_.reset(); y_.reset(); z_.reset(); } 
        const AccVect3 &fromDVect(const DVect3 &v) { x_.fromDouble(v.x());  y_.fromDouble(v.y());  z_.fromDouble(v.z());  return *this; }
        void accumulate(const DVect3 &v) { x_.accumulate(v.x()); y_.accumulate(v.y()); z_.accumulate(v.z()); }
        void accumulateEx(const AccVect3 &v) { x_.accumulateEx(v.x_); y_.accumulateEx(v.y_); z_.accumulateEx(v.z_); }
        void multiply(double d) { x_.multiply(d); y_.multiply(d); z_.multiply(d); }
        void accumulateExMultiply(AccVect3 d,double m) { x_.accumulateExMultiply(d.x(),m); y_.accumulateExMultiply(d.y(),m); z_.accumulateExMultiply(d.z(),m);}
        DVect3 toDVect() const { return DVect3(x_.toDouble(),y_.toDouble(),z_.toDouble()); }
        // In case you need to query details, should never really be used except for debugging/validation.
        const AccOnly &x() const { return x_; }
        const AccOnly &y() const { return y_; }
        const AccOnly &z() const { return z_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    private:
        AccOnly x_;
        AccOnly y_;
        AccOnly z_;
    };


    class UTILITY_EXPORT Accumulator : public AccOnly {
    public:
        Accumulator() { }
        Accumulator(const Accumulator &a) : AccOnly(a) { }
        explicit Accumulator(double d) : AccOnly(d) {  }
        const Accumulator &fromDouble(double d) { AccOnly::fromDouble(d);  return *this; }

        // Non-deterministic conversion functions, to use for efficiency.
        void   nondetAccumulate(double d) { direct() += d; }
        double nondetFromDouble(double d) { direct() = d; return direct(); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    };

    class UTILITY_EXPORT Accumulator2 {
    public:
        Accumulator2() { }
        Accumulator2(const Accumulator2 &a) : x_(a.x_), y_(a.y_) { }
        explicit Accumulator2(const DVect2 &v) { fromDVect(v); }
        explicit Accumulator2(double d) : x_(d), y_(d) { }
        inline void reset() { x_.reset(); y_.reset(); } 
        const Accumulator2 &fromDVect(const DVect2 &v) { x_.fromDouble(v.x());  y_.fromDouble(v.y()); return *this; }
        void accumulate(const DVect2 &v) { x_.accumulate(v.x()); y_.accumulate(v.y()); }
        DVect2 toDVect() const { return DVect2(x_.toDouble(),y_.toDouble()); }
        // Non-deterministic conversion functions, to use for efficiency in nondeterministic mode.
        void   nondetAccumulate(const DVect2 &d) { x_.nondetAccumulate(d.x()); y_.nondetAccumulate(d.y()); }
        DVect2 nondetFromDVect(const DVect2 &d) { x_.nondetFromDouble(d.x()); y_.nondetFromDouble(d.y());  return toDVect(); }
        // In case you need to query details, should never really be used except for debugging/validation.
        const Accumulator &x() const { return x_; }
        const Accumulator &y() const { return y_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    private:
        Accumulator x_;
        Accumulator y_;
    };

    class UTILITY_EXPORT Accumulator3 {
    public:
        Accumulator3() { }
        Accumulator3(const Accumulator3 &a) : x_(a.x_), y_(a.y_), z_(a.z_) { }
        explicit Accumulator3(const double &d) : x_(d), y_(d), z_(d) { }
        explicit Accumulator3(const DVect3 &v) { fromDVect(v); }
        void reset() { x_.reset(); y_.reset(); z_.reset(); } 
        const Accumulator3 &fromDVect(const DVect3 &v) { x_.fromDouble(v.x());  y_.fromDouble(v.y());  z_.fromDouble(v.z());  return *this; }
        void accumulate(const DVect3 &v) { x_.accumulate(v.x()); y_.accumulate(v.y()); z_.accumulate(v.z()); }
        DVect3 toDVect() const { return DVect3(x_.toDouble(),y_.toDouble(),z_.toDouble()); }
        // Non-deterministic conversion functions, to use for efficiency in nondeterministic mode.
        void nondetAccumulate(const DVect3 &d) { x_.nondetAccumulate(d.x()); y_.nondetAccumulate(d.y()); z_.nondetAccumulate(d.z()); }
        DVect3 nondetFromDVect(const DVect3 &d) { x_.nondetFromDouble(d.x()); y_.nondetFromDouble(d.y()); z_.nondetFromDouble(d.z()); return toDVect(); }
        // In case you need to query details, should never really be used except for debugging/validation.
        const Accumulator &x() const { return x_; }
        const Accumulator &y() const { return y_; }
        const Accumulator &z() const { return z_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    private:
        Accumulator x_;
        Accumulator y_;
        Accumulator z_;
    };

} // namespace itasca
namespace archive {
    template <> class Traits<itasca::AccOnly> : public TraitsObject<itasca::AccOnly> {};
    template <> class Traits<itasca::AccVect2> : public TraitsObject<itasca::AccVect2> {};
    template <> class Traits<itasca::AccVect3> : public TraitsObject<itasca::AccVect3> {};
    template <> class Traits<itasca::Accumulator>  : public TraitsObject<itasca::Accumulator> {};
    template <> class Traits<itasca::Accumulator2> : public TraitsObject<itasca::Accumulator2> {};
    template <> class Traits<itasca::Accumulator3> : public TraitsObject<itasca::Accumulator3> {};
}

// EoF
