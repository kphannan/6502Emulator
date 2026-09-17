

#include "6502.hpp"
#include "memory.hpp"
#include "AddressMode.hpp"
#include "InstructionSet.hpp"

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
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    void CPU::InstructionInterrupt::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

//        hardware::Address address = cpu.decodePipeline().addressMode->execute();
//        hardware::Address value = cpu.addressSpace.readAddress(address);
    }

    //----------------------------------------
    // BRK (BReaK)
    //
    // Affects Flags: B
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       BRK           $00  1   7
    //
    // BRK causes a non-maskable interrupt and increments the program counter by one.
    // Therefore an RTI will go to the address of the BRK +2 so that BRK may be used
    // to replace a two-byte instruction for debugging and the subsequent RTI will be
    // correct.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    void CPU::InstructionBreak::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        // BRK has a 2nd byte that is skipped and often used as a 'signature' byte for interrupts
        // cpu.PC(cpu.PC() + 1);
        cpu.registers.PC++;
        // Push Program Counter (after reading instruction and pad byte)
        cpu.push(cpu.registers.PC);
        cpu.push(cpu.registers.P); // Status register

        cpu.setI();
        cpu.clearD();
        cpu.setB();

        // Load the IRQ vector to the ProgramCounter
        hardware::Address irqVector = cpu.addressSpace.readAddress(hardware::Address(std::to_underlying(HardwareVector::IRQ)));

        cpu.PC(irqVector);
    }
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

    //----------------------------------------
    // RTI (ReTurn from Interrupt)
    //
    // Affects Flags: all
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       RTI           $40  1   6
    //
    // RTI retrieves the Processor Status Word (flags) and the Program Counter from the stack in that order (interrupts push the PC first and then the PSW).
    // Note that unlike RTS, the return address on the stack is the actual address rather than the address-1.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    void CPU::InstructionReturnFromInterrupt::execute(InstructionTarget dst, InstructionTarget src)
    {
        Instruction::execute(dst, src);

        cpu.P(cpu.pop());      // Status register
        cpu.PC(cpu.popWord()); // Program Counter
    }

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

    // ----- Other () -----
}
