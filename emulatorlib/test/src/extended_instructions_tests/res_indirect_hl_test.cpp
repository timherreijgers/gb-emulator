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

struct ResIndirectHLInstruction
{
    std::byte instruction;
    std::byte bitMask;
};

} // namespace

class ResIndirectHLTest : public InstructionTestBase, public ::testing::WithParamInterface<ResIndirectHLInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(ResIndirectHLTest, ExecutingOpcode)
{
    const auto [instruction, bitmask] = GetParam();

    constexpr auto address = static_cast<uint16_t>(0x1234);
    constexpr auto originalValue = 0xFF_b;
    const auto originalFlags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();
    const auto expectedValue = originalValue & ~GetParam().bitMask;
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = originalFlags;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(originalValue));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(originalValue));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, expectedValue)).Times(1);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(ResIndirectHLTest, ExecutingOpcode_ClearsSelectedMemoryBitAndPreservesFlags)
{
    const auto [instruction, bitmask] = GetParam();

    constexpr auto address = static_cast<uint16_t>(0x1234);
    constexpr auto originalValue = 0xFF_b;
    const auto originalFlags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();
    const auto expectedValue = originalValue & ~GetParam().bitMask;
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = originalFlags;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(originalValue));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(originalValue));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, expectedValue)).Times(1);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));
}

INSTANTIATE_TEST_SUITE_P(
    ResIndirectHLTest, ResIndirectHLTest,
    ::testing::Values(
        ResIndirectHLInstruction{0x86_b, UtilityLib::BitMask<0>},
        ResIndirectHLInstruction{0x8E_b, UtilityLib::BitMask<1>},
        ResIndirectHLInstruction{0x96_b, UtilityLib::BitMask<2>},
        ResIndirectHLInstruction{0x9E_b, UtilityLib::BitMask<3>},
        ResIndirectHLInstruction{0xA6_b, UtilityLib::BitMask<4>},
        ResIndirectHLInstruction{0xAE_b, UtilityLib::BitMask<5>},
        ResIndirectHLInstruction{0xB6_b, UtilityLib::BitMask<6>},
        ResIndirectHLInstruction{0xBE_b, UtilityLib::BitMask<7>}),
    [](const testing::TestParamInfo<ResIndirectHLTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
