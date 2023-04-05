#pragma once
/**
  * \file
  * \brief Interface to the structural elements base class
  */

#include "module/interface/ilinktype.h"

namespace itasca {
    class IFishParam;
    class IGroup;
    class IThing;
}

namespace sel {
    using namespace itasca;

    class ISELNode;

    /// \brief Interface to the structural elements base class
    /// \ingroup FLAC3D
    class ISEL {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a02;
        /// Yield states
        enum YieldState {
            NO,  ///< no yield (has never yielded)
            NOW, ///< now yielding
            PAST ///< has yielded in the past 
        };

        virtual TType               getType() const=0;
        /// Returns a const IThing pointer
        virtual const IThing *      getIThing() const=0;
        /// Returns a IThing pointer
        virtual IThing *            getIThing()=0;
        /// Returns a const pointer to the next ISEL object from the global list of structural elements, 
        /// Returns the keyword use dto identify the type of SEL element on the command line
        virtual QString             getKeyword() const=0;
        /// or 0 for the last element ofthe list. See ISELList::getFirstSEL()
        virtual const ISEL *        getNext() const=0;
        /// Returns the number of sel-nodes comprising this structural element
        virtual quint32             getNumNd() const =0;
        /// Returns a const pointer to a sel-node \b located at loc_ndnum in the sel-node array  <b> (Zero-based) </b>
        virtual const ISELNode *    getNdPtr(quint32 loc_ndnum) const =0;
        /// Returns a pointer to a sel-node \b located at loc_ndnum in the sel node-array  <b> (Zero-based) </b>
        virtual ISELNode *          getNdPtr(quint32 loc_ndnum) =0;
        /// Returns true if structural element is 'marked', see setMark()
        virtual bool                getMark() const=0;
        /// Sets the structural element Mark flag, see getMark()
        virtual void                setMark(bool b) =0;
        /// Returns the structural element density, see setDensity()
        virtual double              getDensity() const =0;
        /// Sets the structural element density, see getDensity()
        virtual void                setDensity(double d) =0;
        /// Returns the axes of the structural element local coordinate system
        virtual Axes3D              getLocalSys() const =0;
        /// Returns the local axes system used to represent material properties.
        virtual Axes3D              getMaterialSys() const=0;
        /// Returns the structural element thermal expansion coefficient, see setThExp()
        virtual double              getThExp() const =0;
        /// Sets the structural element thermal expansion coefficient, see getThExp()
        virtual void                setThExp(double d) =0;
        /// Returns the name of the state for this structural element. Possible values are <b> no, now, past </b>
        virtual QString             getYieldName(YieldState state) const=0;
        /// Returns the one of the components of the force vector exerted in a given node on the structural element\n
        /// Valid values for \b dof are in the range 0 to 5 -  e.g. 0 = translational x, 3 = rotational x\n
        /// Valid values for \b end are  in the range 0 to getNumNd()-1 \n
        /// \b local set to true will return the nodal force in the local coordinate system, otherwise it returns the force in the global coordinate system
        virtual double              getForceAtNode2(quint32 dof,quint32 end,bool local=false) const=0;
        /// Returns the force exerted by a given node on the structural element\n
        /// Valid values for \b end are  in the range 0 to getNumNd()-1 \n
        /// \b trans should be set to true to obtain the translation vector and false for the rotation vector. \n
        /// \b local set to true will return the nodal force in the local coordinate system, otherwise it returns the force in the global coordinate system
        virtual DVect               getForceAtNode2(quint32 end,bool trans,bool local=false) const=0;
        /// Returns the average gravitational load actuating in one node: density * volume / number of nodes.
        virtual double              getGravityLoadOnNodes() const =0;
        /// Returns the next SEL in the list of SELs connected to node index /a node.
        virtual CLinkType<ISEL>     getNextLink(quint32 node) const=0;
        virtual double              getArea() const=0;
        virtual QStringList         getProperties() const=0;
        virtual QVariant            getProperty(quint32 index) const=0;
        virtual bool                setProperty(quint32 index,const QVariant &val)=0;
        virtual bool                IsEmbedded() const=0;
        virtual bool                IsDowel() const = 0;
        virtual bool                supportsEmbedding() const=0;
    };
} // namespace sel
// EoF
