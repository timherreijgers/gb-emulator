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
    std::function<std::byte(const CpuRegisters&)> registerGetter;
};

} // namespace

class LoadRegN8Test : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(LoadRegN8Test, ExecutingCommand_WithValueToLoadAs0xFF_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    m_program.WriteProgram({instructionByte, 0xFF_b, 0x00_b});

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xFF_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(LoadRegN8Test, ExecutingCommand_WithValueToLoadAs0xFE_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());
    const auto valueToLoad = 0xFE_b;

    m_program.WriteProgram({instructionByte, valueToLoad, 0x00_b});

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(valueToLoad));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(valueToLoad));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRegN8Test, LoadRegN8Test,
                         ::testing::Values(
                             InstructionPair{0x06_b, [](const CpuRegisters& registers) {
                                                 return registers.bRegister.value;
                                             }},
                             InstructionPair{0x0E_b, [](const CpuRegisters& registers) {
                                                 return registers.cRegister.value;
                                             }},
                             InstructionPair{0x16_b, [](const CpuRegisters& registers) {
                                                 return registers.dRegister.value;
                                             }},
                             InstructionPair{0x1E_b, [](const CpuRegisters& registers) {
                                                 return registers.eRegister.value;
                                             }},
                             InstructionPair{0x26_b, [](const CpuRegisters& registers) {
                                                 return registers.hRegister.value;
                                             }},
                             InstructionPair{0x2E_b, [](const CpuRegisters& registers) {
                                                 return registers.lRegister.value;
                                             }},
                             InstructionPair{0x3E_b, [](const CpuRegisters& registers) {
                                                 return registers.accumulator.value;
                                             }}),
                         [](const testing::TestParamInfo<LoadRegN8Test::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test