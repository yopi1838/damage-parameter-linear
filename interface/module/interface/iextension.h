#pragma once
/** \file iextension.h
    * \brief Interface class for acessing extension stored in the base class of other things.
    */

namespace itasca {
    class Archive2;
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

/** 
    * \brief Interface class for acessing extension stored in the base class of other things.
    *
    *  This interface serves as a base class for extensions stored in the base of other things
    *  (For instance, the dynamic extensions of nodes in FLAC3D, or Thermal extensions to Balls in PFC)
    *  A Dynamic extension (IDynNode) would be derived from IExtension, for instance.
    * \ingroup module
    */
    class IExtension {
    public:
        /// Notice emitted to inform that host was destroyed
        virtual void        hostDestroyNotice()=0;
        /// Save/restore data
        virtual void        save(Archive2 &a) const=0;
        virtual bool        restore(Archive2 &a,quint64 label)=0;
        virtual void        remap(Archive2 &a)=0;
        /// Returns the extension name. Needs to be the same as the registered name, or typeid(T).name()
        virtual QString     getName() const=0; 
        /// Allocates memory and returns a  poniter to an object that inherits from IExtension
        virtual IExtension *clone() const=0;
    };
} // namespace itascaxd
// EOF
