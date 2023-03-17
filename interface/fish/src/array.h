#pragma once

#include "parameter.h"
#include "safecountptr.h"
#include "base/src/farray.h"
#include <vector>

namespace fish {
    class Matrix;
    
    class ArrayBase : public SafeCountData<ArrayBase>,
                      public std::vector<Parameter> {
        friend class Array;
    public:
        FISH_EXPORT ArrayBase(bool autoID=true);
        FISH_EXPORT ArrayBase(const FArray<quint64> &size);
        FISH_EXPORT virtual ~ArrayBase();

        bool operator==(const ArrayBase &m) const;
        bool operator<(const ArrayBase &m) const;

        quint32 dim() const { return to<quint32>(size_.size()); }
        const FArray<quint64> &size() const { return size_; }
        quint64 size(quint32 di) const { return di<dim() ? size_.at(di) : 0; }
        FISH_EXPORT quint64 totalSize() const;
        spinlock &lock() const { return lock_; }

        Parameter &get(const FArray<quint64> &index) { return get(getIndex(index)); }
        const Parameter &get(const FArray<quint64> &index) const { return get(getIndex(index)); }
        Parameter &get(quint64 index) { return operator[](index); }
        const Parameter &get(quint64 index) const { return operator[](index); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
        void fixDroppedPointer(quint64 *missed);
        //void savePointerDetails() const;

    private:
        FISH_EXPORT quint64 getIndex(const FArray<quint64> &index) const;

        FArray<quint64> size_;
        mutable spinlock lock_;
    };

    // Uses reference counting
#pragma warning(push)
#pragma warning(disable:26110) // Failing to hold lock before calling function - spurious
    class Array {
    public:
        //Array() { d = NEWC(ArrayBase(true)); }
        Array() { d = nullptr;  }
        Array(const FArray<quint64> &size) { d = NEWC(ArrayBase(size)); }
        Array(const Array &m) : d(m.d) { }
        ~Array() { }

        const Array &operator=(const Array &m) { d = m.d; return *this; }
        bool operator==(const Array &m) const { if (d==m.d) return true;   return *d == *m.d; }
        bool operator<(const Array &m) const { if (d==m.d) return false;   return *d < *m.d; }

        quint64 id() const { if (!d) return 0;  std::lock_guard lock(d->lock());  return d->id(); }
        quint32 dim() const { if (!d) return 0;  std::lock_guard lock(d->lock());  return d->dim(); }
        void fillSize(FArray<quint64> *fill) const { if (d && fill) { std::lock_guard lock(d->lock());  *fill = d->size(); } else if (fill) fill->clear(); }
        quint64 size(quint32 dim) const { if (!d) return 0;  std::lock_guard lock(d->lock());  return d->size(dim); }
        quint64 totalSize() const { if (!d) return 0;  std::lock_guard lock(d->lock());  return d->totalSize(); }
        FArray<quint64> sizeIndex() const { if (!d) return FArray<quint64>();  std::lock_guard lock(d->lock());  return d->size(); }
        QString toString() const;
        SymTensor toTensor() const;
        void destroy() { if (!d) return;  ArrayBase *b = d;  delete b;  assert(!d); }
        spinlock *lock() const { return d ? &d->lock() : nullptr; }

        Parameter &getForSet(const FArray<quint64> &index) { check();  std::lock_guard lock(d->lock());  return d->get(index); }
        const Parameter &get(const FArray<quint64> &index) const { check(); std::lock_guard lock(d->lock());  return d->get(index); }
        Parameter &getForSet(quint64 index) { check();  std::lock_guard lock(d->lock());  return d->get(index); }
        const Parameter &get(quint64 index) const { check();  std::lock_guard lock(d->lock());  return d->get(index); }

        void save(Archive2 &a) const { d.save(a); }
        bool restore(Archive2 &a,quint64 label) { return d.restore(a,label); }
        void remap(Archive2 &a) { d.remap(a); }
        void fixDroppedPointer(quint64 *missed) { d->fixDroppedPointer(missed); }
        //void savePointerDetails() const { d->savePointerDetails(); }

    private:
        void check() const { if (!d) throw std::out_of_range("Attempt to access null array."); }
        SafeCountPointer<ArrayBase> d;
    };
#pragma warning(pop)

    Array arrayFromMatrix(const Matrix & m);
} // namespace fish
// EoF
