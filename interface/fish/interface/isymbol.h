#pragma once

namespace itasca {
    class IThing;
}

namespace fish {
    class IParameter;

    class ISymbol {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b6d;
        /// Returns a const IThing pointer for this FISH symbol.
        virtual const itasca::IThing *  getIThing() const=0;
        /// Returns a IThing pointer for this FISH symbol.
        virtual itasca::IThing *        getIThing()=0;

        virtual QString                 getName() const=0;
        virtual const fish::IParameter *getValue() const=0;
        virtual bool                    getIsFunction() const=0;
        virtual bool                    getIsOperator() const=0;
        virtual quint32                 getNumArguments() const=0;
    };
} // namespace fish
// EoF
