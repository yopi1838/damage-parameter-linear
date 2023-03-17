#pragma once
// ISELgeogrid.h
/**
  * \file
  * \brief Interface to geogrid structural elements
  */

namespace itasca {
    class IThing;
}
namespace sel {
    using namespace itasca;

    class ISEL;
    class ISELShell;

    /// \brief Interface to geogrid structural elements
    /** This interface inherits from ISELShell.
    * \ingroup SEL
    */
    class ISELGeogrid {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815a06;
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
        /// Returns the cohesive strength (coupling spring cohesion), see setCSSCOH()
        virtual double                 getCSSCOH()const=0;
        /// Returns the coupling spring friction angle in degrees, see setCSSFric()
        virtual double                 getCSSFric()const=0;
        /// Returns the geogrid stiffness, see setCSSK()
        virtual double                 getCSSK()const=0;
        /// Returns the confining stress at node \b n. \n
        /// Valid values for \b n are 0, 1 or 2. \n
        /// The confining stress acts perpendicular to the geogrid surface
        virtual double                 getRConf(int n)const=0;
        /// Returns the displacement magnitude at node \b n \n
        /// Valid values for \b n are 0, 1 or 2.\n
        /// Direction in which the force is acting is given by getRSTRDir()
        virtual double                 getRDisp(int n)const=0;
        /// Returns the stress magnitude at node \b n \n
        /// Valid values for \b n are 0, 1 or 2.\n
        /// Direction in which the force is acting is given by getRSTRDir()
        virtual double                 getRSTR(int n)const=0;
        /// Returns the direction (unit vector) in which the force is acting for a given node \b n.
        /// Valid values for \b n are 0, 1 or 2.\n
        virtual DVect3                 getRSTRDir(int n)const=0;
        /// Returns the yield state for a given node \b n.
        /// Valid values for \b n are 0, 1 or 2.\n
        /// Return values are: 1 = never yielded, 2 = now yielding and 3 = has yielded in past
        virtual int                    getRYield(int n)const=0; 
        /// Returns the large-strain flag. Returns true if large-strain sliding is on. See setSlide()
        virtual bool                   getSlide()const=0;
        /// Returns the large-strain sliding tolerance, see setSlideTol()
        virtual double                 getSlideTol()const=0;

        /// Sets the cohesive strength (coupling spring cohesion), see getCSSCOH()
        virtual void                   setCSSCOH(const double &n)=0;
        /// Sets the coupling spring friction friction angle in degrees, see getCSSFric()
        virtual void                   setCSSFric(const double &n)=0;
        /// Sets the geogrid stiffness, see getCSSK()
        virtual void                   setCSSK(const double &n)=0;
        /// Sets the large-strain flag. If \b n is true,  the large-strain sliding will be set. See getSlide()
        virtual void                   setSlide(bool n)=0;
        /// Sets the large-strain sliding tolerance, see getSlideTol()
        virtual void                   setSlideTol(const double &n)=0;
    };
} // namespace sel
// EoF

