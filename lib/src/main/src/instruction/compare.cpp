

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

    //----------------------------------------
    // CMP (CoMPare accumulator)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CMP #$44      $C9  2   2
    // Zero Page     CMP $44       $C5  2   3
    // Zero Page,X   CMP $44,X     $D5  2   4
    // Absolute      CMP $4400     $CD  3   4
    // Absolute,X    CMP $4400,X   $DD  3   4+
    // Absolute,Y    CMP $4400,Y   $D9  3   4+
    // Indirect,X    CMP ($44,X)   $C1  2   6
    // Indirect,Y    CMP ($44),Y   $D1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // Compare sets flags as if a subtraction had been carried out. If the value in the accumulator is equal or greater than the compared value, the Carry will be set. The equal (Z) and negative (N) flags will be set based on equality or lack thereof and the sign (i.e. A>=$80) of the accumulator.
    //----------------------------------------

    void CPU::InstructionCompare::compare(hardware::Byte v1, hardware::Byte v2)
    {
        // Widen arguments
        int vi1 = v1;
        int vi2 = v2;
        int value = vi1 - vi2;
        // int value = v1 - v2;
        // bit7 set indicates negative in 2's compliment
        value & 0x80 ? cpu.setN() : cpu.clearN();

        value == 0 ? cpu.setZ() : cpu.clearZ();

        // check carry
        // v1 - v2 is a 2's compliment addition
        // v2 is complimented then add one  (1)

        // value = (uint16_t)v1 + ~(uint16_t)v2 + 1;
        value = vi1 + ~vi2 + 1;
        // Carry occurs if there is any bit higher than bit 7 is set.
        // mask out the low byte  (8 bits) from the int...
        value &= ~0xFF;
        value == 0 ? cpu.clearC() : cpu.setC();
    }

    void CPU::InstructionCompare::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);
//        hardware::Byte v1 = ....;

        switch (dst)
        {
            case InstructionTarget::A:
                compare( cpu.registers.A, value );
                break;
            case InstructionTarget::X:
                compare( cpu.registers.X, value );
                break;
            case InstructionTarget::Y:
                compare( cpu.registers.Y, value );
                break;
//            case InstructionTarget::A:
//                switch( src )
//                {
//                    case InstructionTarget::A:
//                        compare( cpu.registers.A, value );
//                        break;
//                    case InstructionTarget::X:
//                        compare( cpu.registers.X, value );
//                        break;
//                    case InstructionTarget::Y:
//                        compare( cpu.registers.Y, value );
//                        break;
//                }
//            break;
                
            default:
                std::cout << "Illegal destination of a flag compare operation" << std::endl;
                break;
        }
    }

    // Addressing Modes
    // ----- Immediate #$BB

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL

    // ----- ZeroPage,X $LL,X

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ----- AbsoluteX $LLHH,X

    // ----- AbsoluteY $LLHH,Y

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)

    // ----- Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // CPX (ComPare X register)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CPX #$44      $E0  2   2
    // Zero Page     CPX $44       $E4  2   3
    // Absolute      CPX $4400     $EC  3   4
    //
    // Operation and flag results are identical to equivalent mode accumulator CMP ops.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL

    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // CPY (ComPare Y register)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CPY #$44      $C0  2   2
    // Zero Page     CPY $44       $C4  2   3
    // Absolute      CPY $4400     $CC  3   4
    //
    // Operation and flag results are identical to equivalent mode accumulator CMP ops.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL

    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
