

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

    // ----- Generic Store -----
    void CPU::InstructionStore::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);
        // TODO define a src and dst that is set during opcode decode
        // src: memory - addressMode
        // dst: A register
        // cpu.A(cpu.decodePipeline().operand);
        // TODO Need to deal with implied, Accumulator, Immediate addressingModes
        // TODO possible lambda for retrieving the src and a setter on the destination
        hardware::Address address = cpu.decodePipeline().addressMode->execute();
        hardware::Byte value = cpu.addressSpace.read(address);
        switch (dst)
        {
        case InstructionTarget::MEMORY:
            switch (src)
            {
            case InstructionTarget::A:
                value = cpu.registers.A;
                break;
            case InstructionTarget::X:
                value = cpu.registers.X;
                break;
            case InstructionTarget::Y:
                value = cpu.registers.Y;
                break;
                // Mem -> Mem not a feature of CPU
            // case InstructionTarget::MEMORY:
            //     break;
            default:
                break;
            }
            break;
        case InstructionTarget::A:
            switch (src)
            {
            case InstructionTarget::MEMORY:
                cpu.A(value);
                break;
            default:
                break;
            }
            break;
        case InstructionTarget::X:
            break;
        case InstructionTarget::Y:
            switch (src)
            {
            case InstructionTarget::MEMORY:
                // cpu.addressSpace.write( address, cpu.registers.Y );
                value = cpu.registers.Y;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

        cpu.addressSpace.write(address, value);
    }

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

}
