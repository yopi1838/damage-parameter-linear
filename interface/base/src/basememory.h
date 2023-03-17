#include <stdlib.h>

#pragma once
/** \file basememory.h
 * \brief Comment point for memory allocation in all modules
 *
 * See documentation in MEMORY.CPP for a discussion of what is done in debug mode.
 * Exceptions thrown by these routines are all derived from std::runtime_error.
  * \addtogroup Base Base interface specification
  * @{
 */

#include "basedef.h"
#include <new>
 //#pragma warning(disable:3663)

namespace itasca {
    namespace memory {
        typedef void(*MemoryCleanupFunction)();

        void initialize();
        void shutdown();
        BASE_EXPORT bool                  heapCheck(void *memptr = nullptr);
        BASE_EXPORT unsigned long long    getMemory();
        BASE_EXPORT MemoryCleanupFunction addCleanupFunction(MemoryCleanupFunction func);
        BASE_EXPORT void *                imalloc(size_t s, const char *file, unsigned len);
        BASE_EXPORT void *                imalloc(size_t s);
        BASE_EXPORT void                  ifree(void *v);
        BASE_EXPORT void *                irealloc(void *v, size_t s);
    } // namespace memory
} // namespace itasca

#ifdef _WIN32
#ifdef __INTEL_COMPILER
#pragma warning(disable:3663)
inline void *operator new(size_t s) { return itasca::memory::imalloc(s); }
inline void *operator new[](size_t s) { return itasca::memory::imalloc(s); }
inline void  operator delete(void *v) { itasca::memory::ifree(v); }
inline void  operator delete[](void *v) { itasca::memory::ifree(v); }
#endif
# ifndef __PLACEMENT_NEW_INLINE
void *operator new(size_t, void *v) { return(v); }
void  operator delete(void *, void *) { }
#   define __PLACEMENT_NEW_INLINE
# endif
# ifndef __PLACEMENT_VEC_NEW_INLINE
void *operator new[](size_t, void *v) { return(v); }
void  operator delete[](void *, void *) {}
# define __PLACEMENT_VEC_NEW_INLINE
# endif

#  ifdef _DEBUG
/// Using NEW or NEWC adds file and line number info to the allocated memory in debug mode.
///   NEWC automatically makes the comment the constructor call - for example:
///   NEWC(Fred(x,y)) expands to new("Fred(x,y)",__FILE__,__LINE__) Fred(x,y)
#    define NEW(x) new(x,__FILE__,__LINE__)
#    define NEWC(x) new(#x,__FILE__,__LINE__) x ///< \copydoc NEW
inline void *operator new(size_t s, const char *, const char *file, unsigned line)
{
    return itasca::memory::imalloc(s, file, line);
}
inline void *operator new[](size_t s, const char *, const char *file, unsigned line)
{
    return itasca::memory::imalloc(s, file, line);
}
inline void  operator delete(void *v, const char *, const char *, unsigned) { itasca::memory::ifree(v); }
inline void  operator delete[](void *v, const char *, const char *, unsigned) { itasca::memory::ifree(v); }
#  else // _DEBUG
/// Using NEW or NEWC adds file and line number info to the allocated memory in debug mode.
///   NEWC automatically makes the comment the constructor call - for example:
///   NEWC(Fred(x,y)) expands to new("Fred(x,y)",__FILE__,__LINE__) Fred(x,y)
#    define NEW(x) new
#    define NEWC(x) new x ///< \copydoc NEW
#  endif // _DEBUG
#endif // _WIN32


#ifdef __LINUX
  /// These functions are provided to replace the default new and delete functionality of the library.
inline void* operator new(size_t s)  { return itasca::memory::imalloc(s); }
/// These functions are provided to replace the default new and delete functionality of the library.
inline void* operator new[](size_t s) { return itasca::memory::imalloc(s); }
/// These functions are provided to replace the default new and delete functionality of the library.
inline void  operator delete(void* v)  { itasca::memory::ifree(v); } 
/// These functions are provided to replace the default new and delete functionality of the library.
inline void  operator delete[](void* v)  { itasca::memory::ifree(v); }
/*
# ifndef __PLACEMENT_NEW_INLINE
    inline void *operator new(size_t,void *v) { return(v); }
    inline void  operator delete(void *,void *) { }
#   define __PLACEMENT_NEW_INLINE
# endif
# ifndef __PLACEMENT_VEC_NEW_INLINE
    inline void *operator new[](size_t,void *v) { return(v); }
    inline void  operator delete[](void *,void *) { }
# define __PLACEMENT_VEC_NEW_INLINE
# endif
*/
#  ifdef _DEBUG
/// Using NEW or NEWC adds file and line number info to the allocated memory in debug mode.
///   NEWC automatically makes the comment the constructor call - for example:
///   NEWC(Fred(x,y)) expands to new("Fred(x,y)",__FILE__,__LINE__) Fred(x,y)
#    define NEW(x) new(x,__FILE__,__LINE__)
#    define NEWC(x) new(#x,__FILE__,__LINE__) x ///< \copydoc NEW
inline void* operator new(size_t s, const char*, const char* file, unsigned line)
{
    return itasca::memory::imalloc(s, file, line);
}
inline void* operator new[](size_t s, const char*, const char* file, unsigned line)
{
    return itasca::memory::imalloc(s, file, line);
}
inline void  operator delete(void* v, const char*, const char*, unsigned) { itasca::memory::ifree(v); }
inline void  operator delete[](void* v, const char*, const char*, unsigned) { itasca::memory::ifree(v); }
#  else // _DEBUG
/// Using NEW or NEWC adds file and line number info to the allocated memory in debug mode.
///   NEWC automatically makes the comment the constructor call - for example:
///   NEWC(Fred(x,y)) expands to new("Fred(x,y)",__FILE__,__LINE__) Fred(x,y)
#    define NEW(x) new
#    define NEWC(x) new x ///< \copydoc NEW
#  endif // _DEBUG
#endif // ifdef linux


/// @}
// EoF
