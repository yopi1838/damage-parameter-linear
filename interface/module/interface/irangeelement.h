#pragma once

#include "base/src/baseqt.h"
#include "dim.h"
/**
    * \file
    * \brief Interface to a filter element, which is a member of a IRange.
    */

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a filter element, which is a member of a IRange..
    class IRangeElement {
    public:
        /// Enumeration specifying the category of the filter element, for UI organization.
        enum Category { GeometricCategory,     ///< Filtering is based on geometry.
                        PropertyBasedCategory  ///< Filtering is based on object meta-data.
                    };

        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x04c815b9c;

        /// Returns a string uniquely describing the type of filter element it is.
        /// The pointer value of the return string can be used for fast type comparisons.
        virtual TType getType() const=0; 

        /// Returns a short String describing the type.  Like "Cylinder" or "X Position".
        virtual QString getName() const=0;

        /// Returns the category of the 
        virtual Category getCategory() const=0;

        /// This class should return a new object of the derived type.
        virtual IRangeElement *clone() const=0;

        /// Copies the entries in the RangeElement.\n
        /// ire is going to have to be "upcasted" to the derived type.
        /// This function has an ASSERT() that the type is the same, so call this first.
        virtual void copy(const IRangeElement *ire)=0;

        /// If NOT is specified, it reverses the selection criteria for the RangeElement.\n
        /// This is handled OUTSIDE the isIn function - so RangeElement implementations can ignore it.
        virtual bool isNot() const=0;
        /// Sets the NOT flag.
        /// \see isNot()
        virtual void setNot(bool b)=0;

        /// If by has been specified then the range is applied to the entitry help in the QString through
        /// the getAssociatedThings function in the IThing interface.\n
        /// This is handled OUTSIDE the isIn function - so RangeElement implementations can ignore it.
        virtual bool isBy() const=0;
        /// Sets the BY string.
        /// \see isBy()
        virtual void setBy(const QString &)=0;
        /// Get the BY string.
        /// \see isBy()
        virtual const QString & getByString() const=0;
        /// Get the BY TType.
        /// \see isBy()
        virtual TType getByTType() const=0;

        /// Used to specify that this is a FISH filter element so that one can be sure that the gui does 
        /// not have access to this filter element
        virtual bool getIsFish() const=0;

        /// Returns true if the range element is thread safe meaning that it can 
        /// be used in a multithreaded loop. An optional IThing pointer can be provided.
        /// If given the IThing pointer is what the range element is being applied to - the range element
        /// checks the type to ensure that the type isn't used in the range element filter. If it is then
        /// the range element should not be used in a multithreaded loop. Really, in that case, the range element shouldn't
        /// be used at all and should throw an error. 
        virtual bool isThreadSafe() const=0;

        /// If EXTENT is specified, the selection of a object will be based on its extent, otherwise
        /// the object centroid is used.\n
        virtual bool isExtent() const=0;
        /// Sets the EXTENT flag.
        /// \see isExtent()
        virtual void setExtent(bool b)=0;

        /// Returns FALSE if this type of filter element is not valid in the current state.
        virtual bool isValid() const=0;

        /// Returns FALSE if this type of filter element is not a geometric element, for example
        /// filter element type ID is not geometric, but filter element type X is geometric.
        virtual bool isGeometric() const=0;

        virtual QStringList getAllByKeywords() const=0;

        /// Returns a pointer to a QObject representing this object.
        virtual QObject *getQObject()=0;

        /// Destroys the filter element.
        virtual void destroy()=0;

        // for compatibility with the Model pane
        virtual bool isModelPaneCompatible() { return false; } // should this element be used by the Model pane?
        virtual void setDefaultsForModelPane() { }

        // SIGNALS
        /// This signal is emitted when the NOT attribute is changed.
        virtual void notChanged(bool b)=0;
        /// This signal is emitted when the BY attribute is changed.
        virtual void byChanged(const QString &)=0;
        /// This signal should be emitted when any attribute of this filter element is changed, in any derived class.
        virtual void changed()=0;
    };

} // namespace itascaxd
// EoF
