/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadHLFromSPPlusETest : public InstructionTestBase
{
protected:
    void ExecuteInstructionAndValidateSteps(const uint16_t initialHl,
                                            const uint16_t initialSp,
                                            std::byte signedOffset,
                                            const uint16_t expectedHl,
                                            std::byte expectedFlags)
    {
        EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);

        m_cpu.Step();
        ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF8_b));
        ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0101));
        ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(initialSp));
        ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(initialHl));

        m_cpu.Step();
        ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
        ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(signedOffset));
        ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(initialHl));
        ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF8_b));

        m_cpu.Step();
        const auto expectedHlAfterM3 = static_cast<uint16_t>((initialHl & 0xFF00) | (expectedHl & 0x00FF));
        ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF8_b));
        ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
        ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(expectedHlAfterM3));
        ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(initialSp));
        ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(expectedFlags));

        m_cpu.Step();
        ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
        ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0103));
        ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(expectedHl));
        ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(initialSp));
        ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(expectedFlags));
    }

    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadHLFromSPPlusETest, ExecutingOpCode_AdditionWithoutCheckingFlags_BehavesCorrectly)
{
    constexpr auto offset = 0x01_b;
    constexpr auto stackPointer = static_cast<uint16_t>(0x1234);
    constexpr auto initialHl = static_cast<uint16_t>(0x0000);

    m_program.WriteProgram({0xF8_b, offset, 0x00_b});
    m_cpu.Registers().stackPointer = stackPointer;
    m_cpu.Registers().hlRegister = initialHl;

    ExecuteInstructionAndValidateSteps(initialHl, stackPointer, offset, 0x1235, CpuFlags::Zero.AsByte());
}

TEST_F(LoadHLFromSPPlusETest, ExecutingOpCode_AdditionSetsHalfCarryFlag_BehavesCorrectly)
{
    constexpr auto offset = 0x01_b;
    constexpr auto stackPointer = static_cast<uint16_t>(0x120F);
    constexpr auto initialHl = static_cast<uint16_t>(0xA5A5);

    m_program.WriteProgram({0xF8_b, offset, 0x00_b});
    m_cpu.Registers().stackPointer = stackPointer;
    m_cpu.Registers().hlRegister = initialHl;

    ExecuteInstructionAndValidateSteps(initialHl, stackPointer, offset, 0x1210, CpuFlags::HalfCarry.AsByte());
}

TEST_F(LoadHLFromSPPlusETest, ExecutingOpCode_AdditionSetsCarryFlag_BehavesCorrectly)
{
    constexpr auto offset = 0x10_b;
    constexpr auto stackPointer = static_cast<uint16_t>(0x12F0);
    constexpr auto initialHl = static_cast<uint16_t>(0x0001);

    m_program.WriteProgram({0xF8_b, offset, 0x00_b});
    m_cpu.Registers().stackPointer = stackPointer;
    m_cpu.Registers().hlRegister = initialHl;

    ExecuteInstructionAndValidateSteps(initialHl, stackPointer, offset, 0x1300, CpuFlags::Carry.AsByte());
}

TEST_F(LoadHLFromSPPlusETest, ExecutingOpCode_AdditionSetsHalfCarryAndCarryFlags_BehavesCorrectly)
{
    constexpr auto offset = 0x01_b;
    constexpr auto stackPointer = static_cast<uint16_t>(0x12FF);
    constexpr auto initialHl = static_cast<uint16_t>(0xBEEF);

    m_program.WriteProgram({0xF8_b, offset, 0x00_b});
    m_cpu.Registers().stackPointer = stackPointer;
    m_cpu.Registers().hlRegister = initialHl;

    ExecuteInstructionAndValidateSteps(
        initialHl,
        stackPointer,
        offset,
        0x1300,
        (CpuFlags::HalfCarry | CpuFlags::Carry).AsByte());
}

} // namespace EmulatorLib::Test
