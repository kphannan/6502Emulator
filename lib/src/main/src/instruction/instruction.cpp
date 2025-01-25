

// TODO should be temporary
#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "InstructionSet.hpp"

namespace m6502
{

    const char *CPU::Instruction::targetName(InstructionTarget target)
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
        case InstructionTarget::MEMORY:
            name = "Memory";
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
        }

        return name;
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
