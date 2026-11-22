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

struct SwapInstruction
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class SwapTest : public InstructionTestBase, public ::testing::WithParamInterface<SwapInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(SwapTest, ExecutingOpcode)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(SwapTest, ExecutingOpcode_ExchangesNibbles)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(SwapTest, ExecutingOpcode_ResultIsZero_SetsZeroFlag)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(SwapTest, ExecutingOpcode_ClearsFlags)
{
    const auto& [instruction, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0xF0_b;
    m_cpu.Registers().flags = (CpuFlags::Carry | CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(
    SwapTest, SwapTest,
    ::testing::Values(
        SwapInstruction{0x30_b, FunctionWrapper8Bit<RegisterB>()},
        SwapInstruction{0x31_b, FunctionWrapper8Bit<RegisterC>()},
        SwapInstruction{0x32_b, FunctionWrapper8Bit<RegisterD>()},
        SwapInstruction{0x33_b, FunctionWrapper8Bit<RegisterE>()},
        SwapInstruction{0x34_b, FunctionWrapper8Bit<RegisterH>()},
        SwapInstruction{0x35_b, FunctionWrapper8Bit<RegisterL>()},
        SwapInstruction{0x37_b, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<SwapTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
