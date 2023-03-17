//ISELLinklist.h
#pragma once

/**
  * \file
  * \brief Interface to the global list of all structural elements links.
  */

namespace itasca {
    class IContainer;
    class IThing;
}

namespace sel {
    using namespace itasca;

    class ISELLink;

    /// \brief Interface to the global list of all structural elements links.
    /// \ingroup SEL
    class ISELLinkList {
    public:
        /// returns a const IThing pointer
        virtual const IThing *getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *getIThing()=0;
        /// Returns a const pointer to the IContainer interface representing the global list of structural elements links.
        virtual const IContainer *getIContainer() const=0;
        /// Returns a pointer to the IContainer interface representing the global list of structural elements links.
        virtual IContainer *      getIContainer() =0;
        /// Return a const pointer to the first structural element link stored.\n
        /// ISELLink::getNext() can be used to iterate on the list.
        virtual const ISELLink *    getFirstSELLink() const=0;
        /// Return a pointer to the first structural element link stored.\n
        /// ISELLink::getNext() can be used to iterate on the list.
        virtual ISELLink *          getFirstSELLink()=0;
        /// Given a strucutal elemnent link \b id, returns a const pointer to the SEL link.
        virtual const ISELLink *    findSELLinkWithID(quint64 id) const=0;

        /// Returns the general SEL link type
        virtual quint32           getSELLinkType() const=0;
    };
} // namespace sel
// EoF
