// izoneinterpolate.h
#pragma once

/**
* \file
* \brief Interface for accessing the zone interpolation class 
*/

namespace itasca {
    class Archive2;
    class IParse;
}

namespace zone {
    using itasca::Archive2;
    using itasca::IParse;
    class IGp;
    class IZoneData;

    /// \brief Interface for accessing the zone interpolation class
    /// \ingroup Zone
    class IZoneInterpolate {
    public:
        /// Interpolation method enumeration.
        enum Method { 
            ConstMethod,    /*!< Constant in zone. */
            AverageMethod,  /*!< Volumetric Averaging. */
            IDWMethod,      /*!< Inverse Distance Weighting. */
#ifdef THREED
            PolyMethod      /*!< Polynomial Extrapolation. */    
#endif
        };
        /// Returns a const pointer to a QObject representing this object.
        virtual const QObject *getQObject() const=0;
        /// Returns a pointer to a QObject representing this object.
        virtual QObject *      getQObject()=0;
        /// Returns an integer corresponding to the active interpolation method.
        /** See enum Method for details
        */
        virtual int            getMethod() const=0;
        virtual bool           getMethodActive() const=0;
        /// Returns a QStringList comprising the keywords corresponding to all interpolation methods, separated by commas.
        virtual QStringList    getMethodKeywords() const=0;
        /// Returns a QStringList comprising names corresponding to all interpolation methods, separated by commas.
        virtual QStringList    getMethodNames() const=0;
        /// Returns the radii ratio used by the Inverse Distance Weighting method.
        virtual double         getRadRatio() const=0;
        /// Returns the power used by the Inverse Distance Weighting method.
        virtual double         getPower() const=0;
        /// Returns the tolerance used to assess quantity gradient by the Polynomial Extrapolation method.
        virtual double         getTolerance() const=0;
        /// Returns true if the radii ratio parameter is used during the computation.
        /** So far, returns true if the active method is the Inverse Distance Weighting method. */
        virtual bool           getRadRatioActive() const=0;
        /// Returns true if the power parameter is used during the computation.
        /** So far, returns true if the active method is the Inverse Distance Weighting method. */
        virtual bool           getPowerActive() const=0;
        /// Returns true if the tolerance parameter is used during the computation.
        /** So far, returns true if the active method is the Polynomial Extrapolation method. */
        virtual bool           getToleranceActive() const=0;
        /// Returns the interpolated field value at gridpoint \b gp.
        /**
        \param gp is the gridpoint
        \param zoneData is the Zone data field 
        */
        virtual double getGpValue(const IGp *gp,const IZoneData *zoneData,quint32 thread=0,quint32 block=0) const=0;

        /// Indicates that the interpolation system should ingnore zones with null mechanical model (default is true)
        virtual void setHideMechNull(bool b)=0;
        /// Indicates that the interpolation system should ingnore zones with null fluid model (default is false)
        virtual void setHideFluidNull(bool b)=0;
        /// Indicates that the interpolation system should ingnore zones with null thermal model (default is false)
        virtual void setHideThermNull(bool b)=0;
        /// Returns true if the interplation system is ignoring zones with a null mechanical model.
        virtual bool getHideMechNull() const = 0;
        /// Returns true if the interplation system is ignoring zones with a null fluid model.
        virtual bool getHideFluidNull() const = 0;
        /// Returns true if the interplation system is ignoring zones with a null thermal model.
        virtual bool getHideThermNull() const = 0;

        /// Initializes the interpolation computation
        /**
        \param zoneData is the Zone data field 
        */
        virtual void initialize(const IZoneData *zoneData)=0;
        virtual void reset()=0;
        /// Destroys the current instanciation 
        virtual void destroy()=0;

        virtual void save(Archive2 &a) const=0;
        virtual bool restore(Archive2 &a,quint64 label)=0;
        virtual bool parse(IParse &par)=0;
        virtual QStringList getCommandSwitches() const=0;

        // public slots
        virtual void setActive(bool active)=0;
        /// Sets the current interpolation method.
        /**
        \param method is an integer between 0 and 2. See Method enumeration for details. \n
        Calling this function will result in resetting the interpolated data field if the interpolation method is changed. 
        */
        virtual void setMethod(int method)=0;
        /// Sets the <b> radii ratio </b> parameter to be used by the Inverse Distance Weighting method.
        virtual void setRadRatio(const double &radRatio)=0;
        /// Sets the \b power parameter to be used by the Inverse Distance Weighting method.
        virtual void setPower(const double &power)=0;
        /// Sets the \b tolerance parameter to be used by the Polynomial Extrapolation method.
        virtual void setTolerance(const double &tol)=0;

        // signals
        /// Signal emitted when the interpolation method is changed.
        virtual void methodChanged(int method)=0;
        /// Signal emitted when the <b> radii ratio </b> parameter is changed.
        virtual void radRatioChanged(const double &radRatio)=0;
        /// Signal emitted when the \b power parameter is changed.
        virtual void powerChanged(const double &power)=0;
        /// Signal emitted when the \b tolerance parameter is changed.
        virtual void toleranceChanged(const double &tol)=0;
        virtual void methodActiveChanged(bool b)=0;
        /// Signal emitted when the <b> radii ratio </b> parameter becomes active.
        virtual void radRatioActiveChanged(bool b)=0;
        /// Signal emitted when the \b power parameter becomes active.
        virtual void powerActiveChanged(bool b)=0;
        /// Signal emitted when the \b tolerance parameter becomes active.
        virtual void toleranceActiveChanged(bool b)=0;
    };
} // namespace zone
// EoF
