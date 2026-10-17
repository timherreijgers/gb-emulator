/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <array>
#include <cstddef>
#include <cstdint>

export module emulatorlib.ram;

import emulatorlib.address_bus;

export namespace EmulatorLib
{

class Ram : public AddressBusAddressable
{
public:
    void WriteToAddress(uint16_t address, std::byte data) noexcept override;
    [[nodiscard]] auto ReadFromAddress(uint16_t address) const noexcept -> std::byte override;

private:
    static constexpr uint16_t g_startAddress = 0xC000;
    static constexpr uint16_t g_endAddress = 0xDFFF;

    std::array<std::byte, g_endAddress - g_startAddress + 1> m_data{};
};

} // namespace EmulatorLib