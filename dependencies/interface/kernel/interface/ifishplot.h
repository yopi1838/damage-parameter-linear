// ifishplot.h
#pragma once

/**
    * \file
    * \brief This interface exposes a plot data interface (IGeneralPlot) to FISH.
    */

#include "module/interface/dim.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    using namespace itasca;

    class IGeneralPlot;

    /// \brief This interface exposes a plot data interface (IGeneralPlot) to FISH.
    /// \ingroup kernel
    class IFishPlot {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bdf;
        /// Returns a ponter to IGeneralPlot.  
        virtual IGeneralPlot *getGeneralPlotInterface() const=0;
        /// Returns a const pointer to IGeneralPlot, via IThing.  
        virtual const IThing *toIThing() const=0;
        /// Returns a pointer to IGeneralPlot, via IThing.  
        virtual IThing *      toIThing()=0;
        /// Sets the IGeneralPlot for FISH.  
        virtual void          setGeneralPlotInterface(IGeneralPlot *)=0;
    };
} // namespace itascaxd

// EoF