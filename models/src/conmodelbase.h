#pragma once

#include "base/src/base.h"

#ifdef CONMODEL_LIB
#  define CONMODEL_EXPORT EXPORT_TAG
#else
#  define CONMODEL_EXPORT IMPORT_TAG
#endif

// EoF
