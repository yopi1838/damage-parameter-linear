#pragma once

namespace pfc {
    class IFWall;

    class IHalfVertexParent {
    public:
        /// TType type used for type casting.
        inline static const TType type_ = 0x558c8c6f;

        virtual IFWall *        getParentFWall() =0;
        virtual const IFWall *  getParentFWall() const=0;
    };
}