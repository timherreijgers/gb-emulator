/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "register_function_wrappers.h"

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

class IncRRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(IncRRTest, ExecutingOpCode)
{
    const auto& [instruction, targetRegister] = GetParam();

    targetRegister(m_cpu.Registers()) = 0x1234;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x1234));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x1235));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(IncRRTest, IncRRTest,
                         ::testing::Values(
                             InstructionPair{0x03_b, FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0x13_b, FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0x23_b, FunctionWrapper16Bit<RegisterHL>()},
                             InstructionPair{0x33_b, FunctionWrapper16Bit<RegisterSP>()}),
                         [](const testing::TestParamInfo<IncRRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
