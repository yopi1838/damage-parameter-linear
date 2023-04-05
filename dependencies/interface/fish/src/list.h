#pragma once

#include "exportfish.h"
#include "parameter.h"
#include "shareddata.h"
#include <vector>

namespace itasca {
    class Archive2;
}

namespace fish {
    class GetArgument;
    class Parameter;
    class LocalRuntime;
    using itasca::Archive2;

    class ListBase : public SharedData<ListBase>, 
                     public std::vector<Parameter> {
    public:
        ListBase() { }
        ListBase(quint64 size) : std::vector<Parameter>(size) { }
        ListBase(const ListBase &b) : SharedData<ListBase>(b), std::vector<Parameter>(b) { }
        ListBase(std::initializer_list<Parameter> l) : std::vector<Parameter>(l) { }
        ~ListBase() { }
        qint64 lastIndex() const { return lastIndex_; }
        void   lastIndex(qint64 i) const { lastIndex_ = i; }

        virtual ListBase *getT() { return this;  }
        virtual const ListBase *getT() const { return this;  }
        FISH_EXPORT virtual void save(Archive2 &a) const;
        FISH_EXPORT virtual void restore(Archive2 &a);
        FISH_EXPORT virtual void remap(Archive2 &a);

    private:
        mutable qint64 lastIndex_=0;
    };

    // Uses copy-on-write
    class List { // A list of FISH parameters
    public:
        class Base;
        using opFunc = std::add_pointer_t<void(Parameter &, const Parameter &, bool threading, quint32 threadnum)>;
        using unaryOpFunc = std::add_pointer_t<void(Parameter &, bool threading, quint32 threadnum)>;

                    List() { d_ = NEWC(ListBase()); }
                    List(const List &l) : d_(l.d_) { }
        FISH_EXPORT List(const List &l,quint64 start,quint64 end);
        explicit    List(quint64 size) { d_ = NEWC(ListBase(size)); }
                    List(std::initializer_list<Parameter> l) { d_ = NEWC(ListBase(l)); }
                    List(List &&l) noexcept : d_(std::move(l.d_)) { }
                    ~List() { }

                    const List &operator=(const List &l) { d_ = l.d_;  return *this; }
                    const List &operator=(List &&l) noexcept { d_ = l.d_;  l.d_ = nullptr;  return *this; }
        FISH_EXPORT bool operator==(const List &l) const;
        FISH_EXPORT bool operator<(const List &l) const;
        FISH_EXPORT bool eqWithTol(const List &l) const;
        FISH_EXPORT bool ltWithTol(const List &l) const;

                    quint64 size() const { return d_->size(); }
                    void    resize(quint64 size) { d_->resize(size); }
        FISH_EXPORT QString toString() const;
        FISH_EXPORT SymTensor toTensor() const;
        FISH_EXPORT qint64 toIndex(const Parameter &p,bool write,bool secondArg) const;
                    qint64 lastIndex() const { return d_->lastIndex(); }
                    void   lastIndex(qint64 i) const { d_->lastIndex(i); }

                    Parameter &      getForSet(qint64 index) { if (index<0) return pushFront(); if (to<quint64>(index)>=size()) return push();  return d_->at(index); }
                    const Parameter &get(quint64 index) const { return d_->at(index); }
        FISH_EXPORT void             set(qint64 index,const Parameter &p);
        FISH_EXPORT Parameter &      push();
        FISH_EXPORT Parameter &      pushFront();
        FISH_EXPORT void             append(const List &l);
        FISH_EXPORT void             replace(qint64 start,qint64 end,const List &l);
        FISH_EXPORT bool             applyOperator(opFunc f,const Parameter &second,bool threading,quint32 threadnum);
        FISH_EXPORT bool             applyOperatorRight(opFunc f,Parameter &first,bool threading,quint32 threadnum) const;
        FISH_EXPORT bool             applyUnaryOperator(unaryOpFunc f,bool threading,quint32 threadnum);
        FISH_EXPORT void             getArgument(const GetArgument &arg,Parameter &out) const;
        FISH_EXPORT void             getArgumentList(const List &list,Parameter &out) const;
        FISH_EXPORT void             setArgumentList(const List &arg, const List &newVal);

        FISH_EXPORT void       remove(quint64 index);
        FISH_EXPORT Parameter &insert(quint64 before);
        FISH_EXPORT quint64    count(const Parameter &p,bool isThreading,uint threadNum) const;
        FISH_EXPORT qint64     find(const Parameter &p,quint64 start,bool isThreading,uint threadNum) const;
        FISH_EXPORT List       reverse() const;
        FISH_EXPORT List       sort() const;
        FISH_EXPORT List       resize(qint64 size,const Parameter &n) const;

        FISH_EXPORT Parameter minimum() const;
        FISH_EXPORT Parameter maximum() const;
        FISH_EXPORT Parameter sum() const;

        FISH_EXPORT void save(Archive2 &a) const;
        FISH_EXPORT bool restore(Archive2 &a,quint64 label);
        FISH_EXPORT void remap(Archive2 &a);
        FISH_EXPORT void fixDroppedPointer(quint64 *missed);
        //FISH_EXPORT void savePointerDetails() const;

        FISH_EXPORT static List range(const Parameter &start,const Parameter &end,const Parameter &inc,bool isThreading,uint threadNum);
    private:
        void applyOperator(opFunc f,const List &second,bool threading,quint32 threadnum);

        SharedDataPointer<ListBase> d_;
    };
} // namespace fish
// EOF
