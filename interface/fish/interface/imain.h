#pragma once

#include "librarydata.h"

namespace itasca {
    class IContainer;
}

namespace fish {
    class IGlobalRuntime;
    class IParameter;
    class ISymbol;

    class IMain {
    public:
        virtual LibraryData               getLibraryData(const QString &name,bool throwIfAmbiguous) const=0;
        virtual bool                      isOperator(QChar c) const=0;
        virtual const itasca::IContainer *getSymbolContainer() const=0;
        virtual std::vector<std::pair<QString,ISymbol *>> getAllSymbols() const=0;
        virtual const QStringMap<ISymbol *> & getAllPublicSymbols() const = 0;
        virtual bool                      checkIfSymbolModified() const=0;
        virtual bool                      willAutoCreate() const=0;
        virtual bool                      willBoolConvert() const=0;
        virtual const IGlobalRuntime *    getIGlobalRuntime() const=0;
        virtual IParameter *              createParameter() const=0;
    };
} // namespace fish
// EoF
