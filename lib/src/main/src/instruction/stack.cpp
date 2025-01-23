

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----
    // ----- Stack () -----

    //----------------------------------------
    // Stack Instructions
    //
    // These instructions are implied mode, have a length of one byte and require machine cycles as indicated. The "PuLl" operations are known as "POP" on most other microprocessors. With the 6502, the stack is always on page one ($100-$1FF) and works top down.
    //
    // MNEMONIC                        HEX TIM
    // TXS (Transfer X to Stack ptr)   $9A  2
    // TSX (Transfer Stack ptr to X)   $BA  2
    // PHA (PusH Accumulator)          $48  3
    // PLA (PuLl Accumulator)          $68  4
    // PHP (PusH Processor status)     $08  3
    // PLP (PuLl Processor status)     $28  4
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
