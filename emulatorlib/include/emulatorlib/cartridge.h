/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace EmulatorLib
{

enum class CbgSupport
{
    GAMEBOY,
    GAMEBOY_COLOR_MONOCHROME_SUPPORT,
    GAMEBOY_COLOR_ONLY
};

enum class CartridgeType : std::uint8_t
{
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    MBC6 = 0x20,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HUC3 = 0xFE,
    HUC1_RAM_BATTERY = 0xFF,
};

struct MismatchedRomSizeException : std::runtime_error
{
    explicit MismatchedRomSizeException(size_t diskRomSize, size_t headerRomSize)
        : std::runtime_error(std::format("ROM size in header ({}) does not match actual ROM size ({})", headerRomSize, diskRomSize)) {}
};

struct InvalidRomException : std::runtime_error
{
    explicit InvalidRomException(const std::filesystem::path& romPath)
        : std::runtime_error(std::format("ROM file {} is invalid", romPath.string())) {}
};

class Cartridge
{
public:
    Cartridge(std::filesystem::path romPath);

    [[nodiscard]] auto HasValidNintendoLogo() const noexcept -> bool;
    [[nodiscard]] auto Title() const noexcept -> std::string_view;
    // [[nodiscard]] auto ManufacturerCode() const noexcept -> std::string_view;
    // [[nodiscard]] auto CgbFlag() const noexcept -> CbgSupport;
    // [[nodiscard]] auto LicenseeCode() const noexcept -> std::string_view;
    [[nodiscard]] auto CartridgeType() const noexcept -> EmulatorLib::CartridgeType;
    [[nodiscard]] auto RomSize() const noexcept -> size_t;
    // [[nodiscard]] auto RamSize() const noexcept -> size_t;
    [[nodiscard]] auto ValidHeaderChecksum() const noexcept -> bool;

    [[nodiscard]] auto RomData() const noexcept -> const std::vector<std::byte>&;
    [[nodiscard]] auto RomPath() const noexcept -> const std::filesystem::path&;

private:
    std::filesystem::path m_romPath;
    std::vector<std::byte> m_romData;
};

} // namespace EmulatorLib
