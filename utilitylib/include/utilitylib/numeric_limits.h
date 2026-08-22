/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <limits>

namespace UtilityLib
{

template <typename T>
struct numeric_limits : std::numeric_limits<T>
{
};

template <>
struct numeric_limits<std::byte> : std::numeric_limits<unsigned char>
{
};

} // namespace UtilityLib