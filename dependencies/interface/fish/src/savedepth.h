#pragma once

// Use this class to track the save depth on an archive instance.
// Aggregate types do something different depending on how nested 
//    they end up being, and if the archive is a full Save/Results file
//    or a Buffer.
#ifdef __LINUX
#include "stdafx.h"
#endif

namespace fish {
    class SaveDepth {
    public:
        class Inc {
        public:
            Inc(quint32 instance) : instance_(instance) { SaveDepth::increment(instance);  }
            ~Inc() { SaveDepth::decrement(instance_);  }
            quint64 depth() const { return SaveDepth::depth(instance_);  }
        private:
            quint32 instance_ = 0;
        };
        static constexpr quint64 limit_ = 128;
        
        static quint64 increment(quint32 instance);
        static quint64 decrement(quint32 instance);
        static quint64 depth(quint32 instance);
    };
} // namespace fish
// EoF

