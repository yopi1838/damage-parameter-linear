#pragma once

#include <vector>
#ifdef __LINUX
#include "stdafx.h"
#endif

namespace fish {
    // Struct describing details about a library function
    struct LibraryData {
        QString           desc_;                    ///< Full original description
        QString           name_;                    ///< Fully expanded name
        UVect2            arguments_ = UVect2(0);   ///< Min and max number of valid arguments
        bool              canRead_ = true;          ///< TRUE if can read from the function
        bool              canWrite_ = false;        ///< TRUE if can write to the function
        bool              readThreadSafe_ = false;  ///< TRUE if can read from function in a thread
        bool              writeThreadSafe_ = false; ///< TRUE if can write to function in a thread
        bool              plugin_ = false;         ///< TRUE If was loaded as a plutin
        std::vector<bool> reference_;               ///< If TRUE, that argument will be implicitly referenced.
    };
} // namespace fish
// EoF
