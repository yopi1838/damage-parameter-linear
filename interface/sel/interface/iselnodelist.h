//iselnodelist.h
#pragma once

/**
  * \file
  * \brief Interface to the global list of all structural elements nodes.
  */

namespace itasca {
    class IContainer;
    class IThing;
}

namespace sel {
    using namespace itasca;

    class ISELNode;

    /// \brief Interface to the global list of all structural elements nodes.
    /// \ingroup SEL
    class ISELNodeList {
    public:
        /// returns a const IThing pointer
        virtual const IThing *getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *getIThing()=0;
        /// Returns a const pointer to the IContainer interface representing the global list of structural elements nodes.
        virtual const IContainer * getIContainer() const=0;
        /// Returns a pointer to the IContainer interface representing the global list of structural elements nodes.
        virtual IContainer *       getIContainer() =0;
        /// Return a const pointer to the first structural element node stored.\n
        /// ISELNode::getNext() can be used to iterate on the list.
        virtual const ISELNode *     getFirstSELNode() const=0;
        /// Return a pointer to the first structural element node stored.\n
        /// ISELNode::getNext() can be used to iterate on the list.
        virtual ISELNode *           getFirstSELNode()=0;
        /// Given a strucutal elemnent node \b id, returns a const pointer to the SEL node.
        virtual const ISELNode *     findSelNodeWithID(quint64 id) const=0;
        /// Given a strucutal elemnent node \b id, returns a pointer to the SEL node.
        virtual ISELNode *           findSelNodeWithID(quint64 id) =0;
        /// Returns a const pointer to the structural element node closest to \b position. 
        ///   Returns 0 if the element is not found.
        virtual const ISELNode *     findSelNode(const DVect &position,double radius=limits<double>::max()) const=0;
        /// Returns a const pointer to the structural element node closest to \b position. 
        ///   Returns 0 if the element is not found.
        virtual ISELNode *           findSelNode(const DVect &position,double radius=limits<double>::max()) =0;
        /// Returns the general SEL node type
        virtual quint32            getSELNodeType() const=0;
        /// Returns the tag for the structural element node applied forces change notice
        virtual uint               getFapNotice() const=0;
        /// Returns the tag for the structural element node out of balance forces change notice
        virtual uint               getFobNotice() const=0;
        /// Returns the tag for the structural element node geomentry change notice
        virtual uint               getGeometryNotice() const=0;
        /// Returns the tag for the structural element node fixity change notice
        virtual uint               getFixityNotice() const =0;
        /// Returns the tag for the structural element node velocity change notice
        virtual uint               getVelocityNotice() const=0;
        /// Returns the tag for the structural element node displacement change notice
        virtual uint               getDisplacementNotice() const=0;
        /// Returns the tag for the structural element node link state change notice
        virtual uint               getLinkStateNotice() const=0;
        /// Returns the tag for the structural element node fish extra variable change notice
        virtual uint               getFISHNotice() const=0;
        /// Returns the tag for the structural element group change notice
        virtual uint               getGroupNotice() const=0;
        virtual uint               getRatioTargetNotice() const=0;
    };
} // namespace itasca
// EoF

