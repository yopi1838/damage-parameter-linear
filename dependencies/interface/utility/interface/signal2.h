#pragma once
/** \file signal2.h
* \brief Definition of a Signal2 object, holding a list of ISlot2 objects.
*/

#include "isignalbase.h"

namespace itasca {
    /** \brief Definition of a Signal2 object, holding a list of ISlot2 objects.

    * This is part of a custom, small-granularity callback utility whose purpose is to mimic the Qt
    * signal/slot paradigm without the overhead.  A Signal2 object holds a list of ISlot2 objects.
    * Signal2 is derived from ISlotBase so that a Signal2 can be chained or attached to another
    * Signal2 (see attachSignal() and removeSignal()).  The attached methods are invoked with
    * execute().
    * Internally Slots are created via the AutoSlot0, AutoSlot1 and AutoSlot2 template classes.
    * These classes correspond to methods taking 0, 1 or 2 arguments.
    * Thus it is possible to attach a method taking either 0, 1 or 2 arguments to a Signal2 object.
    * This is accomplished by the attachMethod0(), attachMethod1() and attachMethod2() functions,
    * respectively.  There are matching removeMethod0(), removeMethod1() and removeMethod2()
    * functions as well.  The attach() and remove() methods require ISlot2 arguments.
    * The syntax for attaching a method inside the class where the method is defined
    * (Note: the class must be derived from Slot) is
    *   \code signal2->attachMethod2<Fred,&Fred::func>(this); \endcode
    * where \a signal2 is the Signal2 object and \a Fred is the class (derived from Slot)
    * where the member function \a func resides.
    * Also the attach method can be used:
    *   \code signal2->attach<Fred,&Fred::func>(&fred); \endcode
    * where \a fred is a particular instance of the class Fred.
    * The remove sytax is similar: \code signal2->remove(&fred,&Fred::func); \endcode
    * \ingroup utilitysrc
    */
    template <class Param1,class Param2> class Signal2 : public ISignalBase {
    public:

        /// Define SlotType as an ISlot2 object with template arguments \a Param1 and \a Param2
        typedef ISlot2<Param1,Param2> SlotType;

        /// Default constructor - no values are set.
        Signal2() { }
        Signal2(const Signal2 &) = delete;
        Signal2(Signal2 &&) = delete;
        void operator=(const Signal2 &) = delete;
        void operator=(Signal2 &&) = delete;


        /// Virtual destructor.  Upon destruction, all Signal2 objects and SlotType objects attached to
        /// this Signal2 are removed.
        virtual ~Signal2();

        /// Invokes execute() on all attached Signal2 and SlotType objects.  Depending on the method, either
        /// 0, 1 or 2 arguments are passed to the method.
        /// \param v1 First parameter conveyed to the attached method.
        /// \param v2 Second parameter conveyed to the attached method.
        virtual void execute(Param1,Param2);

        /// Attach the SlotType object to this Signal2 object.  SlotType is now owned by this Signal2 object,
        /// and will be deleted when the Signal2 object is destroyed.  The SlotType object is identified by a
        /// QString that can be used for removal.
        /// \param slot Pointer to a SlotType object to be attached.
        /// \param name Reference to a QString used to identify the SlotType for removal.
        virtual void attach(SlotType *,const QString &);

        /// Remove the SlotType \a slot from this Signal2 object.  The defualt null QString \a name is used for
        /// removal.  If \a name is null then all SlotType objects with matching base are removed.  If \a slot is
        /// null then all SlotType objects with matching base are removed.  If both are null then all SlotTypes
        /// are removed.
        /// \param slot Pointer to a SlotType object to be removed.
        /// \param name Default null QString name of the SlotType to remove.
        bool remove(ISlotBase *slot,const QString &name=QString());

        /// Used to signal that an ISignalBase object is to be attached to this Signal2.
        /// \param signal ISignalBase pointer that is attached.
        virtual void attachNotice(ISignalBase *signal);
        /// Used to signal that an ISignalBase object is to be removed from this Signal2.
        /// \param signal ISignalBase pointer that is removed.
        virtual void removeNotice(ISignalBase *signal);

        // Creates a SlotType implementation that calls the method MFP on instance t of class T, using two arguments.
        // T must be derived from ISlotBase.
        /// Uses the AutoSlot2 template class to attach the method \a MFP of the instance \a t of class \a T
        /// which takes two arguments.
        /// Warning - attempting to do this to a function that is overloaded will cause a compiler error in VS2005.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)(Param1,Param2)>
        void attachMethod2(T *t) {
            AutoSlot2<T,MFP> *s = NEW("AutoSlot") AutoSlot2<T,MFP>(t);
            attach(s,typeid(*s).name());
        }

        // Creates a SlotType implementation that calls the method MFP on instance t of class T, using one argument.
        // T must be derived from ISlotBase.
        /// Uses the AutoSlot1 template class to attach the method \a MFP of the instance \a t of class \a T
        /// which takes one argument.
        /// Warning - attempting to do this to a function that is overloaded will cause a compiler error in VS2005.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)(Param1)>
        void attachMethod1(T *t) {
            AutoSlot1<T,MFP> *s = NEW("AutoSlot") AutoSlot1<T,MFP>(t);
            attach(s,typeid(*s).name());
        }

        // Creates a SlotType implementation that calls the method MFP on instance t of class T, using no arguments.
        // T must be derived from ISlotBase.
        /// Uses the AutoSlot0 template class to attach the method \a MFP of the instance \a t of class \a T
        /// which takes no arguments.
        /// Warning - attempting to do this to a function that is overloaded will cause a compiler error in VS2005.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)()>
        void attachMethod0(T *t) {
            AutoSlot0<T,MFP> *s = NEW("AutoSlot") AutoSlot0<T,MFP>(t);
            attach(s,typeid(*s).name());
        }

        // Removes a slot connection created to method MFP on instance to of class T, using two arguments.
        /// Removes the method \a MFP of the instance \a t of class \a T taking 2 arguments by creating a
        /// temporary AutoSlot2 object and removing it.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)(Param1,Param2)>
        void removeMethod2(T *t) {
            AutoSlot2<T,MFP> s = AutoSlot2<T,MFP>(t);
            remove(t,typeid(s).name());
        }

        // Removes a slot connection created to method MFP on instance to of class T, using one argument.
        /// Removes the method \a MFP of the instance \a t of class \a T taking 1 argument by creating a
        /// temporary AutoSlot1 object and removing it.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)(Param1)>
        void removeMethod1(T *t) {
            AutoSlot1<T,MFP> s = AutoSlot1<T,MFP>(t);
            remove(t,typeid(s).name());
        }

        // Removes a slot connection created to method MFP on instance to of class T, using no arguments.
        /// Removes the method \a MFP of the instance \a t of class \a T taking no argument by creating a
        /// temporary AutoSlot0 object and removing it.
        /// \param t Pointer to the instance of \a T that has the method \a MFP.
        template <class T,void (T::*MFP)()>
        void removeMethod0(T *t) {
            AutoSlot0<T,MFP> s = AutoSlot0<T,MFP>(t);
            remove(t,typeid(s).name());
        }

        // Attaches a signal (same two arguments) to this signal, as a slot, causing the signals to chain.
        /// The Signal2 \a sig with the same two arguments is attached to this Signal2 object, resulting
        /// in chained signals (e.g., when the methods in this Signal2 object are executed, the methods in all
        /// attached Signal2 objects are also executed with the same arguments).
        /// \param sig Signal2 to be attached to this Signal2 object.
        void attachSignal(Signal2<Param1,Param2> *sig) {
            attachMethod2<Signal2<Param1,Param2>,&Signal2<Param1,Param2>::execute>(sig);
        }

        // Removes a signal previously attached
        /// The Signal2 \a sig that has previously been attached is removed from this Signal2 object.
        /// \param sig Signal2 to be removed from this Signal2 object.
        void removeSignal(Signal2<Param1,Param2> *sig) {
            removeMethod2<Signal2<Param1,Param2>,&Signal2<Param1,Param2>::execute>(sig);
        }

    private:
        typedef QMultiMap<QString,SlotType *> SlotMap;
        QList<ISignalBase *> signals_;
        SlotMap              map_;

        // Derived from SlotType - used when a method is attached via attachMethod2().
        template <class T,void (T::*MFP)(Param1,Param2)> class AutoSlot2 : public SlotType {
        public:
            AutoSlot2(T *t) : t_(t) { assert(t_); }
            virtual void execute(Param1 v1,Param2 v2) {
                assert(t_);
                (t_->*MFP)(v1,v2);
            }
            virtual ISlotBase *getBase() { return t_; }
        private:
            T *t_;
        };

        // Derived from SlotType - used when a method is attached via attachMethod1().
        template <class T,void (T::*MFP)(Param1)> class AutoSlot1 : public SlotType {
        public:
            AutoSlot1(T *t) : t_(t) { assert(t_); }
            virtual void execute(Param1 v1,Param2) {
                assert(t_);
                (t_->*MFP)(v1);
            }
            virtual ISlotBase *getBase() { return t_; }
        private:
            T *t_;
        };

        // Derived from SlotType - used when a method is attached via attachMethod0().
        template <class T,void (T::*MFP)()> class AutoSlot0 : public SlotType {
        public:
            AutoSlot0(T *t) : t_(t) { assert(t_); }
            virtual void execute(Param1,Param2) {
                assert(t_);
                (t_->*MFP)();
            }
            virtual ISlotBase *getBase() { return t_; }
        private:
            T *t_;
        };
    };

    template <class Param1,class Param2>
    Signal2<Param1,Param2>::~Signal2() {
        while (signals_.size()) {
            ISignalBase *base = signals_.last();
            base->remove(this);
        }
        while (map_.size()) {
            typename SlotMap::iterator it = map_.begin();
            SlotType *slot = it.value();
            map_.erase(it);
            slot->getBase()->removeNotice(this);
            delete slot;
        }
    }

    template <class Param1,class Param2>
    void Signal2<Param1,Param2>::execute(Param1 v1,Param2 v2) {
        for (typename SlotMap::iterator it=map_.begin();it!=map_.end();++it)
            it.value()->execute(v1,v2);
    }

    template <class Param1,class Param2>
    void Signal2<Param1,Param2>::attach(SlotType *slot,const QString &name) {
        assert(slot);
        map_.insert(name,slot);
        slot->getBase()->attachNotice(this);
    }

    template <class Param1,class Param2>
    bool Signal2<Param1,Param2>::remove(ISlotBase *slot,const QString &name) {
        if (slot && name.length()) { // First first entry
            for (typename SlotMap::iterator it=map_.find(name);it!=map_.end() && it.key()==name;++it) {
                SlotType *st = it.value();
                if (st->getBase()==slot) {
                    map_.erase(it);
                    st->getBase()->removeNotice(this);
                    delete st;
                    return true;
                }
            }
            return false;
        }
        if (slot) { // Remove all entries matching that slot base
            QList<QPair<QString,SlotType *> > toRemove;
            for (typename SlotMap::iterator it=map_.begin();it!=map_.end();++it) {
                if (it.value()->getBase()==slot)
                    toRemove.push_back(QPair<QString,SlotType *>(it.key(),it.value()));
            }
            for (int i=0;i<toRemove.size();++i) {
                QPair<QString,SlotType *> &p = toRemove[i];
                map_.remove(p.first,p.second);
                p.second->getBase()->removeNotice(this);
                delete p.second;
            }
            return toRemove.size() ? true : false;
        }
        if (name.length()) { // Remove all slots with matching name
            QList<QPair<QString,SlotType *> > toRemove;
            for (typename SlotMap::iterator it=map_.find(name);it!=map_.end() && it.key()==name;++it)
                toRemove.push_back(QPair<QString,SlotType *>(it.key(),it.value()));
            for (int i=0;i<toRemove.size();++i) {
                QPair<QString,SlotType *> &p = toRemove[i];
                map_.remove(p.first,p.second);
                p.second->getBase()->removeNotice(this);
                delete p.second;
            }
            return toRemove.size() ? true : false;
        }
        // Remove all slots
        while (map_.size()) {
            typename SlotMap::iterator it = map_.begin();
            SlotType *slotType = it.value();
            map_.erase(it);
            slotType->getBase()->removeNotice(this);
            delete slotType;
        }
        return true;
    }

    template <class Param1,class Param2>
    void Signal2<Param1,Param2>::attachNotice(ISignalBase *signal) {
        signals_.push_back(signal);
    }

    template <class Param1,class Param2>
    void Signal2<Param1,Param2>::removeNotice(ISignalBase *signal) {
        int i = signals_.indexOf(signal);
        if (i<0) return;
        signals_.removeAt(i);
    }
} // namespace Itasca
// Eof
