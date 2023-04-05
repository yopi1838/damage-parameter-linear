#pragma once
// ISELNode.h

/**
  * \file
  * \brief Interface to provide access to structural element nodes.
  */

#include "module/interface/ilinktype.h"

namespace itasca {
    class IFishParam;
    class IThing;
}

namespace sel {
    using namespace itasca;
    using namespace itascaxd;

    class ISEL;
    class ISELLink;

    class IDynSELNode
    {
    public:
        /// Returns dynamic acceleration vector
        virtual DVect  getAcc(bool tran_comp) const=0;
    };

    /// \brief Interface to provide access to structural element nodes.
    /** Each node is associated with six degrees of freedom, comprised of three 
    translational components and three rotational components. Each node has 
    its own orthogonal coordinate system. The node-local system provides 
    the directions in which the equations of motion for the node are solved, 
    and also defines the direction in which the node can be attached to a 
    target entity via a link.
    * \ingroup FLAC3D
    */
    class ISELNode {
    public:
        inline static const int maxSides_ = 16;
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bfb;
        /// returns a const IThing pointer
        virtual const IThing *    getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *          getIThing()=0;
        /// Returns a const pointer to the next ISELNode object in the global list of structural elements links, 
        /// or 0 for the last element of the list. See ISELNodeList::getFirstSELNd()  
        virtual const ISELNode *   getNext() const=0;
        /// Returns a pointer to the next ISELNode object in the global list of structural elements links, 
        /// or 0 for the last element of the list. See ISELNodeList::getFirstSELNd()  
        virtual       ISELNode *   getNext() =0;
        /// Returns the const pointer to the link associated this this node, or 0 if no link is associated this the node.\n
        /// Valid values for \b iSide are 0 and 1 for side 1 and side 2 respectively.
        virtual const ISELLink *  getLink(quint32 side) const=0;
        /// Returns the pointer to the link associated this this node, or 0 if no link is associated this the node.\n
        /// Valid values for \b iSide are 0 and 1 for side 1 and side 2 respectively.
        virtual ISELLink *        getLink(quint32 side)=0;
        virtual quint32           getNumLinks() const=0;
        /// Returns true is the mark flag is on. See setMark().
        virtual bool              getMark() const=0;
        /// Sets the mark flag is to on, if parameter \b b is true. See getMark().
        virtual void              setMark(bool b) =0;
        /// Returns the node position, see setPos().\n
        /// If parameter \b cur is set to \b true, returns the current position.\n
        /// If parameter \b cur is set to \b false, returns the reference position.\n
        /// The current position is updated after each time step. The reference position is 
        ///   the configuration for which the stiffness matrices have been formed and does not 
        ///   change during a small-strain analysis.  During a large strain analysis, the 
        ///   reference point is set equal to the current position during each large-strain update.
        virtual DVect             getPosition(bool cur) const =0;
        /// Sets the node position to \b newPos, see getPos().\n
        /// If parameter \b cur is set to \b true, returns the current position.\n
        /// If parameter \b cur is set to \b false, returns the reference position.\n
        virtual void              setPosition(bool cur, const DVect &newPos) =0;
        /// Returns current position if the large-strain flag is on. Otherwise returns the reference position. See getPos().
        virtual DVect             getGeoPos() const=0;
        /// Returns the generalized velocity expressed in local-system coordinates if parameter \b local_sys is \b true.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        /// See setVel()
        virtual DVect             getVel(bool local_sys, bool tran_comp ) const=0;
        /// Sets the one of the components of the node velocity, local-system coordinates. Valid values for parameter \b dof_comp are in the range 0 to 5.
        virtual void              setVel2(double val, quint32 dof_comp) =0;
        /// Sets the one of the components of the node velocity, global-system coordinates. Valid values for parameter \b dof_comp are in the range 0 to 5.
        virtual void              setVelGlb2(double val, quint32 comp) =0;
        /// Returns the generalized acceleration expressed in local-system coordinates if parameter \b local_sys is \b true.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        /// See setVel()
        virtual DVect             getAcc(bool local_sys, bool tran_comp ) const=0;
        /// Sets the one of the components of the node acceleration, local-system coordinates. Valid values for parameter \b dof_comp are in the range 0 to 5.
        virtual void              setAcc2(double val, quint32 dof_comp) =0;
        /// Sets the one of the components of the node acceleration, global-system coordinates. Valid values for parameter \b dof_comp are in the range 0 to 5.
        virtual void              setAccGlb2(double val, quint32 comp) =0;
        /// Returns the generalized accumulated displacement expressed in local-system coordinates if parameter \b local_sys is \b true.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        /// See setDisp()
        virtual DVect             getDisp(bool local_sys, bool tran_comp ) const=0;
        /// Sets one of the components of the the node accumulated displacement, global-system coordinates. Valid values for parameter \b dof_comp are in the range 0 to 5.
        virtual void              setDisp2(double val, quint32 dof_comp) =0;
        /// Returns the generalized out-of-balance force expressed in local-system coordinates if parameter \b local_sys is \b true.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        virtual DVect             getFOB(bool local_sys, bool tran_comp ) const=0;
        /// Returns true if applied forces are valid.\n
        /// If parameter \b bForce is true, the function tests the translational components, otherwise the rotational components.\n
        virtual bool              getApForValid(bool bForce) const =0;
        /// Return the coordinate system in which generalized forces are expressed.\n
        /// True indicates global coordinate system, and false local coordinate system. 
        virtual bool              getApForSysGbl() const=0;
        /// Returns the generalized applied force. Use in conjunction with getApForSysGbl(). See setFAP(). \n
        /// If parameter \b bForce is true, returns the translational components, otherwise the rotational components.\n
        virtual DVect             getFAP( bool bForce ) const =0;
        /// Sets the generalized applied force. Use in conjunction with getApForSysGbl(). See getFAP(). \n
        /// If parameter \b bForce is true, sets the translational components, otherwise the rotational components.\n
        virtual void              setFAP( bool bForce, const DVect &dvVal )=0;
        /// Removes the applied force or moment condition on the node.\n
        /// If \a bForce is true removes the applied force condition, otherwise removes the applied moment.
        virtual void              setFAPRemove(bool bForce)=0;
        /// Sets the coordinate system in which generalized forces are expressed.\n
        /// If parameter \b bGlobal is true, then the global coordinate system will be used. 
        virtual void              setFAPSys(bool bGlobal)=0;
        /// Returns the applied force in the global coordinate system for plotting purposes only.\n
        /// If parameter \b bForce is true, returns the translational components, otherwise the rotational components.\n
        virtual DVect             getFAPGlobal(bool bForce) const=0;
        /// Return true to indicate that the local system fixity flag is on, see setFixFreeLocalSys()
        virtual bool              getLocalSysFixed() const =0;
        /// If parameter \b b is true, sets the local system fixity to on, see getFixFreeLocalSys()
        virtual void              setFixFreeLocalSys(bool b) =0;
        /// Sets a specific degree of freedom component to fixed, if parameter \b b is true.\n
        /// Valid values for parameter \b dof are in the 0 to 5 range.
        virtual void              setFixFree2(quint32 dof, bool b) =0;
        /// Sets the local system coordinates\n
        /// Parameter \b make_invalid is true, will set the node to "invalid".
        virtual void              setLocalSys( const DVect3& n1, const DVect3& n2, const DVect3& n3, bool  make_invalid = true)=0;
        /// Returns the local damping factor, see setLocalDampFac().
        virtual double            getLocalDampFac() const=0;
        /// Sets the local damping factor, see getLocalDampFac().
        virtual void              setLocalDampFac(double val) =0;
        /// Returns true if the node is slaved. Valid values for parameter \b dof are in the 0 to 5 range.
        virtual bool              getSlaved2(quint32 dof) const=0;
        /// Returns the node stiffness sum. Valid values for parameter \b dof are in the 0 to 5 range.
        virtual double            getGsum2(quint32 dof) const=0;
        /// Returns the local system axis.
        virtual Axes              getLocalSys() const=0;
        /// Return true is surface coordinate system is valid.
        virtual bool              getSurfaceSysValid(uint index) const=0;
        /// Returns the axis for the surface coordinate system, see setSurfSys()
        virtual Axes              getSurfaceSys(uint index) const=0;
        /// Sets the the surface coordinate system, see getSurfaceSys()
        virtual void              setSurfSys(uint index,const Axes3D &axes)=0;
        /// The components of the return vector will be set to 1 if the specific degree of freedom component is fixed.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        virtual IVect             getFix(bool tran_comp) const=0;
        /// Returns the mass in node-local sytem.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        virtual DVect             getMass(bool tran_comp) const=0;
        /// Returns the added mass in node-local sytem.\n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        virtual DVect             getAddedMass(bool tran_comp) const=0;
        /// Set the added mass in node-local system. \n
        /// If parameter \b tran_comp is true, returns the translational components, otherwise the rotational components.\n
        virtual void              setAddedMass(const double &newmass, uint index)=0;
        /// Returns the temperature increment seen by this node.  
        /// If \a local is true, returns only the value set in the node, not any value returned by links.
        /// If \a local is false, then returns the sum of the value set in the node and that returned by links.
        virtual double            getDTemp(bool local=false) const=0;
        /// Sets the local value of temperature increment in the node.
        virtual void              setDTemp(const double &t)=0;
        /// Returns TRUE if any structural element is connected to this node.
        virtual bool              getInUse() const=0;
        /// Returns the start link of the first SEL connected to this node
        virtual CLinkType<ISEL>   getStartLink() const=0;
        virtual double            getStressResultant(uint index,quint32 component)=0;
        virtual double            getLocalRatio() const=0;
        virtual double            getRatioTarget() const=0;
        virtual void              setRatioTarget(double d)=0;
        virtual double            getConvergence() const=0;
        virtual double            getDynamicCriticalTimestep() const=0;
    };
} // namespace f3
// EoF
