/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus_addressable.h"
#include "emulatorlib/interrupt/interrupt_acknowedgable.h"
#include "emulatorlib/interrupt/interrupt_requestable.h"
#include "emulatorlib/register.h"

namespace EmulatorLib
{

class InterruptController : public AddressBusAddressable, public InterruptRequestable, public InterruptAcknowledgeable
{
public:
    InterruptController() noexcept = default;
    ~InterruptController() override = default;

    void WriteToAddress(uint16_t address, std::byte data) noexcept override;
    [[nodiscard]] auto ReadFromAddress(uint16_t address) const noexcept -> std::byte override;

    void RequestInterrupt(Interrupt interrupt) noexcept override;
    void Acknowledge(Interrupt interrupt) noexcept override;
    auto HighestPriorityPending() -> std::optional<Interrupt> override;

private:
    std::byte m_interruptRequested = 0x00_b;
    std::byte m_interruptEnable = 0x00_b;
};

} // namespace EmulatorLib