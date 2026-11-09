/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"

#include <cstddef>
#include <format>

namespace EmulatorLib::Test
{

namespace
{

struct DaaTestParam
{
    std::byte flags;
    std::byte accumulator;
    std::byte expectedFlags;
    std::byte expectedAccumulator;
};

} // namespace

class DaaTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x27_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

class DaaTestWithoutSubtractFlag : public DaaTest, public ::testing::WithParamInterface<DaaTestParam>
{
};

TEST_P(DaaTestWithoutSubtractFlag, ExecuteOpCode)
{
    const auto& [flags, accumulator, expectedFlags, expectedAccumulator] = GetParam();
    m_cpu.Registers().flags = flags;
    m_cpu.Registers().accumulator = accumulator;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x27_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(expectedFlags));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(expectedAccumulator));
}

INSTANTIATE_TEST_SUITE_P(
    DaaTest, DaaTestWithoutSubtractFlag,
    ::testing::Values(
        DaaTestParam{0x00_b, 0x00_b, CpuFlags::Zero.AsByte(), 0x00_b},
        DaaTestParam{0x00_b, 0x0A_b, 0x00_b, 0x10_b},
        DaaTestParam{CpuFlags::HalfCarry.AsByte(), 0x00_b, 0x00_b, 0x06_b},
        DaaTestParam{CpuFlags::Carry.AsByte(), 0x00_b, CpuFlags::Carry.AsByte(), 0x60_b},
        DaaTestParam{(CpuFlags::HalfCarry | CpuFlags::Carry).AsByte(), 0x00_b, CpuFlags::Carry.AsByte(), 0x66_b},
        DaaTestParam{0x00_b, 0x9A_b, (CpuFlags::Zero | CpuFlags::Carry).AsByte(), 0x00_b}),
    [](const testing::TestParamInfo<DaaTestWithoutSubtractFlag::ParamType>& info) {
        return std::format("{}_Flags{:02X}_Accumulator{:02X}", OpCodeToInstructionName(0x27_b),
                           std::to_integer<unsigned>(info.param.flags),
                           std::to_integer<unsigned>(info.param.accumulator));
    });

class DaaTestWithSubtractFlag : public DaaTest, public ::testing::WithParamInterface<DaaTestParam>
{
};

TEST_P(DaaTestWithSubtractFlag, ExecuteOpCode)
{
    const auto& [flags, accumulator, expectedFlags, expectedAccumulator] = GetParam();
    m_cpu.Registers().flags = flags;
    m_cpu.Registers().accumulator = accumulator;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x27_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(expectedFlags));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(expectedAccumulator));
}

INSTANTIATE_TEST_SUITE_P(
    DaaTest, DaaTestWithSubtractFlag,
    ::testing::Values(
        DaaTestParam{CpuFlags::Subtract.AsByte(), 0x00_b, (CpuFlags::Subtract | CpuFlags::Zero).AsByte(), 0x00_b},
        DaaTestParam{CpuFlags::Subtract.AsByte(), 0x10_b, CpuFlags::Subtract.AsByte(), 0x10_b},
        DaaTestParam{(CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte(), 0x06_b,
                     (CpuFlags::Subtract | CpuFlags::Zero).AsByte(), 0x00_b},
        DaaTestParam{(CpuFlags::Subtract | CpuFlags::Carry).AsByte(), 0x60_b,
                     (CpuFlags::Subtract | CpuFlags::Carry | CpuFlags::Zero).AsByte(), 0x00_b},
        DaaTestParam{(CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte(), 0x66_b,
                     (CpuFlags::Subtract | CpuFlags::Carry | CpuFlags::Zero).AsByte(), 0x00_b}),
    [](const testing::TestParamInfo<DaaTestWithSubtractFlag::ParamType>& info) {
        return std::format("{}_Flags{:02X}_Accumulator{:02X}", OpCodeToInstructionName(0x27_b),
                           std::to_integer<unsigned>(info.param.flags),
                           std::to_integer<unsigned>(info.param.accumulator));
    });

} // namespace EmulatorLib::Test
