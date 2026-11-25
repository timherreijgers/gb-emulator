/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/interrupt/interrupt.h"

#include <optional>

namespace EmulatorLib
{

class InterruptAcknowledgeable
{
public:
    InterruptAcknowledgeable() noexcept = default;
    virtual ~InterruptAcknowledgeable() = default;
    InterruptAcknowledgeable(InterruptAcknowledgeable const&) = delete;
    InterruptAcknowledgeable(InterruptAcknowledgeable&&) = delete;
    InterruptAcknowledgeable& operator=(InterruptAcknowledgeable const&) = delete;
    InterruptAcknowledgeable& operator=(InterruptAcknowledgeable&&) = delete;

    virtual void Acknowledge(Interrupt interrupt) noexcept = 0;
    virtual auto HighestPriorityPending() -> std::optional<Interrupt> = 0;
};

} // namespace EmulatorLib