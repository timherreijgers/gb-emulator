/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/instruction_translation.h"
#include "utilitylib/byte_utils.h"

#include <cstddef>
#include <cstdint>
#include <format>

namespace EmulatorLib::Test
{

namespace
{

struct ConditionalRelativeJump
{
    std::byte instruction;
    std::byte flagsWhenJumping;
    std::byte flagsWhenNotJumping;
    std::byte offset;
    std::byte expectedZResult;
    std::byte expectedWRegister;
    std::uint16_t expectedProgramCounterAfterJump;
};

} // namespace

class JumpRelativeConditionalE8Test : public InstructionTestBase, public ::testing::WithParamInterface<ConditionalRelativeJump>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction, GetParam().offset});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(JumpRelativeConditionalE8Test, JumpsWhenConditionIsMet)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping, offset, expectedZResult, expectedWRegister,
                 expectedProgramCounterAfterJump] = GetParam();
    m_cpu.Registers().flags = flagsWhenJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(offset));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(expectedZResult));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(expectedWRegister));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(expectedProgramCounterAfterJump));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(JumpRelativeConditionalE8Test, ContinuesWhenConditionIsNotMet)
{
    const auto& [instruction, flagsWhenJumping, flagsWhenNotJumping, offset, expectedZResult, expectedWRegister,
                 expectedProgramCounterAfterJump] = GetParam();
    m_cpu.Registers().flags = flagsWhenNotJumping;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(offset));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0103));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(JumpRelativeConditionalE8Test, JumpRelativeConditionalE8Test,
                         ::testing::Values(ConditionalRelativeJump{0x20_b, 0x00_b, 0x80_b, 0x05_b, 0x07_b, 0x01_b,
                                                                   0x0108},
                                           ConditionalRelativeJump{0x20_b, 0x00_b, 0x80_b, 0xFB_b, 0xFD_b, 0x00_b,
                                                                   0x00FE},
                                           ConditionalRelativeJump{0x20_b, 0x00_b, 0x80_b, 0x00_b, 0x02_b, 0x01_b,
                                                                   0x0103},
                                           ConditionalRelativeJump{0x28_b, 0x80_b, 0x00_b, 0x05_b, 0x07_b, 0x01_b,
                                                                   0x0108},
                                           ConditionalRelativeJump{0x28_b, 0x80_b, 0x00_b, 0xFB_b, 0xFD_b, 0x00_b,
                                                                   0x00FE},
                                           ConditionalRelativeJump{0x28_b, 0x80_b, 0x00_b, 0x00_b, 0x02_b, 0x01_b,
                                                                   0x0103},
                                           ConditionalRelativeJump{0x30_b, 0x00_b, 0x10_b, 0x05_b, 0x07_b, 0x01_b,
                                                                   0x0108},
                                           ConditionalRelativeJump{0x30_b, 0x00_b, 0x10_b, 0xFB_b, 0xFD_b, 0x00_b,
                                                                   0x00FE},
                                           ConditionalRelativeJump{0x30_b, 0x00_b, 0x10_b, 0x00_b, 0x02_b, 0x01_b,
                                                                   0x0103},
                                           ConditionalRelativeJump{0x38_b, 0x10_b, 0x00_b, 0x05_b, 0x07_b, 0x01_b,
                                                                   0x0108},
                                           ConditionalRelativeJump{0x38_b, 0x10_b, 0x00_b, 0xFB_b, 0xFD_b, 0x00_b,
                                                                   0x00FE},
                                           ConditionalRelativeJump{0x38_b, 0x10_b, 0x00_b, 0x00_b, 0x02_b, 0x01_b,
                                                                   0x0103}),
                         [](const testing::TestParamInfo<JumpRelativeConditionalE8Test::ParamType>& info) {
                             return std::format("{}_OFFSET_{:02X}", OpCodeToInstructionName(info.param.instruction),
                                                std::to_integer<std::uint8_t>(info.param.offset));
                         });

} // namespace EmulatorLib::Test
