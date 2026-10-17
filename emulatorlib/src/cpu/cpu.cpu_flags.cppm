/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <cstddef>
#include <cstdint>

export module emulatorlib.cpu:cpu_flags;

export namespace EmulatorLib
{

class CpuFlags
{
public:
    static CpuFlags Zero;
    static CpuFlags Subtract;
    static CpuFlags HalfCarry;
    static CpuFlags Carry;

    [[nodiscard]] constexpr auto operator|(const CpuFlags& other) const noexcept -> CpuFlags
    {
        return CpuFlags{static_cast<uint8_t>(m_value | other.m_value)};
    }

    [[nodiscard]] constexpr auto operator*() const noexcept -> uint8_t
    {
        return m_value;
    }

    [[nodiscard]] constexpr auto AsByte() const noexcept -> std::byte
    {
        return static_cast<std::byte>(m_value);
    }

private:
    uint8_t m_value = 0;

    explicit constexpr CpuFlags(uint8_t value)
        : m_value(value)
    {
    }
};

} // namespace EmulatorLib