/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once
#include <cstddef>
#include <cstdint>

namespace EmulatorLib
{

class AddressBusAddressable
{
public:
    AddressBusAddressable() = default;
    virtual ~AddressBusAddressable() = default;
    AddressBusAddressable(const AddressBusAddressable&) = delete;
    AddressBusAddressable& operator=(const AddressBusAddressable&) = delete;
    AddressBusAddressable(AddressBusAddressable&&) = delete;
    AddressBusAddressable& operator=(AddressBusAddressable&&) = delete;

    virtual void WriteToAddress(uint16_t address, std::byte data) noexcept = 0;
    [[nodiscard]] virtual auto ReadFromAddress(uint16_t address) const noexcept -> std::byte = 0;
};

} // namespace EmulatorLib