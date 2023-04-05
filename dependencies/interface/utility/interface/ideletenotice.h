#pragma once

#include <cassert>

namespace itasca {
    class IThing;

    // Note:  This class does not auto-remove itself on destruction!
    class IDeleteNotice {
    public:
        inline IDeleteNotice() { }
        virtual inline ~IDeleteNotice() { assert(next_ == nullptr); assert(prev_ == nullptr); }
        virtual void onDelete(IThing *pnt)=0;
        virtual void changeToNewPointer(IThing *oldPnt,IThing *newPnt)=0;
        inline void insert(IDeleteNotice **head);
        inline void remove(IDeleteNotice **head);
        inline bool isInserted() const { return next_ ? true : false; }
        inline IDeleteNotice *nextDeleteNotice() { return next_; }
    private:
        IDeleteNotice(const IDeleteNotice &) = delete;
        IDeleteNotice(IDeleteNotice &&) = delete;
        void operator=(const IDeleteNotice &) = delete;
        IDeleteNotice *next_ = nullptr;
        IDeleteNotice *prev_ = nullptr;
    };

    inline void IDeleteNotice::insert(IDeleteNotice **head) {
        assert(prev_==nullptr);
        assert(next_==nullptr);
        assert(head);
        next_ = *head;
        if (*head) (*head)->prev_ = this;
        *head = this;
    }

    inline void IDeleteNotice::remove(IDeleteNotice **head) {
        assert(head);
        if (prev_) prev_->next_ = next_;
        else if (*head==this)  *head = next_;
        if (next_) next_->prev_ = prev_;
        next_ = prev_ = nullptr;
    }

} // namespace itasca
// EoF
