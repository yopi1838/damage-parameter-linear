#pragma once
// inode.h
/**
* \file
* \brief Interface to access node data
*/

namespace itasca {
    class IThing;
}

namespace zone {
    using namespace itascaxd;
    class IDynNode;

    /// \brief Interface to access node data
    /// \ingroup Zone
    class INode {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bef;


        /// returns a const IThing pointer
        virtual const IThing *      getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing *            getIThing()=0;

        /// returns the node displacement
        virtual DVect               getDisplacement(bool local=false) const=0;
        /// returns the node small displacement
        virtual DVect               getSmallDisplacement(bool local=false) const=0;
        /// Returns the local force ratio of this gridpoint after the last cycle.
        virtual double              getLocalForceRatio() const=0;
        /// returns the node location
        virtual DVect3              getLocation() const=0;
        virtual double              getMultiplier() const=0;
        virtual bool                setMultiplier(double d)=0;

//#ifdef THREED
        /// returns a const pointor to IDynNode
        virtual const IDynNode  *   getIDynNode() const=0;
//#endif
    };
}
// EoF
