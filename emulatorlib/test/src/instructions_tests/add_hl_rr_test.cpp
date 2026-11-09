/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/test/src/register_function_wrappers.h"

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"

#include <functional>
#include <string>

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::function<Register16Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class AddHlRRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(AddHlRRTest, ExecutingOpcode)
{
    const auto& [instruction, sourceRegister] = GetParam();

    m_cpu.Registers().hlRegister = 0x0FFF;
    sourceRegister(m_cpu.Registers()) = 0x0FFF;
    const auto initialHl = m_cpu.Registers().hlRegister.value;
    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0101));
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(initialHl));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0101));
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x0FFE));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x1FFE));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_Overflow_SetsCarryAndHalfCarry)
{
    m_cpu.Registers().hlRegister = 0xFFFF;
    GetParam().sourceRegister(m_cpu.Registers()) = 0xFFFF;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0xFFFE));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_NoCarry_CarryFlagIsCleared)
{
    m_cpu.Registers().hlRegister = 0x0100;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0100;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddHlRRTest, ExecutingOpcode_NoHalfCarry_HalfCarryFlagIsCleared)
{
    m_cpu.Registers().hlRegister = 0x0100;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0100;
    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddHlRRTest, ExecutingOpcode_HalfCarryWithoutCarry_HalfCarryFlagIsSet)
{
    m_cpu.Registers().hlRegister = 0x0800;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0800;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_ZeroResult_ZeroFlagRemainsCleared)
{
    m_cpu.Registers().hlRegister = 0x0000;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0000;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x0000));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddHlRRTest, ExecutingOpcode_SubtractFlagIsSet_SubtractFlagIsCleared)
{
    m_cpu.Registers().hlRegister = 0x0100;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0100;
    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(AddHlRRTest, ExecutingOpcode_NonzeroResult_ZeroFlagRemainsSet)
{
    m_cpu.Registers().hlRegister = 0x0100;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0100;
    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x0200));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_CarryWithoutHalfCarry_CarryFlagIsSet)
{
    m_cpu.Registers().hlRegister = 0x8000;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x8000;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_LowByteCarries_PropagatesCarryToHighByte)
{
    m_cpu.Registers().hlRegister = 0x0FFF;
    GetParam().sourceRegister(m_cpu.Registers()) = 0x0FFF;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x1FFE));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_P(AddHlRRTest, ExecutingOpcode_AddsSelectedSourceRegister)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_cpu.Registers().bcRegister = 0x0102;
    m_cpu.Registers().deRegister = 0x0304;
    m_cpu.Registers().stackPointer = 0x0506;
    m_cpu.Registers().hlRegister = 0x1000;
    const auto source = sourceRegister(m_cpu.Registers()).value;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x1000 + source));
    if (instruction != 0x29_b)
    {
        ASSERT_THAT(sourceRegister(m_cpu.Registers()), ::testing::Eq(source));
    }
}

INSTANTIATE_TEST_SUITE_P(AddHlRRTest, AddHlRRTest,
                         ::testing::Values(
                             InstructionPair{0x09_b, FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0x19_b, FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0x29_b, FunctionWrapper16Bit<RegisterHL>()},
                             InstructionPair{0x39_b, FunctionWrapper16Bit<RegisterSP>()}),
                         [](const testing::TestParamInfo<AddHlRRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
