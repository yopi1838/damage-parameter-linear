//idynnode.h
#pragma once

/**
* \file
* \brief Interface to access dynamic node data
*/

namespace zone {
    class IGp;

    /// \brief Interface to access dynamic node data
    /// \ingroup Zone
    class IDynNode {
    public:
        /// Returns dynamic acceleration vector
        virtual DVect  getAcc() const=0;
        /// Returns the maximum timestep allowed *for this node*
        virtual double getMaxTimeStep(const IGp *gp) const=0;
    };
}
// EoF
