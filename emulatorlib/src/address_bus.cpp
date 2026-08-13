/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"

#include "utilitylib/byte_utils.h"

#include <utility>

namespace EmulatorLib
{

AddressBus::AddressBus(std::vector<std::reference_wrapper<AddressBusAddressable>> devices)
    : m_devices(std::move(devices))
{
}

void AddressBus::WriteToAddress(uint16_t address, std::byte data) noexcept
{
    for (auto& device : m_devices)
    {
        device.get().WriteToAddress(address, data);
    }
}

auto AddressBus::ReadFromAddress(uint16_t address) const noexcept -> std::byte
{
    std::byte data = 0x00_b;
    for (auto& device : m_devices)
    {
        data |= device.get().ReadFromAddress(address);
    }
    return data;
}

} // namespace EmulatorLib