/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <limits>

namespace std
{

template <>
class numeric_limits<std::byte> : public numeric_limits<unsigned char>
{
};

} // namespace std