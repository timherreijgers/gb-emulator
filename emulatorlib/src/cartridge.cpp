/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cartridge.h"

#include "utilitylib/byte_utils.h"

#include <array>
#include <fstream>
#include <utility>

namespace EmulatorLib
{

namespace
{

constexpr auto g_nintendoLogoRomOffset = 0x104;

constexpr std::array<std::byte, 4 * 12> g_nintendoLogoData = {
    0xCE_b, 0xED_b, 0x66_b, 0x66_b,
    0xCC_b, 0x0D_b, 0x00_b, 0x0B_b,
    0x03_b, 0x73_b, 0x00_b, 0x83_b,
    0x00_b, 0x0C_b, 0x00_b, 0x0D_b,
    0x00_b, 0x08_b, 0x11_b, 0x1F_b,
    0x88_b, 0x89_b, 0x00_b, 0x0E_b,
    0xDC_b, 0xCC_b, 0x6E_b, 0xE6_b,
    0xDD_b, 0xDD_b, 0xD9_b, 0x99_b,
    0xBB_b, 0xBB_b, 0x67_b, 0x63_b,
    0x6E_b, 0x0E_b, 0xEC_b, 0xCC_b,
    0xDD_b, 0xDC_b, 0x99_b, 0x9F_b,
    0xBB_b, 0xB9_b, 0x33_b, 0x3E_b};

[[nodiscard]] auto LoadRomData(const std::filesystem::path& romPath) -> std::vector<std::byte>
{
    std::vector<std::byte> romData;
    std::ifstream romFile(romPath, std::ios::ate | std::ios::binary);

    if (!romFile.is_open())
    {
        throw std::runtime_error(std::format("Failed to open ROM file {}", romPath.string()));
    }

    const auto diskRomSize = romFile.tellg();
    if (diskRomSize < 0x0150 || diskRomSize > 0x800000)
    {
        throw InvalidRomException(romPath);
    }

    romData.resize(diskRomSize);
    romFile.seekg(0);
    romFile.read(reinterpret_cast<char *>(romData.data()), romData.size());

    if (romFile.fail() || romFile.gcount() != static_cast<std::streamsize>(romData.size()))
    {
        throw std::runtime_error(std::format("Failed to read ROM file {}", romPath.string()));
    }

    const auto romSizeShift = std::to_underlying(romData[0x0148]);
    if (romSizeShift > 0x08)
    {
        throw InvalidRomException(romPath);
    }

    const auto calculatedRomSizeFromHeader = 0x8000Uz * (1 << romSizeShift);

    if (calculatedRomSizeFromHeader != romData.size())
    {
        throw MismatchedRomSizeException(romData.size(), calculatedRomSizeFromHeader);
    }

    return romData;
}

} // namespace

Cartridge::Cartridge(std::filesystem::path romPath)
    : m_romPath(std::move(romPath))
    , m_romData(LoadRomData(m_romPath))
{
}

auto Cartridge::HasValidNintendoLogo() const noexcept -> bool
{
    for (size_t i = 0; i < g_nintendoLogoData.size(); ++i)
    {
        if (m_romData[g_nintendoLogoRomOffset + i] != g_nintendoLogoData[i])
        {
            return false;
        }
    }
    return true;
}

auto Cartridge::Title() const noexcept -> std::string_view
{
    return {reinterpret_cast<const char *>(m_romData.data() + 0x134), 16};
}

auto Cartridge::CartridgeType() const noexcept -> EmulatorLib::CartridgeType
{
    return static_cast<EmulatorLib::CartridgeType>(m_romData[0x147]);
}

auto Cartridge::RomSize() const noexcept -> size_t
{
    return 0x8000 * (1 << std::to_underlying(m_romData[0x148]));
}

auto Cartridge::ValidHeaderChecksum() const noexcept -> bool
{
    auto calculatedChecksum = 0x00_b;
    for (size_t i = 0x0134; i < 0x014D; ++i)
    {
        calculatedChecksum = calculatedChecksum - m_romData[i] - 1_b;
    }

    return calculatedChecksum == m_romData[0x014D];
}
auto Cartridge::RomData() const noexcept -> const std::vector<std::byte>&
{
    return m_romData;
}

auto Cartridge::RomPath() const noexcept -> const std::filesystem::path&
{
    return m_romPath;
}

} // namespace EmulatorLib