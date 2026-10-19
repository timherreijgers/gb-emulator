/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <array>

export module emulatorlib.cpu:opcode_handler_map;

import emulatorlib.cpu;

export namespace EmulatorLib
{

[[nodiscard]] auto CreateOpcodeHandlers() noexcept -> std::array<OpcodeHandler, 256>;

} // namespace EmulatorLib
