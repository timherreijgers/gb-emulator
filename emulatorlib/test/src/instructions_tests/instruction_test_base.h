/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "emulatorlib/test/interrupt_acknowedgable_mock.h"
#include "emulatorlib/test/program_stub.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class InstructionTestBase : public ::testing::Test
{
protected:
    testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    testing::NiceMock<InterruptAcknowledgeableMock> m_interruptAcknowledgeableMock;
    ProgramStub m_program{0x100};
    AddressBus m_bus{{m_addressableMock, m_program}};
    Cpu m_cpu{m_bus, m_interruptAcknowledgeableMock};
};

} // namespace EmulatorLib::Test