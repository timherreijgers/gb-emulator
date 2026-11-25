/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/interrupt/interrupt.h"
#include "emulatorlib/interrupt/interrupt_controller.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "emulatorlib/test/program_stub.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

namespace
{

struct InterruptDestinationPair
{
    Interrupt interrupt;
    uint16_t destination;
};

} // namespace

class CpuInterruptHandlingTest : public ::testing::TestWithParam<InterruptDestinationPair>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xD9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,
                                0xD9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,
                                0xD9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,
                                0xD9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,
                                0xD9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b});
    }

    testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    ProgramStub m_program{0x40};
    InterruptController m_interruptController;
    AddressBus m_bus{{m_addressableMock, m_program, m_interruptController}};
    Cpu m_cpu{m_bus, m_interruptController};
};

TEST_P(CpuInterruptHandlingTest, HandlingInterrupt)
{
    [[maybe_unused]] const auto [interrupt, destination] = GetParam();

    // Loading 0x101
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x101));

    // Loading 0x102
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));

    // Enable all interrupts
    m_interruptController.WriteToAddress(0xFFFF, 0x1F_b);
    m_interruptController.RequestInterrupt(interrupt);
    m_cpu.Registers().masterInterruptState = MasterInterruptState::ENABLED;
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x103));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFE));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFD));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFD, 0x01_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFC, 0x02_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(destination));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xD9_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(destination + 1));
}

INSTANTIATE_TEST_SUITE_P(CpuInterruptHandlingTest, CpuInterruptHandlingTest,
                         ::testing::Values(
                             InterruptDestinationPair{Interrupt::VBlank, 0x40},
                             InterruptDestinationPair{Interrupt::Lcd, 0x48},
                             InterruptDestinationPair{Interrupt::Timer, 0x50},
                             InterruptDestinationPair{Interrupt::Serial, 0x58},
                             InterruptDestinationPair{Interrupt::Joypad, 0x60}));

} // namespace EmulatorLib::Test