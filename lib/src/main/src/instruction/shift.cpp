

#include "6502.hpp"
#include "memory.hpp"

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
