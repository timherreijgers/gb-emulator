/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <coroutine>
#include <exception>
#include <variant>

namespace EmulatorLib
{

struct InstructionHandler
{
    struct promise_type
    {
        std::exception_ptr m_exception;

        InstructionHandler get_return_object() { return {InstructionHandler(this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::monostate) noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() noexcept { m_exception = std::current_exception(); }
    };

    InstructionHandler() = default;

    ~InstructionHandler()
    {
        if (m_handle)
            m_handle.destroy();
    }

    InstructionHandler(InstructionHandler&) = delete;
    InstructionHandler operator=(InstructionHandler&) = delete;

    InstructionHandler(InstructionHandler&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    InstructionHandler& operator=(InstructionHandler&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    using handle = std::coroutine_handle<promise_type>;
    handle m_handle;

    explicit InstructionHandler(promise_type * promise)
        : m_handle(handle::from_promise(*promise))
    {
    }

    [[nodiscard]] auto HasException() const noexcept -> bool
    {
        return m_handle && m_handle.promise().m_exception;
    }

    [[nodiscard]] auto Exception() const noexcept -> std::exception_ptr
    {
        return m_handle.promise().m_exception;
    }
};

} // namespace EmulatorLib