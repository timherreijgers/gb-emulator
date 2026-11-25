/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/interrupt/interrupt.h"

#include <utility>

namespace EmulatorLib::Test
{

static_assert(std::to_underlying(Interrupt::VBlank) == 1 << 0);
static_assert(std::to_underlying(Interrupt::Lcd) == 1 << 1);
static_assert(std::to_underlying(Interrupt::Timer) == 1 << 2);
static_assert(std::to_underlying(Interrupt::Serial) == 1 << 3);
static_assert(std::to_underlying(Interrupt::Joypad) == 1 << 4);

} // namespace EmulatorLib::Test