#pragma once

// This is a prototype implementation of support for references.
// It is currently used only in support of built-in functions that require support for implicit references.
// This would have to be redesigned to provide general reference support.

#include "utility/src/tptr.h"

namespace itasca {
    class Archive2;
}

namespace fish {
    class Library;
    class Symbol;
    class Parameter;

    class Reference {
    public:
        enum class Type { Library=1, Symbol=2, Local=3 };
        Reference();
        Reference(const Reference &r);
        Reference(Reference &&r) noexcept;
        explicit Reference(Library *l);
        explicit Reference(Parameter *p,const QString &name,const QString &func);
        explicit Reference(Symbol *s);
        ~Reference();
        const Reference &operator=(const Reference &r);
        const Reference &operator=(Reference &&r) noexcept;
        bool operator==(const Reference &r) const;
        bool operator<(const Reference &r) const;

        bool isSymbol() const;
        bool isLocal() const;
        bool isLibrary() const;
        
        void setLibrary(Library *lib);
        void setSymbol(Symbol *lib);
        void setLocal(Parameter *loc,const QString &name,const QString &function);

        Library *        toLibrary();
        const Library *  toLibrary() const;
        Symbol *         toSymbol();
        const Symbol *   toSymbol() const;
        Parameter *      toLocal();
        const Parameter *toLocal() const;
        const QString &  localName() const;
        const QString &  localFunctionName() const;
        Parameter *      toParameter();
        const Parameter *toParameter() const;

        QString toString() const;
        void save(itasca::Archive2 &a) const;
        bool restore(itasca::Archive2 &a,quint64 label);
        void remap(itasca::Archive2 &a);

    private:
        class Private;
        Private *p_;
    };

} // namespace fish
// EoF

