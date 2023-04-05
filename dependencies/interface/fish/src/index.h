#pragma once

#include "exportfish.h"
#include "shared/src/cheapspinlock.h"
#ifdef __LINUX
#include "stdafx.h"
#endif

namespace itasca {
    class Archive2;
}

namespace fish {
    using itasca::Archive2;

    class Index {
        friend class Base;
    public:
        Index() : index_(0), type_(0) { add(); }
        Index(const Index &fp) : index_(fp.index_), type_(fp.type_) { add(); }
        Index(const quint64 &index,const TType &type) : index_(index), type_(type) { add(); }
        ~Index() { remove(); }

        const Index &operator=(const Index &fp) { index_ = fp.index_; type_ = fp.type_; return *this; }
        bool operator==(const Index &i) const { return index_ == i.index_; }
        bool operator<(const Index &i) const { return index_ < i.index_; }

        void           setType(const TType &type) { type_ = type; }
        const TType &  getType() const { return type_; }
        quint64 &      value() { return index_; }
        const quint64 &value() const { return index_; }

        void           save(Archive2 &a) const;
        bool           restore(Archive2 &a);
        //void           remap(Archive2 &a);

        static void    clearIndex(const quint64 & index);
    private:
        explicit Index(int) { next_ = prev_ = this; }

        void add() {
            std::lock_guard lock(listLock_);
            next_ = &base_;
            prev_ = base_.prev_;
            base_.prev_->next_ = this;
            base_.prev_ = this;
        }
        void remove() {
            std::lock_guard l(listLock_);
            next_->prev_ = prev_;  
            prev_->next_ = next_;
        }

        quint64 index_ = 0;
        TType   type_ = 0;
        Index * next_ = nullptr;
        Index * prev_ = nullptr;

        FISH_EXPORT static Index base_;
        FISH_EXPORT static spinlock listLock_;
    };
} // namespace fish

Q_DECLARE_METATYPE(fish::Index);

// EoF
