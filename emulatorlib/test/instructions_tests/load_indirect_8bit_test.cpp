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

template <typename T>
[[nodiscard]] auto FunctionWrapper8Bit() -> std::function<Register8Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    std::function<Register16Bit&(CpuRegisters&)> destinationRegister;
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class LoadIndirect8BitTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(LoadIndirect8BitTest, ExecutingOpCode)
{
    const auto& [instruction, destinationRegister, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    destinationRegister(m_cpu.Registers()) = 0x1234;
    sourceRegister(m_cpu.Registers()) = 0xFF_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x1234, 0xFF_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadIndirect8BitTest, LoadIndirect8BitTest,
                         ::testing::Values(
                             InstructionPair{0x02_b, FunctionWrapper16Bit<RegisterBC>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x12_b, FunctionWrapper16Bit<RegisterDE>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x70_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x71_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x72_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x73_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x77_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<LoadIndirect8BitTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

using LoadIndirect8BitHLTest = LoadIndirect8BitTest;

TEST_P(LoadIndirect8BitHLTest, ExecutingOpCode)
{
    const auto& [instruction, destinationRegister, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    destinationRegister(m_cpu.Registers()) = 0x1234;
    const auto expectedValue = sourceRegister(m_cpu.Registers()).value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x1234, expectedValue)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadIndirect8BitHLTest, LoadIndirect8BitHLTest,
                         ::testing::Values(
                             InstructionPair{0x74_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x75_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterL>()}),
                         [](const testing::TestParamInfo<LoadIndirect8BitTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test