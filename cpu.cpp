

#include <iostream>
#include <iomanip>

#include "6502.hpp"

namespace m6502
{
    memory::Memory defaultMemory;

    CPU::CPU() : CPU(defaultMemory)
    {
        reset();
    }

    CPU::CPU(memory::Memory &memory)
        : addressSpace(memory)
    // : addressSpace(memory), pipeline(memory, this->model.registers.PC)
    {
        pipeline = new CPU::Pipeline(*this);
        reset();
    }

    // CPU::CPU()

    void CPU::reset()
    {
        model.registers.A = 0xAB;
        model.registers.X = 0xCD;
        model.registers.Y = 0xE5;
        model.registers.S = 0xFF;
        model.P = 0b10100101; // TODO reset it properly...

        // Reset the CPU / decode pipeline from the reset vector
        pipeline->reset(std::to_underlying(HardwareVector::RESET));

        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "PC: " << std::setfill('0') << std::setw(4) << (int)model.registers.PC << " reset vector" << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

    // hardware::Address &CPU::PC()
    // {
    //     return model.registers.PC;
    // }
    // hardware::Byte &CPU::A()
    // {
    //     return cpu.model.registers.A;
    // }
    // hardware::Byte &CPU::X()
    // {
    //     return cpu.model.registers.X;
    // }
    // hardware::Byte &CPU::Y()
    // {
    //     return cpu.model.registers.Y;
    // }

    void CPU::showRegisters()
    {
        // TODO output stream does not treat a Word the same as an int so the format is off
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "6502 Registers" << std::endl;
        std::cout << "         A: " << std::setfill('0') << std::setw(2) << (int)(model.registers.A) << " Accumulator" << std::endl;
        std::cout << "         X: " << std::setfill('0') << std::setw(2) << (int)(model.registers.X) << " Index register X" << std::endl;
        std::cout << "         Y: " << std::setfill('0') << std::setw(2) << (int)(model.registers.Y) << " Index register Y" << std::endl;
        std::cout << "         S: " << std::setfill('0') << std::setw(2) << (int)(model.registers.S) << " Stack pointer" << std::endl;
        std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(model.registers.PC) << " Program Counter" << std::endl;
        std::cout << "         P: N V 1 B D I Z C  CPU Status Register" << std::endl
                  << "            "
                  << std::setw(1)
                  << std::bitset<1>(model.flags.N) << " "
                  << std::bitset<1>(model.flags.V) << " "
                  << std::bitset<1>(model.flags.one) << " "
                  << std::bitset<1>(model.flags.B) << " "
                  << std::bitset<1>(model.flags.D) << " "
                  << std::bitset<1>(model.flags.I) << " "
                  << std::bitset<1>(model.flags.Z) << " "
                  << std::bitset<1>(model.flags.C) << " "
                  << std::endl
                  << std::resetiosflags(std::ios::basefield) << std::setiosflags(std::ios::oct)
                  //   << setf(std::ios::binary)
                  //   << std::bitset<8>(model.P)
                  << std::endl;
        std::cout.unsetf(std::ios::basefield);

        pipeline->showPipeline();
    }
    //    std::cout << " A: " << std:format( "  A: {} ", model.registers.A ) << " Accumulator" << std::endl;
    //     std::cout << " X: " << std:format( "  X: {} ", model.registers.X ) << " Index register X" << std::endl;
    //     std::cout << " Y: " << std:format( "  Y: {} ", model.registers.Y ) << " Index register Y" << std::endl;
    //     std::cout << " S: " << std:format( "  S: {} ", model.registers.S ) << " Stack pointer" << std::endl;
    //     std::cout << "PC: " << std:format( " PC: {} ", model.registers.PC ) << " Program Counter" << std::endl;

    // void CPU::memoryBank(Memory &memory)
    // {
    //     this->memory = memory;
    // }

    memory::Memory &CPU::currentMemory()
    {
        return addressSpace;
    }

    // ===== Execute Instructions =====

    void CPU::execute()
    {
        execute(-1);
    }

    void CPU::execute(hardware::Address address)
    {
        // set PC and execute 1 instruction
    }

    void CPU::execute(int numberOfInstructions)
    {
        pipeline->execute(numberOfInstructions);
    }

}
