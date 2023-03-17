// ibody.h
#pragma once

/// \file
/// \brief Interface for a user-defined geometric point

#include "module/interface/dim.h"
#include "utility/interface/icontainer.h"
#include "utility/interface/igroup.h"

namespace itasca {
    
}

namespace itascaxd{
    class IRange;
    class IGeomSet;
    class IGeomPoint;
    class IGeomEdge;
    class IGeomPoly;
}

namespace body {
using namespace itasca;
using namespace itascaxd; 
    class IBodySet;
    class IBodyPoint;
    class IBodyEdge;
    /// \brief Interface for the global list of all geomtric data sets IBodySet.
    /// \ingroup kernel
    class IBody {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x53ab1fd7;


        /// Returns an IContainer interface for the list of all IBodySet objects.
        virtual IContainer       *getIContainer()=0;
        /// Returns an IContainer interface for the list of all IBodySet objects.
        virtual const IContainer *getIContainer() const=0;

        /// Returns the type identifier for IBodyPoint objects.
        virtual quint32 getIBodyPointType() const=0;
        /// Returns the type identifier for IBodyEdge objects.
        virtual quint32 getIBodyEdgeType() const=0;
        /// Returns the type identifier for IBodyFace objects.
        virtual quint32 getIBodyFaceType() const=0;
        /// Returns the type identifier for IBodyBlock objects.
        virtual quint32 getIBodyBlockType() const=0;
        /// Returns the type identifier for IBodySet objects.
        virtual quint32 getIBodySetType() const=0;
        
        /// MOO TO VAL:  I don't think these are necessary anymore, the IContainer interface should be sufficient.
        /// Returns a pointer to the first IBodySet in the list, or 0 if none are present.  See the IBodySet::getNext() function.
        virtual IBodySet *      getFirstBodySet()=0;
        /// Returns a pointer to the first IBodySet in the list, or 0 if none are present.  See the IBodySet::getNext() function.
        virtual const IBodySet *getFirstBodySet() const=0;
        /// Returns a pointer to the current IBodySet.
        virtual const IBodySet *current() const=0;
        /// Set's current body set
        virtual void current(IBodySet *set)=0;

        /// Returns a pointer to the IBodySet with matching id, or 0 if it doesn't exist.  Guaranteed to take less than linear time.
        virtual IBodySet *      lookupWithID(quint64 id)=0;
        /// Returns a pointer to the IBodySet with matching id, or 0 if it doesn't exist.  Guaranteed to take less than linear time.
        virtual const IBodySet *lookupWithID(quint64 id) const=0;
        /// Returns a pointer to the IBodySet with matching name, or 0 if it doesn't exist.  Guaranteed to take less than linear time.
        virtual IBodySet *      lookupWithName(const QString &name)=0;
        /// Returns a pointer to the IBodySet with matching name, or 0 if it doesn't exist.  Guaranteed to take less than linear time.
        virtual const IBodySet *lookupWithName(const QString &name) const=0;
        /// Returns all body sets.
        virtual QStringList findAllBodySets() const=0;

        /// Creates a new IBodySet with name \a name, and returns a pointer to it.   An \a id may optionally be specified, otherwise one is assigned.
        /// If an Id number is given and a IBodySet already exists with that ID, and exception occurs.
        virtual IBodySet *create(const QString &name,quint64 id=0)=0;
        /// Removes \a set from the list, and destroys it.  Returns a pointer to the next item in the list after set, or 0 if unsuccessful.
        virtual IBodySet *removeSet(IBodySet *set)=0;

        /// Returns a signal that gets called every time anything is added.
        virtual IContainer::signal_type *getAnyAddedSignal2() const=0;
        /// Returns a signal that gets called every time anything is removed.
        virtual IContainer::signal_type *getAnyRemovedSignal2() const=0;
        /// Returns a notification code (see INoticeManager) indicating a current body set changed
        virtual uint   getCurrentChangedCode() const=0;
       /// return geomset from a point
       virtual const IGeomSet * getIGeomSet(const IGeomPoint *)const=0;
       /// return geomset from a edge
       virtual const IGeomSet * getIGeomSet(const IGeomEdge *)const=0;
       /// return geomset from a poly
       virtual const IGeomSet * getIGeomSet(const IGeomPoly *)const=0;
       /// shiff file for BodySet name returns true if it is ok and produced by "body export".
       virtual bool sniffBodySetFileOK(const QString &filename, QString &bsetName) const=0;
    };
} // namespace body
// EoF
