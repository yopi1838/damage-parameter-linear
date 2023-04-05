#pragma once
// icontaineractivecontact.h

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;

    class IContainerContact;

    class IContainerActiveContact {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x51792695;

        virtual IContainer *                getContainer()=0;
        virtual const IContainer *          getContainer() const=0;
        virtual const IContainer           *getContainerContact() const=0;
        virtual IContainer *                getContainerContact()=0;
         
    };
} // namespace itascaxd

// EOF
