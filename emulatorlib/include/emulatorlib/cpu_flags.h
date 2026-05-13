/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace EmulatorLib
{

class CpuFlags
{
public:
    static CpuFlags Zero;
    static CpuFlags Subtract;
    static CpuFlags HalfCarry;
    static CpuFlags Carry;

    [[nodiscard]] auto operator|(const CpuFlags& other) const noexcept -> CpuFlags;
    [[nodiscard]] auto operator*() const noexcept -> uint8_t;
    [[nodiscard]] auto AsByte() const noexcept -> std::byte;

private:
    uint8_t m_value = 0;

    explicit CpuFlags(uint8_t value);
};

} // namespace EmulatorLib