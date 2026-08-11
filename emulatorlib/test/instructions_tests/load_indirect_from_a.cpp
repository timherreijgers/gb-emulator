/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"

#include "instruction_handlers/register_io_helpers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class LoadIndirectN8FromATest : public ::testing::Test
{
protected:
    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadIndirectN8FromATest, ExecutingOpCode)
{
    ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(0xE0_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0xAA_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0102)).WillByDefault(::testing::Return(0x00_b));

    m_cpu.Registers().accumulator = 0xEA_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE0_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0101)).Times(1).WillOnce(::testing::Return(0xAA_b));
    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAA_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFAA, 0xEA_b)).Times(1);
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0102)).Times(1).WillOnce(::testing::Return(0x00_b));
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test