/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_translation.h"
#include "instructions_tests/instruction_test_base.h"
#include "register_function_wrappers.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib::Test
{

namespace
{

struct RlInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class RlTest : public InstructionTestBase, public ::testing::WithParamInterface<RlInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(RlTest, ExecutingOpcode)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0xE0_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(RlTest, ExecutingOpcode_RotatesThroughCarry)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x80_b;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();

    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x01_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    RlTest, RlTest,
    ::testing::Values(
        RlInstruction{0x10_b, FunctionWrapper8Bit<RegisterB>()},
        RlInstruction{0x11_b, FunctionWrapper8Bit<RegisterC>()},
        RlInstruction{0x12_b, FunctionWrapper8Bit<RegisterD>()},
        RlInstruction{0x13_b, FunctionWrapper8Bit<RegisterE>()},
        RlInstruction{0x14_b, FunctionWrapper8Bit<RegisterH>()},
        RlInstruction{0x15_b, FunctionWrapper8Bit<RegisterL>()},
        RlInstruction{0x17_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<RlTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
