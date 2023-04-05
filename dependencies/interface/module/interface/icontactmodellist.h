#pragma once
// icontactmodellist.h

namespace itascaxd {
    class IContactModelList {
    public:
        virtual QStringList          allModels() const=0;
        // Returns a set of all properties in all contact models current registered with the program.
        // If notFixedOnly is set to true, then *Fixed* (readOnly) properties are not returned.
        virtual QStringList          allProperties(const QString &mn=QString(),bool notFixedOnly=true) const=0;
        // Return a list of properties that are not strings or vectors
        virtual QStringList          nonStringVectorProperties(const QString &mn=QString(),bool not_fixed_only=true) const=0;
        // Return a list of properties that are strings only
        virtual QStringList          stringProperties(const QString &mn=QString(),bool not_fixed_only=true) const=0;
        // Return a list of properties that are only vectors
        virtual QStringList          vectorProperties(const QString &mn=QString(),bool not_fixed_only=true) const=0;
        // Return a property index given a model name and a property name.
        virtual uint                 getPropertyIndex(const QString &modelName,const QString &propertyName)=0; 
        // Returns a set of all properties in all contact models current registered with the program.
        virtual QStringList          allMethods(const QString &mn=QString()) const=0;
        // Return a method index given a model name and a property name.
        virtual uint                 getMethodIndex(const QString &modelName,const QString &propertyName)=0; 
        // Returns a list of all arguments available under the method name methodName for contact model modelName.
        virtual QStringList          getMethodArguments(const QString &modelName,const QString &methodName) const=0;
        // Returns a list of all arguments available under the method name methodName in all contact models current registered with the program.
        virtual QStringList          getAllMethodArguments(const QString &methodName) const=0;
        // Return true is the model was registered as a plugin
        virtual bool                 plugin(const QString &name) const =0;
        // Return all energies 
        virtual QStringList          allEnergies(const QString &mn=QString()) const = 0;
        // Return the energy index  
        virtual uint                 getEnergyIndex(const QString &modelName,const QString &energyName)=0; 

    };
} // namespace itascaxd
// EoF
