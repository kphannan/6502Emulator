

#include "6502.hpp"
#include "memory.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

/*
        switch ( src )
        {
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
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
            break;
            switch ( dst )
            {
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
            case InstructionTarget::MEMORY:
            case InstructionTarget::STACK:
            case InstructionTarget::IMPLIED:
                break;
            }
        }
*/

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

    //----------------------------------------
    // Branch Instructions
    //
    // Affect Flags: none
    //
    // All branches are relative mode and have a length of two bytes. Syntax is
    // "Bxx Displacement" or (better) "Bxx Label". See the notes on the Program Counter
    // for more on displacements.
    //
    // Branches are dependant on the status of the flag bits when the op code is encountered.
    // A branch not taken requires two machine cycles. Add one if the branch is taken and add
    // one more if the branch crosses a page boundary.
    //
    // MNEMONIC                       HEX
    // BPL (Branch on PLus)           $10
    // BMI (Branch on MInus)          $30
    // BVC (Branch on oVerflow Clear) $50
    // BVS (Branch on oVerflow Set)   $70
    // BCC (Branch on Carry Clear)    $90
    // BCS (Branch on Carry Set)      $B0
    // BNE (Branch on Not Equal)      $D0
    // BEQ (Branch on EQual)          $F0
    //
    // There is no BRA (BRanch Always) instruction but it can be easily emulated by branching
    // on the basis of a known condition. One of the best flags to use for this purpose is
    //  the oVerflow which is unchanged by all but addition and subtraction operations.
    // A page boundary crossing occurs when the branch destination is on a different page than
    // the instruction AFTER the branch instruction. For example:
    //
    //   SEC
    //   BCS LABEL
    //   NOP
    // A page boundary crossing occurs (i.e. the BCS takes 4 cycles) when (the address of)
    // LABEL and the NOP are on different pages. This means that
    //         CLV
    //         BVC LABEL
    //   LABEL NOP
    // the BVC instruction will take 3 cycles no matter what address it is located at.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ----- Relative $BB
    void CPU::InstructionBranch::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        // src: memory - addressMode
        // dst: A register
        // cpu.A(cpu.decodePipeline().operand);
        // std::cout << "   GENERIC LOAD " << std::endl;

        // hardware::Address address = cpu.decodePipeline().addressMode->execute();
        // hardware::Byte value = cpu.addressSpace.read(address);
        switch (src)
        {
        case InstructionTarget::FLAG_N:
            // set boolean for branch (true) or (false) from bit.
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
            switch (dst)
            {
            case InstructionTarget::PC:
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                break;
            }
            break;
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            break;
        }
    }

    void CPU::InstructionBranchCarryClear::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_C:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (!cpu.isC())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchCarrySet::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_C:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (cpu.isC())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchEqualToZero::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_Z:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (cpu.isZ())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchNotEqualToZero::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_Z:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (!cpu.isZ())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchMinus::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_N:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (cpu.isN())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }


    void CPU::InstructionBranchOnPlus::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_N:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (!cpu.isN())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_V:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchOverflowClear::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_V:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (!cpu.isV())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    void CPU::InstructionBranchOverflowSet::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        hardware::Address address = cpu.decodePipeline().addressMode->execute();

        switch (src)
        {
        case InstructionTarget::FLAG_V:
            switch (dst)
            {
            case InstructionTarget::PC:
                if (cpu.isV())
                    cpu.registers.PC = address;
                break;
            case InstructionTarget::A:
            case InstructionTarget::X:
            case InstructionTarget::Y:
            case InstructionTarget::S:
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
            case InstructionTarget::IMPLIED:
            default:
                throw std::domain_error(std::format("Invalid operation destination '{}'", targetName( src )));
                break;
            }
            break;
        case InstructionTarget::FLAG_N:
        case InstructionTarget::FLAG_B:
        case InstructionTarget::FLAG_D:
        case InstructionTarget::FLAG_I:
        case InstructionTarget::FLAG_Z:
        case InstructionTarget::FLAG_C:
        case InstructionTarget::A:
        case InstructionTarget::X:
        case InstructionTarget::Y:
        case InstructionTarget::S:
        case InstructionTarget::PC:
        case InstructionTarget::PSR:
        case InstructionTarget::MEMORY:
        case InstructionTarget::STACK:
        case InstructionTarget::IMPLIED:
        default:
            throw std::domain_error(std::format("Invalid operation source '{}'", targetName( src )));
            break;
        }
    }

    // ..... Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
