#pragma once

namespace zone
{
  /// \brief Interface provides access to the constitutive model of a FLAC3D interface.
  /// \ingroup Zone
  class IIntModel
  {
  public:
    enum State { InContact       =0x020, 
                 InShearFailNow  =0x100, 
                 InShearFailPast =0x080, 
                 InTensionFail   =0x200, 
                 InShearBondFail =0x400  };

    /// Given a property name, returns its ID.\n
    /// Possible return values are from 0 to getNumProperties()-1.\n
    /// A return value of -1 indicates no match for \a propName.
    /// Uses current setting for case sensitivity.
    virtual qint32                  getPropertyNumber(const QString &propName) const=0;
    /// Given a property ID, returns its value.\n
    /// Parameter \a propNo must be from 0 to getNumProperties()-1.
    /// \sa setPropertyValue() getNumProperties()
    virtual QVariant                getPropertyValue(int propNo) const=0;
    /// Sets the value for a given property ID number.\n
    /// Parameter \a propNo must be from 0 to getNumProperties()-1. 
    /// \sa getPropertyValue() getNumProperties()
    virtual void                    setPropertyValue(int propNo, const QVariant &val)=0;
    /// Returns the number of properties in the model.
    virtual int                     getNumProperties()=0; 
    /// Given a property ID, returns its name.\n
    /// Parameter \b propNo must be from 0 to getNumProperties(). For a ID number outside the range, returns "Unknown".
    virtual QString                 getPropertyName(int propNo)=0;
    /// Returns true if the model considers itself in contact with something.
    virtual bool                    isInContact() const=0;
    /// Returns true if is slipping now.
    virtual bool                    isSlippingNow() const=0;
    /// Returns true if has slipped in the past.
    virtual bool                    hasSlipped() const=0;
    ///
    virtual quint32 GetState() const = 0;
    virtual void    SetState(const quint32 uMask) = 0;
    virtual bool    GetCanFail() const=0;
    virtual QString getShearStateLabel() const=0;
    virtual QString getNormalStateLabel() const=0;
  };
} // namespace zone
// EoF
