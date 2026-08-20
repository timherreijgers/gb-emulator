/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib
{

CpuFlags CpuFlags::Zero{1 << 7};
CpuFlags CpuFlags::Subtract{1 << 6};
CpuFlags CpuFlags::HalfCarry{1 << 5};
CpuFlags CpuFlags::Carry{1 << 4};

CpuFlags::CpuFlags(uint8_t value)
    : m_value(value)
{
}

auto CpuFlags::operator|(const CpuFlags& other) const noexcept -> CpuFlags
{
    return CpuFlags{static_cast<uint8_t>(m_value | other.m_value)};
}

auto CpuFlags::operator*() const noexcept -> uint8_t
{
    return m_value;
}

auto CpuFlags::AsByte() const noexcept -> std::byte
{
    return static_cast<std::byte>(m_value);
}

} // namespace EmulatorLib