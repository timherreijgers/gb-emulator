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
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class AddRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(AddRTest, ExecutingOpCode)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().accumulator = 0x02_b;
    sourceRegister(m_cpu.Registers()) = 0x03_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));
    ASSERT_THAT(sourceRegister(m_cpu.Registers()), ::testing::Eq(0x03_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x05_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(AddRTest, ExecutingOpCode_ClearsSubtractionFlag)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;
    sourceRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddRTest, ExecutingOpCode_SetsZeroFlagTo1)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;
    sourceRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(AddRTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;
    sourceRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddRTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;
    sourceRegister(m_cpu.Registers()) = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddRTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x0F_b;
    sourceRegister(m_cpu.Registers()) = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_P(AddRTest, ExecutingOpCode_SetsCarryTo0)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x10_b;
    sourceRegister(m_cpu.Registers()) = 0x10_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddRTest, ExecutingOpCode_SetsCarryTo1)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0xF0_b;
    sourceRegister(m_cpu.Registers()) = 0x20_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(AddRTest, AddRTest,
                         ::testing::Values(
                             InstructionPair{0x80_b, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x81_b, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x82_b, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x83_b, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x84_b, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x85_b, FunctionWrapper8Bit<RegisterL>()}),
                         [](const testing::TestParamInfo<AddRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
