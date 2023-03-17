#pragma once
#ifdef _WIN32
#  define VC_EXTRALEAN
#  define Rectangle FreddyKreuger
#  include <windows.h>
#  undef Rectangle
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#endif
// EoF
