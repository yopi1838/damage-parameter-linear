#pragma once

namespace pfc {
    class IFWall;

    class IHalfFacetParent {
    public:
        /// TType type used for type casting.
        inline static const TType type_ = 0x558c8ab7;

        virtual IFWall *        getParentFWall() =0;
        virtual const IFWall *  getParentFWall() const=0;
    };
}