#pragma once

#include "exportfish.h"
#include "parameter.h"
#include "shareddata.h"

namespace itasca {
    class Archive2;
}

namespace fish {
    using itasca::Archive2;

    class GlobalRuntime;
    class Parameter;

    class StructureMaster {
    public:
        StructureMaster(GlobalRuntime *global) : global_(global) { }
        ~StructureMaster() { }

                    const QStringList &fields() const { return fields_; }
                    void               fields(const QStringList &list) { fields_ = list; }
                    const QString &    name() const { return name_; }
                    void               name(const QString &s) { name_ = s; }
                    quint64            index() const { return index_; }
                    void               index(quint64 i) { index_ = i; }
                    qint32             fieldToIndex(const QString &field) const;
                    GlobalRuntime *    globalRuntime() const { return global_; }
        FISH_EXPORT void               addAccessors() const;

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);

    private:
        QStringList    fields_;
        QString        name_;
        quint64        index_ = 0; // Index in the global list
        GlobalRuntime *global_ = nullptr; 
    };

    class StructureBase : public SharedData<StructureBase>, 
                          public std::vector<Parameter> {
    public:
        FISH_EXPORT StructureBase(quint64 masterIndex,GlobalRuntime *global);
        StructureBase(const StructureBase &b) : std::vector<Parameter>(b), masterIndex_(b.masterIndex_), global_(b.global_) { }
        StructureBase() : masterIndex_(0), global_(nullptr) { }
        ~StructureBase() { }

        const StructureBase &operator=(const StructureBase &in) { masterIndex_ = in.masterIndex_;  global_ = in.global_;  std::vector<Parameter>::operator=(in);  return *this; }
        bool operator==(const StructureBase &l) const;
        bool operator<(const StructureBase &l) const;
        GlobalRuntime *globalRuntime() const { return global_; }
        void           globalRuntime(GlobalRuntime *g) { global_ = g; }

                    quint64                masterIndex() const { return masterIndex_; }
                    void                   masterIndex(quint64 m) { masterIndex_ = m; }
        FISH_EXPORT const StructureMaster *master() const;

        virtual StructureBase *getT() { return this;  }
        virtual const StructureBase *getT() const { return this;  }
        FISH_EXPORT virtual void save(Archive2 &a) const;
        FISH_EXPORT virtual void restore(Archive2 &a);
        FISH_EXPORT virtual void remap(Archive2 &a);

    private:
        quint64        masterIndex_;
        GlobalRuntime *global_;
    };

    // Uses copy on write
    class Structure { 
    public:
        Structure() { d_ = NEWC(StructureBase()); }
        Structure(const Structure &l) : d_(l.d_) { }
        Structure(Structure &&l) noexcept : d_(std::move(l.d_)) { }
        explicit Structure(quint64 masterIndex_,GlobalRuntime *global) { d_ = NEWC(StructureBase(masterIndex_,global)); }
        explicit Structure(const StructureMaster *master,GlobalRuntime *global) { d_ = NEWC(StructureBase(master->index(),global)); }
        FISH_EXPORT explicit Structure(const QString &name,GlobalRuntime *global);
        ~Structure() { }

        const Structure &operator=(const Structure &l) { d_ = l.d_;  return *this; }
        const Structure &operator=(Structure &&l) noexcept { d_ = std::move(l.d_);  return *this; }
        bool operator==(const Structure &l) const { if (d_==l.d_) return true;  return (*d_) == (*l.d_); }
        bool operator<(const Structure &l) const { if (d_==l.d_) return false;  return (*d_) < (*l.d_); }

        quint32 size() const { return to<quint32>(d_->size()); }
        QString name() const { return d_->master()->name(); }
        QString toString() const;

        quint64                masterIndex() const { return d_->masterIndex(); }
        const StructureMaster *master() const { return d_->master(); }
        const Parameter &      get(qint32 index) const { return d_->operator[](index); }
        Parameter &            getForSet(qint32 index) { return d_->operator[](index); }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
        void fixDroppedPointer(quint64 *missed);
        //void savePointerDetails() const;

    private:
        SharedDataPointer<StructureBase> d_;
    };
} // namespace fish
  // EOF
