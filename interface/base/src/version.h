#pragma once
// Sets framework version numbers
#include "../../baseversion.txt"
namespace version {
    const unsigned major = MAJOR_VERSION; // Currently version 8
    const unsigned update = UPDATE_VERSION;
    const std::string revision = REVISION_VERSION; // SVN revision, set by build server.
    BASE_EXPORT extern const char *targetPlatform;
}
#undef MAJOR_VERSION
#undef UPDATE_VERSION
#undef REVISION_VERSION
// EoF
