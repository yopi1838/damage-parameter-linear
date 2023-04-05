#pragma once

#include "parameter.h"
#include "shareddata.h"
#include "shared/src/cheapspinlock.h"

namespace itasca {
    class Archive2;
}

#include <map>

namespace fish {
    class List;
    class Parameter;
    using itasca::Archive2;

    class MapBase : public SharedData<MapBase>, 
                    public std::map<Parameter,Parameter> {
    public:
        MapBase() { }
        MapBase(const MapBase &b) : SharedData<MapBase>(b), std::map<Parameter,Parameter>(b) { }

        virtual MapBase *getT() { return this;  }
        virtual const MapBase *getT() const { return this;  }
        FISH_EXPORT virtual void     save(Archive2 &) const;
        FISH_EXPORT virtual void     restore(Archive2 &);
        FISH_EXPORT virtual void     remap(Archive2 &);
    };

    // Uses copy on write
    class Map { // A Map of FISH parameters
    public:
        Map() { d_ = NEWC(MapBase()); }
        Map(const Map &l) : d_(l.d_) { }
        Map(Map &&l) noexcept : d_(std::move(l.d_)) { }
        ~Map() { }

        const Map &operator=(const Map &l) { d_ = l.d_; return *this; }
        const Map &operator=(Map &&l) noexcept { d_ = std::move(l.d_); return *this; }
        FISH_EXPORT bool operator==(const Map &l) const;
                    bool operator<(const Map &l) const;

        quint64 size() const { return d_->size(); }
        bool contains(const Parameter &p) const { return d_->count(p)>0 ? true : false; }
        FISH_EXPORT QString toString() const;
        spinlock &lock() const { return lock_; }

                    Parameter &      getForSet(const Parameter &key) { return d_->operator[](key); }
        FISH_EXPORT const Parameter &get(const Parameter &key) const;
        FISH_EXPORT bool             set(const Parameter &key,const Parameter &value,bool canFail=true);
        FISH_EXPORT void             combine(const Map &m);
        FISH_EXPORT bool             remove(const Parameter &key);
        FISH_EXPORT void             add(const List &keys, const List &values);

        MapBase::const_iterator begin() const { return d_->begin(); }
        MapBase::const_iterator end() const { return d_->end(); }

        FISH_EXPORT List values() const;
        FISH_EXPORT List keys() const;

        FISH_EXPORT void save(Archive2 &a) const;
        FISH_EXPORT bool restore(Archive2 &a,quint64 label);
        FISH_EXPORT void remap(Archive2 &a);
        FISH_EXPORT void fixDroppedPointer(quint64 *missed);
        //FISH_EXPORT void savePointerDetails() const;

    private:
        SharedDataPointer<MapBase> d_;
        mutable spinlock lock_;
    };
} // namespace fish
  // EOF
