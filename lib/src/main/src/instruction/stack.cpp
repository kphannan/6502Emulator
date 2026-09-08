

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----
    // ----- Stack () -----

    //----------------------------------------
    // Stack Instructions
    //
    // These instructions are implied mode, have a length of one byte and require machine cycles as indicated. The "PuLl" operations are known as "POP" on most other microprocessors. With the 6502, the stack is always on page one ($100-$1FF) and works top down.
    //
    // MNEMONIC                        HEX TIM
    // TXS (Transfer X to Stack ptr)   $9A  2   ; see transfer.cpp
    // TSX (Transfer Stack ptr to X)   $BA  2   ; see transfer.cpp
    // PHA (PusH Accumulator)          $48  3
    // PLA (PuLl Accumulator)          $68  4
    // PHP (PusH Processor status)     $08  3
    // PLP (PuLl Processor status)     $28  4
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB

    // ----- Implied
    // ----- Generic Load -----
    void CPU::InstructionStack::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC STACK " << std::endl;

        switch (dst) // stack, accumulator, status register
        {
        case InstructionTarget::STACK: // To stack; push (PHA, PHP, PHX, PHY)
            switch (src)
            {
            case InstructionTarget::X:  // TXS -- should be in transfer.cpp
                cpu.S(cpu.registers.X); // TODO wrong dst/src
                break;
            case InstructionTarget::A: // PHA - Push Accumulator
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                cpu.addressSpace[address] = cpu.registers.A;
                // hardware::Byte value = cpu.addressSpace.read(address);
                // cpu.addressSpace.write(address, cpu.registers.A);
                // cpu.push(cpu.registers.A);
                break;
            }
            case InstructionTarget::PSR: // PHP - Push Processor Status
            {
                // There is a reference that says the B bit is set in the byte
                // pushed.
                // https://www.nesdev.org/wiki/Instruction_reference#PHA
                // The original MOSTEC document on pg 122 says it it pushed
                // unchanged.
                // https://archive.org/details/6500-50a_mcs6500pgmmanjan76/page/n135/mode/2up
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                cpu.addressSpace[address] = cpu.registers.P;
                // cpu.addressSpace[address] = cpu.setBit(cpu.registers.P, BrkBit );
                // hardware::Byte value = cpu.addressSpace.read(address);
                // cpu.addressSpace.write(address, cpu.registers.P);
                // cpu.push(cpu.setB(cpu.registers.P));
                break;
            }
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::S: // Nonsense to push stack pointer to stack
            case InstructionTarget::STACK:
            default:
                break;
            }
            break;

        case InstructionTarget::PSR: // To Processor Status Flag Register
            switch (src)
            {
            case InstructionTarget::STACK: // PLP - Pull status from Stack
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace[address]; //cpu.addressSpace.read(address);
                cpu.registers.P = value;
//                cpu.P(cpu.pop());
                break;
            }
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::A:
            case InstructionTarget::S:
            case InstructionTarget::PSR:
            default:
                break;
            }
            break;

        case InstructionTarget::A: // To accumulator
            switch (src)
            {
            case InstructionTarget::STACK: // from stack PLA
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                cpu.A(cpu.addressSpace[address]);
                // hardware::Byte value = cpu.addressSpace.read(address);
                // cpu.addressSpace.write(address, cpu.registers.A);
                // cpu.A( cpu.addressSpace.read(address));
                // cpu.A(cpu.pop());
                break;
            }
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::A:
            case InstructionTarget::S:
            default:
                break;
            }
            break;

        case InstructionTarget::S: // n/a
            switch (src)
            {
            case InstructionTarget::X:             // -- should be in transfer.cpp
                cpu.registers.S = cpu.registers.X; // no flags affected
                break;
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::STACK:
            case InstructionTarget::A:
            case InstructionTarget::S:
            default:
                break;
            }
            break;
        case InstructionTarget::X: // n/a
            switch (src)
            {
            case InstructionTarget::S: // TSX      X <- S -- should be in transfer.cpp
                cpu.X(cpu.registers.S);
                break;
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::STACK: //
                                           // no instruction to pop stack into X register
                                           // cpu.X(cpu.pop());
            case InstructionTarget::A:
            default:
                break;
            }
            break;
        case InstructionTarget::Y: // n/a
            switch (src)
            {
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::STACK:
            case InstructionTarget::A:
            case InstructionTarget::S:
            default:
                break;
            }
            break;
        case InstructionTarget::MEMORY: // n/a
            switch (src)
            {
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::MEMORY:
            case InstructionTarget::STACK:
            case InstructionTarget::A:
            case InstructionTarget::S:
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ..... Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Decrement & Increment () -----
    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
