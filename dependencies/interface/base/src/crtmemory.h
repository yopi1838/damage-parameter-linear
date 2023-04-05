#pragma once
// C runtime library memory tracking.
//  Not being used right now.

#include "basedef.h"
#ifdef _DEBUG

namespace itasca {
    namespace memory {
        void  crtInitialize();
        void  crtShutdown();
        bool  crtHeapCheck();
        void *crtAlloc(size_t s,const char *file,int line);
        void  crtFree(void *v);
        bool isValidHeapPointer(void *p);
    } // namespace memory
} // namespace itasca

#endif
