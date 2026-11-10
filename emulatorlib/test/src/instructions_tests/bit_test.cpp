/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/test/src/register_function_wrappers.h"

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::byte bitMask;
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class BitTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        [[maybe_unused]] const auto& [instruction, mask, sourceRegister] = GetParam();

        m_program.WriteProgram({0xCB_b, instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(BitTest, ExecutingOpCode_WhenBitIsNotSet_SetsZeroFlag)
{
    [[maybe_unused]] const auto& [instruction, bitmask, sourceRegister] = GetParam();
    sourceRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq((CpuFlags::Zero | CpuFlags::HalfCarry).AsByte()));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(BitTest, ExecutingOpCode_WhenBitNotSet_DoesntSetZeroFlag)
{
    [[maybe_unused]] const auto& [instruction, bitmask, sourceRegister] = GetParam();
    sourceRegister(m_cpu.Registers()) = bitmask;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::HalfCarry).AsByte()));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(BitTest, ExecutingOpCode_SetsAllFlagsToCorrectValue_FromAllOnes)
{
    [[maybe_unused]] const auto& [instruction, bitmask, sourceRegister] = GetParam();
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Carry | CpuFlags::Subtract).AsByte();
    sourceRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte()));
}

INSTANTIATE_TEST_SUITE_P(BitTest, BitTest,
                         ::testing::Values(
                             InstructionPair{0x40_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x41_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x42_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x43_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x44_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x45_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x47_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x48_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x49_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x4A_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x4B_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x4C_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x4D_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x4F_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x50_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x51_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x52_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x53_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x54_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x55_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x57_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x58_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x59_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x5A_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x5B_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x5C_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x5D_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x5F_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x60_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x61_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x62_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x63_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x64_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x65_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x67_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x68_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x69_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x6A_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x6B_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x6C_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x6D_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x6F_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x70_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x71_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x72_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x73_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x74_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x75_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x77_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x78_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x79_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x7A_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x7B_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x7C_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x7D_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x7F_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<BitTest::ParamType>& info) {
                             return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test
