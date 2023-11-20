// sources:
// - https://itnext.io/c-20-coroutines-complete-guide-7c3fc08db89d (nice flow graph)
// - https://www.youtube.com/watch?v=w-dmOHhBX9o&t=44s

// returns:
// Before coroutine call
// FunctionTask::promise_type::promise_type()
// FunctionTask Task::promise_type::get_return_object()
// FunctionTask::Task()
// Functionstd::__n4861::suspend_never Task::promise_type::initial_suspend()
// Inside coroutine
// Functionvoid Task::promise_type::return_void()
// Functionstd::__n4861::suspend_never Task::promise_type::final_suspend()
// FunctionTask::promise_type::~promise_type()
// After coroutine call
// FunctionTask::~Task()

#include <coroutine>
#include <iostream>

#define LOG_FUNCTION_NAME std::cout << "Function" << __PRETTY_FUNCTION__ << '\n';

// The caller-level type
struct Task {
    Task() {
        LOG_FUNCTION_NAME
    }

    ~Task() {
        LOG_FUNCTION_NAME
    }

    // The coroutine level type
    struct promise_type {
        promise_type() {
            LOG_FUNCTION_NAME
        }

        ~promise_type() {
           LOG_FUNCTION_NAME
        }

        Task get_return_object() {
            LOG_FUNCTION_NAME
            return {};
        }
       
        std::suspend_never initial_suspend() {
            LOG_FUNCTION_NAME
            return {};
        }
        
        std::suspend_never final_suspend() noexcept {
            LOG_FUNCTION_NAME
            return {};
        }
        
        void return_void() {
            LOG_FUNCTION_NAME
        }

        void unhandled_exception() {
            LOG_FUNCTION_NAME
        }
    };
};

Task myCoroutine() {
    std::cout << "Inside coroutine" << '\n';
    co_return; // make it a coroutine
}

int main() {
    std::cout << "Before coroutine call" << '\n';
    Task x = myCoroutine();
    std::cout << "After coroutine call" << '\n';
}