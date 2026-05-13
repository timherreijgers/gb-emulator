/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <coroutine>
#include <variant>

namespace EmulatorLib
{

struct InstructionHandler
{
    struct promise_type
    {
        InstructionHandler get_return_object() { return {InstructionHandler(this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::monostate) noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    using handle = std::coroutine_handle<promise_type>;
    handle m_handle;

    explicit InstructionHandler(promise_type * promise)
        : m_handle(handle::from_promise(*promise))
    {
    }
};

} // namespace EmulatorLib