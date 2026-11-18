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

struct SetInstruction
{
    std::byte instruction;
    std::byte bitMask;
    std::function<Register8Bit&(CpuRegisters&)> targetRegister;
};

} // namespace

class SetRegisterTest : public InstructionTestBase, public ::testing::WithParamInterface<SetInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
        m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Carry).AsByte();
    }
};

TEST_P(SetRegisterTest, ExecutingOpcode_SetsSelectedBitAndPreservesFlags)
{
    const auto& [instruction, bitMask, targetRegister] = GetParam();
    targetRegister(m_cpu.Registers()) = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x0_b));
    ASSERT_THAT(targetRegister(m_cpu.Registers()), ::testing::Eq(bitMask));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::Carry).AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    SetRegisterTest, SetRegisterTest,
    ::testing::Values(
        SetInstruction{0xC0_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xC1_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xC2_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xC3_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xC4_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xC5_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xC7_b, UtilityLib::BitMask<0>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xC8_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xC9_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xCA_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xCB_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xCC_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xCD_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xCF_b, UtilityLib::BitMask<1>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xD0_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xD1_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xD2_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xD3_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xD4_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xD5_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xD7_b, UtilityLib::BitMask<2>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xD8_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xD9_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xDA_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xDB_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xDC_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xDD_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xDF_b, UtilityLib::BitMask<3>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xE0_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xE1_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xE2_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xE3_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xE4_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xE5_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xE7_b, UtilityLib::BitMask<4>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xE8_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xE9_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xEA_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xEB_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xEC_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xED_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xEF_b, UtilityLib::BitMask<5>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xF0_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xF1_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xF2_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xF3_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xF4_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xF5_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xF7_b, UtilityLib::BitMask<6>, FunctionWrapper8Bit<RegisterA>()},
        SetInstruction{0xF8_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterB>()},
        SetInstruction{0xF9_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterC>()},
        SetInstruction{0xFA_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterD>()},
        SetInstruction{0xFB_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterE>()},
        SetInstruction{0xFC_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterH>()},
        SetInstruction{0xFD_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterL>()},
        SetInstruction{0xFF_b, UtilityLib::BitMask<7>, FunctionWrapper8Bit<RegisterA>()}),
    [](const testing::TestParamInfo<SetRegisterTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
