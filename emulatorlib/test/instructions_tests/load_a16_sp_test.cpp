/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "utilitylib/byte_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class LoadA16SPTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(0x08_b));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0x33_b));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0102)).WillByDefault(::testing::Return(0x44_b));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0103)).WillByDefault(::testing::Return(0x00_b));
    }

    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadA16SPTest, ExecutingCommand_WithValueToLoadAs0x1234_BehavesCorrectly)
{
    constexpr auto opCode = 0x08_b;

    // TODO: Registers getter should be const. If we validate that LoadN16SP works, we can use that to load data into the stack pointer.
    // For this we first need to make sure that we have a better way of creating "programs" than setting up mocks
    m_cpu.Registers().stackPointer = 0x1234;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x33_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x33_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x44_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x4433, 0x34_b)).Times(1);
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x4434, 0x12_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test