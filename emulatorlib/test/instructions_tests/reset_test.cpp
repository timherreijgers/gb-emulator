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

template <typename T>
[[nodiscard]] auto FunctionWrapper16Bit() -> std::function<Register16Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    uint16_t targetAddress;
};

} // namespace

class ResetTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x00_b, GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0xFFFE)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(ResetTest, ExecuteOpCodeCorrectly)
{
    const auto& [instruction, targetAddress] = GetParam();

    // Skip the first no-op to get a 0x0102 pc address to save on the stack
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFE));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFD));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFD, 0x01_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFC, 0x02_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(targetAddress));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(targetAddress + 1));
}

INSTANTIATE_TEST_SUITE_P(ResetTest, ResetTest,
                         ::testing::Values(
                             InstructionPair{0xC7_b, 0x00},
                             InstructionPair{0xCF_b, 0x08},
                             InstructionPair{0xD7_b, 0x10},
                             InstructionPair{0xDF_b, 0x18},
                             InstructionPair{0xE7_b, 0x20},
                             InstructionPair{0xEF_b, 0x28},
                             InstructionPair{0xF7_b, 0x30},
                             InstructionPair{0xFF_b, 0x38}),
                         [](const testing::TestParamInfo<ResetTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test