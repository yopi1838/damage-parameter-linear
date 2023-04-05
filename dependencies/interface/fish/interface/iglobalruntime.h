#pragma once

namespace fish {
    class ISymbol;

    class IGlobalRuntime {
    public:
        virtual const ISymbol *findISymbol(const QString &name) const=0;
    };
} // namespace fish
// EoF