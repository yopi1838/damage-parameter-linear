#pragma once
// icontactmodule.h

namespace itascaxd {
    class IAllContactContainer;
    class IContactModelList;

    class IContactModule {
    public:
        // Get a pointer to the allcontactcontainer for the specified contact class name
        virtual IAllContactContainer *getAllContactContainer(const QString &className)=0;
        // Get a const pointer to the allcontactcontainer for the specified contact class name
        virtual const IAllContactContainer *getAllContactContainer(const QString &className) const=0;
        // Get a pointer to the contactmodellist for the specified contact class name
        virtual IContactModelList *getContactModelList(const QString &className)=0;
        // Get a const pointer to the contactmodellist for the specified contact class name
        virtual const IContactModelList *getContactModelList(const QString &className) const=0;
        // Return the list of processes that the contact models have been registered with
        // Return a list of all type names registered with the allcontactcontainer of a specific contact class;
        virtual QStringList getTypeNames(const QString &className) const =0;
        // Return a list of all types registered with the allcontactcontainer of a specific contact class;
        virtual QList<TType> getTypes(const QString &className) const=0;
        virtual QStringList getClassNames() const=0;
        // Return the name of the default process
        virtual QString getDefaultClassName() const=0;
    };

} // namespace itascaxd
// EoF
