/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once
#include <cstddef>
#include <string_view>

namespace EmulatorLib
{

[[nodiscard]] auto OpCodeToInstructionName(std::byte opcode) noexcept -> std::string_view;

} // namespace EmulatorLib