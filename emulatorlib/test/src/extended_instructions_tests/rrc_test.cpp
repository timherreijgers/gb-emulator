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

struct RrcInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class RrcTest : public InstructionTestBase, public ::testing::WithParamInterface<RrcInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(RrcTest, ExecutingOpcode)
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

TEST_P(RrcTest, ExecutingOpcode_RotatesBitZeroIntoBitSevenAndCarry)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x80_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(RrcTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(RrcTest, ExecutingOpcode_ClearsFlags)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x01_b;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    RrcTest, RrcTest,
    ::testing::Values(
        RrcInstruction{0x08_b, FunctionWrapper8Bit<RegisterB>()},
        RrcInstruction{0x09_b, FunctionWrapper8Bit<RegisterC>()},
        RrcInstruction{0x0A_b, FunctionWrapper8Bit<RegisterD>()},
        RrcInstruction{0x0B_b, FunctionWrapper8Bit<RegisterE>()},
        RrcInstruction{0x0C_b, FunctionWrapper8Bit<RegisterH>()},
        RrcInstruction{0x0D_b, FunctionWrapper8Bit<RegisterL>()},
        RrcInstruction{0x0F_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<RrcTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
