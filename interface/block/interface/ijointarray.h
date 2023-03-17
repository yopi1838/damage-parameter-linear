#pragma once
// ijointarray.h

/**
 * \file
 * \brief Container to store JointFaceThings.
*/

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IJointFaceThing;

  /// \brief This interface provides access to jointthing array.
  /// \ingroup THREEDEC
  class IJointFaceArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IJointArray.
    virtual const IThing *          getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IJointArray.
    virtual IThing *                getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of joints.
    virtual const IContainer *      getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of joints.
    virtual IContainer *            getIContainer() =0;
    /// Returns number of joints in the container
    virtual quint64                getNumJoints() const=0;
    /// Returns a const pointer to the joint face, with the ID \b id, or nullptr if the face does not exist.
    virtual const IJointFaceThing *   findJointWithID(quint64 id) const=0;
    /// Returns a pointer to the joint face, with the ID \b id, or nullptr if the face does not exist.
    virtual IJointFaceThing *         findJointWithID(quint64 id)=0;
    /// Returns the joint face type identifier
    virtual quint32            getJointFaceType() const=0;
    /// force an update to the joint plane plot data
    virtual bool updateNewJointPlotData(bool plotJoined, QString *errorMessage=0) const=0;
  };

} // end namespace block
// EOF