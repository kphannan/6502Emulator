

#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "memory.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----

    //----------------------------------------
    // Register Instructions
    //
    // Affect Flags: N Z
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                 HEX
    // TAX (Transfer A to X)    $AA
    // TXA (Transfer X to A)    $8A
    // DEX (DEcrement X)        $CA     ; see increment.cpp
    // INX (INcrement X)        $E8     ; see increment.cpp
    // TAY (Transfer A to Y)    $A8
    // TYA (Transfer Y to A)    $98
    // DEY (DEcrement Y)        $88     ; see increment.cpp
    // INY (INcrement Y)        $C8     ; see increment.cpp
    // TXS (Transfer X to Stack ptr)   $9A  2   ; see stack.cpp
    // TSX (Transfer Stack ptr to X)   $BA  2   ; see stack.cpp
    //----------------------------------------

    // ----- Generic Load -----
    void CPU::InstructionTransfer::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC TRANSFER " << std::endl;

        switch (dst)
        {
        case InstructionTarget::A:
            switch (src)
            {
            case InstructionTarget::X:
                cpu.A(cpu.registers.X);
                break;
            case InstructionTarget::Y:
                cpu.A(cpu.registers.Y);
                break;
            case InstructionTarget::MEMORY:
            case InstructionTarget::A:
            case InstructionTarget::S:
            default:
                break;
            }
            break;
        case InstructionTarget::S: // TODO xfer to S
            switch (src)
            {
            case InstructionTarget::MEMORY:
            case InstructionTarget::A:
            case InstructionTarget::S:
            case InstructionTarget::X: // TXS
                // cpu.registers.S = hardware::Address(CPU::AddressModeStack::stackPage, cpu.registers.X);
                cpu.S(cpu.registers.X);
                break;
            case InstructionTarget::Y:
            default:
                break;
            }
            break;
        case InstructionTarget::X: // TODO xfer to X
            switch (src)
            {
            case InstructionTarget::A:
                cpu.X(cpu.registers.A);
                break;
            case InstructionTarget::MEMORY:
            case InstructionTarget::S:
                cpu.X(cpu.registers.S); // flags effected
                break;
            case InstructionTarget::X:
            case InstructionTarget::Y:
            default:
                break;
            }
            break;
        case InstructionTarget::Y: // TODO xfer to Y
            switch (src)
            {
            case InstructionTarget::A:
                cpu.Y(cpu.registers.A);
                break;
            case InstructionTarget::MEMORY:
            case InstructionTarget::S:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
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

    // ----- Stack () -----
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
