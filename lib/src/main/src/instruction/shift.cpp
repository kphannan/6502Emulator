

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
    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----

    //----------------------------------------
    // ASL (Arithmetic Shift Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ASL A         $0A  1   2
    // Zero Page     ASL $44       $06  2   5
    // Zero Page,X   ASL $44,X     $16  2   6
    // Absolute      ASL $4400     $0E  3   6
    // Absolute,X    ASL $4400,X   $1E  3   7
    //
    // ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

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
    // LSR (Logical Shift Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   LSR A         $4A  1   2
    // Zero Page     LSR $44       $46  2   5
    // Zero Page,X   LSR $44,X     $56  2   6
    // Absolute      LSR $4400     $4E  3   6
    // Absolute,X    LSR $4400,X   $5E  3   7
    //
    // LSR shifts all bits right one position. 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------

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
    // ROL (ROtate Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROL A         $2A  1   2
    // Zero Page     ROL $44       $26  2   5
    // Zero Page,X   ROL $44,X     $36  2   6
    // Absolute      ROL $4400     $2E  3   6
    // Absolute,X    ROL $4400,X   $3E  3   7
    //
    // ROL shifts all bits left one position. The Carry is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

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
    // ROR (ROtate Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROR A         $6A  1   2
    // Zero Page     ROR $44       $66  2   5
    // Zero Page,X   ROR $44,X     $76  2   6
    // Absolute      ROR $4400     $6E  3   6
    // Absolute,X    ROR $4400,X   $7E  3   7
    //
    // ROR shifts all bits right one position. The Carry is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------
    hardware::Byte CPU::InstructionRotateRight::rotateRight(hardware::Byte value)
    {
//        std::cout.setf(std::ios::hex, std::ios::basefield);
//        std::cout << "before: "
//                  << std::setfill('0') << std::setw(2) << (int)value
//                  << " C:" << (cpu.isC() ? "set" : "clear")
//                  << " PSR: 0b" << std::bitset<8>(cpu.P()) << std::endl;
//        std::cout.unsetf(std::ios::basefield);

        bool carryBit = cpu.isC();
        bool isLsbSet = value & 0x01;
        value >>= 1;
        if (carryBit)
            value |= 0x80;

        isLsbSet ? cpu.setC() : cpu.clearC();
        value & 0x80 ? cpu.setN() : cpu.clearN();
        
//        std::cout.setf(std::ios::hex, std::ios::basefield);
//        std::cout << " after: "
//                  << std::setfill('0') << std::setw(2) << (int)value
//                  << " C:" << (cpu.isC() ? "set" : "clear")
//                  << " PSR: 0b" << std::bitset<8>(cpu.P()) << std::endl;
//        std::cout.unsetf(std::ios::basefield);
        return value;
    }

    void CPU::InstructionRotateRight::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        std::cout << "   GENERIC ROTATE RIGHT " << std::endl;

        switch (dst)
        {
        case InstructionTarget::MEMORY: // M -> M
            switch (src)
            {
            case InstructionTarget::MEMORY:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);
//                std::cout.setf(std::ios::hex, std::ios::basefield);
//                std::cout << "start: M -> M from:" << std::setfill('0') << std::setw(4) << address
//                << " value: " << std::setfill('0') << std::setw(2) << (int)value
//                << " PSR: 0b" << std::bitset<8>(cpu.P())
//                << std::endl;
//                std::cout.unsetf(std::ios::basefield);
                // do the  ROR
                value = rotateRight(value);
                cpu.addressSpace.write(address, value);
//                std::cout.setf(std::ios::hex, std::ios::basefield);
//                std::cout << "  end: M -> M from:" << std::setfill('0') << std::setw(4) << address
//                << " value: " << std::setfill('0') << std::setw(2) << (int)value
//                << " PSR: 0b" << std::bitset<8>(cpu.P())
//                << std::endl;
//                std::cout.unsetf(std::ios::basefield);
                break;
            }
            default:
                break;
            }
            break;
        case InstructionTarget::A: // A -> A
            switch (src)
            {
            case InstructionTarget::A:
            {
                //                hardware::Byte carryBit = cpu.isC();
                //                hardware::Byte a = cpu.A();

                // doROR
                //                bool isLsbSet = a & 0x01;
                //                a >>= 1;
                //                if (carryBit)
                //                    a |= 0x80;
                //                if (isLsbSet)
                //                    cpu.setC();
                //                else
                //                    cpu.clearC();

                //                cpu.A(a);
                cpu.A(rotateRight(cpu.A()));
                break;
            }
            default:
                break;
            }
            break;
        default:
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

    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
