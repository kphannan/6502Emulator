

#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    memory::Memory defaultMemory;

    CPU::CPU() : CPU(defaultMemory)
    {
        reset();
    }

    CPU::CPU(memory::Memory &memory)
        : addressSpace(memory)
    // : addressSpace(memory), pipeline(memory, this->registers.PC)
    {
        // ===== Addressing Modes =====
        _addressModeUndefined = new AddressModeUndefined(*this);
        _addressModeImplied = new AddressModeImplied(*this);
        _addressModeAccumulator = new AddressModeAccumulator(*this);
        _addressModeZeroPage = new AddressModeZeroPage(*this);
        _addressModeZeroPageIndexedX = new AddressModeZeroPageIndexedX(*this);
        _addressModeZeroPageIndexedY = new AddressModeZeroPageIndexedY(*this);
        _addressModeRelative = new AddressModeRelative(*this);
        _addressModeAbsolute = new AddressModeAbsolute(*this);
        _addressModeAbsoluteIndexedX = new AddressModeAbsoluteIndexedX(*this);
        _addressModeAbsoluteIndexedY = new AddressModeAbsoluteIndexedY(*this);
        _addressModeIndirect = new AddressModeIndirect(*this);
        _addressModeIndexedIndirectX = new AddressModeIndexedIndirectX(*this);
        _addressModeIndirectIndexedY = new AddressModeIndirectIndexedY(*this);
        _addressModeImmediate = new AddressModeImmediate(*this);

        // ===== Instructions =====
        _instructionUndefined = new Instruction(*this, "Illegal Instruction", "bad");

        // ===== Transfer Instructions
        // ----- Load
        _instructionLoad = new InstructionLoad(*this, "load instruction", "load");
        // _instructionLoad = new InstructionLoad(*this);
        // --- LDA
        // _instructionLoadA = new InstructionLoadA(*this);
        _instructionLoadA = _instructionLoad;
        // --- LDX
        // _instructionLoadX = new InstructionLoadX(*this);
        _instructionLoadX = _instructionLoad;
        // --- LDY
        // _instructionLoadY = new InstructionLoadY(*this);
        _instructionLoadY = _instructionLoad;
        //

        // ----- Store
        _instructionStore = new InstructionStore(*this);
        // --- STA
        // --- STX
        // --- STY

        // ----- Interregister transfer
        _instructionTransfer = new InstructionTransfer(*this);
        // --- TAX
        _instructionTransferAtoX = _instructionTransfer;
        // --- TAY
        // --- TSX
        _instructionTransferStoX = _instructionTransfer;
        // --- TXA
        // --- TXS
        // --- TYA

        // ===== Stack Instructions
        // --- PHA
        // --- PHP
        // --- PLA
        // --- PLP
        // ===== Decrement & Increment
        // --- DEC
        // --- DEX
        // --- DEY
        // --- INC
        // --- INX
        // --- INY
        // ===== Arithmetic Instructions
        // --- ADC
        // --- SBC
        // ===== Logical Instructions
        _instructionLogical = new InstructionLogical(*this);
        // --- AND
        // --- EOR
        // --- ORA
        // ===== Shift & Rotate Instructions
        // --- ASL
        // --- LSR
        // --- ROL
        // --- ROR
        // ===== Flag Instructions
        // --- CLC
        // --- CLD
        // --- CLI
        // --- CLV
        // --- SEC
        // --- SED
        // --- SEI
        // ===== Comparison Instructions
        // --- CMP
        // --- CPX
        // --- CPY
        // ===== Conditional Branch Instructions
        // --- BCC
        // --- BCS
        // --- BEQ
        // --- BMI
        // --- BNE
        // --- BPL
        // --- BVC
        // --- BVS
        // ===== Jumps & Subroutines Instructions
        // --- JUMP
        // --- JSR
        // --- RTS
        // ===== Interrupts Instructions
        // --- BRK
        // --- RTI
        // ===== Other Instructions
        // --- BIT
        // --- NOP

        pipeline = new CPU::Pipeline(*this);

        reset();
    }

    // CPU::CPU()

    void CPU::reset()
    {
        registers.A = 0x00;
        registers.X = 0x00;
        registers.Y = 0x00;
        registers.S = StackPointerDefault;
        registers.P = 0b00100000; // TODO reset it properly...

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << std::endl;
        // std::cout << "----- Cpu(RESET) -----" << std::endl;

        // Reset the CPU / decode pipeline from the reset vector
        pipeline->reset(std::to_underlying(HardwareVector::RESET));

        // std::cout << "PC: " << std::setfill('0') << std::setw(4) << (int)registers.PC << " reset vector" << std::endl;
        // std::cout.unsetf(std::ios::basefield);
    }

    void CPU::showRegisters()
    {
        // TODO output stream does not treat a Word the same as an int so the format is off
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "6502 Registers" << std::endl;
        std::cout << "         A: " << std::setfill('0') << std::setw(2) << (int)(registers.A) << " Accumulator" << std::endl;
        std::cout << "         X: " << std::setfill('0') << std::setw(2) << (int)(registers.X) << " Index register X" << std::endl;
        std::cout << "         Y: " << std::setfill('0') << std::setw(2) << (int)(registers.Y) << " Index register Y" << std::endl;
        std::cout << "         S: " << std::setfill('0') << std::setw(2) << (int)(registers.S) << " Stack pointer" << std::endl;
        std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(registers.PC) << " Program Counter" << std::endl;
        std::cout << "         P: N V 1 B D I Z C  CPU Status Register" << std::endl
                  << "            "
                  << std::setw(1)
                  << (isN() ? 1 : 0) << " "
                  << (isV() ? 1 : 0) << " "
                  << 1 << " "
                  << (isB() ? 1 : 0) << " "
                  << (isD() ? 1 : 0) << " "
                  << (isI() ? 1 : 0) << " "
                  << (isZ() ? 1 : 0) << " "
                  << (isC() ? 1 : 0) << " "
                  << std::endl
                  << std::resetiosflags(std::ios::basefield) << std::setiosflags(std::ios::oct)
                  //   << setf(std::ios::binary)
                  //   << std::bitset<8>(model.P)
                  << std::endl;
        std::cout.unsetf(std::ios::basefield);

        pipeline->showPipeline();
    }

    // ===== Execute Instructions =====

    void CPU::executeFromAddress(hardware::Address address, uint32_t stepCount)
    {
        // set PC and execute 1 instruction
        registers.PC = address;
        execute(stepCount);
    }

    void CPU::execute(int numberOfInstructions)
    {
        pipeline->execute(numberOfInstructions);
    }

}
