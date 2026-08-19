/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::function<uint16_t(const CpuRegisters&)> registerGetter;
};

} // namespace

class LoadRegN16Test : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(LoadRegN16Test, ExecutingCommand_WithValueToLoadAs0x1234_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    m_program.WriteProgram({instructionByte, 0x34_b, 0x12_b, 0x00_b});

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x34_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x34_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x12_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0x1234));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(LoadRegN16Test, ExecutingCommand_WithValueToLoadAs0x5678_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    m_program.WriteProgram({instructionByte, 0x78_b, 0x56_b, 0x00_b});

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x78_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x78_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x56_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0x5678));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRegN16Test, LoadRegN16Test,
                         ::testing::Values(
                             InstructionPair{0x01_b, [](const CpuRegisters& registers) {
                                                 return registers.bcRegister.value;
                                             }},
                             InstructionPair{0x11_b, [](const CpuRegisters& registers) {
                                                 return registers.deRegister.value;
                                             }},
                             InstructionPair{0x21_b, [](const CpuRegisters& registers) {
                                                 return registers.hlRegister.value;
                                             }},
                             InstructionPair{0x31_b, [](const CpuRegisters& registers) {
                                                 return registers.stackPointer.value;
                                             }}),
                         [](const testing::TestParamInfo<LoadRegN16Test::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test