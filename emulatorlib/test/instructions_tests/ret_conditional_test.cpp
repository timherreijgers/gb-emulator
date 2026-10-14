/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"

namespace EmulatorLib::Test
{

namespace
{

struct ConditionalCall
{
    std::byte instruction;
    std::byte flagsWhenJumping;
    std::byte flagsWhenNotJumping;
};

} // namespace

class RetConditionalTest : public InstructionTestBase, public ::testing::WithParamInterface<ConditionalCall>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction});
        m_cpu.Registers().stackPointer = 0xFFF0;
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(RetConditionalTest, ExecutingOpCode_WhereReturnPathIsTaken)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping] = GetParam();
    m_cpu.Registers().flags = flagsWhenJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    // Empty Cpu cycle to check condition
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFF0)).WillOnce(::testing::Return(0x01_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFF1));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x01_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFF1)).WillOnce(::testing::Return(0x02_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFF2));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0201));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(RetConditionalTest, ExecutingOpCode_WhereReturnPathIsNotTaken)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping] = GetParam();
    m_cpu.Registers().flags = flagsWhenNotJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    // Empty Cpu cycle to check condition
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
}

INSTANTIATE_TEST_SUITE_P(RetConditionalTest, RetConditionalTest,
                         ::testing::Values(ConditionalCall{0xC0_b, 0x00_b, CpuFlags::Zero.AsByte()},
                                           ConditionalCall{0xD0_b, 0x00_b, CpuFlags::Carry.AsByte()},
                                           ConditionalCall{0xC8_b, CpuFlags::Zero.AsByte(), 0x00_b},
                                           ConditionalCall{0xD8_b, CpuFlags::Carry.AsByte(), 0x00_b}),
                         [](const testing::TestParamInfo<RetConditionalTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test