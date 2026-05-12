/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class CpuTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ON_CALL(m_addressableMock, ReadFromAddress(0x0000)).WillByDefault(::testing::Return(0x01_b));
    }

    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(CpuTest, Initialization_InitializesRegistersCorrectly)
{
    const auto& registers = m_cpu.Registers();

    ASSERT_THAT(registers.instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registers.accumulator, ::testing::Eq(0x01_b));
    ASSERT_THAT(registers.flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
    ASSERT_THAT(registers.bRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registers.cRegister, ::testing::Eq(0x13_b));
    ASSERT_THAT(registers.dRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registers.eRegister, ::testing::Eq(0xD8_b));
    ASSERT_THAT(registers.hRegister, ::testing::Eq(0x01_b));
    ASSERT_THAT(registers.lRegister, ::testing::Eq(0x4D_b));
    ASSERT_THAT(registers.wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registers.zRegister, ::testing::Eq(0x00_b));

    ASSERT_THAT(registers.programCounter, ::testing::Eq(0x0100));
    ASSERT_THAT(registers.stackPointer, ::testing::Eq(0xFFFE));
}

TEST_F(CpuTest, FirstTick_ReadsFromCorretStartAddress)
{
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0100)).Times(1);

    m_cpu.Step();
}

TEST_F(CpuTest, FirstTick_IncrementsProgramCounter)
{
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0101));
}

TEST_F(CpuTest, FirstTick_PutsDataFromBus_InInstructionRegister)
{
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillOnce(::testing::Return(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x02_b));
}

} // namespace EmulatorLib::Test