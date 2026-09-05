

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
    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----

    void CPU::InstructionChangeProgramCounter::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Address value = cpu.addressSpace.readWord(address);

        // // TODO check this no stack manipulation....
        // switch (dst)
        // {
        // case InstructionTarget::IMPLIED:
        //     switch (src)
        //     {
        //     case InstructionTarget::IMPLIED:
        //         // TODO someting with SP and put PC on stack.
        //         break;
        //     default:
        //         break;
        //     }
        //     break;
        // default:
        //     break;
        // }
    }

    //----------------------------------------
    // JMP (JuMP)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Absolute      JMP $5597     $4C  3   3
    // Indirect      JMP ($5597)   $6C  3   5
    //
    // JMP transfers program execution to the following address (absolute)
    // or to the location contained in the following address (indirect).
    // Note that there is no carry associated with the indirect jump so:
    // AN INDIRECT JUMP MUST NEVER USE A
    // VECTOR BEGINNING ON THE LAST BYTE
    // OF A PAGE
    // For example if address $3000 contains $40, $30FF contains $80, and $3100
    // contains $50, the result of JMP ($30FF) will be a transfer of control to
    // $4080 rather than $5080 as you intended i.e. the 6502 took the low byte
    // of the address from $30FF and the high byte from $3000.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y

    // ----- Indirect ($LLHH)

    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Generic Jump -----
    void CPU::InstructionJump::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        // hardware::Address value = cpu.addressSpace.readWord(address);

        cpu.PC(address);
    }

    //----------------------------------------
    // JSR (Jump to SubRoutine)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Absolute      JSR $5597     $20  3   6
    //
    // JSR pushes the address-1 of the next operation on to the stack before transferring program control to the following address. Subroutines are normally terminated by a RTS op code.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y

    // ----- Indirect ($LLHH)

    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Generic JSR -----
    void CPU::InstructionJumpSubroutine::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        cpu.push(cpu.PC());
        cpu.PC(address);
    }

    //----------------------------------------
    // RTS (ReTurn from Subroutine)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       RTS           $60  1   6
    //
    // RTS pulls the top two bytes off the stack (low byte first) and transfers program control to that address+1. It is used, as expected, to exit a subroutine invoked via JSR which pushed the address-1.
    // RTS is frequently used to implement a jump table where addresses-1 are pushed onto the stack and accessed via RTS eg. to access the second of four routines:
    //
    //  LDX #1
    //  JSR EXEC
    //  JMP SOMEWHERE
    //
    // LOBYTE
    //  .BYTE <ROUTINE0-1,<ROUTINE1-1
    //  .BYTE <ROUTINE2-1,<ROUTINE3-1
    //
    // HIBYTE
    //  .BYTE >ROUTINE0-1,>ROUTINE1-1
    //  .BYTE >ROUTINE2-1,>ROUTINE3-1
    //
    // EXEC
    //  LDA HIBYTE,X
    //  PHA
    //  LDA LOBYTE,X
    //  PHA
    //  RTS
    //
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB

    // ----- Implied
    void CPU::InstructionReturnFromSubroutine::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
//        hardware::Address value = cpu.addressSpace.readWord(address);

        cpu.PC(cpu.popWord()); // Program Counter

        // TODO Finish implementation
//        switch (dst)
//        {
//        case InstructionTarget::PC:
//            switch (src)
//            {
//            case InstructionTarget::MEMORY:
//                cpu.PC(value);
//                break;
//            default:
//                break;
//            }
//            break;
//        default:
//            break;
//        }
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

    // ----- Interrupt () -----
    // ----- Other () -----
}
