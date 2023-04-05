#pragma once

#include "dependencies/interface/base/src/base.h"
#ifdef JMODEL_LIB 
#  define JMODEL_EXPORT EXPORT_TAG
#else
#  define JMODEL_EXPORT IMPORT_TAG
#endif

// EoF
