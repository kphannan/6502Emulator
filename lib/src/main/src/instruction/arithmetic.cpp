

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

    //----------------------------------------
    // ADC (ADd with Carry)
    //
    // Affects Flags: N V Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     ADC #$44      $69  2   2
    // Zero Page     ADC $44       $65  2   3
    // Zero Page,X   ADC $44,X     $75  2   4
    // Absolute      ADC $4400     $6D  3   4
    // Absolute,X    ADC $4400,X   $7D  3   4+
    // Absolute,Y    ADC $4400,Y   $79  3   4+
    // Indirect,X    ADC ($44,X)   $61  2   6
    // Indirect,Y    ADC ($44),Y   $71  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // ADC results are dependant on the setting of the decimal flag. In decimal mode, addition is carried out on the assumption that the values involved are packed BCD (Binary Coded Decimal).
    // There is no way to add without carry.
    //----------------------------------------

    void CPU::InstructionAdd::add(hardware::Byte v1, hardware::Byte v2)
    {
        // Widen arguments
        int value = v1 + v2 + (cpu.isC() ? 1 : 0);

        cpu.A(value);

        // Carry occurs if there is any bit higher than bit 7 is set.
        // mask out the low byte  (8 bits) from the int...
        //        value &= ~0xFF;
        value & ~0xFF ? cpu.setC() : cpu.clearC();

        // TODO add this to a root class of add/subtract also for BIT
        bool overflow = !((v1 ^ v2) & 0x80) && ((v1 ^ value) & 0x80);
        overflow ? cpu.setV() : cpu.clearV();
    }

    void CPU::InstructionAdd::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);

        // switch ( src )
        // {
        //     case InstructionTarget::A:
        //     case InstructionTarget::X:
        //     case InstructionTarget::Y:
        //     case InstructionTarget::S:
        //     case InstructionTarget::PC:
        //     case InstructionTarget::PSR:
        //     case InstructionTarget::FLAG_N:
        //     case InstructionTarget::FLAG_V:
        //     case InstructionTarget::FLAG_B:
        //     case InstructionTarget::FLAG_D:
        //     case InstructionTarget::FLAG_I:
        //     case InstructionTarget::FLAG_Z:
        //     case InstructionTarget::FLAG_C:
        //     case InstructionTarget::MEMORY:
        //     case InstructionTarget::STACK:
        //     case InstructionTarget::Undefined:
        //         break;
        // }

        switch (dst)
        {
        case InstructionTarget::A:
            switch (src)
            {
            case InstructionTarget::MEMORY:
                add(cpu.registers.A, value);
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
            case InstructionTarget::PC:
            case InstructionTarget::PSR:
            case InstructionTarget::FLAG_N:
            case InstructionTarget::FLAG_V:
            case InstructionTarget::FLAG_B:
            case InstructionTarget::FLAG_D:
            case InstructionTarget::FLAG_I:
            case InstructionTarget::FLAG_Z:
            case InstructionTarget::FLAG_C:
            case InstructionTarget::STACK:
            case InstructionTarget::IMPLIED:
            case InstructionTarget::Undefined:
                throw std::domain_error("Illegal addend (source) of ADC operation");
                break;
            }
            break;
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::Undefined:
        default:
            std::cout << "Illegal destination of a ADC operation" << std::endl;
            throw std::domain_error("Illegal augend (target) of ADC operation");
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
    // SBC (SuBtract with Carry)
    //
    // Affects Flags: N V Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     SBC #$44      $E9  2   2
    // Zero Page     SBC $44       $E5  2   3
    // Zero Page,X   SBC $44,X     $F5  2   4
    // Absolute      SBC $4400     $ED  3   4
    // Absolute,X    SBC $4400,X   $FD  3   4+
    // Absolute,Y    SBC $4400,Y   $F9  3   4+
    // Indirect,X    SBC ($44,X)   $E1  2   6
    // Indirect,Y    SBC ($44),Y   $F1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // SBC results are dependant on the setting of the decimal flag. In decimal mode, subtraction is carried out on the assumption that the values involved are packed BCD (Binary Coded Decimal).
    // There is no way to subtract without the carry which works as an inverse borrow. i.e, to subtract you set the carry before the operation. If the carry is cleared by the operation, it indicates a borrow occurred.
    //----------------------------------------

    /*
    Re: Help with ADC/SBC and Carry/Overflow Flags
    Post by whartung » Tue Dec 12, 2017 9:56 pm

    This is my SBC code.

    I jump through hoops to calculate the Overflow properly. But the basic math is simple.

    Code:
    Select all

        public void SBC(int value) {
            int result;
            int carryValue = (isCarry() ? 0 : 1);

            result = acc - value - carryValue;

            status &= ~(CARRY_MASK + ZERO_MASK + OVERFLOW_MASK + NEGATIVE_MASK);
            if (result == 0) {
                status |= ZERO_MASK + CARRY_MASK;
            } else if (result > 0) {
                status |= CARRY_MASK;
            }
            if (!isDecimal()) {
                int signedAcc = signed(acc);
                int signedValue = signed(value);
                int signedResult = signedAcc - signedValue - carryValue;

                if (signedResult > 127 || signedResult < -128) {
                    status |= OVERFLOW_MASK;
                }
            }
            setFlagsNZ(result);

            acc = result & 0xff;
        }
    All I can vouch for is that this passes the 6502 test code suite that's floating around.*/

    /*
         Dec   C   binary
          25   1  00011001
         -24     -00011000
         ---     ---------
           1   1  00000001

         Dec   C   binary
          24   1  00011000      1  00011000         1  00011000
         -25     -00011001.        11100110 + 1      + 11100111
         ---     ---------       ----------         -----------
               0  11111111         11111110 + 1        11111111
         */

    void CPU::InstructionSubtract::subtract(hardware::Byte minuend, hardware::Byte subtrahend)
    {
        // Widen arguments
        //        int vi1 = v1;
        //        int vi2 = v2;
        int carryValue = cpu.isC() ? 0 : 1;
        int value = minuend - subtrahend - carryValue;
        bool carryNeeded = value < 0;

        cpu.A(value & 0xFF);

        carryNeeded ? cpu.setC() : cpu.clearC();

        // bit7 set indicates negative in 2's compliment
        // value & 0x80 ? cpu.setN() : cpu.clearN();

        // value == 0 ? cpu.setZ() : cpu.clearZ();

        // check carry
        // v1 - v2 is a 2's compliment addition
        // v2 is complimented then add one  (1)

        // value = (uint16_t)v1 + ~(uint16_t)v2 + 1;
        //        value = vi1 + ~vi2 + 1;
        // Carry occurs if there is any bit higher than bit 7 is set.
        // mask out the low byte  (8 bits) from the int...
        //        value &= ~0xFF;
        // // TODO check carry handling
        // value & ~0xFF ? cpu.clearC() : cpu.setC();

        // // TODO add this to a root class of add/subtract also for BIT
        // bool overflow = !((v1 ^ v2) & 0x80) && ((v1 ^ value) & 0x80);
        // overflow ? cpu.setV() : cpu.clearV();
    }

    void CPU::InstructionSubtract::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace[address];
        //        value = memory[address];
        //        hardware::Byte value = cpu.addressSpace.read(address);
        //        hardware::Byte v1 = ....;

        subtract(cpu.A(), value);
        // int res = cpu.A() - value;
        // hardware::Byte result = cpu.A() - value;
        // cpu.A( result );
        // TODO set flags.....
        //
    }

    //        switch ( dst )
    //        {
    //            case InstructionTarget::A:
    //            case InstructionTarget::X:
    //            case InstructionTarget::Y:
    //            case InstructionTarget::S:
    //            case InstructionTarget::PC:
    //            case InstructionTarget::PSR:
    //            case InstructionTarget::FLAG_N:
    //            case InstructionTarget::FLAG_V:
    //            case InstructionTarget::FLAG_B:
    //            case InstructionTarget::FLAG_D:
    //            case InstructionTarget::FLAG_I:
    //            case InstructionTarget::FLAG_Z:
    //            case InstructionTarget::FLAG_C:
    //            case InstructionTarget::MEMORY:
    //            case InstructionTarget::STACK:
    //            case InstructionTarget::Undefined:
    //            default:
    //                std::cout << "Illegal destination of a SBC operation" << std::endl;
    //                std::domain_error("Illegal augend (target) of SBC operation");
    //                break;
    //        }

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

    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
