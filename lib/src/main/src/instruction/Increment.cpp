

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----
    // ----- Stack () -----
    // ----- Decrement & Increment () -----

    //----------------------------------------
    // DEC (DECrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     DEC $44       $C6  2   5
    // Zero Page,X   DEC $44,X     $D6  2   6
    // Absolute      DEC $4400     $CE  3   6
    // Absolute,X    DEC $4400,X   $DE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL

    // ----- ZeroPage,X $LL,X

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ----- AbsoluteX $LLHH,X

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // INC (INCrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     INC $44       $E6  2   5
    // Zero Page,X   INC $44,X     $F6  2   6
    // Absolute      INC $4400     $EE  3   6
    // Absolute,X    INC $4400,X   $FE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL

    // ----- ZeroPage,X $LL,X

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ----- AbsoluteX $LLHH,X

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

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

#include <iostream>
#include <iomanip>

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
    // ----- Decrement & Increment () -----

    //----------------------------------------
    // DEC (DECrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     DEC $44       $C6  2   5
    // Zero Page,X   DEC $44,X     $D6  2   6
    // Absolute      DEC $4400     $CE  3   6
    // Absolute,X    DEC $4400,X   $DE  3   7
    //----------------------------------------

    // ----- Generic Decrement -----
    void CPU::InstructionDecrement::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC Decrement " << std::endl;

        // TODO Need to deal with implied, Accumulator, Immediate addressingModes
        switch (dst)
        {
        case InstructionTarget::MEMORY:
        {
            hardware::Address address = cpu.decodePipeline().addressMode->execute();
            hardware::Byte value = cpu.addressSpace.read(address);

            value--;
            //                  clear N & Z bits           Negative           Zero
            cpu.registers.P = (cpu.registers.P & 0x7D) | (value & 0x80) | (value == 0 ? 0x02 : 0x00);
            cpu.addressSpace.write(address, value);
        }
        break;
        // case InstructionTarget::A:
        // cpu.A(cpu.registers.A - 1);
        // break;
        // case InstructionTarget::S: // TODO xfer to S
        //     break;
        case InstructionTarget::X: // TODO xfer to X
            cpu.X(cpu.registers.X - 1);
            break;
        case InstructionTarget::Y: // TODO xfer to Y
            cpu.Y(cpu.registers.Y - 1);
            break;
        default:
            std::cout << "InstructionDecrement::execute() - Unhandled target: " << (int)dst << std::endl;
            break;
        }
    }

    //----------------------------------------
    // INC (INCrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     INC $44       $E6  2   5
    // Zero Page,X   INC $44,X     $F6  2   6
    // Absolute      INC $4400     $EE  3   6
    // Absolute,X    INC $4400,X   $FE  3   7
    //----------------------------------------

    // ----- Generic Increment -----
    void CPU::InstructionIncrement::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC Increment " << std::endl;

        // TODO Need to deal with implied, Accumulator, Immediate addressingModes
        switch (dst)
        {
        case InstructionTarget::MEMORY:
        {
            hardware::Address address = cpu.decodePipeline().addressMode->execute();
            hardware::Byte value = cpu.addressSpace.read(address);

            value++;
            //                  clear N & Z bits           Negative           Zero
            cpu.registers.P = (cpu.registers.P & 0x7D) | (value & 0x80) | (value == 0 ? 0x02 : 0x00);
            cpu.addressSpace.write(address, value);
        }
        break;
        // case InstructionTarget::A:
        // cpu.A(cpu.registers.A - 1);
        // break;
        // case InstructionTarget::S: // TODO xfer to S
        //     break;
        case InstructionTarget::X: // TODO xfer to X
            cpu.X(cpu.registers.X + 1);
            break;
        case InstructionTarget::Y: // TODO xfer to Y
            cpu.Y(cpu.registers.Y + 1);
            break;
        default:
            std::cout << "InstructionDecrement::execute() - Unhandled target: " << (int)dst << std::endl;
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
