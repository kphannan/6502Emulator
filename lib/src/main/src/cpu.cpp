

#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    // const memory::Memory defaultMemory;

    CPU::CPU() : CPU( *(new memory::Memory()))
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
        //        _addressModeStack = new AddressModeStack(*this);
        _addressModeStackPull = new AddressModeStackPull(*this);
        _addressModeStackPush = new AddressModeStackPush(*this);

        // ===== Instructions =====
        _instructionUndefined = new Instruction(*this, "Illegal Instruction", "bad");

        // ===== Transfer Instructions
        // ----- Load
        _instructionLoad = new InstructionLoad(*this, "load instruction", "load");
        // _instructionLoad = new InstructionLoad(*this);
        // --- LDA
        _instructionLoadA = _instructionLoad;
        // --- LDX
        _instructionLoadX = _instructionLoad;
        // --- LDY
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
        _instructionTransferAtoY = _instructionTransfer;
        // --- TSX
        _instructionTransferStoX = _instructionTransfer;
        // --- TXA
        _instructionTransferXtoA = _instructionTransfer;
        // --- TXS
        _instructionTransferXtoS = _instructionTransfer;
        // --- TYA
        _instructionTransferYtoA = _instructionTransfer;

        // ===== Stack Instructions
        _instructionStack = new InstructionStack(*this);
        // --- PHA
        // --- PHP
        // --- PLA
        // --- PLP
        // ===== Decrement & Increment
        _instructionDecrement = new InstructionDecrement(*this);
        _instructionIncrement = new InstructionIncrement(*this);
        // --- DEC
        // --- DEX
        _instructionDecrementX = _instructionDecrement;
        // --- DEY
        _instructionDecrementY = _instructionDecrement;
        // --- INC
        // --- INX
        _instructionIncrementX = _instructionIncrement;
        // --- INY
        _instructionIncrementY = _instructionIncrement;
        // ===== Arithmetic Instructions
        // --- ADC
        _instructionAdd = new InstructionAdd(*this);
        // --- SBC
        _instructionSubtract = new InstructionSubtract(*this);
        // ===== Logical Instructions
        // _instructionLogical = new InstructionLogical(*this);
        // --- AND
        _instructionLogicalAnd = new InstructionLogicalAnd(*this);
        // --- EOR
        _instructionLogicalXor = new InstructionLogicalXor(*this);
        // --- ORA
        _instructionLogicalOr = new InstructionLogicalOr(*this);
        // ===== Shift & Rotate Instructions
        // --- ASL
        _instructionShiftLeft = new InstructionShiftLeft(*this);
        // --- LSR
        _instructionShiftRight = new InstructionShiftRight(*this);
        // --- ROL
        _instructionRotateLeft = new InstructionRotateLeft(*this);
        // --- ROR
        _instructionRotateRight = new InstructionRotateRight(*this);
        // ===== Flag Instructions
        _instructionFlagClear = new CPU::InstructionFlagClear(*this);
        _instructionFlagSet = new CPU::InstructionFlagSet(*this);
        // --- CLC
        // --- CLD
        // --- CLI
        // --- CLV
        // --- SEC
        // --- SED
        // --- SEI
        // ===== Comparison Instructions
        _instructionCompare = new CPU::InstructionCompare(*this);
        // --- CMP
        // --- CPX
        // --- CPY
        // ===== Conditional Branch Instructions
        // _instructionBranch = new CPU::InstructionBranch(*this);
        // --- BCC
        _instructionBranchCarryClear = new CPU::InstructionBranchCarryClear(*this);
        // --- BCS
        _instructionBranchCarrySet = new CPU::InstructionBranchCarrySet(*this);
        // --- BEQ
        _instructionBranchEqualToZero = new CPU::InstructionBranchEqualToZero(*this);
        // --- BMI
        _instructionBranchMinus = new CPU::InstructionBranchMinus(*this);
        // --- BNE
        _instructionBranchNotEqualToZero = new CPU::InstructionBranchNotEqualToZero(*this);
        // --- BPL
        _instructionBranchOnPlus = new CPU::InstructionBranchOnPlus(*this);
        // --- BVC
        _instructionBranchOverflowClear = new CPU::InstructionBranchOverflowClear(*this);
        // --- BVS
        _instructionBranchOverflowSet = new CPU::InstructionBranchOverflowSet(*this);
        // ===== Jumps & Subroutines Instructions
        // --- JUMP
        _instructionJump = new CPU::InstructionJump(*this);
        // --- JSR
        _instructionJumpSubroutine = new CPU::InstructionJumpSubroutine(*this);
        // --- RTS
        _instructionReturnFromSubroutine = new CPU::InstructionReturnFromSubroutine(*this);
        // ===== Interrupts Instructions
        // --- BRK
        _instructionBreak = new CPU::InstructionBreak(*this);
        // --- RTI
        _instructionReturnFromInterrupt = new CPU::InstructionReturnFromInterrupt(*this);
        // ===== Other Instructions
        // --- BIT
        _instructionLogicalBit = new InstructionLogicalBit(*this);
        // --- NOP
        _instructionNoOp = new InstructionNoOp(*this);

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

        // Reset the CPU / decode pipeline from the reset vector
        // pipeline->reset(std::to_underlying(HardwareVector::RESET));
        pipeline->reset(hardware::Address(std::to_underlying(HardwareVector::RESET)));
        // pipeline->reset(hardware::Address(HardwareVector::RESET));
    }

    void CPU::showRegisters()
    {
        // TODO output stream does not treat a Word the same as an int so the format is off
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "6502 Registers" << std::endl;
        std::cout << "         A: " << std::setfill('0') << std::setw(2) << (int)(registers.A) << " Accumulator" << std::endl;
        std::cout << "         X: " << std::setfill('0') << std::setw(2) << (int)(registers.X) << " Index register X" << std::endl;
        std::cout << "         Y: " << std::setfill('0') << std::setw(2) << (int)(registers.Y) << " Index register Y" << std::endl;
        std::cout << "         S: " << std::setfill('0') << std::setw(2) << (int)(registers.S.current()) << " Stack pointer" << std::endl;
        std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(registers.PC) << " Program Counter" << std::endl;
        //        std::cout << "         S: " << std::setw(2) << registers.S << " Stack pointer" << std::endl;
        //        std::cout << "        PC: " << registers.PC << " Program Counter" << std::endl;
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

    void CPU::executeFromAddress(const unsigned int address, uint32_t stepCount)
    {
        executeFromAddress( hardware::Address( address), stepCount );
    }

    void CPU::executeFromAddress(const hardware::Address& address, uint32_t stepCount)
    {
        // set PC and execute 1 instruction
        registers.PC = address;
        execute(stepCount);
    }

    void CPU::execute(int numberOfInstructions)
    {
        pipeline->execute(numberOfInstructions);
    }

    // ===== CPU helper methods =====
    // --- push byte on stack
    void CPU::push(const hardware::Byte &value)
    {
        // stack grows down
        addressSpace[registers.S] = value;
        registers.S--;
    }

    // --- push word on stack
    // before      after
    // SP --> hh
    //        ll
    //             <-- SP (after)
    void CPU::push(const hardware::Word &value)
    {
        // stack grows down
        registers.S--;
        addressSpace.writeWord(registers.S, value);
        registers.S--;
    }

    void CPU::push(const hardware::Address &address)
    {
        // stack grows down
        registers.S--;
        addressSpace.writeWord(registers.S, address.value.address);
        registers.S--;
    }

    //
    hardware::Byte CPU::pop()
    {
        return addressSpace[++registers.S];
    }

    // before      after
    //        hh   <-- SP
    //        ll
    // SP -->
    hardware::Word CPU::popWord()
    {
        hardware::Word value = addressSpace.readWord(++registers.S);
        ++registers.S;

        return value;
    }

    // before      after
    //        hh   <-- SP
    //        ll
    // SP -->
    hardware::Address CPU::popAddress()
    {
        hardware::Address value = addressSpace.readAddress(++registers.S);
        ++registers.S;

        return value;
    }

}
