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

struct RrInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class RrTest : public InstructionTestBase, public ::testing::WithParamInterface<RrInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(RrTest, ExecutingOpcode)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x78_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(RrTest, ExecutingOpcode_RotatesThroughCarry)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x01_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x80_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(RrTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(RrTest, ExecutingOpcode_ClearsFlags)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x01_b;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Carry | CpuFlags::Zero).AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    RrTest, RrTest,
    ::testing::Values(
        RrInstruction{0x18_b, FunctionWrapper8Bit<RegisterB>()},
        RrInstruction{0x19_b, FunctionWrapper8Bit<RegisterC>()},
        RrInstruction{0x1A_b, FunctionWrapper8Bit<RegisterD>()},
        RrInstruction{0x1B_b, FunctionWrapper8Bit<RegisterE>()},
        RrInstruction{0x1C_b, FunctionWrapper8Bit<RegisterH>()},
        RrInstruction{0x1D_b, FunctionWrapper8Bit<RegisterL>()},
        RrInstruction{0x1F_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<RrTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
