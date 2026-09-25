#include "emulatorlib/address_bus.h"
#include "emulatorlib/cartridge.h"
#include "emulatorlib/cpu.h"
#include "utilitylib/byte_utils.h"

#include <iostream>

template <>
struct std::formatter<EmulatorLib::CpuRegisters>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const EmulatorLib::CpuRegisters& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "A: {}\nF: {}\nB: {}\nC: {}\nD: {}\nE: {}\nH: {}\nL: {}\nSP: 0x{:04X}\nPC: 0x{:04X}",
                              obj.accumulator.value,
                              obj.flags.value,
                              obj.bRegister.value,
                              obj.cRegister.value,
                              obj.dRegister.value,
                              obj.eRegister.value,
                              obj.hRegister.value,
                              obj.lRegister.value,
                              obj.stackPointer.value,
                              obj.programCounter.value);
    }
};

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        std::println("Usage: emulator <rom_file>");
        std::exit(-1);
    }

    const std::string romFilePath = argv[1];

    EmulatorLib::Cartridge cartridge(romFilePath);

    EmulatorLib::AddressBus addressBus{std::vector<std::reference_wrapper<EmulatorLib::AddressBusAddressable>>{std::ref(cartridge)}};
    EmulatorLib::Cpu cpu{addressBus};

    try
    {
        while (true)
        {
            cpu.Step();
        }
    }
    catch (const std::exception& e)
    {
        std::println("Exception caught: {}", e.what());
        std::println("Register state: \n{}", cpu.Registers());

        std::exit(-1);
    }

    return 0;
}
