#pragma once
// ifaceblockaarray.h

/**
 *  \file
 *  \brief Container to store blockThings that are face blocks.
 *
 */

#include "iblockdef.h"

namespace itasca
{
  class IContainer;
  class IThing;
}

namespace block
{
  using namespace itasca;

  class IBlockThing;
  class FaceBlockThing;

  /// \brief Container to store blockThings that are face blocks.
  ///  ID of BlockThing is stored in offset $KBID in Fortran block linked list
  /// \ingroup THREEDEC
  class IFaceBlockArray
  {
  public:

    /// Returns a const pointer to an IThing interface representing the IBlockArray.
    virtual const IThing *     getIThing() const=0;
    /// Returns a pointer to an IThing interface representing the IBlockArray.
    virtual IThing *           getIThing()=0;
    /// Returns a const pointer to the IContainer interface representing the global list of blocks.
    virtual const IContainer * getIContainer() const=0;
    /// Returns a pointer to the IContainer interface representing the global list of blocks.
    virtual IContainer *       getIContainer() =0;
    /// Returns a const pointer to the block, IBlock, with the ID \b id, or 0 if the block does not exist.
    virtual const IBlockThing *      findBlockWithID(quint64 id) const=0;
    /// Returns a pointer to the block, IBlock, with the ID \b id, or 0 if the block does not exist.
    virtual IBlockThing *            findBlockWithID(quint64 id)=0;
    /// Returns the block type identifier
    virtual quint32            getBlockType() const = 0;

    /// Returns the tag for the block extra variable change notice
    virtual uint               getBlockExtraNotice() const=0;
    /// Returns the tag for the block group change notice
    virtual uint               getGroupNotice() const=0;
    /// Returns the tag for the block geometry change notice
    virtual uint               getGeometryNotice() const=0;
    /// Returns the tag for join condition change notice
    virtual uint               getJoinNotice() const=0;
    /// Returns the tag for the boundary geometry change notice
    virtual uint               getBoundaryNotice() const=0;
    /// Returns the tag for the material change notice
    virtual uint               getMaterialNotice() const=0;
    /// Returns the tag for the block hide state notice
    virtual uint                 getHideNotice() const=0;
    /// Return block closest to input location
    virtual FaceBlockThing *     findBlock(const DVect3 &dv)=0;

  };

} // end namespace block
// EOF