#pragma once

#include "conmodelbase.h"

/**
* \file
* \brief The base class for constitutive model plug-ins.
* \addtogroup models
* @{
*/

namespace models {
    /// \brief Converts Bulk and Shear Modulus to Young's Modulus and Poisson's Ratio.
    CONMODEL_EXPORT void getYPfromBS(const Double &bulk,const Double &shear,Double *young,Double *poisson);
    /// \brief Converts Young's Modulus and Poisson's Ratio to Bulk and Shear Modulus.
    CONMODEL_EXPORT void getBSfromYP(const Double &young,const Double &poisson,Double *bulk,Double *shear);
} // namespace models

/// @}
// EoF
