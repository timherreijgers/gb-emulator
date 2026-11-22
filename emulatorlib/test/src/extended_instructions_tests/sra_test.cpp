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

struct SraInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class SraTest : public InstructionTestBase, public ::testing::WithParamInterface<SraInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(SraTest, ExecutingOpcode)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0xF8_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(SraTest, ExecutingOpcode_PreservesBitSevenAndMovesBitZeroIntoCarry)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x81_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0xC0_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(SraTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(SraTest, ExecutingOpcode_ClearsFlags)
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
    SraTest, SraTest,
    ::testing::Values(
        SraInstruction{0x28_b, FunctionWrapper8Bit<RegisterB>()},
        SraInstruction{0x29_b, FunctionWrapper8Bit<RegisterC>()},
        SraInstruction{0x2A_b, FunctionWrapper8Bit<RegisterD>()},
        SraInstruction{0x2B_b, FunctionWrapper8Bit<RegisterE>()},
        SraInstruction{0x2C_b, FunctionWrapper8Bit<RegisterH>()},
        SraInstruction{0x2D_b, FunctionWrapper8Bit<RegisterL>()},
        SraInstruction{0x2F_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<SraTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
