/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_handlers/register_io_helpers.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

namespace
{

template <typename T>
[[nodiscard]] auto FunctionWrapper8Bit() -> std::function<Register8Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class IncRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(IncRTest, ExecutingOpCode)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x02_b + 1));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(IncRTest, ExecutingOpCode_SetsSubtractionFlagTo0)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(IncRTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(IncRTest, ExecutingOpCode_SetsZeroFlagTo1_AndHalfCarryTo1)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    targetRegister(m_cpu.Registers()) = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_P(IncRTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    targetRegister(m_cpu.Registers()) = 0x04_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(IncRTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    targetRegister(m_cpu.Registers()) = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_P(IncRTest, ExecutingOpCode_CarryFlagRemainsUnchangedWhenZero)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    targetRegister(m_cpu.Registers()) = 0x04_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(IncRTest, ExecutingOpCode_CarryFlagRemainsUnchangedWhenOne)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    targetRegister(m_cpu.Registers()) = 0x04_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(IncRTest, IncRTest,
                         ::testing::Values(
                             InstructionPair{0x04_b, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x0C_b, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x14_b, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x1C_b, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x24_b, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x2C_b, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x3C_b, FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<IncRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test