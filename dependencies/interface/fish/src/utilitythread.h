#pragma once

#include "exportfish.h"
#include <deque>
#if defined __LINUX || __WINCMAKE
#   include <functional>
#   include <thread>
#   include <mutex>
#   include <condition_variable>
#endif

// This class represents a background utility thread, to defer execution of things
// Created for deletion of deeply nested objects so you don't blow out the C++ stack
//   in theory will be useful for many other purposes as well.

namespace fish {
    class UtilityThread {
    public:
        FISH_EXPORT UtilityThread();
        FISH_EXPORT ~UtilityThread();
        FISH_EXPORT static UtilityThread *instance() { return global_;  }

        FISH_EXPORT void execute(std::function<void()> func);
        // May want to add ability to specify a callback function when complete.

    private:
        void threadFunc();

        std::thread                       thread_;  // Thread used to execute in background
        std::mutex                        lock_;
        std::condition_variable           event_;
        std::deque<std::function<void()>> tasks_;
        bool                              exit_{false}; // TRUE if background thread should exit

        FISH_EXPORT static UtilityThread *global_;
    };
}
// namespace fish
