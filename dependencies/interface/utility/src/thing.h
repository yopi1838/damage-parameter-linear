#pragma once

#include "lowest.h"
#include "shared/src/cheapspinlock.h"
#include "shared/src/archive2.h"
#include <vector>
#include "../interface/ithing.h"

/** \file thing.h
* \brief Base class for items stored in containers.
*
* \addtogroup utilitysrc
*/

namespace fish {
    class Parameter;
}

namespace itasca {
    /** \brief Base class for items stored in containers.
    *
    * This implementation of the IThing interface defines the support needed for tracking pointers, and automatic 
    * pointer conversion during archiving.
    * This class also has a primitive run-time type identification system, maintains an integer ID, and
    *   returns a single location in space associated with the object.
    *
    * The Run-Time Typing system assumes the following:
    * First - each type (including Interface types) is assigned a unique ID (of type TType) as a static const.
    *         This should be declared as a putlic static const named "type_".
    *         The value of these IDs should be unique across all types.  I suggest using the result of time(),
    *         treated as a 32 bit integer.  Then we should only get a conflict if two people happen to create a class
    *         id at exactly the same second.  For convenience I have added a "-moo id" command to the frameword 
    *         that generates such an id and puts it in the clipboard, so you can just paste it into the editor.
    * Second - Each type implementation level should implement a "virtual const void *convertToType(const TType &type) const"
    *          function, that returns the right base pointer for the type represented by type for itself and any interfaces
    *          that it implements, then calls the base class if no matches.  If you don't do this, pointer conversions
    *          in FISH and elsewhere using convert_cast<> will fail.
    * Third - Any type that *will actually get instantiated* (final type), should implement the "virtual QString getTypeName() const"
    *         method.  This is used in FISH and other places to identify pointer types by name.  In some cases (such as Histories)
    *         it is OK to have the general type return a name for all objects of that kind, in other words to have the History class 
    *         create a getTypeName() method that returns "History" so that all the specific History classes derived from it do not
    *         have to do so.
    * This means that for a class derived from thing, the following functions/members are generally going to be present:
    *
    *    inline static const TType type_ = 0x4a56789a;
    *    virtual TType       getType() const { return type_; }
    *    virtual QString     getTypeName() const { return "Fred"; } // Only required if "Fred" objects will be created.
    *    virtual const void *convertToType(const TType &type) { if (type==type_) return this;  return Base::convertToType(type); }
    *
    * \ingroup utilitysrc
    */

    class Archive2;

    class UTILITY_EXPORT Thing : public IThing {
    public:
        /// Static constant \a type_ which is "thing" by deafult.
        static const TType type_ = 0x4c815abe;

        /// Defualt constructor.
        Thing();
        /// Copy constructor.
        Thing(const Thing &);
        /// Default destructor.
        virtual ~Thing();

        /// Assignment operator that does nothing.
        const Thing &operator=(const Thing &) { return *this; }

        /// IThing override -- See IThing documentation
        virtual const IThing *    getIThing() const { return this; } 
        virtual IThing *          getIThing() { return this; } ///< \copydoc getIThing
        virtual TType             getType() const { return type_; } ///< \copydoc getIThing
        virtual QString           getTypeName() const=0; ///< \copydoc getIThing
        virtual quint32           getGeneralType() const { return getType(); }    ///< \copydoc getIThing
        virtual const void *      convertToType(const TType &type) const { if (type==Thing::type_ || type==IThing::type_) return this;  return 0; } ///< \copydoc getIThing
        virtual void *            convertToType(const TType &type) { return const_cast<void *>(const_cast<const Thing *>(this)->convertToType(type)); } ///< \copydoc getIThing
        virtual quint64           getID() const { return 0; } ///< \copydoc getIThing
        virtual quint64           getCollectionID() const { return getID(); } ///< \copydoc getIThing
        virtual bool              getIsSet() const { return false; } ///< \copydoc getIThing
        virtual const IThing *    getSet() const { return 0; } ///< \copydoc getIThing
        virtual void              getAssociatedThings(const TType &type,FArray<const IThing*> *ret) const { type; assert(ret); ret->clear(); ret->push_back(this); } ///< \copydoc getIThing
        virtual void              resetAssociatedThings(FArray<const IThing*> *) { } ///< \copydoc getIThing
        virtual QString           getName() const { return getTypeName(); } ///< \copydoc getIThing
        virtual DVect3            getLocation() const { return DVect3(0,0,0); } ///< \copydoc getIThing
        virtual DExtent3          getExtent() const { return DExtent3::nothing(); } ///< \copydoc getIThing
        virtual DVect3            getClosest(const DVect3 &) const { return getLocation(); }
        virtual DVect3            getNormal() const override { return DVect3(0); }
        virtual DVect3            getDisplacementThing() const override { return DVect3(0); }
        virtual DVect3            getVelocityThing() const override { return DVect3(0); }
        virtual double            getVolume() const override { return 0.0; }
        virtual bool              isWithinOrientation(const Orientation3 &o,const DVect2 &tol) const override { o;  tol;  return false; }
        virtual bool              isOnSurface(bool mech=true,bool therm=false,bool fluid=false,bool _or=false) const override { mech;  therm;  fluid;  _or; return false; }
        virtual const IContainer *getContainer() const { return 0; }  ///< \copydoc getIThing
        virtual IContainer *      getContainer() { return 0; }  ///< \copydoc getIThing
        virtual bool              addGroup(const IGroupID &id) override { id;  return false; }
        virtual bool              removeGroup(const IGroupID &id) override { id;  return false; }
        virtual uint              isInGroup(const FArray<IGroupID> &ids,TType type=0,bool only=false) const override {  ids; type; only; return 0; }
        virtual const IGroup *    getGroup(const ISlotID &id=ISlotID((uint)0)) const override { id; return nullptr; }
        virtual QString           getGroupName(const ISlotID &id=ISlotID()) const override { id;  return QString(); }
        virtual uint              getGroupList(FArray<IGroupID> *list) const override { if (list) list->clear();  return 0; }
        virtual void              copyGroups(const IThing *t) override { t;  }
        virtual uint              getExtraSize() const { return 0; }
        virtual const fish::IParameter *getExtra(uint index) const { index; return nullptr; }
        virtual void                    setExtra(uint index,const fish::IParameter &p) { index; p; }
        virtual void              clearExtra() { }
        virtual void              clearGroup() { }
        virtual bool              getHidden() const { return false; }
        virtual bool              setHidden(bool b) { b; return false; }
        virtual bool              getSelected() const { return false; }
        virtual bool              setSelected(bool b) {  b;  return false; }
        virtual bool              getGhost() const override { return false; }
        virtual void              setGhost(bool) override { assert(0); }
        virtual quint64           getContainerOrder() const override { return 0;  }
        virtual void              setContainerOrder(quint64) override { assert(0); }
        virtual void              save(Archive2 &a) const override;
        virtual bool              restore(Archive2 &a,quint64 label) override;
        virtual void              remap(Archive2 &a) override;
        virtual quint64           getRemapHandle(Archive2 &a) const override;
        virtual void              clearRemapHandle();
        virtual void              destroy() { delete this; } ///< \copydoc getIThing

        /// Set the container to which this Thing belongs.  Does nothing at this level.
        virtual void setContainer(IContainer *) { }

        /// Necessary for any thing that can be entered into the cell space
        /// This can be overridden to check that the conformal extent intersects an extent 
        bool  cellIntersects(const DExtent3 &) { return true; }
        bool  cellIntersects(const DExtent2 &) { return true; }
        bool  checkCellIntersects(const DExtent3 &,const DExtent3 &) { return true; }
        bool  checkCellIntersects(const DExtent2 &,const DExtent2 &) { return true; }

        /// The mapped state is set to FALSE upon restore, and TRUE at the end of Remap.
        /// If isMapped() returns false, member pointers of the object cannot be trusted to
        ///   point to valid locations.
        /// isMapped() should be checked during destruction of anything derived from Thing().
        static bool isMapped() { return mapped_; }
        /// Reset the mapped stated to \a b.  See isMapped().
        /// \param b Boolean to set the mapped state.
        static void setMapped(bool b) { mapped_ = b; }

        static Thing * lookupHandle(quint64 handle);
        static void    clearAllHandles();
        static quint64 getNumberOfHandles();
        static void    setLimitAndOffset(quint64 limit,quint64 offset);
        static quint64 adjustHandle(quint64 input);

    protected:
        struct Internal {
            virtual ~Internal() { }
            mutable quint64 handle_ = 0;    // Save/restore/remap handle;
        };
        Thing(Internal *t);
        Internal *d() { return internal_; }
        const Internal *d() const { return internal_; }

    private:
        static bool mapped_;
        Internal *internal_ = nullptr;
    };

    /** \brief Base class for items stored in containers with unique ids.
    *
    * This class is used to assign unique ids to Things.  Note that Body and Contact (among others)
    * are derived from ThingID.
    * \ingroup utilitysrc
    */

    class UTILITY_EXPORT ThingID : public Thing {
    public:
        /// Static constant \a type_ which is "thingid" by deafult.
        static const TType type_ = 0x4c815b68; 

        /// Constructor taking quint64 of the id (0 by default).
        /// \param id quint64 to identify this Thing.
        ThingID(quint64 id=0);
        /// Copy constructor that sets the id to 0 and copies \a t.
        /// \param t Const ThingID reference.
        ThingID(const ThingID &t);
        /// Default destructor.
        virtual ~ThingID() { }
        /// Assignment operator that uses the Thing assignment operator.
        /// \param t Const ThingID reference.
        const ThingID &operator=(const ThingID &t) { Thing::operator=(t);  return *this; }

        /// IThing override -- See IThing documentation
        virtual TType       getType() const { return type_; }
        virtual const void *convertToType(const TType &type) const { if (type==type_) return this;  return Thing::convertToType(type); }
        /// Return the quint64 identifying this Thing.  
        virtual quint64 getID() const { return d()->id_; }
        /// Archives the Thing to a file (if appropriate).
        virtual void     save(Archive2 &a) const;
        virtual bool     restore(Archive2 &a,quint64 label);
        virtual void     remap(Archive2 &a);
        /// Return the quint64 identifying this Thing.  This should be used in time sensitive operations.
        quint64 id() const { return d()->id_; }
        /// Set the id to \a i.  This should be used in time sensitive operations.
        /// \param i quint64 of the new id.
        void    id(quint64 i);

    protected:
        struct Internal : public Thing::Internal {
            quint64 id_ = 0;
        };
        ThingID(Internal *t,quint64 id=0) : Thing(t) { d()->id_ = id; }
        Internal *d() { return static_cast<Internal *>(Thing::d()); }
        const Internal *d() const { return static_cast<const Internal *>(Thing::d()); }

#ifdef _DEBUG
    private:
        quintptr id_ = 0;
#endif
    };
} // namespace itasca
// EoF
