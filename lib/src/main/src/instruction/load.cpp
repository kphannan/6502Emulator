

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

    // ----- Generic Load -----
    void CPU::InstructionLoad::execute()
    {
        Instruction::execute();
        // TODO define a src and dst that is set during opcode decode
        // src: memory - addressMode
        // dst: A register
        // cpu.A(cpu.decodePipeline().operand);
    }

    //----------------------------------------
    // LDA (LoaD Accumulator)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDA #$44      $A9  2   2
    // Zero Page     LDA $44       $A5  2   3
    // Zero Page,X   LDA $44,X     $B5  2   4
    // Absolute      LDA $4400     $AD  3   4
    // Absolute,X    LDA $4400,X   $BD  3   4+
    // Absolute,Y    LDA $4400,Y   $B9  3   4+
    // Indirect,X    LDA ($44,X)   $A1  2   6
    // Indirect,Y    LDA ($44),Y   $B1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    void CPU::InstructionLoadA::execute()
    {
        Instruction::execute();
        // TODO define a src and dst that is set during opcode decode
        // src: memory - addressMode
        // dst: A register
        cpu.A(cpu.decodePipeline().operand);
    }

    //----------------------------------------
    // LDX (LoaD X register)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDX #$44      $A2  2   2
    // Zero Page     LDX $44       $A6  2   3
    // Zero Page,Y   LDX $44,Y     $B6  2   4
    // Absolute      LDX $4400     $AE  3   4
    // Absolute,Y    LDX $4400,Y   $BE  3   4+
    //
    // + add 1 cycle if page boundary crossed
    void CPU::InstructionLoadX::execute()
    {
        Instruction::execute();
        // TODO define a src and dst that is set during opcode decode
        // src: memory - addressMode
        // dst: X register
        cpu.X(cpu.decodePipeline().operand);
    }

    //----------------------------------------
    // LDY (LoaD Y register)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDY #$44      $A0  2   2
    // Zero Page     LDY $44       $A4  2   3
    // Zero Page,X   LDY $44,X     $B4  2   4
    // Absolute      LDY $4400     $AC  3   4
    // Absolute,X    LDY $4400,X   $BC  3   4+
    //
    // + add 1 cycle if page boundary crossed

}