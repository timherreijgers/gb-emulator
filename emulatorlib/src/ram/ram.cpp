/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.ram;

import std;

using std::uint16_t;

namespace EmulatorLib
{

void Ram::WriteToAddress(uint16_t address, std::byte data) noexcept
{
    if (address >= g_startAddress && address <= g_endAddress)
    {
        m_data[address - g_startAddress] = data;
    }
}

auto Ram::ReadFromAddress(uint16_t address) const noexcept -> std::byte
{
    if (address < g_startAddress || address > g_endAddress)
    {
        return {};
    }

    return m_data[address - g_startAddress];
}

} // namespace EmulatorLib
