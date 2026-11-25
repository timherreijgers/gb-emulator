/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/interrupt/interrupt_controller.h"
#include "utilitylib/integral_suffixes.h"

namespace EmulatorLib
{

namespace
{

constexpr auto g_interruptRequestedRegisterAddress = 0xFF0F_u16;
constexpr auto g_interruptEnableRegisterAddress = 0xFFFF_u16;


} // namespace

void InterruptController::WriteToAddress(uint16_t address, std::byte data) noexcept
{
    if (address == g_interruptRequestedRegisterAddress)
    {
        m_interruptRequested = data;
    }

    if (address == g_interruptEnableRegisterAddress)
    {
        m_interruptEnable = data;
    }
}

auto InterruptController::ReadFromAddress(uint16_t address) const noexcept -> std::byte
{
    if (address == g_interruptRequestedRegisterAddress)
    {
        return m_interruptRequested;
    }

    if (address == g_interruptEnableRegisterAddress)
    {
        return m_interruptEnable;
    }

    return 0x00_b;
}

void InterruptController::RequestInterrupt(Interrupt interrupt) noexcept
{
    m_interruptRequested |= std::byte{std::to_underlying(interrupt)};
}

void InterruptController::Acknowledge(Interrupt interrupt) noexcept
{
    m_interruptRequested &= ~std::byte{std::to_underlying(interrupt)};
}

auto InterruptController::HighestPriorityPending() -> std::optional<Interrupt>
{
    const auto pendingInterrupts = (m_interruptEnable & m_interruptRequested);

    if (pendingInterrupts == 0x00_b)
    {
        return std::nullopt;
    }

    if ((pendingInterrupts & std::byte{std::to_underlying(Interrupt::VBlank)}) != 0x00_b)
    {
        return Interrupt::VBlank;
    }

    if ((pendingInterrupts & std::byte{std::to_underlying(Interrupt::Lcd)}) != 0x00_b)
    {
        return Interrupt::Lcd;
    }

    if ((pendingInterrupts & std::byte{std::to_underlying(Interrupt::Timer)}) != 0x00_b)
    {
        return Interrupt::Timer;
    }

    if ((pendingInterrupts & std::byte{std::to_underlying(Interrupt::Serial)}) != 0x00_b)
    {
        return Interrupt::Serial;
    }

    return Interrupt::Joypad;
}

} // namespace EmulatorLib