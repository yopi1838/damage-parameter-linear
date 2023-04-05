#pragma once
// iblockaarray.h

/**
 *  \file
 *  \brief Container to store blockThings.
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
  class BlockThing;

  /// \brief Container to store blockThings.
  ///  ID of BlockThing is stored in offset $KBID in Fortran block linked list
  /// \ingroup THREEDEC
  class IBlockArray
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
    virtual quint32            getBlockType() const=0;

    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimBit() returns a bit index that can be used with the IBlock::getBit() and
    ///   IBlock::setBit() functions.
    /// This function is thread-safe (IBlock::getBit and IBlock::setBit are not).
    /// \sa releaseBit() IBlock::setBit() IBlock::getBit()
    /// \brief Claims a bit from the blocks utility bit field.
    virtual int                claimBit() const=0;
    /// \brief Releases a bit index previously obtained with claimBit().  Thread-safe.
    virtual void               releaseBit(int bit) const=0;
    /// It is sometimes useful to be able to store flags and data in a zone
    ///   during calculations unrelated to the actual model cycling logic.\n
    /// Two utility arrays are available for this purpose, they are primarily
    ///   intended to be briefly claimed, used, and then released for back
    ///   for use in other algorithms.\n
    /// claimValue() returns a value index that can be used with the IBlock::getValue() and
    ///   IBlock::setValue() functions.
    /// This function is thread-safe (IBlock::getValue and IBlock::setValue are not).
    /// \sa releaseBit() IBlock::setValue() IBlock::getValue()
    /// \brief Claims a bit from the blocks utility value array.
    virtual int                claimValue() const=0;
    /// \brief Releases a value index back to the zones utility value array.  Thread-safe.
    virtual void               releaseValue(int bit) const=0;
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
    virtual BlockThing *     findBlock(const DVect3 &dv, bool visible=false)=0;
    /// Return block that contains the input location dv.
    /// Input visible indicates if all blocks are searched (false) or only visible blocks (true)
    /// Return nullptr if not found.
    virtual BlockThing *     findBlockContaining(const DVect3 &dv,bool visible)=0;
  };

} // end namespace block
// EOF