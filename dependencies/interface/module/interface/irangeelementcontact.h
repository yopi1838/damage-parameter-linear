#pragma once

#include "irangeelement.h"

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a RangeElement of contacts specifying activity, type, model name, etc. 
    class IRangeElementContact {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x50a65886;
        /// Returns a pointer to itself.
        virtual IRangeElement *getIRangeElement()=0;

        /// State of the contact
        enum ActivityState { All=1, Active, Inactive};
     
        /// Returns the list of contact model names associated with this filter element.
        virtual QSet<QString> getModelNames() const=0;
        /// Sets the list of contact model names.
        virtual void setModelNames(const QSet<QString> &ns)=0;
        /// Adds a model name to the filter.
        virtual void addModelName(const QString &n)=0;
        /// Removes a model name from this filter.
        virtual void removeModelName(const QString &n)=0; 
        /// Returns the list of contact type names associated with this filter element.
        virtual QSet<QString> getTypeNames() const=0;
        /// Sets the list of contact type names.
        virtual void setTypeNames(const QSet<QString> &ns)=0;
        /// Adds a type name to the filter.
        virtual void addTypeName(const QString &n)=0;
        /// Removes a type name from this filter.
        virtual void removeTypeName(const QString &n)=0;   
        /// Returns the list of contact process names associated with this filter element.
        virtual QSet<QString> getProcessNames() const=0;
        /// Sets the list of contact process names.
        virtual void setProcessNames(const QSet<QString> &ns)=0;    
        /// Adds a process name to the filter.
        virtual void addProcessName(const QString &n)=0;
        /// Removes a process name from this filter.    
        virtual void removeProcessName(const QString &n)=0; 
        /// Set the activity state
        virtual void setActivityState(const ActivityState &)=0;
        /// Get the activity state
        virtual const ActivityState & getActivityState() const=0;


    signals:
        /// Signal emitted when the contact model list is changed.
        virtual void modelNamesChanged(const QSet<QString> &ns)=0;
        /// Signal emitted when a contact model name is added.
        virtual void modelNameAdded(const QString &n)=0;
        /// Signal emitted when a contact model name is removed.
        virtual void modelNameRemoved(const QString &n)=0;
        /// Signal emitted when the activity state has changed.
        virtual void activityStateChanged(const ActivityState &)=0;
    };

} // namespace itascaxd
// EoF