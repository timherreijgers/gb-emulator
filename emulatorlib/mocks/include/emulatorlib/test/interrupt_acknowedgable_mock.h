/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/interrupt/interrupt_acknowedgable.h"

#include <gmock/gmock.h>

namespace EmulatorLib
{

class InterruptAcknowledgeableMock : public InterruptAcknowledgeable
{
public:
    MOCK_METHOD(void, Acknowledge, (Interrupt), (override, noexcept));
    MOCK_METHOD(std::optional<Interrupt>, HighestPriorityPending, (), (override));
};

} // namespace EmulatorLib