#pragma once
// ISELLink.h

/**
  * \file
  * \brief Interface to provide access to structural element links.
  */

#include "isellinkmodel.h"

namespace itasca {
    class IThing;
}

namespace sel {
    using namespace itasca;
    class ISELNode;

    /// \brief Interface to provide access to structural element links.
    /** Structural elements can be joined to one another either by sharing a node or 
    having one of its nodes linked to either node or to a zone. If two or more 
    structural elements share a node, all forces and moments are transfered 
    between the structural elements at the node.
    *  \ingroup FLAC3D
    */
    class ISELLink {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bfe;
        /// Possible attachment conditions to rotational and translational directions
        enum AttachType { 
            AttachFree=0,   /*!< free       */ 
            AttachRigid,    /*!< rigid      */ 
            AttachDeform    /*!< deformable */ 
        };

        /// returns a const IThing pointer
        virtual const IThing *      getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *            getIThing()=0;
        /// Returns a const pointer to the next ISELLink object from the global list of structural elements links, 
        /// or 0 for the last element ofthe list. See ISELLinkList::getFirstSELLk()  
        virtual const ISELLink *    getNext() const=0;
        /// Returns a pointer to the next ISELLink object from the global list of structural elements links, 
        /// or 0 for the last element ofthe list. See ISELLinkList::getFirstSELLk()  
        virtual       ISELLink *    getNext() =0;
        /// Returns a const pointer to the node from which the link emanates. Each link originates from a node.
        virtual const ISELNode *    getSourceNode() const=0;
        /// Returns 1 if the link is a side 1 link; returns 2 if the link is a side 2 link.
        virtual quint32             getSide() const=0;
        /// Returns the id of the target link. For a node-to-node link, the target link is the node. For a 
        ///  node-to-zone link, the target is the zone. see getTypeID().
        virtual quint64            getTargetID() const=0;
        /// Returns the link type.
        virtual TType               getType() const=0;
        /// Return the structura element entity that is using this link.  
        ///  If not being used by a specific type, then return 0. See SEL::getType()
        virtual TType               getUsedBySELType() const=0;
        /// Given a \b dof in the range 0 to 5, returns the attachment condition. See getAttachConditionSymbol().
        virtual AttachType          getAttachCondition(quint32 dof) const =0;
        /// Given a \b dof in the range 0 to 5, returns a descriptive name for the link attachemnt condition. See getAttachCondition()\n
        /// Possible values are: - (SELLK_FREE), + (SELLK_RIGID), LIN (SELLK_DEFORM and SELCM::SELCM_LIN), 
        ///   SY (SELLK_DEFORM and SELCM::SELCM_SY), SY (SELLK_DEFORM and SELCM::SELCM_NY),
        ///   PY (SELLK_DEFORM and SELCM::SELCM_PY), PYDP (SELLK_DEFORM and SELCM::SELCM_PYDP)
        ///   ? (default)
        virtual QString             getAttachConditionSymbol(quint32 dof) const=0;
        /// Given a \b dof in the range 0 to 5, returns the link constitutive model. \n
        /// If the attachment condition is different from SELLK_DEFORM, function will return SELCM::SELCM_NONE.
        virtual TType               getConstitType(quint32 dof) const =0;
        /// Returns true if the large-strain sliding flag is on. This function only 
        ///   applies to node-to-zone links and has no effect on node-to-node links. See setSlide()
        virtual bool                getSlide() const =0;
        /// Sets he large-strain sliding flag is on. This function only 
        ///   applies to node-to-zone links and has no effect on node-to-node links. See getSlide()
        virtual void                setSlide(bool)=0;
        /// Returns the large-strain sliding tolerance. This function only applies to node-to-zone links 
        ///   and has no effect on node-to-node links. See setSlideTol()
        virtual double              getSlideTol() const =0;
        /// Sets the large-strain sliding tolerance. This function only applies to node-to-zone links 
        ///   and has no effect on node-to-node links. gee setSlideTol()
        virtual void                setSlideTol(const double&)=0;
        /// Given a \b dof in the range 0 to 5, returns a pointer to the link constitutive model. 
        virtual ISELLinkModel *     getConstitModel(quint32 dof) const =0;  
        /// Returns a pointer to the target for this link.
        virtual const IThing *      getTarget() const=0;
        /// Returns the keyword used to identify this type of link on the command line.
        virtual QString             getTargetKeyword() const=0;
        /// Returns a name indicating the type of object the link connects to.
        virtual QString             getTargetTypeName() const = 0;
        /// Returns the generic type indicator of the type the object connects to.
        virtual TType               getTargetType() const = 0;
        virtual DVect               getTargetPosition() const = 0;
        ///// Returns an object of that type given an ID number, or nullptr if no object of that type exists.
        //virtual IThing *            findTargetWithID(quint64 id) const=0;
        ///// Search for an object at pos that would produce a valid link, returns nullptr if no such object exists.
        //virtual IThing *            findTargetFromPosition(const DVect &pos,const ISELNode *node,uint side) const=0;
        /// Sets the current target for this link to thing.  
        /// Throws an error if thing is of the wrong type or if the link would be invalid.
        virtual void                setTarget(IThing *thing)=0;
        virtual double              getTargetPorePressure() const=0;
        virtual std::vector<std::pair<QString,QVariant>> getTypeEntryData() const=0;
        /// Gets the dowel node locations and puts in the vector intersections.
        /// These will always be pairs (two sides of a joint)
        /// Input beginning (p1) and end (p2) of host segment and characteristic distance (element length)
        /// Return false if no joint crossings found
        virtual bool findDowelPosFromPos(const DVect& p1, const DVect& p2, double charDist, std::vector< std::pair<DVect, DVect> >* intersections) const = 0;
    };
} // namespace sel
// EoF
