#pragma once
// ibodythermal.h

// Interface for functionality of all thermal bodies. 

namespace itascaxd {
    class IBody;
    class IPieceThermal;

    class IBodyThermal {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4f6358d0;
        /// Return the IBody pointer.
        virtual const IBody    *getIBody() const=0;
        /// Return the IBody pointer.
        virtual IBody          *getIBody()=0;
        /// Return the IBody pointer of the mechanical body
        virtual IBody          *getBodyMech()=0;
        /// Return the IBody pointer of the mechanical body
        virtual const IBody    *getBodyMech() const=0;
        /// Return the property of the piece 
        virtual QVariant        getPieceProperty(const QString &name,const IPiece *p=0) const=0;
        /// Return the temperature.    
        virtual double          getTemperature()  const=0;
        /// Return the specific heat at constant volume.    
        virtual double          getSpecificHeat() const=0;
        /// Return the coefficient of linear expansion.    
        virtual double          getExpCoef()      const=0;
        /// Return the applied power.    
        virtual double          getAppliedPower() const=0;
        /// Return the temperature fixity flag.    
        virtual bool            getFix()          const=0;
        /// Return the fixity label.
        virtual QString         getFixLabel()     const=0;
        /// Return the temperature increment during last thermal step.    
        virtual double          getTempInc()      const=0;
        /// Return the out-of-balance power after last thermal step.    
        virtual double          getPower()        const=0;

        /// Set the property name.
        virtual bool            setPieceProperty(const QString &name,const QVariant &v,IPiece *p=0) =0;
        /// Set the temperature.    
        virtual void            setTemperature(const double &d)  =0;
        /// Set the specific heat at constant volume.    
        virtual void            setSpecificHeat(const double &d) =0;
        /// Set the coefficient of linear expansion.    
        virtual void            setExpCoef(const double &d)      =0;
        /// Set the applied power.    
        virtual void            setAppliedPower(const double &d) =0;
        /// Set the temperature fixity flag.    
        virtual void            setFix(bool b)                   =0;
        /// Set the temperature increment.    
        virtual void            setTempInc(const double &d)      =0;
        /// Set the out-of-balance power.    
        virtual void            setPower(const double &d)        =0;

    };
} // namespace itascaxd

// EoF
