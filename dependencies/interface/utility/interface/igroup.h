#pragma once

#include "base/src/baseqt.h"

namespace itasca {
    class IThing;

    /// Interface to a group object.
    class IGroup {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        static constexpr TType type_ = 0x4ecd4d65;
#if defined __LINUX || __WINCMAKE
        inline static QStringView nullName() { return u"None"; }
#else
        inline static QStringView nullName() { return L"None"; }
#endif
        static constexpr uint null_ = 0xFFFFFFFF;
        /// Return IThing interface associated with this group.
        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0; ///< \copydoc getIThing
        /// Return the group name.
        virtual QString       getName() const=0;
    };

    /// Interface to a  slot object.
    class ISlot {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        static constexpr TType type_ = 0x4ecd4d66;
        static constexpr uint any_ = 0xFFFFFFFF; 
        static constexpr uint invalid_ = 0xFFFFFFFE;
        static constexpr uint unresolved_ = 0xFFFFFFFD;
#if defined __LINUX || __WINCMAKE
        inline static QStringView anyName() { return u"Any"; }
        inline static QStringView defaultName() { return u"Default"; }
        inline static QStringView invalidName() { return u"Invalid"; }
#else
        inline static QStringView anyName() { return L"Any"; }
        inline static QStringView defaultName() { return L"Default"; }
        inline static QStringView invalidName() { return L"Invalid"; }
#endif
        /// Return IThing interface associated with this group.
        virtual IThing *      getIThing()=0;
        virtual const IThing *getIThing() const=0; ///< \copydoc getIThing
                                                   /// Return the group name.
        virtual QString       getName() const=0;
        virtual quint32       getIndex() const = 0;
    };

    class ISlotID {
    public:
        static constexpr uint any_ = ISlot::any_; 
        static constexpr uint unresolved_ = ISlot::unresolved_;
        static constexpr uint invalid_ = ISlot::invalid_;
        inline static QStringView anyName() { return ISlot::anyName(); }
        inline static QStringView defaultName() { return ISlot::defaultName(); }
        inline static QStringView invalidName() { return ISlot::invalidName(); }
    
        inline          ISlotID(const ISlot *slot) : slot_(slot), slotIndex_(slot ? ISlot::unresolved_ : ISlot::invalid_) { }
        inline          ISlotID(uint slot=any_) : slotIndex_(slot) { } // Any slot for read, default slot for write.
        inline          ISlotID(const QString &name) : slotIndex_(name.length() ? ISlot::unresolved_ : ISlot::any_), slotName_(name)  { }
        inline          ISlotID(const ISlotID &id) : slot_(id.slot_), slotIndex_(id.slotIndex_), slotName_(id.slotName_), thread_(id.thread_) {}
        inline          ISlotID(ISlotID &&id) noexcept : slot_(id.slot_), slotIndex_(id.slotIndex_), slotName_(std::move(id.slotName_)), thread_(id.thread_) {}
        inline const ISlotID &operator=(const ISlotID &id) { slot_ = id.slot_;  slotIndex_ = id.slotIndex_;  slotName_ = id.slotName_;  thread_ = id.thread_;  return *this; }
        inline const ISlotID &operator=(ISlotID &&id) noexcept { slot_ = id.slot_;  slotIndex_ = id.slotIndex_;  slotName_ = std::move(id.slotName_);  thread_ = id.thread_;  return *this; }
        inline bool           anySlot() const { return slot_ ? slot_->getIndex()==any_ : (slotIndex_!=unresolved_ ? slotIndex_==ISlot::any_ : slotName_.compare(anyName(), Qt::CaseInsensitive)==0 ); }
        inline bool           unresolvedSlot() const { return slot_==nullptr && (slotIndex_!=unresolved_ || slotName_.length()); }
        inline bool           invalidSlot() const {  return slotIndex_ == invalid_; }
        inline bool           hasSlot() const { return slot_ || slotIndex_!=unresolved_ || slotName_.length(); }
        inline void           setSlot(uint slot) { slot_ = nullptr;  slotIndex_ = slot;  slotName_ = QString(); }
        inline void           setSlot(const QString &name) { slot_ = nullptr;  slotName_ = name;  slotIndex_ = name.length() ? unresolved_ : any_; }
        inline void           setSlot(const ISlot *slot) { slot_ = slot;  slotName_ = QString(); slotIndex_ = slot ? unresolved_ : invalid_;  }
        inline void           setSlot(const ISlotID &id) { operator=(id);  }
        inline void           setAny() { slot_ = nullptr; slotIndex_ = any_;  slotName_ = QString();  }
        inline const ISlot *  slot() const { return slot_;  }
        inline uint           slotIndex() const { return slot_ ? slot_->getIndex() : slotIndex_; }
        inline QString        slotName() const { return slot_ ? slot_->getName() : slotName_; }
        inline const QString &givenSlotName() const { return slotName_;  }
        inline bool operator <(const ISlotID &slot) const { return slotIndex()<slot.slotIndex(); }
        inline quint32 thread() const { return thread_;  }
        inline void    thread(quint32 t) { thread_ = t;  }
    private:
        // Used to determine slot in this order of priority
        const ISlot *slot_ = nullptr; // If non-null 
        quint32      slotIndex_ = ISlot::unresolved_; // if not unresolved
        QString      slotName_;       // Last resort
        quint32      thread_ = 0;
    };
    
    class IGroupID : public ISlotID {
    public:
        inline static QStringView nullName() { return IGroup::nullName(); }

        inline IGroupID() { }
        inline IGroupID(const IGroup *group) : group_(group) { } 
        inline IGroupID(const IGroup *group,const ISlot *slot) : ISlotID(slot), group_(group) { }
        inline IGroupID(const QString &groupName) : groupName_(groupName) {}
        inline IGroupID (const IGroup *group,const ISlotID &slot) : ISlotID(slot), group_(group) { }
        inline IGroupID(const QString &groupName,const QString &slotName) : ISlotID(slotName), group_(nullptr), groupName_(groupName) { }
        inline IGroupID(const QString &groupName,quint32 slot) : ISlotID(slot), groupName_(groupName) { }
        inline IGroupID(const IGroupID &id) : ISlotID(id), group_(id.group_), groupName_(id.groupName_) {}
        inline IGroupID(IGroupID &&id) noexcept : ISlotID(std::move(id)), group_(id.group_), groupName_(std::move(id.groupName_)) {}
        inline IGroupID(const ISlotID &s) : ISlotID(s) {}
        inline IGroupID(ISlotID &&s) noexcept : ISlotID(std::move(s)) {}
        inline const IGroupID &operator=(const IGroupID &id) { ISlotID::operator=(id); group_ = id.group_;  groupName_ = id.groupName_;  return *this; }
        inline const IGroupID &operator=(IGroupID &&id) noexcept { ISlotID::operator=(std::move(id));  group_ = id.group_;  groupName_ = std::move(id.groupName_);  return *this; }
        inline bool           hasGroup() const { if (group_ || groupName_.length()) return true;  return false; }
        inline bool           unresolvedGroup() const { return !group_ && groupName_.length();  }
        inline bool           unresolved() const { return unresolvedGroup() || unresolvedSlot();  }
        inline void           setGroup(const IGroup *group) { group_ = group;  groupName_ = QString();  }
        inline void           setGroup(const QString &name) { groupName_ = name; group_ = nullptr; }
        inline const IGroup * group() const { return group_; }
        inline QString        groupName() const { return group_ ? group_->getName() : groupName_; }
        inline const QString &givenGroupName() const { return groupName_;  }
        inline bool operator <(const IGroupID &group) const { if (slotIndex()!=group.slotIndex()) return slotIndex()<group.slotIndex(); return groupName().compare(group.groupName(),Qt::CaseInsensitive)<0; }
    private:
        const IGroup *group_ = nullptr;     // If non-null groupName_ is ignored.
        QString       groupName_; // Used to find group_, must be non-null if group_ is null.
    };
} // namespace itasca
// EoF
