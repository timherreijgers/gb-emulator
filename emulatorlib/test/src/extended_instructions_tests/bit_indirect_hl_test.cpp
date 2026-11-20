/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_translation.h"
#include "instructions_tests/instruction_test_base.h"

#include "emulatorlib/cpu_flags.h"

#include "utilitylib/bit_mask.h"

namespace EmulatorLib::Test
{

namespace
{

struct BitIndirectHLInstruction
{
    std::byte instruction;
    std::byte bitMask;
};

} // namespace

class BitIndirectHLTest : public InstructionTestBase, public ::testing::WithParamInterface<BitIndirectHLInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(BitIndirectHLTest, ExecutingOpcode)
{
    [[maybe_unused]] const auto& [instruction, bitMask] = GetParam();

    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(~bitMask));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(~bitMask));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::HalfCarry).AsByte()));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(BitIndirectHLTest, ExecutingOpcode_WhenBitIsClear_SetsZeroFlag)
{
    [[maybe_unused]] const auto& [instruction, bitMask] = GetParam();

    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(~bitMask));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(~bitMask));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::HalfCarry).AsByte()));
}

TEST_P(BitIndirectHLTest, ExecutingOpcode_WhenBitIsSet_DoesNotSetZeroFlag)
{
    [[maybe_unused]] const auto& [instruction, bitMask] = GetParam();
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(bitMask));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(bitMask));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_P(BitIndirectHLTest, ExecutingOpcode_SetsAllFlagsToCorrectValue_FromAllOnes)
{
    [[maybe_unused]] const auto& [instruction, bitMask] = GetParam();
    m_cpu.Registers().hlRegister = 0x1234;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Carry | CpuFlags::Subtract).AsByte();
    m_cpu.Step();
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x1234)).WillOnce(::testing::Return(0x00_b));
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq((CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte()));
}

INSTANTIATE_TEST_SUITE_P(
    BitIndirectHLTest, BitIndirectHLTest,
    ::testing::Values(
        BitIndirectHLInstruction{0x46_b, UtilityLib::BitMask<0>},
        BitIndirectHLInstruction{0x4E_b, UtilityLib::BitMask<1>},
        BitIndirectHLInstruction{0x56_b, UtilityLib::BitMask<2>},
        BitIndirectHLInstruction{0x5E_b, UtilityLib::BitMask<3>},
        BitIndirectHLInstruction{0x66_b, UtilityLib::BitMask<4>},
        BitIndirectHLInstruction{0x6E_b, UtilityLib::BitMask<5>},
        BitIndirectHLInstruction{0x76_b, UtilityLib::BitMask<6>},
        BitIndirectHLInstruction{0x7E_b, UtilityLib::BitMask<7>}),
    [](const testing::TestParamInfo<BitIndirectHLTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
