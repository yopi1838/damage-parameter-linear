#pragma once

// OK some description of what is going on:
// IEEE double precision floating point format:
// Bits:  SEEEEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//        1 bit Sign
//        11 bits Exponent, 3ff is 0, so smaller is negative exponent and larger is positive
//                = 0 means "subnormal", for this purpose we'll just treat those as zero.
//                = 0x7ff (max) means error (Mant=0 infinity, otherwise NAN).
//        52 bits Matiessa.  The leading 1 is *implicit*, not actually stored, 
//                            since it should always be in the same place assuming 
//                            the exponent is correct.
// Storage in the extended precision classes
// Base Bits:  Same As above
// Extra Words: 64 bits - extended mantissa
#if defined __LINUX || __WINCMAKE
#include "stdafx.h"
#endif

namespace itasca {
    class UTILITY_EXPORT ExtendedPrecision  {
    public:
        ExtendedPrecision() { }
        explicit ExtendedPrecision(double d) { fromDouble(d); }
        ExtendedPrecision(const ExtendedPrecision &e) : basePos_(e.basePos_), baseNeg_(e.baseNeg_), extPos_(e.extPos_), extNeg_(e.extNeg_) {  }
        ExtendedPrecision &operator=(const ExtendedPrecision &c) { 
            basePos_ = c.basePos_;
            baseNeg_ = c.baseNeg_;
            extPos_ = c.extPos_;
            extNeg_ = c.extNeg_;
            return *this;
        }
        void reset() { basePos_ = baseNeg_ = 0; extPos_ = extNeg_ = 0; }
        inline const ExtendedPrecision &fromDouble(double d);
        inline double toDouble() const;
        inline void accumulate(double d);
        inline void accumulateEx(ExtendedPrecision e);
        void multiply(const double &d);

        double &direct() { return reinterpret_cast<double &>(basePos_); }
        const double &direct() const { return reinterpret_cast<const double &>(basePos_); }

        // Used for debugging/analysis
        void   getData(std::vector<quint64> *data) const;
        void   setData(const std::vector<quint64> &data);
    private:
        static const quint64 firstBit      = 0x8000000000000000; // Mask to find sign bit from double storage
        static const quint64 expMask       = 0x7FF0000000000000; // Mask to find exponent from double storage
        static const quint64 expAddMask    = 0x0010000000000000; // Value to add to increase exponent by one, or decrease exponent by one
        static const quint64 mantMask      = 0x000FFFFFFFFFFFFF; // Mask to find manitssa in double storage, without leading 1.
        static const quint64 expCheckMask  = 0x0020000000000000; // Check to see if addition overflow happened.
        static const quint64 lastBit       = 0x0000000000000001; 

        // Return base value mantissa with implicit one present
        inline void accumulate(quint64 baseAdd,quint64 extAdd,quint64 *base,quint64 *ext);
        inline void shiftUp(uint shift,quint64 *mant,quint64 *ext);
        inline void shiftUpOne(quint64 *exp,quint64 *mant,quint64 *ext);

        quint64 basePos_ = 0;  // Base value for positive storage, same as IEEE format 
        quint64 baseNeg_ = 0;  // Base value for negative storage, same as IEEE format 
        quint64 extPos_ = 0; // Extension value for position storage, 64 more bits
        quint64 extNeg_ = 0; // Extension value for negative storage, 64 more bits
    };

    const ExtendedPrecision &ExtendedPrecision::fromDouble(double d) {
        const quint64 &intVer = reinterpret_cast<const quint64 &>(d);
        if (intVer & firstBit) { // Sigh check
            baseNeg_ = intVer;
            basePos_ = 0;
        } else {
            basePos_ = intVer;
            baseNeg_ = 0;
        }
        extPos_ = extNeg_ = 0;
        return *this;
    }

    double ExtendedPrecision::toDouble() const {
        const double &dp = reinterpret_cast<const double &>(basePos_);
        const double &dn = reinterpret_cast<const double &>(baseNeg_);
        double ret = dp + dn;
        return ret;
    }

    void ExtendedPrecision::accumulate(double d) {
        if (!d) return;
        const quint64 &intVer = reinterpret_cast<const quint64 &>(d);
        if (intVer & firstBit) { // Sign check
            if (baseNeg_) 
                accumulate(intVer,0,&baseNeg_,&extNeg_);
            else
                baseNeg_ = intVer;
        } else {
            if (basePos_)
                accumulate(intVer,0,&basePos_,&extPos_);
            else
                basePos_ = intVer;
        }
    }

    void ExtendedPrecision::accumulateEx(ExtendedPrecision e) {
        if (e.basePos_) {
            if (basePos_)
                accumulate(e.basePos_,e.extPos_,&basePos_,&extPos_);
            else {
                basePos_ = e.basePos_;
                extPos_ = e.extPos_;
            }
        }
        if (e.baseNeg_) {
            if (baseNeg_)
                accumulate(e.baseNeg_,e.extNeg_,&baseNeg_,&extNeg_);
            else {
                baseNeg_ = e.baseNeg_;
                extNeg_ = e.extNeg_;
            }
        }
    }

    void ExtendedPrecision::accumulate(quint64 baseAdd,quint64 extAdd,quint64 *base,quint64 *ext) {
        quint64 expBase = (*base) & expMask; // Base and add exponent
        quint64 expAdd = baseAdd & expMask;
        quint64 mantBase = (*base & mantMask) + expAddMask; // Base and add mantissa
        quint64 mantAdd = (baseAdd & mantMask) + expAddMask; //  Add back in implicit 1
                                                             // if there is a difference in exp, shift one of them up so they both match
        if (expBase > expAdd) {
            quint32 shift = static_cast<quint32>((expBase - expAdd) >> 52);
            shiftUp(shift,&mantAdd,&extAdd);
            // Don't care about acc exp, since they will match 
        } else if (expBase < expAdd) {
            quint32 shift = static_cast<quint32>((expAdd - expBase) >> 52);
            shiftUp(shift,&mantBase,ext);
            expBase = expAdd;
        }
        quint64 tempExt = *ext;
        if (tempExt > 0xFFFFFFFFFFFFFFFF - extAdd) // Will overflow, so add to mant
            mantBase += 1;
        tempExt += extAdd;
        mantBase += mantAdd;
        // Note:  Possible MatBase +1 increment worst case is 0x001FFFFFFFFFFFFF to 0x0020000000000000
        //        Still can't overflow when added to max valid mantAdd 0x001FFFFFFFFFFFFF = 0x03FFFFFFFFFFFFF
        if (mantBase & expCheckMask) // Need to raise exp 1 and shift over 1
            shiftUpOne(&expBase,&mantBase,&tempExt);
        mantBase &= mantMask; // Remove implicit one again
                              // Reassemble base
        (*base) &= firstBit; // Clear all but the sign bit
        (*base) += expBase + mantBase; // Add back in exp and base mantissa
        (*ext) = tempExt; // Copy over extension bits
    }

    void ExtendedPrecision::shiftUpOne(quint64 *exp,quint64 *mant,quint64 *ext) {
        (*ext) >>= 1;
        if ((*mant) & lastBit) 
            (*ext) |= firstBit;
        (*mant) >>= 1;
        (*exp) += expAddMask;
        if ((*exp)>expMask) 
            throw Exception("An accumulated value has become greater than the maximum allowed. "
                            "This is normally caused by bad property values in a constitutive model "
                            "or by the model going unstable.");
    }

    // Shift all words one bit to the right, and decrease exp.
    void ExtendedPrecision::shiftUp(uint shift,quint64 *mant,quint64 *ext) {
        if (!shift) return;
        if (shift<53) { // Most common case.
            (*ext) >>= shift; 
            auto mov = (*mant) & (0x001FFFFFFFFFFFFF >> (53-shift));
            (*ext) += mov << (64 - shift);
            (*mant) >>= shift;
        } else if (shift<64) { // Mantissa goes away, and is shift to the left in the ext
            (*ext) >>= shift;
            auto mov = (*mant) & (0x001FFFFFFFFFFFFF << (shift-53));
            (*ext) = mov << (64-shift);
            (*mant) = 0;
        } else if (shift<53+64) {  // Mantissa goes away, and is shifted to the right in the ext which is completely lost
            auto mov = (*mant) & (0x001FFFFFFFFFFFFF << (shift - 64));
            (*ext) = mov >> (shift-64);
            (*mant) = 0;
        } else {   // Everything just goes to zero
            (*ext) = 0;
            (*mant) = 0;
        }
    }
} // namespace itasca
// EoF
