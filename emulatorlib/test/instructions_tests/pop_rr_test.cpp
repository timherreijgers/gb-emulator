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
    std::function<Register16Bit&(CpuRegisters&)> destinationRegister;
    uint16_t mask = 0xFFFF;
};

} // namespace

class PopRRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUpForInstruction(std::byte opCode)
    {
        m_program.WriteProgram({opCode, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(PopRRTest, ExecuteOpCodeCorrectly)
{
    const auto& [instruction, targetRegisterGetter, valueMask] = GetParam();
    const auto& targetRegister = targetRegisterGetter(m_cpu.Registers());

    SetUpForInstruction(instruction);

    m_cpu.Registers().stackPointer = 0xFF00;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFF00)).Times(1).WillOnce(::testing::Return(0x12_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFF01));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x12_b));


    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFF01)).Times(1).WillOnce(::testing::Return(0x34_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFF02));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x34_b));

    m_cpu.Step();
    ASSERT_THAT(targetRegister, ::testing::Eq(0x3412 & valueMask));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(PopRRTest, PopRRTest,
                         ::testing::Values(
                             InstructionPair{0xC1_b, FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0xD1_b, FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0xE1_b, FunctionWrapper16Bit<RegisterHL>()},
                             InstructionPair{0xF1_b, FunctionWrapper16Bit<RegisterAF>(), 0xFFF0}),
                         [](const testing::TestParamInfo<PopRRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test