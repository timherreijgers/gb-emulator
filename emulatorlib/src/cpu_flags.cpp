/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib
{

CpuFlags CpuFlags::Zero{1 << 7};
CpuFlags CpuFlags::Subtract{1 << 6};
CpuFlags CpuFlags::HalfCarry{1 << 5};
CpuFlags CpuFlags::Carry{1 << 4};

} // namespace EmulatorLib