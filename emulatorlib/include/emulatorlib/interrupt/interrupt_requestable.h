/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/interrupt/interrupt.h"

namespace EmulatorLib
{

class InterruptRequestable
{
public:
    InterruptRequestable() noexcept = default;
    virtual ~InterruptRequestable() = default;
    InterruptRequestable(InterruptRequestable const&) = delete;
    InterruptRequestable(InterruptRequestable&&) = delete;
    InterruptRequestable& operator=(InterruptRequestable const&) = delete;
    InterruptRequestable& operator=(InterruptRequestable&&) = delete;

    virtual void RequestInterrupt(Interrupt interrupt) noexcept = 0;
};

} // namespace EmulatorLib