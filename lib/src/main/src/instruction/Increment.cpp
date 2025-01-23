

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

    //----------------------------------------
    // DEC (DECrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     DEC $44       $C6  2   5
    // Zero Page,X   DEC $44,X     $D6  2   6
    // Absolute      DEC $4400     $CE  3   6
    // Absolute,X    DEC $4400,X   $DE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

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
    // INC (INCrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     INC $44       $E6  2   5
    // Zero Page,X   INC $44,X     $F6  2   6
    // Absolute      INC $4400     $EE  3   6
    // Absolute,X    INC $4400,X   $FE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

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

    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
