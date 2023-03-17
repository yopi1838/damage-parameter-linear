#pragma once

/**
    * \file
    * \brief Interface for the list of all IUDTensor (user defined tensor).
    */

#include "iudtypelist.h"

namespace itascaxd {
    class IUDTensor;
    using namespace itasca;

    /// \brief Interface for the list of all IUDTensor (user defined tensor).
    /// \ingroup kernel
    class IUDTensorList : public IUDTypeList<IUDTensor> {
    public:
        /// returns a IThing pointer
        virtual IThing *getIThing()=0;
        /// returns a const IThing pointer
        virtual const IThing *getIThing() const=0;
        /// Imports objects of type T from the file /a filename.  All objects created are assigned a group of /a group in slot /a slot (if specified).
        virtual bool     importFromFile(const QString &filename,const IGroupID &group)=0;
        /// Exports all objects to the file /a filename.   /a binary indicates binary format, text is used otherwise.
        /// /a filter (if given) is used to filter which objects are exported.
        /// /a comment is place at the start of text output.
        virtual bool     exportToFile(const QString &filename,bool binary,const IRange *filter=0,const QString &comment=QString())=0;
    };
} // namespace itascaxd
// EoF