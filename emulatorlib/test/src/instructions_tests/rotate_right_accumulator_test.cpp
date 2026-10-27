/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

namespace
{

struct RraTestParam
{
    std::byte accumulatorValue;
    std::byte expectedValue;
    bool carryFlagIsSet;
    bool carryFlagExpected;
};

} // namespace

class RraTest : public InstructionTestBase, public ::testing::WithParamInterface<RraTestParam>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x1F_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(RraTest, ExecutingOpCode)
{
    const auto [accumulatorValue, expectedValue, carryFlagIsSet, carryFlagExpected] = GetParam();

    m_cpu.Registers().accumulator = accumulatorValue;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Subtract).AsByte() |
                              (carryFlagIsSet ? CpuFlags::Carry.AsByte() : 0x00_b);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(accumulatorValue));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, 0x1F_b);


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, 0x00_b);
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(expectedValue));
    ASSERT_THAT(m_cpu.Registers().flags, carryFlagExpected ? CpuFlags::Carry.AsByte() : 0x00_b);
}

INSTANTIATE_TEST_SUITE_P(RraTest, RraTest,
                         ::testing::Values(RraTestParam{0x07_b, 0x03_b, false, true},
                                           RraTestParam{0xFF_b, 0xFF_b, true, true},
                                           RraTestParam{0xFF_b, 0x7F_b, false, true},
                                           RraTestParam{0xF0_b, 0x78_b, false, false},
                                           RraTestParam{0xF0_b, 0xF8_b, true, false},
                                           RraTestParam{0x0F_b, 0x07_b, false, true},
                                           RraTestParam{0x0F_b, 0x87_b, true, true}));
} // namespace EmulatorLib::Test