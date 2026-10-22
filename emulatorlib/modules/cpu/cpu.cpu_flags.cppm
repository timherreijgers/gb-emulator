/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

export module emulatorlib.cpu:flags;

import std;

using std::uint8_t;

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

CpuFlags CpuFlags::Zero{1 << 7};
CpuFlags CpuFlags::Subtract{1 << 6};
CpuFlags CpuFlags::HalfCarry{1 << 5};
CpuFlags CpuFlags::Carry{1 << 4};

} // namespace EmulatorLib
