/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/test/src/register_function_wrappers.h"

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::function<Register16Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class DecRRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(DecRRTest, ExecutingOpCode)
{
    const auto& [instruction, targetRegister] = GetParam();

    targetRegister(m_cpu.Registers()) = 0x1234;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x1234));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x1233));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(DecRRTest, DecRRTest,
                         ::testing::Values(
                             InstructionPair{0x0B_b, FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0x1B_b, FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0x2B_b, FunctionWrapper16Bit<RegisterHL>()},
                             InstructionPair{0x3B_b, FunctionWrapper16Bit<RegisterSP>()}),
                         [](const testing::TestParamInfo<DecRRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
