

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
    // ----- Stack () -----
    // ----- Decrement & Increment () -----
    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----

    //----------------------------------------
    // Flag (Processor Status) Instructions
    //
    // Affect Flags: as noted
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                       HEX
    // CLC (CLear Carry)              $18
    // SEC (SEt Carry)                $38
    // CLI (CLear Interrupt)          $58
    // SEI (SEt Interrupt)            $78
    // CLV (CLear oVerflow)           $B8
    // CLD (CLear Decimal)            $D8
    // SED (SEt Decimal)              $F8
    //
    // Notes:
    //   The Interrupt flag is used to prevent (SEI) or enable (CLI) maskable interrupts (aka IRQ's). It does not signal the presence or absence of an interrupt condition. The 6502 will set this flag automatically in response to an interrupt and restore it to its prior status on completion of the interrupt service routine. If you want your interrupt service routine to permit other maskable interrupts, you must clear the I flag in your code.
    //
    //   The Decimal flag controls how the 6502 adds and subtracts. If set, arithmetic is carried out in packed binary coded decimal. This flag is unchanged by interrupts and is unknown on power-up. The implication is that a CLD should be included in boot or interrupt coding.
    //
    //   The Overflow flag is generally misunderstood and therefore under-utilised. After an ADC or SBC instruction, the overflow flag will be set if the twos complement result is less than -128 or greater than +127, and it will cleared otherwise. In twos complement, $80 through $FF represents -128 through -1, and $00 through $7F represents 0 through +127. Thus, after:
    //
    //   CLC
    //   LDA #$7F ;   +127
    //   ADC #$01 ; +   +1
    // the overflow flag is 1 (+127 + +1 = +128), and after:
    //   CLC
    //   LDA #$81 ;   -127
    //   ADC #$FF ; +   -1
    // the overflow flag is 0 (-127 + -1 = -128). The overflow flag is not affected by increments, decrements, shifts and logical operations i.e. only ADC, BIT, CLV, PLP, RTI and SBC affect it. There is no op code to set the overflow but a BIT test on an RTS instruction will do the trick.
    //----------------------------------------
    void CPU::InstructionFlagClear::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);
        switch (dst)
        {
        case InstructionTarget::FLAG_B:
            cpu.clearB();
            break;
        case InstructionTarget::FLAG_C:
            cpu.clearC();
            break;
        case InstructionTarget::FLAG_D:
            cpu.clearD();
            break;
        case InstructionTarget::FLAG_I:
            cpu.clearI();
            break;
        case InstructionTarget::FLAG_N:
            cpu.clearN();
            break;
        case InstructionTarget::FLAG_V:
            cpu.clearV();
            break;
        case InstructionTarget::FLAG_Z:
            cpu.clearZ();
            break;
        default:
            std::cout << "Illegal destination of a flag clear operation" << std::endl;
            break;
        }
    }

    void CPU::InstructionFlagSet::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);
        switch (dst)
        {
        case InstructionTarget::FLAG_B:
            cpu.setB();
            break;
        case InstructionTarget::FLAG_C:
            cpu.setC();
            break;
        case InstructionTarget::FLAG_D:
            cpu.setD();
            break;
        case InstructionTarget::FLAG_I:
            cpu.setI();
            break;
        case InstructionTarget::FLAG_N:
            cpu.setN();
            break;
        case InstructionTarget::FLAG_V:
            cpu.setV();
            break;
        case InstructionTarget::FLAG_Z:
            cpu.setZ();
            break;
        default:
            std::cout << "Illegal destination of a flag set operation" << std::endl;
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

    // ----- Comparison () -----

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
