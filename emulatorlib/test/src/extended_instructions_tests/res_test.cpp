/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_translation.h"
#include "instructions_tests/instruction_test_base.h"
#include "register_function_wrappers.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"

#include "utilitylib/bit_mask.h"

namespace EmulatorLib::Test
{

namespace
{

struct ResInstruction
{
    std::byte instruction;
    std::byte bitMask;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class ResRegisterTest : public InstructionTestBase, public ::testing::WithParamInterface<ResInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
    }
};

TEST_P(ResRegisterTest, ExecutingOpcode_ClearsSelectedBitAndPreservesFlags)
{
    const auto& [instruction, bitMask, targetRegister] = GetParam();
    constexpr auto originalValue = 0xFF_b;
    const auto originalFlags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();
    targetRegister(m_cpu.Registers()) = originalValue;
    m_cpu.Registers().flags = originalFlags;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x0_b));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(originalValue & ~bitMask));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));
}

INSTANTIATE_TEST_SUITE_P(
    ResRegisterTest, ResRegisterTest,
    ::testing::Values(
        ResInstruction{0x80_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0x81_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0x82_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0x83_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0x84_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0x85_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0x87_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0x88_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0x89_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0x8A_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0x8B_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0x8C_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0x8D_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0x8F_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0x90_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0x91_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0x92_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0x93_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0x94_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0x95_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0x97_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0x98_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0x99_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0x9A_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0x9B_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0x9C_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0x9D_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0x9F_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0xA0_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0xA1_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0xA2_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0xA3_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0xA4_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0xA5_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0xA7_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0xA8_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0xA9_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0xAA_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0xAB_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0xAC_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0xAD_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0xAF_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0xB0_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0xB1_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0xB2_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0xB3_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0xB4_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0xB5_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0xB7_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterA>()},
        ResInstruction{0xB8_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterB>()},
        ResInstruction{0xB9_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterC>()},
        ResInstruction{0xBA_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterD>()},
        ResInstruction{0xBB_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterE>()},
        ResInstruction{0xBC_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterH>()},
        ResInstruction{0xBD_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterL>()},
        ResInstruction{0xBF_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<ResRegisterTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
