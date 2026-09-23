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

class XorRTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(XorRTest, ExecutingOpCode)
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
    ASSERT_THAT(sourceRegister(m_cpu.Registers()), ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(XorRTest, ExecutingOpCode_ClearsNonZeroFlags)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;
    sourceRegister(m_cpu.Registers()) = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_P(XorRTest, ExecutingOpCode_SetsZeroFlagAndClearsOtherFlags)
{
    const auto& [instruction, sourceRegister] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xA5_b;
    sourceRegister(m_cpu.Registers()) = 0xA5_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

INSTANTIATE_TEST_SUITE_P(XorRTest, XorRTest,
                         ::testing::Values(
                             InstructionPair{0xA8_b, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0xA9_b, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0xAA_b, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0xAB_b, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0xAC_b, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0xAD_b, FunctionWrapper8Bit<RegisterL>()}),
                         [](const testing::TestParamInfo<XorRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test