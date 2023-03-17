#pragma once
/** \file iprogram.h
    * \brief The main interface access point.
    * \defgroup kernel Kernel interface specification
    */

/** \mainpage The ITASCA plugin interface.
    * The Itasca interface allows access to internal data structures and algorithms.\n\n
    * As such it is extremely dangerous -- there is no way (even in theory) to prevent the user
    *   from writing code that will crash the program, or generate incorrect results.\n\n
    * Access to program data generally starts with the IProgram interface, which can be thought of as
    *   the root of a tree of all the interfaces available.\n\n
    * In general program specific data (Flac3D, 3DEC, PFC, etc) will be available in the interface
    *   IFLAC3D, which is defined differently for each program.\n\n
    */

#include "base/src/baseqt.h"

namespace itasca {
    class IImportExport;
    class IKernel;
    class IKernelInput;
    class IKernelOutput;
  
    /** \brief The main program access point.
        *
        * Serves at the root of the "tree" giving access to interface describing the entire code.\n
        * Use the IFLAC3D interface (not defined at the kernel level)
        *   to add application specific data to the interface tree.

        * \note If the getIxxx function is tagged with the Optional comment,
        *       that means that feature may not available and the user interface
        *       should degrade gracefully if it returns 0.
        *
        * \ingroup kernel
        */
    class IProgram {
    public:
        /// Returns the program access point QObject pointer
        virtual QObject *getQObject()=0;
        /// Returns the program access point QObject const pointer
        virtual const QObject *getQObject() const=0;
        /// Returns the code name
        virtual QString getName() const=0;
        /// Returns the code Major version
        virtual uint getCodeVersion() const=0;
        /// Returns the code Minor version
        virtual uint getUpdateVersion() const=0;
        /// Returns a pointer to the IKernelInput object
        virtual IKernelInput *    getIKernelInput()=0;
        /// Returns a pointer to the IKernelOutput object
        virtual IKernelOutput *   getIKernelOutput()=0;
        /// Returns the total number of cycles - This particular functions is thread safe.
        virtual quint64                   getStep() const=0;
        /// Returns the current model extent
        virtual DExtent3                  getModelExtent() const=0;
        /// Returns a pointer to IKernel, the entry point for command processing
        virtual IKernel *                 getIKernel()=0;
        /// Returns a const pointer to IKernel, the entry point for command processing
        virtual const IKernel *           getIKernel() const=0;
        /// The proliferation of getIXXXX() interface functions in IProgram and IFLAC3D is becoming unmanageable.\n
        /// This interface is intended for future use, to replace all that.\n
        /// The idea is that interfaces would be registered with the program under a key name, and people could
        ///   retrieve interface pointers with that name here.\n
        /// Registered interfaces are documented in the registerd_interfaces module
        virtual void *                    findInterface(const QString &name)=0;
        virtual const void *              findInterface(const QString &name) const=0; ///< \copydoc findInterface.
        /// This is a template version of findInterface(const QString &) that uses typeid(T).name() for the string.\n
        /// For example \code IGeometry *geom = prog->findInterface<IGeometry>(); \endcode  \n
        /// Registered interfaces are documented in the registerd_interfaces module
        template <class T> T *            findInterface() { return reinterpret_cast<T *>(findInterface(typeid(T).name())); }
        /// This is a template version of findInterface(const QString &) that uses typeid(T).name() for the string.\n
        /// For example \code IGeometry *geom = prog->findInterface<IGeometry>(); \endcode \n
        /// Registered interfaces are documented in the registerd_interfaces module
        template <class T> const T *      findInterface() const { return reinterpret_cast<const T *>(findInterface(typeid(T).name())); }
        /// The proliferation of getIXXXX() interface functions in IProgram and IFLAC3D is becoming unmanageable.\n
        /// This interface is intended for future use, to replace all that.\n
        /// The idea is that interfaces would be registered with the program under a key name here, and people could
        ///   retrieve interface pointers with that name.\n
        /// Registered interfaces are documented in the registerd_interfaces module
        virtual void                      registerInterface(const QString &name,void *iface,bool errorOnDuplicate=true)=0;
        /// This is a templatized version of registerInterface(const QString &name,void *iface), that converts automatically and
        ///    uses typeid(T).name() for the registration string.   We reccomend you use this if at all possible.
        /// Registered interfaces are documented in the registerd_interfaces module\n
        template <class T> void           registerInterface(T *iface,bool errorOnDuplicate=true) { registerInterface(typeid(T).name(),reinterpret_cast<void *>(iface),errorOnDuplicate); }
        /// Destroy the kernel
        virtual void                      destroy()=0;
        /// Returns FALSE if the model is between the start of RESTORE and the end of mapping,
        ///    so model data and pointers could be invalid.
        virtual bool                      isModelMapped() const=0;
        virtual QString                   getHomePath() const=0;
    };
} // namespace itascaxd
// EoF
