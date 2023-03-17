#pragma once
// irangeelementseltype.h

/**
* \file
* \brief Interface to a filter element that allows user to filter based on a structural element type.
*/

#include "module/interface/irangeelement.h"

namespace sel {
    using namespace itascaxd;

    /// \brief Interface to a filter element that allows user to filter based on a structural element type.
    /// \ingroup FLAC3D
    class IRangeElementSELType {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815be6;
        /// Returns a pointer to \b itself
        virtual IRangeElement *getIRangeElement()=0;
        /// Returns a list of valid SEL names: Any,Shell,Beam,Cable,Pile,Liner and Geogrid
        virtual QStringList    getSELTypeNames() const=0;
        virtual QString        getNameFromType(TType type) const=0;
        virtual int            getIndexFromType(TType type) const=0;
        /// Returns the current type being selected, as an index into the stringlist returned from getSelTypeNames()
        virtual int            getSELTypeIndex() const=0;
        // slots
        /// Sets the SEL type as an index into the list returned from getSelTypeNames().
        virtual void           setSELTypeIndex(int index)=0;
        // signals
        /// Signal emitted when the SEL type has changed, see getSELType().
        virtual void           selTypeIndexChanged(int type)=0;
    };
}
// EoF

