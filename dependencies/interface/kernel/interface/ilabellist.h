#pragma once

/**
    * \file
    * \brief Interface to the list of all labels in the system
    */

#include "module/interface/dim.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;

    class ILabel;

    /// \brief Interface to the list of all labels in the system
    /// \ingroup kernel
    class ILabelList {
    public:
        /// returns a constant pointer to the IContainer class representing a list of labels.
        virtual const IContainer *getIContainer() const=0;
        /// returns a pointer to the IContainer class representing a list of labels.
        virtual IContainer *      getIContainer()=0;

        /// Returns a const pointer to a ITable() with id given, or 0 if no match is found.
        virtual const ILabel *lookupWithName(QString name) const=0;
        /// Returns a pointer to a ITable() with id given, or 0 if no match is found.
        virtual ILabel *      lookupWithName(QString name)=0;

        /// Returns a const pointer to the first ILabel in the list. Use ILabel::getNext() to iterate through the entire list.
        virtual const ILabel *getFirst() const=0;
        /// Returns a pointer to the first ILabel in the list. Use ILabel::getNext() to iterate through the entire list.
        virtual       ILabel *getFirst()=0;

        /// Returns the ID assigned by the NoticeManager for changes to the list of labels.
        /// Indicates when labels are added or removed from the list.
        virtual UInt getNotice() const=0;

        /// Returns the general type of an ILabel.
        virtual quint32 getLabelGeneralType() const=0;
        /// Look for a label with a given id. Returns the interface pointer is the label exists in the list. If
        /// the label does not exists, creates the label and adds it to the list.
        virtual ILabel *findOrCreateWithName(QString name)=0;
        /// Creates the label and adds it to the label list. Sets the position of the label.
        virtual ILabel *create(const DVect &pos,QString name=QString())=0;

        ///Return the type of a label
        virtual TType getType() const=0;
    }; 
} // namespace itascaxd
// EoF