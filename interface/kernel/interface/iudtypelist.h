#pragma once
// iudtypelist.h

/**
 * \file
 * \brief IUDTypeList is a template interface, for a container of IUDType objects.
 */
#include "utility/interface/igroup.h"
#include "module/interface/irange.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a container of user-define data value types.
    /** Enables looking up by ID and by location, also creating new objects of IUDType.
        * The template class type T should be the interface to the class contained in the list.
        * For example a collection of scalars would be defined as IUDTypeList<IUDScalar>
        * The class T is assumed to provide the same basic functionality as an IUDType<>.
        * \ingroup kernel
        */
    template <class T> class IUDTypeList {
    public:
        /// The type T that this list contains.
        typedef T ivalue_type;

        /// returns a IThing pointer
        virtual IThing *getIThing()=0;
        /// returns a const IThing pointer
        virtual const IThing *getIThing() const=0;

        /// Returns a pointer to the container class representing a list of user defined objects.
        virtual IContainer *getIContainer()=0;
        /// Returns a pointer to the container class representing a list of user defined objects.
        virtual const IContainer *getIContainer() const=0;

        /// Returns a const pointer to the first T in the list.  Use T::getNext() to iterate through the entire list.
        virtual const T *getFirst() const=0;

        /// Returns a pointer to the first T in the list.  Use T::getNext() to iterate through the entire list.
        virtual T *      getFirst()=0;

        /// Find a const pointer to the T object with ID \b ul, or 0 if no match is found.
        virtual const T *lookupWithID(quint64 ul) const=0;

        /// Find a pointer to the T object with ID \b ul, or 0 if no match is found.
        virtual T *      lookupWithID(quint64 ul)=0;

        /// Creates a new T with default values.
        virtual T *create()=0;

        /// Returns the NoticeManager Code for extra variables changing.
        virtual uint getExtraNotice() const=0;
        /// Returns the NoticeManager Code for group assignments changing.
        virtual uint getGroupNotice() const=0;
        /// Returns the NoticeManager Code for position of object changing.
        virtual uint getPositionNotice() const=0;
        /// Returns the NoticeManager Code for object values changing.
        virtual uint getValueNotice() const=0;
        /// Returns the NoticeManager Code for hide status changing.
        virtual uint getHideNotice() const=0;
        /// Returns the NoticeManager Code for select status changing.
        virtual uint getSelectNotice() const=0;

        /// Returns the type indicator of the objects it contains.
        virtual TType getType() const=0;
        /// Returns a lable indicating the type of object it contains.
        virtual QString getLabel() const=0;

        /// Returns the object whose position is closest to /a pos.
        virtual T *      findNear(const DVect &pos,const double &rad=limits<double>::max())=0;
        /// Returns the object whose position is closest to /a pos.
        virtual const T *findNear(const DVect &pos,const double &rad=limits<double>::max()) const=0;
        /// Removes the object /a pnt, which is assumed to be in the container.  /a pnt is destroyed.
        virtual void     erase(T *pnt)=0;
        /// Imports objects of type T from the file /a filename.  All objects created are assigned a group of /a group in slot /a slot (if specified).
        virtual bool     importFromFile(const QString &filename,const IGroupID &group)=0;
        /// Exports all objects to the file /a filename.   /a binary indicates binary format, text is used otherwise.
        /// /a filter (if given) is used to filter which objects are exported.
        /// /a comment is place at the start of text output.
        virtual bool     exportToFile(const QString &filename,bool binary,const IRange *filter=0,const QString &comment=QString())=0;
    };
} // namespace itascaxd
/// EoF
