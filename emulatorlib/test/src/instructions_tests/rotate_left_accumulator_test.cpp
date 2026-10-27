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

struct RlaTestParam
{
    std::byte accumulatorValue;
    std::byte expectedValue;
    bool carryFlagIsSet;
    bool carryFlagExpected;
};

} // namespace

class RlaTest : public InstructionTestBase, public ::testing::WithParamInterface<RlaTestParam>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x17_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(RlaTest, ExecutingOpCode)
{
    const auto [accumulatorValue, expectedValue, carryFlagIsSet, carryFlagExpected] = GetParam();

    m_cpu.Registers().accumulator = accumulatorValue;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Subtract).AsByte() |
                              (carryFlagIsSet ? CpuFlags::Carry.AsByte() : 0x00_b);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(accumulatorValue));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, 0x17_b);


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, 0x00_b);
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(expectedValue));
    ASSERT_THAT(m_cpu.Registers().flags, carryFlagExpected ? CpuFlags::Carry.AsByte() : 0x00_b);
}

INSTANTIATE_TEST_SUITE_P(RlaTest, RlaTest,
                         ::testing::Values(RlaTestParam{0x07_b, 0x0E_b, false, false},
                                           RlaTestParam{0xFF_b, 0xFF_b, true, true},
                                           RlaTestParam{0xFF_b, 0xFE_b, false, true},
                                           RlaTestParam{0xF0_b, 0xE0_b, false, true},
                                           RlaTestParam{0xF0_b, 0xE1_b, true, true},
                                           RlaTestParam{0x0F_b, 0x1E_b, false, false},
                                           RlaTestParam{0x0F_b, 0x1F_b, true, false}));
} // namespace EmulatorLib::Test