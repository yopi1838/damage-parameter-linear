#pragma once

/**
    * \file
    * \brief Interface to a single label.
    */

#include "module/interface/dim.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a single label.
    /// \ingroup kernel
    class ILabel {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bdd;
        /// Returns a const pointer to the IThing interface for a label.
        virtual const IThing *getIThing() const =0;
        /// Returns a pointer to the IThing interface for a label.
        virtual IThing *      getIThing()=0;
        /// Returns the general type for the ILabel class.
        virtual TType         getType() const=0;

        /// Returns the label position, set setPos() 
        virtual DVect   getPos()  const =0;
        /// Returns the position where the end of the arrow points to
        virtual DVect   getEnd()  const =0;
        /// Returns the label text, see setText()
        virtual QString getText() const =0;
        /// Returns true is arrow is ON, see setArrow()
        virtual bool    isArrow()  const =0;
        /// Returns true is arrow end is defined, see setEnd() and setEndImplicitly()
        virtual bool    isEndDefined()  const =0;

        /// Sets the label position, set getPos() 
        virtual void setPos(const DVect &dv)=0;
        /// Sets the end vertex AND sets boolean endDefined_ to true, see getEnd() and setEndImplicitly
        virtual void setEnd(const DVect &dv)=0;
        /// Sets the end vertex BUT DO NOT MODIFY endDefined_, see setEnd() 
        virtual void setEndImplicitly(const DVect &dv)=0;
        /// Sets the arrow property ON, see getArrow()
        virtual void setArrow(bool b)=0;
        /// Set the label text, see getText()
        virtual void setText(const QString &str)=0;

        /// Returns the next const ILabel() object in the global ILabel list, or 0 if it is the last one. 
        /// Use ILabelList::getFirst() to obtain the first label in the list.
        virtual const ILabel *getNext() const=0;
        /// Returns the next ILabel() object in the global ILabel list, or 0 if it is the last one.
        /// Use ILabelList::getFirst() to obtain the first label in the list.
        virtual       ILabel *getNext()=0;
    };
} // namespace itasca
// EoF