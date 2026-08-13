/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "utilitylib/byte_utils.h"

#include "instruction_handlers/register_io_helpers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class LoadIndirectCTestFromA : public ::testing::Test
{
protected:
    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadIndirectCTestFromA, ExecutingOpCode)
{
    ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(0xE2_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0x00_b));

    m_cpu.Registers().accumulator = 0x88_b;
    m_cpu.Registers().cRegister = 0x66_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE2_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFF66, 0x88_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test