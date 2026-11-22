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

struct SrlInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class SrlTest : public InstructionTestBase, public ::testing::WithParamInterface<SrlInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(SrlTest, ExecutingOpcode)
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

TEST_P(SrlTest, ExecutingOpcode_ShiftsBitZeroIntoCarryAndClearsBitSeven)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x81_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x40_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(SrlTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(SrlTest, ExecutingOpcode_ClearsFlags)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x02_b;
    m_cpu.Registers().flags = (CpuFlags::Carry | CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(
    SrlTest, SrlTest,
    ::testing::Values(
        SrlInstruction{0x38_b, FunctionWrapper8Bit<RegisterB>()},
        SrlInstruction{0x39_b, FunctionWrapper8Bit<RegisterC>()},
        SrlInstruction{0x3A_b, FunctionWrapper8Bit<RegisterD>()},
        SrlInstruction{0x3B_b, FunctionWrapper8Bit<RegisterE>()},
        SrlInstruction{0x3C_b, FunctionWrapper8Bit<RegisterH>()},
        SrlInstruction{0x3D_b, FunctionWrapper8Bit<RegisterL>()},
        SrlInstruction{0x3F_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<SrlTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
