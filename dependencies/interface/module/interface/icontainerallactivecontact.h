#pragma once
// iallactivecontactcontainer.h

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;

    class IAllContactContainer;

    class IContainerAllActiveContact {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x51779454;

        virtual IContainer *                getContainer()=0;
        virtual const IContainer *          getContainer() const=0;
        virtual quint64                     getSize(int index) const=0;
         
    };
} // namespace itascaxd

// EOF
