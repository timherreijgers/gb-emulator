/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus_addressable.h"

#include <functional>
#include <vector>

namespace EmulatorLib
{

class AddressBus
{
public:
    explicit AddressBus(std::vector<std::reference_wrapper<AddressBusAddressable>> devices);
    virtual ~AddressBus() = default;
    AddressBus(const AddressBus&) = delete;
    AddressBus& operator=(const AddressBus&) = delete;
    AddressBus(AddressBus&&) = delete;
    AddressBus& operator=(AddressBus&&) = delete;

    void WriteToAddress(uint16_t address, std::byte data) noexcept;
    [[nodiscard]] auto ReadFromAddress(uint16_t address) const noexcept -> std::byte;

private:
    std::vector<std::reference_wrapper<AddressBusAddressable>> m_devices;
};

} // namespace EmulatorLib