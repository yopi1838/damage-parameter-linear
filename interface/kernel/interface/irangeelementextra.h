#pragma once
// irangeelementextra.h

// Needs to specify extra index involved

namespace itascaxd {
    /// This part of a filter element interface describes which extra index is being referred to.
    class IRangeElementExtra {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bc2;
        /// Returns the extra index, >= 0.
        virtual int  getIndex() const=0;
        /// Sets the extra index, must be >=0 or will be ignored. 
        virtual void setIndex(int index)=0;
        /// Returns a pointer to the IRangeElement object
        virtual IRangeElement *getIRangeElement()=0;
        /// This SIGNAL is emitted when the extra index is changed.
        virtual void indexChanged(int index)=0;
    };

} // namespace itascaxd
// EoF
