#pragma once

#include "base/src/export.h"
#ifdef __LINUX
#include <vector>
#endif

#ifdef __LINUX
#   ifdef FISH_LIB
#       define FISH_EXPORT 
#   else
#       define FISH_EXPORT 
#   endif
#else
#   ifdef FISH_LIB
#       define FISH_EXPORT Q_DECL_EXPORT
#   else
#       define FISH_EXPORT Q_DECL_IMPORT
#   endif
#endif

namespace fish {
    class Iterator;

    typedef std::vector<Iterator> IteratorList;
}


// EoF