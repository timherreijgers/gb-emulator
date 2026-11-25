/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/interrupt/interrupt_controller.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class InterruptControllerTest : public ::testing::Test
{
protected:
    InterruptController m_interruptController{};
};

TEST_F(InterruptControllerTest, ReadFromAllAddresses_ReturnsZero)
{
    for (uint16_t address = 0; address < 0xFFFF; address++)
    {
        ASSERT_THAT(m_interruptController.ReadFromAddress(address), ::testing::Eq(0x00_b)) << std::format("Address: 0x{:04X}", address);
    }
}

TEST_F(InterruptControllerTest, WriteToAddressFF0F_SetsValueInRegister)
{
    m_interruptController.WriteToAddress(0xFF0F, 0xEE_b);
    ASSERT_THAT(m_interruptController.ReadFromAddress(0xFF0F), ::testing::Eq(0xEE_b));
}

TEST_F(InterruptControllerTest, WriteToAddressFFFF_SetsValueInRegister)
{
    m_interruptController.WriteToAddress(0xFFFF, 0xEE_b);
    ASSERT_THAT(m_interruptController.ReadFromAddress(0xFFFF), ::testing::Eq(0xEE_b));
}


TEST_F(InterruptControllerTest, WriteToAddressNotSupported_DoesNotSetValueInAddress)
{
    for (uint16_t address = 0; address < 0xFFFF; address++)
    {
        if (address == 0xFF0F || address == 0xFFFF)
        {
            continue;
        }

        m_interruptController.WriteToAddress(address, 0xEE_b);
        ASSERT_THAT(m_interruptController.ReadFromAddress(address), ::testing::Eq(0x00_b));
    }
}

TEST_F(InterruptControllerTest, HighestPriorityPending_WithoutAnyInterruptsPending_ReturnsNullopt)
{
    ASSERT_THAT(m_interruptController.HighestPriorityPending(), ::testing::Eq(std::nullopt));
}

TEST_F(InterruptControllerTest, HighestPriorityPending_WithAllInterruptRequestedAndInterruptEnabled_PendingInterruptsInCorrectOrder)
{
    constexpr std::array expectedOrder = {Interrupt::VBlank, Interrupt::Lcd, Interrupt::Timer, Interrupt::Serial, Interrupt::Joypad};

    m_interruptController.WriteToAddress(0xFF0F, 0x1F_b);
    m_interruptController.WriteToAddress(0xFFFF, 0x1F_b);

    for (const auto interrupt : expectedOrder)
    {
        ASSERT_THAT(m_interruptController.HighestPriorityPending(), ::testing::Eq(interrupt)) << std::format("Interrupt {} is not the highest priority pending", std::to_underlying(interrupt));
        m_interruptController.Acknowledge(interrupt);
    }
}

class InterruptControllerParamTest : public InterruptControllerTest, public ::testing::WithParamInterface<Interrupt>
{
};

TEST_P(InterruptControllerParamTest, RequestInterrupt_SetsCorrectBitAtAddressFF0F)
{
    const auto requestedInterrupt = GetParam();
    m_interruptController.RequestInterrupt(requestedInterrupt);

    ASSERT_THAT(m_interruptController.ReadFromAddress(0xFF0F), ::testing::Eq(std::byte{std::to_underlying(requestedInterrupt)}));
}

TEST_P(InterruptControllerParamTest, Acknowledge_ClearsCorrectBitAtAddressFF0F)
{
    const auto requestedInterrupt = GetParam();
    m_interruptController.RequestInterrupt(requestedInterrupt);
    m_interruptController.Acknowledge(requestedInterrupt);

    ASSERT_THAT(m_interruptController.ReadFromAddress(0xFF0F), ::testing::Eq(0x00_b));
}

TEST_P(InterruptControllerParamTest, Acknowledge_WhenAllInterruptsAreRequested_ClearsCorrectBitAtAddressFF0F)
{
    const auto requestedInterrupt = GetParam();
    m_interruptController.WriteToAddress(0xFF0F, 0x1F_b);
    m_interruptController.Acknowledge(requestedInterrupt);

    const auto expectedValue = ~std::byte{std::to_underlying(requestedInterrupt)} & 0x1F_b;
    ASSERT_THAT(m_interruptController.ReadFromAddress(0xFF0F), ::testing::Eq(expectedValue));
}

TEST_P(InterruptControllerParamTest, HighestPriorityPending_WithInterruptRequested_WithoutInterruptEnabled_ReturnsNullopt)
{
    const auto requestedInterrupt = GetParam();
    m_interruptController.RequestInterrupt(requestedInterrupt);

    ASSERT_THAT(m_interruptController.HighestPriorityPending(), ::testing::Eq(std::nullopt));
}

TEST_P(InterruptControllerParamTest, HighestPriorityPending_WithInterruptRequestedAndInterruptEnabled_ReturnsPendingInterrupt)
{
    const auto requestedInterrupt = GetParam();
    m_interruptController.RequestInterrupt(requestedInterrupt);
    m_interruptController.WriteToAddress(0xFFFF, 0x1F_b);

    ASSERT_THAT(m_interruptController.HighestPriorityPending(), ::testing::Eq(requestedInterrupt));
}

INSTANTIATE_TEST_SUITE_P(InterruptControllerParamTest, InterruptControllerParamTest,
                         ::testing::Values(
                             Interrupt::VBlank,
                             Interrupt::Lcd,
                             Interrupt::Timer,
                             Interrupt::Serial,
                             Interrupt::Joypad));

} // namespace EmulatorLib::Test