#pragma once

/**
* \file
* \brief Provides interface to IExtrudeSetList, an IContainer of IExtrudeSet objects.
*/

namespace itasca {
    class IContainer;
    class IThing;
}

namespace extruder {
    using namespace itasca;

    class IExtrudeSet;

    // (See convert_getcast in ithing.h).
    /// \brief Provides interface to IExtrudeSetList, an IContainer of IExtrudeSet objects.
    /// \ingroup FLAC3D
    class IExtrudeSetList {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4d2227cc;

        /// returns its IThing pointer
        virtual IThing *           getIThing()=0;
        /// returns its const IThing pointer
        virtual const IThing *     getIThing() const=0;
        /// returns its const QObject pointer
        virtual const QObject *    getQObject() const=0;
        /// returns its QObject pointer
        virtual QObject *          getQObject()=0;
        /// Returns a const pointer to the current body set (IExtrudeSet). May Return nullptr!
        virtual const IExtrudeSet *getCurrent() const=0; 
        /// Returns a pointer to the current body set (IExtrudeSet). May Return nullptr!
        virtual IExtrudeSet *      getCurrent()=0;
        /// Clears its ExtrudeList container
        virtual void               clear()=0;
        /// Returns a pointer to a IExtrudeSet which id is <b>id</b>
        virtual IExtrudeSet *      lookupWithID(quint64 id)=0;
        /// Returns a const pointer to a IExtrudeSet which id is <b>id</b>
        virtual const IExtrudeSet *lookupWithID(quint64 id) const=0;
        /// Returns a pointer to a IExtrudeSet which name is parameter <b>name</b>
        virtual IExtrudeSet *      lookupWithName(const QString &name)=0;
        /// Returns a const pointer to a IExtrudeSet which name is parameter <b>name</b>
        virtual const IExtrudeSet *lookupWithName(const QString &name) const=0;
        /// Creates a IExtrudeSet object and adds it to the container. Returns a pointer to the object.
        virtual IExtrudeSet *      create(const QString &name)=0;
        /// Remove a IExtrudeSet from container. Returns a pointer to the object.
        virtual void               removeSet(IExtrudeSet *set)=0;

        /// Returns an IContainer interface for the list of all IExtrudeSet objects.
        virtual IContainer *      getIContainer()=0;
        /// Returns an IContainer interface for the list of all IExtrudeSet objects.
        virtual const IContainer *getIContainer() const=0;

        /// Return global meshing parameters
        virtual const bool        getMeshUnstructuredOnly() const=0;
        virtual const bool        getMeshModeQuad() const=0;
        virtual const bool        getMeshStructuredFlag() const=0;
        virtual const bool        getMeshMultiStructuredFlag() const=0;
        virtual const int         getMeshOptimizationLevel() const=0;
        virtual const double      getMeshShapeQualityWeight() const=0;
        virtual const double      getMeshQuadrangleWeight() const=0;
        virtual const double      getMeshTargteMetric() const=0;
        virtual const double      getMeshMaxGradation() const=0;


        /// Return global default edge parameters
        virtual void              activateEdgeDialog(bool b) const=0;
        virtual const bool        getNumZoneEdgeEnabled() const=0;
        virtual const bool        getLengthZoneEdgeEnabled() const=0;
        virtual const quint32     getDefaultZoneEdgeNum() const=0;
        virtual const double      getDefaultZoneEdgeLength() const=0;
        virtual const quint32     getDefaultEdgeSize(double) const=0;

        // SIGNALS
        /// signal emitted when current ExtrudeSet set is changed
        virtual void currentChanged(const QString &current)=0;
    };

} // namespace f3
// EoF
