#pragma once
/**
  * \file
  * \brief Interface to hybrid structural elements
  */

namespace itasca {
    class IThing;
}

namespace sel {
    using namespace itasca;

    class ISEL;
    class ISELCable;
    class ISELDowel;

    /// \brief Interface to hybrid structural elements
    /** This interface inherits from ISELCable.
    * \ingroup SEL
    */
    class ISELHybrid {
    public:
        /// Type ID number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x5f987f0e;
        /// Returns a const IThing pointer.
        virtual const IThing* getIThing() const = 0;
        /// Returns a IThing pointer.
        virtual IThing* getIThing() = 0;
        /// Returns a const pointer to object base class.
        virtual const ISEL* getISEL() const = 0;
        /// Returns a pointer to object base class.
        virtual ISEL* getISEL() = 0;
        /// Returns a const pointer to the class that it inherits from.
        virtual const ISELCable* getISELCable() const = 0;
        /// Returns a pointer to the class that it inherits from.
        virtual ISELCable* getISELCable() = 0;

        virtual quint32 getNumDowels() const = 0;
        virtual const ISELDowel* getDowel(quint32 dowelnum) const = 0;
        virtual ISELDowel* getDowel(quint32 dowelnum) = 0;
        virtual bool       getDowelRupture() const = 0;

    };

} // namespace