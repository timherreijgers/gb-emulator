/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/instruction_translation.h"
#include "instruction_handlers/register_io_helpers.h"

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
    std::function<Register16Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class PushRRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUpForInstruction(std::byte opCode)
    {
        m_program.WriteProgram({opCode, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(PushRRTest, ExecuteOpCodeCorrectly)
{
    const auto& [instruction, sourceRegisterGetter] = GetParam();
    const auto& sourceRegister = sourceRegisterGetter(m_cpu.Registers());

    SetUpForInstruction(instruction);

    m_cpu.Registers().stackPointer = 0xFFFE;
    sourceRegister.value = 0xAA88;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFE));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFD));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFD, 0xAA_b)).Times(1);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFC, 0x88_b)).Times(1);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(PushRRTest, PushRRTest,
                         ::testing::Values(
                             InstructionPair{0xC5_b, FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0xD5_b, FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0xE5_b, FunctionWrapper16Bit<RegisterHL>()},
                             InstructionPair{0xF5_b, FunctionWrapper16Bit<RegisterAF>()}),
                         [](const testing::TestParamInfo<PushRRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test