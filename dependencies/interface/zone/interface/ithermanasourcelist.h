#pragma once


/**
  * \file
  * \brief Interface to the global list of all thermal analytical sources.
  */

namespace itasca {
    class IContainer;
    class IThing;
}

namespace zone {
    using namespace itasca;

    class IThermAnaSource;

    /// \brief Interface to the global list of all thermal analytical sources.
    /// \ingroup ZONE
    class IThermAnaSourceList {
    public:
        /// returns a const IThing pointer
        virtual const IThing* getIThing() const = 0;
        /// returns a IThing pointer
        virtual IThing* getIThing() = 0;
        /// returns thermal analytical source type
        virtual quint32 getThermAnaSourceType() const = 0;
        /// Returns a const pointer to the IContainer interface representing the global list of thermal analytical sources.
        virtual const IContainer* getIContainer() const = 0;
        /// Returns a pointer to the IContainer interface representing the global list of thermal analytical sources.
        virtual IContainer* getIContainer() = 0;
        /// Return a const pointer to the first thermal analytical source link stored.\n
        /// IThermAnaSource::getNext() can be used to iterate on the list.
        virtual const IThermAnaSource* getFirstSource() const = 0;
        /// Return a pointer to the first thermal analytical source link stored.\n
        /// IThermAnaSource::getNext() can be used to iterate on the list.
        virtual IThermAnaSource* getFirstSource() = 0;
        /// Given a strucutal element link \b id, returns a const pointer to the thermal analytical source.
        virtual const IThermAnaSource* findSourceWithID(quint64 id) const = 0;

        ///  Notices
        virtual uint getExtraNotice() const = 0;
        virtual uint getGroupNotice() const = 0;
        virtual uint getPositionNotice() const = 0;
        virtual uint getStrengthNotice() const = 0;
        virtual uint getTimeNotice() const = 0;

    };
} // namespace sel
// EoF

