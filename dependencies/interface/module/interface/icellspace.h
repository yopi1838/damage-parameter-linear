#pragma once
// icontainercell.h

/**
    * \file
    * \brief Interface to a "refining" cell space.
    */


#include "dim.h"

namespace itascaxd {

    /// \brief Interface to a "refining" cell space.
    /** 
        This is an implementation of a "refining" cell space
        A "refining" cell space refers to a space subdivided into square cells and an array of storage
        representing the cells in the domain.  Each cell can contain another cell space.
        Objects are mapped into the cells along with their extents.  
        The cell space assumes that the when an object extent changes that the user has ensured that the object 
        has been removed and inserted back into the cell space with the appropriate extent.  
    * \ingroup module
    */
    class ICellSpace {
    public:
        /// Returns a pointer to its QObject 
        virtual const QObject *     getObject() const=0;
        /// Returns the cell space corresponding to a given an index
        virtual const ICellSpace *  isRefined(int) const=0;
        /// Returns true if there is a corresponding cell space corresponding to a given an index 
        virtual bool                isNonEmpty(int) const=0;
        /// Returns a vector that contains the numbers of cells in each coordinate direction.
        virtual IVect               getNumCells() const=0;
        /// Returns the length of cell side (the cell is square)
        virtual double              getCellLen() const=0;
        /// Returns the cell extent
        virtual DExtent             getCellExtent() const=0;
        /// Returns the NoticeManager ID for changes in the topology of the cell space.
        virtual uint                getTopologyChangedNotice() const=0;
    };
}
// EoF
