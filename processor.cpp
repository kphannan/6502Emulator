

#include <iostream>
#include <iomanip>

#include "6502.hpp"

namespace cpu
{

    Memory defaultMemory;

    Processor::Processor() : Processor(defaultMemory)
    {
        reset();
    }

    Processor::Processor(Memory &memory) : memory(memory)
    {
        reset();
    }

    void Processor::reset()
    {
        model.registers.A = 0xAB;
        model.registers.X = 0xCD;
        model.registers.Y = 0xE5;
        model.registers.S = 0xFF;

        // Load the reset vector and jump to the address stored at that location
        // model.registers.PC = (Word)HardwareVector::RESET;
        model.registers.PC = std::to_underlying(HardwareVector::RESET);

        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "PC: " << std::setfill('0') << std::setw(4) << (int)model.registers.PC << " reset vector" << std::endl;
        std::cout.unsetf(std::ios::basefield);

        model.registers.PC = memory.readWord(model.registers.PC); // jump to address
        // Load PC with the address at the reset vector

        model.P = 0b10100101; // TODO reset it properly...
    }

    void Processor::showRegisters()
    {
        // TODO output stream does not treat a Word the same as an int so the format is off
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "6502 Registers" << std::endl;
        std::cout << "         A: " << std::setfill('0') << std::setw(2) << (int)(model.registers.A) << " Accumulator" << std::endl;
        std::cout << "         X: " << std::setfill('0') << std::setw(2) << (int)(model.registers.X) << " Index register X" << std::endl;
        std::cout << "         Y: " << std::setfill('0') << std::setw(2) << (int)(model.registers.Y) << " Index register Y" << std::endl;
        std::cout << "         S: " << std::setfill('0') << std::setw(2) << (int)(model.registers.S) << " Stack pointer" << std::endl;
        std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(model.registers.PC) << " Program Counter" << std::endl;
        std::cout << "         P: N V 1 B D I Z C  Processor Status Register" << std::endl
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
    }
    //    std::cout << " A: " << std:format( "  A: {} ", model.registers.A ) << " Accumulator" << std::endl;
    //     std::cout << " X: " << std:format( "  X: {} ", model.registers.X ) << " Index register X" << std::endl;
    //     std::cout << " Y: " << std:format( "  Y: {} ", model.registers.Y ) << " Index register Y" << std::endl;
    //     std::cout << " S: " << std:format( "  S: {} ", model.registers.S ) << " Stack pointer" << std::endl;
    //     std::cout << "PC: " << std:format( " PC: {} ", model.registers.PC ) << " Program Counter" << std::endl;

    // void Processor::memoryBank(Memory &memory)
    // {
    //     this->memory = memory;
    // }

    Memory Processor::currentMemory()
    {
        return memory;
    }

    // ===== Execute Instructions =====

    void Processor::execute()
    {
        execute(-1);
    }

    void Processor::execute(Address address)
    {
        // set PC and execute 1 instruction
    }

    // Instruction Pipeline
    // fetch OpCode (advance PC)
    // decode OpCode
    // fetch operand (advance PC as defined by operand)
    // evaluate OpCode (set status flags)

    void Processor::execute(int numberOfInstructions)
    {
        std::cout << " execute instruction pipeline " << std::endl;

        // Pseudo instruction pipeline....
        fetchOpCode();
        fetchOperand();

        // TODO evaluate the instruction
    }

    void Processor::fetchOpCode()
    {
        int opcode = memory.read(model.registers.PC++);
        instruction = opcode;
        std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " opcode: " << std::setfill('0') << std::setw(2) << instruction << std::endl;
        std::cout << " OpCode: " << opcode << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

    void Processor::fetchOperand()
    {
        std::cout << " fetch operand " << std::endl;
        model.registers.A = memory.read(model.registers.PC++);
        // std::cout << " implement fetch opcode based on the addressing mode of the instruction" << std::end;
    }

}
