//ibodypoint.h
#pragma once
/**
* \file
* \brief Interface to provide to a specifics related to IBodyPoint (related to Body snapon tool but not general GeomPoint) 
*/
namespace body{
using namespace itasca;
using namespace itascaxd;
    class IBodyPoint {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_=0x54f4f824;
        /// returns a const IThing pointer
        virtual const IThing    *   getIThing() const=0;
        /// returns a IThing pointer
        virtual IThing          *   getIThing()=0;
        /// return GUI specific select flag
        virtual bool getGUIselect() const=0;
        /// set GUI specific select flag
        virtual void setGUIselect(bool flag) const=0;
    };
}
