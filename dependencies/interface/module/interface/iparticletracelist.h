#pragma once

#include "dim.h"
#include "iparticletrace.h"
#include "utility/interface/icontainer.h"

/**
    * \file
    * \brief Interface to the list of all IParticleTraces being taken by the particle trace logic.
    */
namespace itascaxd {
    using namespace itasca;
    /// \brief Interface to the list of all IParticleTraces being taken by the particle trace logic.
    /// \ingroup module
    class IParticleTraceList {
    public:
        /// Returns a pointer to the container class representing a list of particle traces.
        virtual IContainer *getIContainer()=0;

        /// Returns the number of distinct types of IParticleTraces available.
        virtual uint getNumberRegistered() const=0;

        /// Returns the base IParticleTrace class registered in index.\n
        /// Assumes index<getNumberRegistered().
        virtual const IParticleTrace *getRegistered(uint index) const=0;

        /// Returns the first particle trace in the list.\n
        /// Use IParticleTrace::getNext() to iterate through the entire list.
        virtual const IParticleTrace *getFirst() const=0;

        /// Find the IParticleTrace object with Name name, or 0 if no match is found.
        virtual const IParticleTrace *lookupWithName(QString name) const=0;

        /// Indicates at what interval particle traces should be stored.\n
        /// For instance if getNStep() = 10 then particle traces will be stored every 10 steps.
        virtual uint getNStep() const=0;

        /// Retrieves particle trace data.  start is a file position that is assumed to be value (first is always 0).\n
        /// Fills the position array, the velocity array and the valid array with the particle traces in that record. \n
        /// Also returns the step number of the record in *step.
        /// Returns the file position after retrieving the data (can be used as start to get next record).
        virtual qint64 getData(qint64 start,QVector<DVect> *pos=0,QVector<DVect> *vel=0,QVector<bool> *valid=0,int *step=0) const=0;

        /// Returns the NoticeManager ID for particle traces added/removed from the list, or additional records taken.
        virtual uint getNotice() const=0;

        /// Returns the general type for an IParticleTrace.
        virtual quint32 getParticleTraceGeneralType() const=0;
    };
} // namespace itascaxd
// EoF
