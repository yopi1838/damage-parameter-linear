#pragma once

#include "module/interface/dim.h"
#include "iparse.h"

/**
    * \file
    * \brief Interface to the global list of all named filters.
    */

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;
     
    class IRange;
    class IRangeElement;

    /// \brief Interface to the global list of all named filters.
    /// \ingroup module
    class IRangeList {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b97;

        /// Return a pointer to the first named Range stored.\n
        /// IRange::getNext() can be used to iterate.
        virtual const IRange *getFirst() const=0;

        /// Attempts to find a named filter matching name.  Returns 0 if not found.\n
        /// CaseSensitivity is determined by the Settings object.
        virtual const IRange *findFilterWithName(const QString &name) const=0;
        virtual IRange *      findFilterWithName(const QString &name)=0; ///< \copydoc findFilterWithName

        /// Creates a new Range object (but does not add it to the list).\n
        /// If prev#0 a copy of prev is created, otherwise a new empty filter is created.
        virtual IRange *createFilter(const IRange *prev)=0;

        /// Attemps to parse the tokens starting at p into a filter.\n
        /// Does not check for a RANGE keyword first - assumes one has already been found.\n
        /// Will not throw error if tokens left on list that are not parsed.\n
        /// Checks tokens against keywords, if find a match then ends,
        ///<   you can use this to protect against filter name conflicts in subsequent keywords.\n
        /// If this filter is given a name a *copy* of it will 
        ///<   automatically be added to the named filter list.\n
        /// rng will be cleared (and name removed) at start.
        virtual void parse(IRange *rng,const IParse &par,const QString &keywords=QString(),IParse::IMark *p=0,bool includeFish=true)=0;

        /// Returns the IContainer interface const pointer representing the list of filters.
        virtual IContainer *getIContainer()=0;
        /// Returns the IContainer interface pointer representing the list of filters.
        virtual const IContainer *getIContainer() const=0;

        /// Returns a list of all named filters (not sorted).
        virtual QStringList getAllNames(bool includeFish=true) const=0;

        /// Adds a *copy* of rng to the named filter list\n
        /// NOTE: may throw exception if recursion is detected.
        virtual IRange *    add(const IRange &rng)=0;
    };
} // namespace itascaxd

// EoF
