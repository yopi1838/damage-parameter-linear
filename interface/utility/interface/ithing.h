#pragma once
/// \file ithing.h
/// \brief Base class for items that will be stored in containers.
/// \defgroup utility Utility interface specification

#include "ideletenotice.h"
#include "igroup.h"
#include "base/src/farray.h"
#include "base/src/spinlock.h"

namespace fish {
    class IParameter;
}

namespace itasca {
#ifndef DOXYGEN
    class Archive2;
    class IContainer;
    class IGroup;
#endif

//#define ITHINGMUTEX

    /** \brief Base class for items that will be stored in containers.
    *
    * The interface defines the support needed for tracking pointers, and automatic pointer conversion during archiving.
    * This class also has a primitive run-time type identification system, maintains an integer ID, and
    *   returns a single location in space associated with the object.
    * \ingroup utility
    */
    class IThing {
#ifdef ITHINGMUTEX
        using LockType = std::mutex;
#else
        using LockType = spinlock;
#endif
    public:
        class Lock : public std::lock_guard<LockType> {
        public:
            inline Lock(const IThing *t) : std::lock_guard<LockType>(t->thingLock_) {  }
        };

        class LockIf : public conditional_lock<LockType> {
        public:
            inline LockIf(const IThing *t, bool b) : conditional_lock<LockType>(t ? &t->thingLock_ : nullptr,b) { }
        };

        class BreakLock : public reverse_lock<LockType> {
        public:
            inline BreakLock(const IThing *t) : reverse_lock<LockType>(t->thingLock_) {}
        };

        /// The base type of an IThing.
        inline static const TType type_ = 0x4c815a59;
        /// Maximum number of group slots per object
        inline static const uint maxGroupSlot_ = 128;
        /// Maximum number of extra FISH variables per object
        inline static const uint maxExtraIndex_ = 128;

        inline IThing() { }
        //inline ~IThing() {  sendDeleteNotice(); }

        /// Return the IThing interface.
        virtual const IThing *getIThing() const=0;
        virtual IThing *      getIThing()=0;   ///< \copydoc getIThing

        /// This function returns a character string indicating the type.
        /// It may be compared directly by pointer value (fast),
        ///   or a string comparison may be given.
        /// Used mostly for save/restore -- to indicate the object type for polymorphic containers.
        virtual TType getType() const=0;

        /// Returns a description of the type of the class.
        virtual QString getTypeName() const=0;

        /// This function returns a constant character string indicating the "general" type.
        /// For non-polymorphic objects this will be the same as getType().
        /// Polymorphic objects should override this function to return a "base" type,
        ///   use to indicate the general class of object.
        /// For instance for histories, getType() will return historytime and historyzone,
        ///   but getGeneralType() should be overridden to return "history" for all of them.
        virtual TType getGeneralType() const=0;

        /// This functions is intended as a more efficient version of dynamic_cast, available only for things
        ///   in the IThing tree, and used when you cannot guarantee that the class hasn't had a base shift.
        /// If no match is found, then 0 is returned.
        /// See the template version below for type safety, and automatic exception throwing.
        /// As a special case, if id is 0, then the return value should be an "upcast" to the most-derived type.
        virtual const void *convertToType(const TType &id) const=0;
        virtual void *      convertToType(const TType &id)=0;  ///< \copydoc convertToType

        /// Type specific version of convertToType, that assume that the type id is in T::type_.
        /// It also automatically converts the return type to
        template <class T> const T *convert() const { return (const T *)convertToType(T::type_);  }
        template <class T> T *      convert()       { return (T *)convertToType(T::type_); } ///< \copydoc convert

        /// Returns a value - the exact meaning of which is defined by the container and the implementing class.
        virtual quint64 getID() const=0;

        /// Returns a value that indicates the ID of some larger grouping of objects.
        /// For instance, each SEL element might have a unique ID.
        /// But all elements in Cable 5 will have a getCollectionID() of 5.
        /// The exact meaning of this is defined by the container and the implementing class.
        virtual quint64 getCollectionID() const=0;

        /// Returns a boolean indicating whether or not this is a set - a set is a container of things. The RangeElementSet
        /// works based on this identifier
        virtual bool getIsSet() const=0;

        /// Returns the iThing that is the set pointer. When checking with the RangeElementSet this is the IThing that is checked
        /// for being in filter if the returned IThing is a set (i.e., returns true for getIsSet).
        virtual const IThing * getSet() const=0;

        /// Returns a list of things associated with this thing with TType \a type.
        virtual void getAssociatedThings(const TType &type,FArray<const IThing*> *ret) const=0;

        /// Resets any data used with the associated things
        virtual void resetAssociatedThings(FArray<const IThing*> *ret) = 0;

        /// Returns a name string - the exact meaning of which is defined by the container and the implementing class.
        virtual QString getName() const=0;

        /// Returns a location in space associated with the IThing.  By default, returns the origin.
        /// This value is used by the Range logic to filter objects based on position.
        virtual DVect3 getLocation() const=0;

        /// Returns an extent in space associated with the IThing.  By default, returns an empty extent (DExtent::nothing).
        /// This value is used by the cell space logic.
        virtual DExtent3 getExtent() const=0;

        /// Returns the closest point on the IThing to the position pos.
        /// By default, returns the same value returns by getLocation().
        /// IThing implementations may choose different degrees to which they return the exact closest point.
        virtual DVect3 getClosest(const DVect3 &pos) const=0;

        /// Returns a normal vector representing the orientation of the object, if this is appropriate. 0 vector if it isn't.
        virtual DVect3 getNormal() const=0;

        /// Returns the accumulated displacement for things supporting displacement.
        virtual DVect3 getDisplacementThing() const=0;

        /// Returns the instantaneous velocity for things supporting velocity.
        virtual DVect3 getVelocityThing() const=0;

        /// Returns the volume if this is appropraite. 0 is returned otherwise.
        virtual double getVolume() const=0;

        /// Returns TRUE if object can be considered to be part of the orientation specified, withing the tolerance.
        /// If Orientation is wasDefinedByNormal() then only tol.x() is considered, which is an angle in radians.
        /// Otherwise tol.x() is the angle from dip, and tol.y() is the angle from dip direction.
        virtual bool isWithinOrientation(const Orientation3 &orientation,const DVect2 &tol) const=0;

        /// Returns TRUE if this is a "Surface", based on the three process criteria
        virtual bool isOnSurface(bool mech=true,bool therm=false,bool fluid=false,bool _or=false) const=0;

        /// Returns a const pointer to the container this IThing is stored in.
        /// May return 0, indicating that this type of thing doesn't keep track of that.
        virtual const IContainer *getContainer() const=0;
        /// Returns a pointer to the container this IThing is stored in.
        /// May return 0, indicating that this type of thing doesn't keep track of that.
        virtual IContainer *      getContainer()=0;

        /// Adds a group entry to this object.  Returns true on success.
        /// If no slot is specified, the default slot is assumed.
        //virtual bool addGroup(IGroupID *id)=0;
        virtual bool addGroup(const IGroupID &id) = 0;
        /// Removes all entries matching \a g or \a name from this object, if not slot is specified.
        /// Returns true if one or more entries were removed.
        /// If by_ is not 0, an error occurs.
        virtual bool removeGroup(const IGroupID &id)=0;
        /// Returns number of matches with ALL groups in ids.
        /// Applied either to this object directly, or possibly to connected objects.
        /// (For example, gridpoints in FLAC3D are considered a member of a group if any zones
        ///       connected to them are in that group).
        /// If the type field is non-zero, will find matches in that type only in the heirarchy.
        /// If ONLY is given, no groups can be in the object in addition to those specified in ids.
        virtual uint isInGroup(const FArray<IGroupID> &ids,TType type=0,bool only=false) const=0;
        /// Returns the group entry directly in this object at that slot, or null if it does not exist.
        /// An invalid slot will always return null, and ANY slot will use slot 0 (default).
        virtual const IGroup *getGroup(const ISlotID &slot) const=0;
        /// Returns the group name directly in this object at that slot, or the null name if it does not exist.
        /// The group_ and groupName_ parts of id are ignored.
        /// If all slots are specified, then returns a concatenation of all groups in all slots.
        virtual QString       getGroupName(const ISlotID &slot=ISlotID()) const=0;
        /// Return all groups and all slots assigned to the object in a list.
        virtual uint          getGroupList(FArray<IGroupID> *list) const=0;
        /// Copies group data from one IThing to this, all original group data is lost
        virtual void          copyGroups(const IThing *t)=0;
        virtual void          clearGroup()=0;

        /// Returns the number of extra FISH extra variables for this object.
        virtual uint                    getExtraSize()  const=0;
        /// Returns an interface to the FISH extra variables for this object.
        /// May be null if no extra variables have ever been specified.
        virtual const fish::IParameter *getExtra(uint index) const=0;
        virtual void                    setExtra(uint index,const fish::IParameter &p)=0;  ///< \copydoc getExtra()
        virtual void                    clearExtra()=0;

        /// Returns the Hide flag at index.
        virtual bool getHidden() const=0;
        /// Sets the hidden flag at index.
        virtual bool setHidden(bool b)=0;

        /// Returns the selected flag at index.
        virtual bool getSelected() const=0;
        /// Sets the selected flag at index.
        virtual bool setSelected(bool b)=0;

        // MPI Support functions
        /// Indicates if the thing is a "ghost", that is actually ownned by a different node
        virtual bool getGhost() const=0;
        virtual void setGhost(bool b)=0;
        /// Indicates the *original* order location of the thing in the main container
        ///   before mpi distribution.
        virtual quint64 getContainerOrder() const = 0;
        virtual void    setContainerOrder(quint64 i) = 0;

        /// Archives the Thing to a file (if appropriate).
        virtual void    save(Archive2 &) const=0;
        virtual bool    restore(Archive2 &,quint64)=0;
        virtual void    remap(Archive2 &)=0;
        virtual quint64 getRemapHandle(Archive2 &) const=0;
        //virtual void    clearRemapHandle()=0;

        /// Calls \code delete this; \endcode
        /// Necessary because deleting an interface pointer is not possible due to the lack of virtual destructors.
        virtual void destroy()=0;

        ///
        inline void addDeleteNotice(IDeleteNotice *delnot) const;
        inline void removeDeleteNotice(IDeleteNotice *delnot) const;
        inline void sendDeleteNotice();
        inline void changeToNewPointer(IThing *newPnt);
    private:
        mutable IDeleteNotice *head_ = nullptr;
        mutable LockType thingLock_;        // Generic spin lock for access to this IThing
        mutable LockType deleteNoticeLock_; // Specific to delete notice - using the same one was causing race conditions in intrinsics
    };

    inline void IThing::addDeleteNotice(IDeleteNotice *delnot) const {
        std::lock_guard<LockType> l(deleteNoticeLock_);
        delnot->insert(&head_);
    }

    inline void IThing::removeDeleteNotice(IDeleteNotice *delnot) const {
        std::lock_guard<LockType> l(deleteNoticeLock_);
        delnot->remove(&head_);
    }

    inline void IThing::sendDeleteNotice() {
        std::lock_guard<LockType> l(deleteNoticeLock_);
        while (head_) {
            auto dn = head_;
            dn->remove(&head_);
            reverse_lock<LockType> r(deleteNoticeLock_);
            dn->onDelete(this);
        }
    }

    inline void IThing::changeToNewPointer(IThing *newPnt) {
        std::vector<IDeleteNotice *> all;
        {
            std::lock_guard<LockType> l(deleteNoticeLock_);
            for (auto *dn = head_; dn; dn = dn->nextDeleteNotice())
                all.push_back(dn);
        }
        for (auto *dn : all)
            dn->changeToNewPointer(this,newPnt);
    }

    /// A cast operator (use similar to dynamic_cast) for types derived from IThing (const).
    template <class Dest> inline const Dest *convert_cast(const IThing *src) { return src ? src->convert<Dest>() : 0; }
    /// A cast operator (use similar to dynamic_cast) for types derived from IThing (non-const).
    template <class Dest> inline Dest *      convert_cast(IThing *src) { return src ? src->convert<Dest>() : 0; } ///< \copydoc convert_cast

    /// A cast operator for Interface types that define getThing() (const).
    template <class Dest, class Src> inline const Dest* convert_getcast(const Src* src) { return src ? src->getIThing()->template convert<Dest>() : 0; }
    /// A cast operator for Interface types that define getThing() (non-const)
    template <class Dest, class Src> inline Dest* convert_getcast(Src* src) { return src ? src->getIThing()->template convert<Dest>() : 0; } ///< \copydoc convert_getcast
} // namespace itasca

// EoF
