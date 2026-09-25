/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/instruction_translation.h"
#include "utilitylib/byte_utils.h"

namespace EmulatorLib::Test
{

namespace
{

struct ConditionalJump
{
    std::byte instruction;
    std::byte flagsWhenJumping;
    std::byte flagsWhenNotJumping;
};

} // namespace

class JumpConditionalA16Test : public InstructionTestBase, public ::testing::WithParamInterface<ConditionalJump>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction, 0x50_b, 0x40_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(JumpConditionalA16Test, JumpsWhenConditionIsMet)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping] = GetParam();
    static_cast<void>(flagsWhenNotJumping);
    m_cpu.Registers().flags = flagsWhenJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x40_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4050));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4051));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(JumpConditionalA16Test, ContinuesWhenConditionIsNotMet)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping] = GetParam();
    static_cast<void>(flagsWhenJumping);
    m_cpu.Registers().flags = flagsWhenNotJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x40_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0104));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(JumpCcA16Test, JumpConditionalA16Test,
                         ::testing::Values(ConditionalJump{0xC2_b, 0x00_b, 0x80_b},
                                           ConditionalJump{0xD2_b, 0x00_b, 0x10_b},
                                           ConditionalJump{0xCA_b, 0x80_b, 0x00_b},
                                           ConditionalJump{0xDA_b, 0x10_b, 0x00_b}),
                         [](const testing::TestParamInfo<JumpConditionalA16Test::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test