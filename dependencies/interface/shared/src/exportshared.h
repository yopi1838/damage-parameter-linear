#pragma once

#include "base/src/basedef.h"

#ifdef SHARED_LIB
#  define SHARED_EXPORT EXPORT_TAG
#else
#  define SHARED_EXPORT IMPORT_TAG
#endif

// EoF
