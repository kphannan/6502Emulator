

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

        switch (dst)
        {
        case InstructionTarget::X:
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
                break;
            case InstructionTarget::A:
                break;
            case InstructionTarget::S:
                cpu.X(cpu.registers.S);
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::Y:
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
                break;
            case InstructionTarget::A:
                break;
            case InstructionTarget::S:
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::MEMORY:
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
                break;
            case InstructionTarget::A:
                break;
            case InstructionTarget::S:
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::STACK: // To stack
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
                break;
            case InstructionTarget::A: // PHA
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                // hardware::Byte value = cpu.addressSpace.read(address);
                cpu.addressSpace.write(address, cpu.registers.A);
                break;
            }
            case InstructionTarget::S: // Nonsense to push stack pointer to stack
                break;
            case InstructionTarget::PSR: // PHP
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                // hardware::Byte value = cpu.addressSpace.read(address);
                cpu.addressSpace.write(address, cpu.registers.P);
                break;
            }
            default:
                break;
            }
            break;

        case InstructionTarget::PSR: // To stack
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);
                cpu.registers.P = value;
                break;
            }
            case InstructionTarget::A: // PHA
                break;
            case InstructionTarget::S: // Nonsense to push stack pointer to stack
                break;
            case InstructionTarget::PSR: // PHP
                break;
            default:
                break;
            }
            break;

        case InstructionTarget::A: // To accumulator
            switch (src)
            {
            case InstructionTarget::X:
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK: // from stack PLA
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);
                // cpu.addressSpace.write(address, cpu.registers.A);
                // cpu.A( cpu.addressSpace.read(address));
                cpu.A(value);
                break;
            }
            case InstructionTarget::A:
                break;
            case InstructionTarget::S:
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::S:
            switch (src)
            {
            case InstructionTarget::X:
                cpu.registers.S = cpu.registers.X; // no flags affected
                break;
            case InstructionTarget::Y:
                break;
            case InstructionTarget::MEMORY:
                break;
            case InstructionTarget::STACK:
                break;
            case InstructionTarget::A:
                break;
            case InstructionTarget::S:
                break;
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
