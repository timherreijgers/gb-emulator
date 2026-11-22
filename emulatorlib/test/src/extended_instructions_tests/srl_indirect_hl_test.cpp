/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instructions_tests/instruction_test_base.h"

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib::Test
{

class SrlIndirectHLTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, 0x3E_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SrlIndirectHLTest, ExecutingOpcode)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x3E_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(0xF0_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xF0_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, 0x78_b)).Times(1);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x3E_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(SrlIndirectHLTest, ExecutingOpcode_ShiftsMemoryBitZeroIntoCarryAndClearsBitSeven)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(0x81_b));
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, 0x40_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_F(SrlIndirectHLTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(0x00_b));
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, 0x00_b)).Times(1);
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_F(SrlIndirectHLTest, ExecutingOpcode_ClearsFlags)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = (CpuFlags::Carry | CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(0x02_b));
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, 0x01_b)).Times(1);
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test
