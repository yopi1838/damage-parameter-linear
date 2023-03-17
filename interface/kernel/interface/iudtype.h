#pragma once
// iudtype.h

/**
 * \file
 * \brief IUDType is a template interface class, for values of different types that can be used as user-defined data.
 */

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IUDData;
    class IUDPos;

    /// \brief Interface to data values of different types associated with position in space, as user-defined data.
    /** For instance, the type could be doubles (scalar), vectors, or tensors.
        * \ingroup kernel
        */
    template <class T> class IUDType {
    public:
        /// Specifies the value stored in the object
        typedef T value_type;

        /// Returns an IThing interface to this object
        virtual IThing *          getIThing()=0;
        /// Returns a const IThing interface to this object
        virtual const IThing *    getIThing() const=0; ///< \copydoc getIThing
        /// Returns an IUDPos interface to this object
        virtual IUDPos *          getIUDPos()=0;
        /// Returns a const IUDPos interface to this object
        virtual const IUDPos *    getIUDPos() const=0; ///< \copydoc getIUDPos
        /// Returns the next object in whatever global container it exists in.
        virtual IUDType<T> *      getNext()=0;
        /// Returns the const next object in whatever global container it exists in.
        virtual const IUDType<T> *getNext() const=0; ///< \copydoc getNext

        /// Returns the current value associated with the user-defined data.
        virtual T    getValue() const=0;
        /// Sets the value associated with the user-defined data.
        virtual void setValue(const T &t)=0;
    };
} // namespace itascaxd
// EoF
