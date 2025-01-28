

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

    hardware::Byte CPU::InstructionShiftLeft::shiftLeft(hardware::Byte value)
    {
        // hardware::Byte originalValue = value;
        // Set carry bit to the value of bit 0
        value & 0x80 ? cpu.setC() : cpu.clearC();

        // Shift the bits to the right
        value <<= 1;

        // Set the N flag to the value of bit 7
        // 2's complement negative
        value & 0x80 ? cpu.setN() : cpu.clearN();

        value == 0x00 ? cpu.setZ() : cpu.clearZ();

        return value;
    }

    void CPU::InstructionShiftLeft::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC SHIFT LEFT " << std::endl;

        switch (dst)
        {
        case InstructionTarget::MEMORY: // M -> M
            switch (src)
            {
            case InstructionTarget::MEMORY:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);

                value = shiftLeft(value);
                cpu.addressSpace.write(address, value);
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
                cpu.A(shiftLeft(cpu.A()));
                break;
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

    hardware::Byte CPU::InstructionShiftRight::shiftRight(hardware::Byte value)
    {
        // hardware::Byte originalValue = value;
        // Set carry bit to the value of bit 0
        value & 0x01 ? cpu.setC() : cpu.clearC();

        // Shift the bits to the right
        value >>= 1;

        // N is always clear as msb is set to zero
        cpu.clearN();

        value == 0x00 ? cpu.setZ() : cpu.clearZ();

        return value;
    }

    void CPU::InstructionShiftRight::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC SHIFT RIGHT " << std::endl;

        switch (dst)
        {
        case InstructionTarget::MEMORY: // M -> M
            switch (src)
            {
            case InstructionTarget::MEMORY:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);

                value = shiftRight(value);
                cpu.addressSpace.write(address, value);
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
                cpu.A(shiftRight(cpu.A()));
                break;
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
    // ..... Implied
    // ----- Accumulator

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

    hardware::Byte CPU::InstructionRotateLeft::rotateLeft(hardware::Byte value)
    {
        hardware::Byte originalValue = value;
        // Shift the bits to the left
        value <<= 1;

        // Copy the carry bit into bit 0
        if (cpu.isC())
            value |= 0x01;

        // Set carry bit to the value of bit 7
        originalValue & 0x80 ? cpu.setC() : cpu.clearC();

        // Set the N flag to the value of bit 7
        // 2's complement negative
        value & 0x80 ? cpu.setN() : cpu.clearN();

        value == 0x00 ? cpu.setZ() : cpu.clearZ();

        return value;
    }

    void CPU::InstructionRotateLeft::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC ROTATE LEFT " << std::endl;

        switch (dst)
        {
        case InstructionTarget::MEMORY: // M -> M
            switch (src)
            {
            case InstructionTarget::MEMORY:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);

                value = rotateLeft(value);
                cpu.addressSpace.write(address, value);
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
                cpu.A(rotateLeft(cpu.A()));
                break;
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
    // ..... Implied

    // ----- Accumulator

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

    /**
     * @brief Rotate bits to the right, through the carry flag.
     *
     * Rotate bits to the right, with bit 0 shifted into the carry flag
     * and the carry flag shifted into bit 7.
     * S V B D I Z C
     * x 0 0 0 0 x x <+
     *             |  |
     * +<----<-----+  |
     * - - - - - - -  | value
     * 7 6 4 3 2 1 0>-+
     *
     * @param value the byte to rotate
     * @return hardware::Byte
     */
    hardware::Byte CPU::InstructionRotateRight::rotateRight(hardware::Byte value)
    {
        hardware::Byte originalValue = value;
        // Shift the bits to the right
        value >>= 1;

        // Copy the carry bit into bit 7
        if (cpu.isC())
            value |= 0x80;

        // Set carry bit to the value of bit 0
        originalValue & 0x01 ? cpu.setC() : cpu.clearC();

        // Set the N flag to the value of bit 7
        // 2's complement negative
        value & 0x80 ? cpu.setN() : cpu.clearN();

        value == 0x00 ? cpu.setZ() : cpu.clearZ();

        return value;
    }

    void CPU::InstructionRotateRight::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // std::cout << "   GENERIC ROTATE RIGHT " << std::endl;

        switch (dst)
        {
        case InstructionTarget::MEMORY: // M -> M
            switch (src)
            {
            case InstructionTarget::MEMORY:
            {
                hardware::Address address = cpu.decodePipeline().addressMode->execute();
                hardware::Byte value = cpu.addressSpace.read(address);

                value = rotateRight(value);
                cpu.addressSpace.write(address, value);
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
                cpu.A(rotateRight(cpu.A()));
                break;
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
