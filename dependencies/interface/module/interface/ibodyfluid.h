#pragma once
// ibodyfluid.h

// Interface for functionality of all fluid bodies. 

namespace itascaxd {
    class IBody;

    class IBodyFluid {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4fe8df85;
        /// Return the IBody pointer.
        virtual const IBody    *getIBody() const=0;
        /// Return the IBody pointer.
        virtual IBody          *getIBody()=0;
        /// Return the IBody pointer of the mechanical body
        virtual IBody          *getBodyMech()=0;
        /// Return the IBody pointer of the mechanical body
        virtual const IBody    *getBodyMech() const=0;
        /// Return the applied fluid force.    
        virtual DVect           getFluidForce()  const=0;

        /// Set the applied fluid force.    
        virtual void            setFluidForce(const DVect &d)  =0;

    };
} // namespace itascaxd

// EoF
