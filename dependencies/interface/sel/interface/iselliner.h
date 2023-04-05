#pragma once
// iselshell.h

/**
  * \file
  * \brief Interface to liner structural elements
  */

#include "isel.h"

namespace itasca {
    class IThing;
}
namespace sel {
    class ISELShell;

    /// \brief Interface to liner structural elements
    /** This interface inherits from ISELShell.
    * \ingroup SEL
    */
    class ISELLiner {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a31;
        /// returns a const IThing pointer
        virtual const IThing *         getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *               getIThing()=0;
        /// Returns a const pointer to object base class
        virtual const ISEL *           getISEL() const=0;
        /// Returns a pointer to object base class
        virtual ISEL *                 getISEL()=0;
        /// Returns a const pointer to the class that it inherits from
        virtual const ISELShell *      getISELShell() const=0;
        /// Returns a pointer to the class that it inherits from
        virtual ISELShell *            getISELShell()=0;
        /// Returns the normal coupling spring tensile strength for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSNCUT()
        virtual double                 getCSNCUT(int s)const=0;
        /// Returns the normal coupling spring stiffness for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSNK()
        virtual double                 getCSNK(int s)const=0;
        /// Returns the shear coupling spring cohesion for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSSCOH()
        virtual double                 getCSSCOH(int s)const=0;
        /// Returns the shear coupling spring residual cohesion for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSSCOHRES()
        virtual double                 getCSSCOHRES(int s)const=0;
        /// Returns the shear coupling spring friction angle for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSSFRIC()
        virtual double                 getCSSFRIC(int s)const=0;
        /// Returns the shear coupling spring stiffness for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See setCSSK()
        virtual double                 getCSSK(int s)const=0;
        /// Returns the displacement magnitude in cloupling springs at \b node, side 1. \n
        /// Valid values for parameter \b s are 1 = normal, any other integer value indicates shear.\n
        /// Valid values for \b node are 0,1 or 2.\n
        /// Direction in which the force is acting is given by getRSTRDir()
        virtual double                 getRDisp(int s, int node)const=0; 
        /// Returns the stress magnitude in cloupling springs at \b node, side 1 \n
        /// Valid values for parameter \b s are 1 = normal, any other integer value indicates shear.\n
        /// Valid values for \b node are 0,1 or 2.\n
        /// Direction in which the force is acting is given by getRSTRDir()
        virtual double                 getRStr(int s, int node)const=0;
        /// Returns the stress magnitude in cloupling springs at \b node and \b side.\n
        /// If norm is true, uses the normal direction, otherwise indicates shear.\n
        /// Valid values for \b node are 0,1 or 2.\n
        /// Valid values for \b side are 0 (side 1) or 1 (side 2).\n
        /// Direction in which the force is acting is given by getRSTRDir()
        virtual double                 getRStr(bool norm, int node, int side) const=0; 
        /// Returns the direction (unit vector in global system) in which the linerSEL is being
        ///  loaded by coupling spring \b ori at \b node.\n
        /// Valid values for parameter \b ori are 1 = normal, any other integer value indicates shear.\n
        /// Valid values for \b node are 0,1 or 2.
        virtual DVect3                 getRSTRDir(int ori, int node)const=0; 
        /// Returns the yield state of shear coupling spring at \b node.
        /// Valid values for \b node are 1, 2 or 3.\n
        /// Return values are: 1 = never yielded, 2 = now yielding and 3 = has yielded in past
        virtual int                    getRYield(int node,int side)const=0;
        /// Returns the large-strain flag. Returns true if large-strain sliding is on. See setSlide()
        virtual bool                   getSlide()const=0;
        /// Returns the large-strain sliding tolerance, see setSlideTol()
        virtual double                 getSlideTol()const=0;
        /// Returns true is lier is embedded
        virtual bool                   getEmbedded()const=0;


        /// Sets the normal coupling spring tensile strength for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSNCUT()
        virtual void                   setCSNCUT(int s,const double &d)=0;
        /// Sets the normal coupling spring stiffness for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSNK()
        virtual void                   setCSNK(int s,const double &d)=0;
        /// Sets the shear coupling spring cohesion for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSSCOH()
        virtual void                   setCSSCOH(int s,const double &d)=0;
        /// Sets the shear coupling spring residual cohesion for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSSCOHRES()
        virtual void                   setCSSCOHRES(int s,const double &d)=0;
        /// Sets the shear coupling spring friction angle for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSSFRIC()
        virtual void                   setCSSFRIC(int s,const double &d)=0;
        /// Sets the shear coupling spring stiffness for one of the liner sides.\n
        /// Valid values for \b s are 0 (side 1) and 1 (side 2). See getCSSK()
        virtual void                   setCSSK(int s,const double &d)=0;
        /// Sets the generalized nodal force (acting on the lineSEL) at \b node.\n
        /// Valid values for \b node are 0,1 or 2.\n
        /// Valid values for \b component are in the range 0 to 5, 
        ///   where 0 is the x normal component and 3 is the x shear component.\n
        /// These forces are expressed in the global coordinate system.
        virtual void                   setNForce(int node, int component,const double &v)=0;
        /// Sets the large-strain flag. If \b n is true,  the large-strain sliding will be set. See getSlide()
        virtual void                   setSlide(bool b)=0;
        /// Sets the large-strain sliding tolerance, see getSlideTol()
        virtual void                   setSlideTol(const double &d)=0;  
    };
} // namespace sel
// EoF
