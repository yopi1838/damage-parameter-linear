#pragma once
// iallcontactcontainer.h

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    using namespace itasca;

    class IContact;
    class IContainerContact;
    class IContainerAllActiveContact;

    class IAllContactContainer {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b81;

        virtual IContainer *                   getContainer()=0;
        virtual const IContainer *             getContainer() const=0;
        virtual uint                           getNumberOfTypes() const=0;
        virtual QStringList                    getTypeNames() const=0;
        virtual QList<TType>                   getTypes() const =0;
        virtual quint64                        getSize(int index) const=0;
        virtual const IContainerContact *      getContainerContact(uint index) const=0;
        virtual IContainerContact *            getContainerContact(uint index)=0;
        virtual const IContainerAllActiveContact *getAllActiveContactContainer() const=0;
        virtual IContainerAllActiveContact *      getAllActiveContactContainer()=0;
        virtual const IContact *               findWithIDAndType(const TType &type,quint64 id) const=0;
        virtual const IContact *               findWithEndIDsAndType(const TType &type,quint64 id1,quint64 id2) const=0;
        virtual uint                           getGeometryNotice() const=0;
        virtual uint                           getExtraNotice() const=0;
        virtual uint                           getGroupNotice() const=0;
        virtual uint                           getPropertyNotice() const=0;
         
    };
} // namespace itascaxd

// EOF
