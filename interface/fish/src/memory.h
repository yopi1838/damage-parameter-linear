#pragma once

#include "parameter.h"
#include "safecountptr.h"

namespace fish {
    class Parameter;

    class MemoryBase : public SafeCountData<MemoryBase>, 
                       public std::vector<Parameter> {
        friend class Memory;
    public:
        MemoryBase() : MemoryBase(true, 0) {}
        MemoryBase(bool autoID,quint64 size=0) : SafeCountData<MemoryBase>(autoID), std::vector<Parameter>(size) { }
        virtual ~MemoryBase() { }

        Parameter &get(quint64 index) { return at(index); }
        const Parameter &get(quint64 index) const { return at(index); }
        spinlock &lock() const { return lock_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
        void fixDroppedPointer(quint64 *missed);
        //void savePointerDetails() const;

    private:
        mutable spinlock lock_;
    };

    // Uses reference counting
    class Memory {
    public:
        Memory(quint64 size=0) { d = size ? NEWC(MemoryBase(true,size)) : nullptr; }
        Memory(const Memory &m) : d(m.d), offset_(m.offset_) { }
        ~Memory() { }

        const Memory &operator=(const Memory &m) { d = m.d;  offset_ = m.offset_; return *this; }
        bool operator==(const Memory &m) const;
        bool operator<(const Memory &m) const { return id()<m.id(); }

        void add(qint64 offset) { checkOffset(offset); offset_ += offset; }
        quint64 id() const { return d ? d->id() : 0; }
        quint64 size() const { return d ? d->size() : 0; }
        quint64 offset() const { return offset_; }
        QString toString() const;
        void destroy() { if (!d) return;  MemoryBase *b = d;  delete b;  assert(!d); }
        spinlock *lock() const { return d ? &d->lock() : nullptr; }

        Parameter &getForSet(quint64 index) { check(index);  return d->get(index+offset_); }
        const Parameter &get(quint64 index) const { check(index);  return d->get(index+offset_); }

        void save(Archive2 &a) const { d.save(a); }
        bool restore(Archive2 &a,quint64 label) { return d.restore(a,label); }
        void remap(Archive2 &a) { d.remap(a); }
        void fixDroppedPointer(quint64 *missed) { d->fixDroppedPointer(missed); }
        //void savePointerDetails() const { d->savePointerDetails(); }

    private:
        inline void check(quint64 index) const;
        inline void checkOffset(qint64 offset) const;
        SafeCountPointer<MemoryBase> d;
        quint64                      offset_=0;
    };

    void Memory::check(quint64 index) const { 
        if (!d) throw Exception("Attempt to access null memory."); 
        if (index+offset_>=size()) throw Exception("Index out of memory range (%1 given, %2 offset size %3).",index,offset_,size());
    }

    void Memory::checkOffset(qint64 offset) const { 
        if ( ((offset>0 ) && (offset_+offset)>=size()) || 
             ((offset<0) && (to<quint64>(-offset) > offset_)) ) 
            throw Exception("Attempt to move memory reference out of range.");
    }
} // namespace fish
  // EoF
