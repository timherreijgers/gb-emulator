/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"

#include <coroutine>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <variant>

namespace EmulatorLib
{

template <typename T>
struct Register
{
    T value;

    auto operator=(std::remove_cvref_t<T> t) -> Register<T>&
    {
        value = t;
        return *this;
    }

    operator T() const noexcept
    {
        return value;
    }

    [[nodiscard]] auto operator+(const T& other) const noexcept -> T
    {
        return value + other;
    }

    void operator+=(const T& other) noexcept
    {
        value += other;
    }

    auto operator++(int) noexcept -> Register
    {
        const auto copy = *this;
        ++value;
        return copy;
    }

    [[nodiscard]] auto
        operator<=>(const Register<T>& other) const noexcept
    {
        return value <=> other.value;
    }

    [[nodiscard]] auto operator<=>(const T& other) const noexcept -> std::strong_ordering
    {
        return value <=> other;
    }

    [[nodiscard]]
    auto LowerByteAsRegister()
        -> Register<std::byte&>
        requires std::same_as<std::remove_cvref_t<T>, std::uint16_t>
    {
        return Register<std::byte&>{*reinterpret_cast<std::byte *>(&value)};
    }

    [[nodiscard]]
    auto UpperByteAsRegister()
        -> Register<std::byte&>
        requires std::same_as<std::remove_cvref_t<T>, std::uint16_t>
    {
        return Register<std::byte&>{*(reinterpret_cast<std::byte *>(&value) + 1)};
    }
};

using Register16Bit = Register<uint16_t>;
using Register8Bit = Register<std::byte>;
using Register8BitRef = Register<std::byte&>;

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

struct CpuFlags
{
private:
    uint8_t m_value = 0;

    explicit constexpr CpuFlags(uint8_t value) : m_value(value) {}

public:
    static CpuFlags Zero;
    static CpuFlags Subtract;
    static CpuFlags HalfCarry;
    static CpuFlags Carry;

    [[nodiscard]] auto operator|(const CpuFlags& other) const noexcept -> CpuFlags
    {
        return CpuFlags{static_cast<uint8_t>(m_value | other.m_value)};
    }

    [[nodiscard]] auto operator*() const noexcept -> uint8_t
    {
        return m_value;
    }

    [[nodiscard]] auto AsByte() const noexcept -> std::byte
    {
        return static_cast<std::byte>(m_value);
    }
};

struct CpuRegisters
{
    CpuRegisters() = default;

    CpuRegisters(CpuRegisters&)
    {
        throw std::runtime_error("Not implemented");
    }

    CpuRegisters operator=(CpuRegisters& other)
    {
        return CpuRegisters(other);
    }

    Register8Bit instructionRegister;
    Register8Bit interruptEnableRegister;
    Register8Bit accumulator;
    Register8Bit flags;

    Register16Bit bcRegister;
    Register8BitRef bRegister = bcRegister.UpperByteAsRegister();
    Register8BitRef cRegister = bcRegister.LowerByteAsRegister();

    Register16Bit deRegister;
    Register8BitRef dRegister = deRegister.UpperByteAsRegister();
    Register8BitRef eRegister = deRegister.LowerByteAsRegister();

    Register16Bit hlRegister;
    Register8BitRef hRegister = hlRegister.UpperByteAsRegister();
    Register8BitRef lRegister = hlRegister.LowerByteAsRegister();

    Register16Bit wzRegister;
    Register8BitRef wRegister = wzRegister.UpperByteAsRegister();
    Register8BitRef zRegister = wzRegister.LowerByteAsRegister();

    Register16Bit programCounter;
    Register16Bit stackPointer;
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

    [[nodiscard]] auto Registers() const noexcept -> const CpuRegisters&;

private:
    std::optional<InstructionHandler> m_handler = std::nullopt;
    AddressBus& m_bus;
    CpuRegisters m_registers{};
};

} // namespace EmulatorLib