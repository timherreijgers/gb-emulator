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

struct SetIndirectHLInstruction
{
    std::byte instruction;
    std::byte bitMask;
};

} // namespace

class SetIndirectHLTest : public InstructionTestBase, public ::testing::WithParamInterface<SetIndirectHLInstruction>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCB_b, GetParam().instruction});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(SetIndirectHLTest, ExecutingOpcode)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    constexpr auto originalValue = 0x55_b;
    const auto originalFlags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();
    const auto expectedValue = originalValue | GetParam().bitMask;
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = originalFlags;
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(GetParam().instruction));

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

TEST_P(SetIndirectHLTest, ExecutingOpcode_SetsSelectedMemoryBitAndPreservesFlags)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    constexpr auto originalValue = 0x55_b;
    const auto originalFlags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();
    const auto expectedValue = originalValue | GetParam().bitMask;
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = originalFlags;
    m_cpu.Step();
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(originalValue));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));

    EXPECT_CALL(m_addressableMock, WriteToAddress(address, expectedValue)).Times(1);
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(originalFlags));
}

INSTANTIATE_TEST_SUITE_P(
    SetIndirectHLTest, SetIndirectHLTest,
    ::testing::Values(
        SetIndirectHLInstruction{0xC6_b, UtilityLib::BitMask<0>},
        SetIndirectHLInstruction{0xCE_b, UtilityLib::BitMask<1>},
        SetIndirectHLInstruction{0xD6_b, UtilityLib::BitMask<2>},
        SetIndirectHLInstruction{0xDE_b, UtilityLib::BitMask<3>},
        SetIndirectHLInstruction{0xE6_b, UtilityLib::BitMask<4>},
        SetIndirectHLInstruction{0xEE_b, UtilityLib::BitMask<5>},
        SetIndirectHLInstruction{0xF6_b, UtilityLib::BitMask<6>},
        SetIndirectHLInstruction{0xFE_b, UtilityLib::BitMask<7>}),
    [](const testing::TestParamInfo<SetIndirectHLTest::ParamType>& info) {
        return std::string{ExtendedOpCodeToInstructionName(info.param.instruction)};
    });

} // namespace EmulatorLib::Test
