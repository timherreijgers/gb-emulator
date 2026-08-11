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

class LoadAFromIndirectN8Test : public ::testing::Test
{
protected:
    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadAFromIndirectN8Test, ExecutingOpCode)
{
    ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(0xF0_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0xAA_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0102)).WillByDefault(::testing::Return(0x00_b));

    const auto initialValue = m_cpu.Registers().accumulator.value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF0_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0101)).Times(1).WillOnce(::testing::Return(0xBB_b));
    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xBB_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFBB)).Times(1).WillOnce(::testing::Return(0x88_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x88_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0102)).Times(1).WillOnce(::testing::Return(0x00_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x88_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test