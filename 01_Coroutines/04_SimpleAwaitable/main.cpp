// sources:
// - https://itnext.io/c-20-coroutines-complete-guide-7c3fc08db89d (nice flow graph)
// - https://www.youtube.com/watch?v=w-dmOHhBX9o&t=44s

// see flow graph at: https://youtu.be/w-dmOHhBX9o?si=bqAgt7AC62cjSiVT&t=320

#include <coroutine>
#include <iostream>

// https://en.cppreference.com/w/cpp/language/coroutines
struct Awaitable {
    // Returning true will skip directly to await_resume.
    // This is usefull when, for example, awaiting a resource that has already been loaded.
    bool await_ready();


    // implement a await_suspend method with one of the following return types: void, bool or std::coroutine_handle<>

    // important: await_suspend is called after the coroutine is suspended.
    // this means that the coroutine can be treated as data, for example, passed between threads.

    // the void version simply returns coroutine to the caller, caller calls .resume
    void await_suspend(std::coroutine_handle<>) {}

    // returning true returns coroutine to the caller, caller calls .resume
    // returning false resumes the coroutine (note: this is diffrent from returning true in
    // await_ready since, in that case, the coroutine is never suspended)
    bool await_suspend(std::coroutine_handle<>) {}

    // this is the most complex option, returning a coroutine handle.
    // the generated code will .resume the returned handle, and resume original coroutine after
    // coroutine behind handle has suspended or finished. this is important for chaining coroutines.
    // a rule of thumb is: if you are resuming a coroutine handle directly inside await_suspend,
    // consider using this version instead.
    std::coroutine_handle<> await_suspend(std::coroutine_handle<>) {}


    // this also exists...
    bool await_resume();
}