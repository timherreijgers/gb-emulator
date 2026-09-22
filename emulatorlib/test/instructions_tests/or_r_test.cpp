/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "register_function_wrappers.h"

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_handlers/register_io_helpers.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class OrRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(OrRTest, ExecutingOpCode)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().accumulator = 0xF0_b;
    sourceRegister(m_cpu.Registers()) = 0x0F_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xF0_b));
    ASSERT_THAT(sourceRegister(m_cpu.Registers()), ::testing::Eq(0x0F_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(OrRTest, ExecutingOpCode_ClearsSubtractionFlag)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;
    sourceRegister(m_cpu.Registers()) = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(OrRTest, ExecutingOpCode_ClearsHalfCarry)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;
    sourceRegister(m_cpu.Registers()) = 0xF0_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(OrRTest, ExecutingOpCode_ClearsCarryFlag)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;
    sourceRegister(m_cpu.Registers()) = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(OrRTest, ExecutingOpCode_SetsZeroFlag)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;
    sourceRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(OrRTest, ExecutingOpCode_ClearsZeroFlag)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;
    sourceRegister(m_cpu.Registers()) = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(OrRTest, OrRTest,
                         ::testing::Values(
                             InstructionPair{0xB0_b, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0xB1_b, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0xB2_b, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0xB3_b, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0xB4_b, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0xB5_b, FunctionWrapper8Bit<RegisterL>()}),
                         [](const testing::TestParamInfo<OrRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
