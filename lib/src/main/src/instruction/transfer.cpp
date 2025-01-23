

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----

    //----------------------------------------
    // Register Instructions
    //
    // Affect Flags: N Z
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                 HEX
    // TAX (Transfer A to X)    $AA
    // TXA (Transfer X to A)    $8A
    // DEX (DEcrement X)        $CA
    // INX (INcrement X)        $E8
    // TAY (Transfer A to Y)    $A8
    // TYA (Transfer Y to A)    $98
    // DEY (DEcrement Y)        $88
    // INY (INcrement Y)        $C8
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ..... Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Stack () -----
    // ----- Decrement & Increment () -----
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
