#pragma once
// iconfigure.h

/**
  * \file
  * \brief This interface provides access to configuration settings.
  */

namespace block
{
  /// \brief This interface provides linked list offsets to access to configuration settings.
  /// \ingroup THREEDEC
  class IConfigure
  {
  public:
    /// thermal mode (must be applied before blocks are created)
    virtual bool thermal() const=0;
    /// dynamic mode (must be applied before blocks are created)
    virtual bool dynamic() const=0;
    /// left handed coordinate system (must be applied before blocks are created)
    virtual bool leftHandedSystem() const=0;
    /// liner mode
    virtual bool liner() const=0;
    /// creep mode
    virtual bool creep() const=0;
    /// user defined constitutive models
    virtual bool cppUserDefinedModels() const=0;
    /// fluid flow
    virtual bool groundWaterFlow() const=0;
    /// higher order tetras (must be applied before blocks are created)
    virtual bool highOrderTetra() const=0;
    /// allows energy to be accumulated
    virtual bool energy() const=0;
    /// allows creation of finite element blocks
    virtual bool feblock() const=0;
    /// size of main data array
    virtual int  mainArraySize() const=0;
  };
} // namespace block
// EoF