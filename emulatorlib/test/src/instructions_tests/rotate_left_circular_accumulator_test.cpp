/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

namespace
{

struct RlcaTestParam
{
    std::byte accumulatorValue;
    std::byte expectedValue;
    bool carryFlagIsSet;
};

[[nodiscard]] constexpr auto CalculateExpectedValue(std::byte val) noexcept -> std::byte
{
    const auto bit7Set = static_cast<bool>(val & 0x80_b);
    return static_cast<std::byte>((static_cast<std::uint16_t>(val) << 1) | bit7Set);
}

} // namespace

class RlcaTest : public InstructionTestBase, public ::testing::WithParamInterface<RlcaTestParam>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x07_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(RlcaTest, ExecutingOpCode)
{
    const auto [accumulatorValue, expectedValue, carryFlagIsSet] = GetParam();

    m_cpu.Registers().accumulator = accumulatorValue;
    m_cpu.Registers().flags = (CpuFlags::Carry | CpuFlags::Zero | CpuFlags::HalfCarry | CpuFlags::Subtract).AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(accumulatorValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(expectedValue));
    ASSERT_THAT(m_cpu.Registers().flags, carryFlagIsSet ? CpuFlags::Carry.AsByte() : 0x00_b);
}

INSTANTIATE_TEST_SUITE_P(RlcaTest, RlcaTest, ::testing::Values(RlcaTestParam{0x07_b, CalculateExpectedValue(0x07_b), false}, RlcaTestParam{0xFF_b, 0xFF_b, true}, RlcaTestParam{0xF0_b, CalculateExpectedValue(0xF0_b), true}, RlcaTestParam{0x0F_b, CalculateExpectedValue(0x0F_b), false}));

} // namespace EmulatorLib::Test