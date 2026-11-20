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

struct SlaInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class SlaTest : public InstructionTestBase, public ::testing::WithParamInterface<SlaInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(SlaTest, ExecutingOpcode)
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

TEST_P(SlaTest, ExecutingOpcode_ShiftsBitSevenIntoCarryAndSetsZero)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x80_b;
    m_cpu.Registers().flags = (CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();

    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::Carry).AsByte()));
}

TEST_P(SlaTest, ExecutingOpcode_WhenResultIsZero_SetsZeroAndClearsOtherFlags)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    m_cpu.Step();

    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    SlaTest, SlaTest,
    ::testing::Values(
        SlaInstruction{0x20_b, FunctionWrapper8Bit<RegisterB>()},
        SlaInstruction{0x21_b, FunctionWrapper8Bit<RegisterC>()},
        SlaInstruction{0x22_b, FunctionWrapper8Bit<RegisterD>()},
        SlaInstruction{0x23_b, FunctionWrapper8Bit<RegisterE>()},
        SlaInstruction{0x24_b, FunctionWrapper8Bit<RegisterH>()},
        SlaInstruction{0x25_b, FunctionWrapper8Bit<RegisterL>()},
        SlaInstruction{0x27_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<SlaTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
