/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class LoadHlIndirectIncrementATest : public ::testing::Test
{
protected:
    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadHlIndirectIncrementATest, ExecuteOpCode_ExecutesCorrectly)
{
    constexpr auto instruction = 0x22_b;
    ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(instruction));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0x00_b));

    // TODO: Registers getter should be const. If we validate that LoadN8A and LoadN16BC works, we can use that to load data into the stack pointer.
    // For this we first need to make sure that we have a better way of creating "programs" than setting up mocks
    m_cpu.Registers().hlRegister = 0x1234;
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x1234, 0xFF_b)).Times(1);
    m_cpu.Step();
    ASSERT_EQ(m_cpu.Registers().hlRegister, 0x1235);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test