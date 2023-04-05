#pragma once
// isellinkmodel.h

/**
  * \file
  * \brief Interface to provide access to a structural element constitutive model.
  * \addtogroup SEL
  * @{
  */

/// \brief Interface to the structural elements base class
namespace sel {
    class ISELLinkModel {
    public:
        /// Possible yield states
        enum YieldState {
            NO,  /*!< no yield (has never yielded) */
            NOW, /*!< now yielding */
            PAST /*!< has yielded in the past */
        };
        /// Returns a constitutive model type
        virtual TType               getType() const=0;
        /// Returns the name of the model
        virtual QString             getName() const=0;
        /// Returns the keyword used to 
        virtual QString             getKeyword() const=0;
        /// Returns the effective area over which CM acts, see setArea()
        virtual double              getArea() const=0;
        /// Sets the effective area over which CM acts, see getArea()
        virtual void                setArea(const double &)=0;
        /// Returns the stiffness per unit area, see setK()
        virtual double              getK() const=0;
        /// Sets the stiffness per unit area, see getK()
        virtual void                setK(const double &)=0;
        /// Returns a flag that indicates if there are gaps when crossing over at F=0, see setGap()
        virtual bool                getGap() const=0;
        /// Sets a flag that indicates if there are gaps when crossing over at F=0, see getGap()
        virtual void                setGap(bool)=0;
        /// Returns the tensile-yield force (positive value) if parameter \b tensile is true, or the
        ///  compressive-yield force (positive value) if tensile is false. See setYForce()
        virtual double              getYForce(bool tensile) const=0;
        /// Sets the tensile-yield force (positive value) if parameter \b tensile is true, or the
        ///  compressive-yield force (positive value) if tensile is false. See getYForce()
        virtual void                setYForce(const double &force,bool tensile)=0;
        /// Returns the current displacement in CM.
        virtual double              getDisp() const=0;
        /// Returns the  force in "spring"
        virtual double              getForce() const=0;
        /// Returns the gap present on side \a s (0 or 1).
        virtual double              getGapVal(quint32 s) const=0;
        /// Returns the current yield state
        virtual YieldState          getYieldState() const=0;
        /// Returns FALSE if the degree-of-freedom \a dof (0-5) can not be used with this model.
        virtual bool                getAllowedDOF(quint32 dof) const=0;
        /// Returns a list of all property names this type of model supports.
        virtual QStringList getProperties() const=0;
        /// Returns the value of the property at index \a index in the list returned from getProperties().
        /// An invalid \a index (too large) will return a null QVariant.
        virtual QVariant    getProperty(quint32 index) const=0;
        /// Sets the value of the property at index \a index int the list returned from getProperties().
        /// If \a index is invalid (too large), or the variant \a var cannot be converted to the required type,
        ///    then FALSE is returns.  Returns TRUE on success.
        virtual bool        setProperty(quint32 index,const QVariant &var)=0;
    };
} // namespace sel
/// @}
// EoF
