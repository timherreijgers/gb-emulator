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

class DecRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(DecRTest, ExecutingOpCode)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(0x02_b - 1));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(DecRTest, ExecutingOpCode_SetsSubtractionFlagTo1)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_P(DecRTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    targetRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_P(DecRTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    targetRegister(m_cpu.Registers()) = 0x04_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_P(DecRTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    const auto& [instruction, targetRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    targetRegister(m_cpu.Registers()) = 0x10_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(DecRTest, DecRTest,
                         ::testing::Values(
                             InstructionPair{0x05_b, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x0D_b, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x15_b, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x1D_b, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x25_b, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x2D_b, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x3D_b, FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<DecRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test