/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <array>
#include <coroutine>
#include <exception>
#include <functional>
#include <string_view>
#include <variant>

export module emulatorlib.cpu;

export import :registers;
export import :cpu_registers;
export import :cpu_flags;
export import :instruction_handler_support;

import emulatorlib.address_bus;

export namespace EmulatorLib
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
        void return_void() noexcept {}
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

struct OpcodeHandler
{
    std::string_view name;
    std::function<InstructionHandler(AddressBus&, CpuRegisters&)> handler;
};

class Cpu
{
public:
    Cpu(AddressBus& bus);
    ~Cpu() = default;
    Cpu(Cpu&) = delete;
    Cpu& operator=(Cpu&) = delete;
    Cpu(Cpu&&) = delete;
    Cpu& operator=(Cpu&&) = delete;

    void Step();

    [[nodiscard]] auto Registers() noexcept -> CpuRegisters&;
    [[nodiscard]] auto Registers() const noexcept -> const CpuRegisters&;

private:
    AddressBus& m_bus;
    CpuRegisters m_registers;

    std::array<OpcodeHandler, 256> m_opcodeHandlers;
    InstructionHandler m_handler;
};

} // namespace EmulatorLib
