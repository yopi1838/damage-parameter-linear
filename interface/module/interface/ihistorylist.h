#pragma once

#include "dim.h"
#include "ihistory.h"
#include "utility/interface/icontainer.h"

/**
    * \file
    * \brief Interface to the list of all IHistories being taken by the history logic.
    */

namespace itascaxd {
    using namespace itasca;
    /// \brief Interface to the list of all IHistories being taken by the history logic.
    /// \ingroup module
    class IHistoryList {
    public:
        /// Returns a pointer to the container class representing a list of histories.
        virtual IContainer *getIContainer()=0;

        /// Returns the first history in the list.\n
        /// You can use IHistory::getNext() to iterate through the entire list.
        virtual const IHistory *getFirst() const=0;

        /// Find the IHistory object with ID ul, or 0 if no match is found.
        virtual const IHistory *lookupWithName(const QString &id) const=0;

        /// This value indications at what interval histories should be stored.
        ///< For instance if getNStep() = 10 then histories will be stored every 10 steps.
        virtual uint getNStep() const=0;

        /// Retrieves history data. start is a file position that is assumed to be value (first is always 0).\n
        /// Fills the values array and the valid array with the histories in that record.\n
        /// Also returns the step number of the record.\n
        /// Returns the file position after retrieving the data (can be used as start to get next record).\n
        virtual qint64 getData(qint64 start,QVector<double> *values=0,QVector<bool> *valid=0,quint64 *step=0) const=0;

        /// Returns the NoticeManager ID for histories added/removed from the list, or additional records taken.
        virtual uint getNotice() const=0;

        /// Returns the general type for an IHistory.
        virtual quint32 getHistoryGeneralType() const=0;
    };
} // namespace itascaxd
// EoF
