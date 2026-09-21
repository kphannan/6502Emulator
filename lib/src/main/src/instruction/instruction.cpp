

// TODO should be temporary
#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "InstructionSet.hpp"

namespace m6502
{

    const char *CPU::Instruction::targetName(const InstructionTarget& target)
    {
        const char *name;

        switch (target)
        {
        case InstructionTarget::Undefined:
            name = "Undefined";
            break;
        case InstructionTarget::A:
            name = "A";
            break;
        case InstructionTarget::X:
            name = "X";
            break;
        case InstructionTarget::Y:
            name = "Y";
            break;
        case InstructionTarget::S:
            name = "S";
            break;
        case InstructionTarget::PC:
            name = "PC";
            break;
        case InstructionTarget::PSR:
            name = "PSR";
            break;
        case InstructionTarget::MEMORY:
            name = "Memory";
            break;
        case InstructionTarget::STACK:
            name = "Stack";
            break;
        case InstructionTarget::FLAG_N:
            name = "N";
            break;
        case InstructionTarget::FLAG_V:
            name = "V";
            break;
        case InstructionTarget::FLAG_B:
            name = "B";
            break;
        case InstructionTarget::FLAG_D:
            name = "D";
            break;
        case InstructionTarget::FLAG_I:
            name = "I";
            break;
        case InstructionTarget::FLAG_Z:
            name = "Z";
            break;
        case InstructionTarget::FLAG_C:
            name = "C";
            break;
        case InstructionTarget::IMPLIED:
            name = "Implied";
            break;
        default:
            name = "Illegal value";
            break;
        }

        return name;
    }


    std::ostream& operator<<(std::ostream& os, const InstructionTarget& target)
    {
        switch( target )
        {
            case InstructionTarget::Undefined:
                return os << "Undefined";
            case InstructionTarget::A:      // Accumulator
                return os << "A";
            case InstructionTarget::X:      // X Index
                return os << "X";
            case InstructionTarget::Y:      // Y Index
                return os << "Y";
            case InstructionTarget::S:      // Stack  Pointer
                return os << "S";
            case InstructionTarget::PC:     // Program Counter
                return os << "PC";
            case InstructionTarget::PSR:    // Status Register
                return os << "PSR";
            case InstructionTarget::FLAG_N: // negative
                return os << "FLAG_N";
            case InstructionTarget::FLAG_V: // overflow
                return os << "FLAG_V";
            case InstructionTarget::FLAG_B: // break
                return os << "FLAG_B";
            case InstructionTarget::FLAG_D: // decimal
                return os << "FLAG_D";
            case InstructionTarget::FLAG_I: // interrupt disable
                return os << "FLAG_I";
            case InstructionTarget::FLAG_Z: // zero
                return os << "FLAG_Z";
            case InstructionTarget::FLAG_C: // carry
                return os << "FLAG_C";
            case InstructionTarget::MEMORY: // Location in the address space (use addressing mode)
                return os << "MEMORY";
            case InstructionTarget::STACK:  // Stack memory
                return os << "STACK";
            case InstructionTarget::IMPLIED:// Implied - no target outside of the instruction
                return os << "IMPLIED";

            default:
                return os << "Unknown";
        }
    }


    // Addressing Modes
    // ----- Immediate #$BB
    // ----- Implied
    // ----- Accumulator
    // ----- ZeroPage $LL
    // ----- ZeroPage,X $LL,X
    // ----- ZeroPage,Y $LL,Y
    // ----- Relative $BB
    // ----- Absolute $LLHH
    // ----- AbsoluteX $LLHH,X
    // ----- AbsoluteY $LLHH,Y
    // ----- Indirect ($LLHH)
    // ----- Indexed Indirect X ($LL,X)
    // ----- Indirect Indexed Y ($LL),Y

    // ----- Generic Logical -----

}
