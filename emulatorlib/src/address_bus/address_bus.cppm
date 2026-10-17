/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <cstddef>
#include <functional>
#include <vector>

export module emulatorlib.address_bus;

export namespace EmulatorLib
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
