

// TODO should be temporary
#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
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
    void CPU::InstructionLogical::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // TODO define a src and dst that is set during opcode decode
        // src: memory - addressMode
        // dst: A register
        // cpu.A(cpu.decodePipeline().operand);
        std::cout << "   GENERIC LOGICAL " << std::endl;

        // TODO Need to deal with implied, Accumulator, Immediate addressingModes
        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);
        switch (dst)
        {
        case InstructionTarget::A:
            switch (src)
            {
            case InstructionTarget::MEMORY:
                cpu.A(cpu.A() | value);
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::X:
            // switch (src)
            // {
            // case InstructionTarget::MEMORY:
            //     cpu.X(value);
            //     break;
            // default:
            //     break;
            // }
            // break;
        case InstructionTarget::Y:
            // switch (src)
            // {
            // case InstructionTarget::MEMORY:
            //     cpu.Y(value);
            //     break;
            // default:
            //     break;
            // }
            // break;
        default:
            std::cout << "Illegal destination of a locical operation" << std::endl;
            break;
        }
    }

}
