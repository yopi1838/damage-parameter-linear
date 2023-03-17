#pragma once

#include "base/src/baseqt.h"
#include "module/interface/dim.h"

/**
    * \file
    * \brief Interface to a filter, used as the main method for filtering objects.
    */

namespace itasca {
    class Archive2;
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IRangeElement;

    /// \brief Interface to a filter, used as the main method for filtering objects.
    /** This is an interface to the main command processing class.
        * The idea is that you pass an IThing to a Range [isIn()], which decides whether it falls
        * inside or outside.\n
        * A Range is a container of Range Elements, and a given IThing must either fall within *all* of 
        * them (intersection, the default) or *any* of them (union, with setUnion()).\n
        * By default a Range is the logical intersection of all its filter elements.
        * If the union flag is set the filter is considered to be the logical union of its elements.
        * \ingroup module
    */
    class IRange {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b93;

        /// Returns a const IThing interface pointer for a filter.
        virtual const IThing *getIThing() const=0;
        /// Returns a IThing interface pointer for a filter.
        virtual IThing *      getIThing()=0;

        /// clear all elements, set to union, and remove name
        virtual void clear()=0;

        /// Copy a filter - including name
        virtual const IRange &operator=(const IRange &ir)=0;

        /// Creates a new instance of a filter - must be destroyed by the caller.
        virtual IRange *clone() const=0;

        /// This function will NOT automatically add this filter
        ///< to the list of named filters, and it does NOT do a recursion check.
        virtual QString getName() const=0;
        /// sets the filter name
        virtual void    setName(const QString &s)=0;

        /// Indicates if filter is a union of filter elements
        virtual bool isUnion() const=0;
        /// Sets the filter union flag
        /// \see isUnion()
        virtual void setUnion(bool b)=0;

        /// Returns the number of RangeElement making up the filter.
        virtual int getNumberOfElements() const=0;

        /// Returns the RangeElement at the given index, where index is less than getNumberOfElements().
        virtual IRangeElement *getElement(int index) const=0;

        /// Removes element at index from the filter, assumes index is less than getNumberOfElements().
        virtual void removeElement(int index)=0;

        /// Returns a StringList that can be merged() into a command that will completely
        ///<   re-create this filter.\n
        /// This does *not* include the starting keyword (i.e. "filter ...").
        virtual QStringList getCommandSwitches() const=0;

        /// Returns the next filter in the global list of all named filters,
        ///<   assuming this filter is a member.\n
        /// Returns 0 if at end of list, or not stored in the list.
        virtual const IRange *getNext() const=0;

        /// Checks to see if an infinitely recursive named filter topology has been created.
        virtual bool recursionCheck() const=0;

        /// Attempts to create a new RangeElement matching name, and adds it to the end of the
        ///<   filter if successful.\n
        /// Does nothing if name is not matched to a RangeElement type.
        virtual void addElement(const QString &name)=0;

        /// Checks if the the IThing is in the list.\n
        /// Uses t->getLocation() for the position of the object.
        virtual bool isIn(const IThing *t) const=0;

        /// Checks if the the IThing is in the list.\n
        /// Uses t->getLocation() for the position of the object.
        virtual bool isIn(const IThing &t) const=0;

        /// Checks if position pos is in the list.\n
        /// RangeElement checking for a specific type of object will always be ignored.
        virtual bool isIn(const DVect &pos) const=0;

        /// Checks for an object at location index1 (with index2 sometimes required depending on the index type).
        ///< type describes the index type being used (must have been registered ahead of time with the filter logic).\n
        /// This is used for FORTRAN objects in the A array, where an IThing pointer cannot be used.
        virtual bool isIn(qint64 index,const TType &type) const=0;

        /// Returns a pointer to a QObject representing this object.
        virtual QObject *getQObject()=0;

        virtual void save(Archive2 &a) const=0;
        virtual bool restore(Archive2 &a,quint64 label)=0;
        virtual void remap(Archive2 &a)=0;

        virtual void saveElement(Archive2 &a,int index) const=0;
        virtual bool restoreElement(Archive2 &a)=0;

        /// Returns true if a filter element is a FISH filter element
        virtual bool getIsFish() const=0;

        /// Returns true if the all range elements are thread safe meaning that the range
        /// can be used in a multithreaded loop. An optional IThing pointer can be provided.
        /// If given the IThing pointer is what the range is being applied to - the range elements
        /// check the type to ensure that the type isn't used in the range element filter. If it is then
        /// the range should not be used in a multithreaded loop. Really, in that case, the range shouldn't
        /// be used at all and should throw an error. 
        virtual bool isThreadSafe() const=0;

        /// Sets the Use Hidden state, which defaults to false.
        /// By default the filter will filter out all IThings with the hidden flag (t->getHidden()),
        ///    If this is set to true the filter will ignore the hidden flag.
        virtual void setUseHidden(bool b)=0;

        /// Returns the current hidden state.  See setUseHidden().
        virtual bool getUseHidden() const=0;
        
        /// Sets the Selected state, which defaults to false.
        /// By default the filter will ignore the selected state of an IThing.
        /// If this is set to true, any object with the selected flag set (t->getSelected()) will
        ///    be considered in a filter element (note union and intersect still apply to other filter elements),
        ///    and the hidden attribute will be ignored.
        virtual bool setSelected(bool b)=0;

        /// Return the current selected state.  See setSelected.
        virtual bool getSelected() const=0;

        virtual bool setDeselected(bool b)=0;
        virtual bool getDeselected() const=0;

        virtual bool isDefault() const=0;

        /// Destroys the filter.
        virtual void destroy()=0;

        // SIGNALS
        /// This signal is emitted when a filter has changed. See addElement(), removeElement(), setName(), setUnion().
        virtual void elementChanged(IRangeElement *elem,int index)=0;
        /// This signal is emitted when a filter has NOT changed.
        virtual void notChanged(IRangeElement *elem,bool notChanged)=0;
        /// This signal is emitted when a filter has BY changed.
        virtual void byChanged(IRangeElement *elem,const QString &)=0;
        /// This signal is emitted when a number of element in the global list filter has changed. See addElement(), removeElement()
        virtual void listChanged(int length)=0;
        /// This signal is emitted when a filter name has changed. See setName();
        virtual void nameChanged(const QString &name)=0;
        /// This signal is emitted when a filter union state has changed. See setUnion();
        virtual void unionChanged(bool b)=0;
    };
} // namespace itascaxd
// EoF
