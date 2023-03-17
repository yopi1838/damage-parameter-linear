#pragma once
/// \file isignalbase.h
/// \brief Interface for the small-granularity callback utility.


#include "base/src/baseqt.h"

namespace itasca {

#ifndef DOXYGEN
    class ISignalBase;
#endif

    /** \brief Slot base class.

    * The slot base class is used to identify a slot and to allow for automatic removal.
    * \ingroup utility
    * @{
    */

    // This is a slot base class - used to identify a slot, and to allow a slot to 
    //   automatically remove itself.
    class ISlotBase {
    public:
        /// Notice that the ISignalBase object \a signal is attached to this ISlotBase. 
        /// \param signal ISignalBase object attached to this ISlotBase.
        virtual void attachNotice(ISignalBase *signal)=0;
        /// Notice that the ISignalBase object \a signal is removed from this ISlotBase. 
        /// \param signal ISignalBase object removed to this ISlotBase.
        virtual void removeNotice(ISignalBase *signal)=0;
    };

    /// @}


    /** \brief Signal base class.

    * The signal base class is the base of all signals of 0, 1 or 2 arguments.  
    * ISignalBase is derived from ISlotBase so that signals can be chained together.
    * \ingroup utility
    * @{
    */

    class ISignalBase : public ISlotBase {
    public:
        /// Remove the ISlotBase \a slot from this ISignalBase object. \n
        /// If \a name is null then all slots with matching base are removed.\n
        /// If \a slot is null then all slots with matching names are moved. \n
        /// If both are null then all slots are removed.\n
        /// \param slot Pointer to an ISlotBase object to be removed.
        /// \param name Default null QString name of the ISlotBase to remove.
        virtual bool remove(ISlotBase *slot,const QString &name=QString())=0;
    };

    /// @}

    /** \brief ISlot2 interface - a two-argument class.

    * The ISlot2 class is a two-argument slot class interface.  An ISlot2 object can be implemented to
    * call methods with 0 or 1 arguments by ignoring arguments in execute().  
    * \ingroup utility
    * @{
    */

    template <class Param1,class Param2> 
    class ISlot2 {
    public:
        virtual ~ISlot2() { }
        /// Execute the attached method with 0, 1 or 2 arguments.
        virtual void       execute(Param1,Param2)=0;
        /// Returns the ISlotBase pointer to the base class from which this ISlot2 is derived.
        virtual ISlotBase *getBase()=0;
    };
    /// @}

    template<class Param1,class Param2> class Signal2;

} // namespace itasca
// EoF