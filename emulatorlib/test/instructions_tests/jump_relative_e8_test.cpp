/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/instruction_translation.h"
#include "utilitylib/byte_utils.h"

#include <format>

namespace EmulatorLib::Test
{

namespace
{

struct RelativeJumpOffset
{
    std::byte offset;
    std::byte expectedZResult;
    std::byte expectedWRegister;
    std::uint16_t expectedProgramCounterAfterJump;
};

} // namespace

class JumpRelativeE8Test : public InstructionTestBase, public ::testing::WithParamInterface<RelativeJumpOffset>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x18_b, GetParam().offset});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(JumpRelativeE8Test, JumpsWithOffset)
{
    const auto& params = GetParam();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x18_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(params.offset));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(params.expectedZResult));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(params.expectedWRegister));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x102));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(params.expectedProgramCounterAfterJump));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(JumpRelativeE8Test, JumpRelativeE8Test,
                         ::testing::Values(RelativeJumpOffset{0x05_b, 0x07_b, 0x01_b, 0x0108},
                                           RelativeJumpOffset{0xFB_b, 0xFD_b, 0x00_b, 0x00FE},
                                           RelativeJumpOffset{0x00_b, 0x02_b, 0x01_b, 0x0103}),
                         [](const testing::TestParamInfo<JumpRelativeE8Test::ParamType>& info) {
                             return std::format("{}", info.param.offset);
                         });

} // namespace EmulatorLib::Test
