#pragma once

#include "archive2.h"
#include "base/src/matrix.h"

namespace archive {
    template <typename T,unsigned SX,unsigned SY>
    class Traits<Matrix<T,SX,SY>> {
    public:
        static void save(itasca::Archive2 &a, const Matrix<T, SX, SY> &v);
        static bool restore(itasca::Archive2 &a, Matrix<T, SX, SY> &v);
        static void remap(itasca::Archive2 &, Matrix<T, SX, SY> &) {}
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <unsigned SX,unsigned SY>
    class Traits<DMatrix<SX, SY>> : public Traits<Matrix<double, SX, SY>> {};

    template <unsigned SX>
    class Traits<DVMatrix<SX>> : public Traits<Matrix<double, SX, 1>> {};

    template <typename T,unsigned SX>
    class Traits<SymMatrix<T,SX>> {
    public:
        static void save(itasca::Archive2 &a, const SymMatrix<T, SX> &v);
        static bool restore(itasca::Archive2 &a, SymMatrix<T, SX> &v);
        static void remap(itasca::Archive2 &, SymMatrix<T, SX> &) {}
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <unsigned SX>
    class Traits<DSymMatrix<SX>> : public Traits<SymMatrix<double, SX>> {};

    template <class T,unsigned SX,unsigned SY>
    void Traits<Matrix<T,SX,SY>>::save(itasca::Archive2 &a, const Matrix<T,SX,SY> &v) {
        a.startArraySave(SX,itasca::Archive2::Type::Array);
        for (unsigned i=0;i<SX;++i) {
            a.startArraySave(SY,itasca::Archive2::Type::Double);
            for (unsigned j=0;j<SY;++j)
                a.saveValue(v(i,j));
            a.stopArray();
        }
        a.stopArray();
    }

    template <class T,unsigned SX,unsigned SY>
    bool Traits<Matrix<T,SX,SY>>::restore(itasca::Archive2 &a, Matrix<T,SX,SY> &v) {
        a.startArrayRestore();
        unsigned i=0;
        while (a.checkArray()) {
            a.startArrayRestore();
            unsigned j = 0;
            while (a.checkArray()) {
                a.restore(v(i,j));
                ++j;
            }
            if (j!=SY) throw Exception("Incorrect Matrix dimension on restore!");
            ++i;
        }
        if (i!=SX) throw Exception("Incorrect Matrix dimension on restore!");
        return true;
    }

    template <class T,unsigned SX> 
    void Traits<SymMatrix<T,SX>>::save(itasca::Archive2 &a,const SymMatrix<T,SX> &v) {
        a.startArraySave(SX,itasca::Archive2::Type::Array);
        for (unsigned i=0;i<SX;++i) {
            a.startArraySave(SX-i,itasca::Archive2::Type::Double);
            for (unsigned j=i;j<SX;++j)
                a.saveValue(v(i,j));
            a.stopArray();
        }
        a.stopArray();
    }

    template <class T,unsigned SX>
    bool Traits<SymMatrix<T,SX>>::restore(itasca::Archive2 &a,SymMatrix<T,SX> &v) {
        a.startArrayRestore();
        unsigned i=0;
        while (a.checkArray()) {
            a.startArrayRestore();
            unsigned j = i;
            while (a.checkArray()) {
                a.restore(v(i,j));
                ++j;
            }
            if (j!=SX) throw Exception("Incorrect Matrix dimension on restore!");
            ++i;
        }
        if (i!=SX) throw Exception("Incorrect Matrix dimension on restore!");
        return true;
    }

}
// EoF

