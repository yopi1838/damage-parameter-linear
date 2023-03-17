//#pragma once
//
//#include "utility/interface/icontainer.h"
//
//namespace zone {
//    using namespace itasca;
//
//    class IApplyItem;
//
//    /// \brief Class provides access to the global list of all IAppItem objects in the model.
//    class IApplyItemList {
//    public:
//        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
//        static const TType type_ = 0x5318f940;
//        /// returns a const IThing pointer
//        virtual const IThing * getIThing() const=0;
//        /// returns a IThing pointer
//        virtual IThing * getIThing()=0;
//        /// Returns a const pointer to the IContainer
//        virtual const IContainer * getIContainer() const=0;
//        /// Returns a pointer to the IContainer
//        virtual IContainer * getIContainer() =0;
//    };
//} // namespace zone
//// EoF