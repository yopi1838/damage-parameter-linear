#pragma once

#include "parameter.h"
#ifdef __LINUX
#include "stdafx.h"
#endif

namespace itasca {
    class IMultiContainer;
}

namespace fish {
    class List;
    class Parameter;

    // Typical syntax
    // Iterator it(arg[0]);
    // for (bool b=it.start(p); !b; b=it.next(p))

    // Works on string, vector2, vector3, tensor, container pointer, list, map, array, or matrix
    class FISH_EXPORT Iterator {
    public:
        Iterator();
        Iterator(const Parameter &p);
        Iterator(const Iterator &it);
        Iterator(Iterator &&it) noexcept;
        ~Iterator();
        const Iterator &operator=(const Iterator &i);
        const Iterator &operator=(Iterator &&i) noexcept;
        quint64 size() const;
        bool isIndex() const;
        bool isMatrix(UVect2 *size) const;
        itasca::IMultiContainer *isMultiContainer();
        bool index(quint64 i,Parameter &p);
        bool start(Parameter &p); // Returns TRUE on fail
        bool next(Parameter &p); // Returns TRUE on fail
        QString toString() const;
        quint32 argument() const { return argument_; }
        void    argument(quint32 argument) { argument_ = argument; }
        void    filter(const List &l,List *out);

        static bool setIndex(Parameter *val,quint64 i, const Parameter &p);
    private:
        struct Private;
        Private *p_ = nullptr;
        quint32 argument_ = 0;
    };
} // namespace fish
// EoF
