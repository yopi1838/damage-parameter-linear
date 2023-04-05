#pragma once

#include "base/src/baseqt.h"
#include "dim.h"

/**
    * \file
    * \brief Interface to a particular history being taken by the history logic.
    */

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    /// \brief Interface to a particular history being taken by the history logic.
    /// \ingroup module
    class IHistory {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b8b;

        /// Get const IThing interface pointer for the history.
        virtual const IThing *getIThing() const=0;
        /// Get IThing interface pointer for the history.
        virtual IThing *      getIThing()=0;

        /// Unique ID associated with the history
        virtual quint64 getID() const=0;

        /// This name is assigned by the code.
        virtual QString getName() const=0;

        /// Label of the history - to be displayed in plots.\n
        /// If unassigned this will return the same as getName().\n
        /// Otherwise it will return a label assigned by the user.
        virtual QString getLabel() const=0;

        /// Returns TRUE if the history is of an object with a distince position.
        ///    Like a node, zone, etc.\n
        /// Returns FALSE for things like out-of-balance force, time, etc.
        virtual bool hasPosition() const=0;

        /// If hasPosition() is true, returns the position of the object.\n
        /// Returns (0,0,0) if the history has no valid position.
        virtual DVect getPosition() const=0;

        /// Returns the min (x) and max (y) values ever taken of the history object.\n
        virtual DVect2 getValueRange() const=0;

        /// Returns TRUE if the history is in a state to take more values.\n
        /// Returns FALSE if not (for instance a history of a zone that has been deleted.)
        virtual bool isValid() const=0;

        /// Returns TRUE if the history is no longer taking values.\n
        /// This could be because it isn't valid (isValid()), or because it has been explicitly de-activated.
        virtual bool isActive() const=0;

        /// This is the position in the history temp file of the first record 
        ///    containing a value from this history.
        virtual qint64 getStartPosition() const=0;

        /// This is the position in the history record of the object.\n
        /// Note that this is distinct from the ID() of the History.
        virtual int getStorageIndex() const=0;

        /// Returns the next IHistory() object in the global list,
        ///   or 0 if it is the last one.
        virtual const IHistory *getNext() const=0;

        /// Used for for dynamic rezoning FLAC3D, returns true if this history changes zone or gp location during dynamic rezoning
        virtual bool getRezoneEnabled() const=0;      
        /// for dynamic rezoning FLAC3D, new location for history zone or gp.
        //virtual void linkToNewLocation(const DVect &)=0;    
    };
} // namespace itascaxd
// EoF
