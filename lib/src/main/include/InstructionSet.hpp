

#ifndef INSTRUCTIONSET_HPP
#define INSTRUCTIONSET_HPP

#include <iostream>
#include <iomanip>

#include "6502.hpp"

namespace m6502
{

    class CPU;

    // 6502 has 56 instructions

    class CPU::Instruction
    {
        // Types
    public:
    protected:
    private:
        // Constants
    public:
    protected:
    private:
        // Constructors
    public:
        Instruction(CPU &cpu, const char *name, const char *mnemonic) : cpu(cpu), modeName(name), modeMnemonic(mnemonic) {};

    protected:
    private:
        // Fields
    private:
        const char *modeName;
        const char *modeMnemonic;

    protected:
        CPU &cpu;

        // Methods
    public:
        const char *name() const { return modeName; };
        const char *mnemonic() const { return modeMnemonic; };
        virtual void execute() { std::cout << name() << " ; " << mnemonic() << " " << cpu.decodePipeline().addressMode->mnemonic() << std::endl; };

    protected:
    private:
        // Operators
    public:
    protected:
    private:
    };

    // ===== Transfer Instructions
    // ----- Load
    class CPU::InstructionLoad : public Instruction
    {
        // Constructors
    public:
        InstructionLoad(CPU &cpu) : Instruction(cpu, "Load instruction", "load") {}
        // InstructionLoad(CPU &cpu, const char *name, const char *mnemonic) {}

        // methods
    public:
        virtual void execute() { Instruction::execute(); };
    };
    // --- LDA
    // --- LDX
    // --- LDY
    //

    // ----- Store
    class CPU::InstructionStore : public Instruction
    {
        // Constructors
    public:
        InstructionStore(CPU &cpu) : Instruction(cpu, "Store instruction", "store") {}
        // InstructionLoad(CPU &cpu, const char *name, const char *mnemonic) {}

        // methods
    public:
        virtual void execute() { Instruction::execute(); };
    };
    // --- STA
    // --- STX
    // --- STY

    // ----- Interregister transfer
    // --- TAX
    // --- TAY
    // --- TSX
    // --- TXA
    // --- TXS
    // --- TYA

    // ===== Stack Instructions (push, pop)
    // --- PHA
    // --- PHP
    // --- PLA
    // --- PLP
    // ===== Decrement & Increment (increment, decrement)
    // --- DEC
    // --- DEX
    // --- DEY
    // --- INC
    // --- INX
    // --- INY
    // ===== Arithmetic Instructions (add, subtract)
    // --- ADC
    // --- SBC
    // ===== Logical Instructions (and, exlusive-or, or)
    class CPU::InstructionLogical : public Instruction
    {
        // Constructors
    public:
        InstructionLogical(CPU &cpu) : Instruction(cpu, "Logical instruction", "logical") {}

        // methods
    public:
        virtual void execute() { Instruction::execute(); };
    };
    // --- AND
    // --- EOR
    // --- ORA
    // ===== Shift & Rotate Instructions (shift, rotate both left and right)
    // --- ASL
    // --- LSR
    // --- ROL
    // --- ROR
    // ===== Flag Instructions (set, clear)
    // --- CLC
    // --- CLD
    // --- CLI
    // --- CLV
    // --- SEC
    // --- SED
    // --- SEI
    // ===== Comparison Instructions (compare)
    // --- CMP
    // --- CPX
    // --- CPY
    // ===== Conditional Branch Instructions (branch wiith compare)
    // --- BCC
    // --- BCS
    // --- BEQ
    // --- BMI
    // --- BNE
    // --- BPL
    // --- BVC
    // --- BVS
    // ===== Jumps & Subroutines Instructions (jump)
    // --- JUMP
    // --- JSR
    // --- RTS
    // ===== Interrupts Instructions (break, return)
    // --- BRK
    // --- RTI
    // ===== Other Instructions
    // --- BIT
    // --- NOP

    //

}

#endif // ifndef INSTRUCTIONSET_HPP

/*

Instructions by Type

Transfer Instructions

Load, store, interregister transfer

LDA
load accumulator
LDX
load X
LDY
load Y
STA
store accumulator
STX
store X
STY
store Y
TAX
transfer accumulator to X
TAY
transfer accumulator to Y
TSX
transfer stack pointer to X
TXA
transfer X to accumulator
TXS
transfer X to stack pointer
TYA
transfer Y to accumulator
Stack Instructions

These instructions transfer the accumulator or status register (flags) to and from the stack. The processor stack is a last-in-first-out (LIFO) stack of 256 bytes length, implemented at addresses $0100 - $01FF. The stack grows down as new values are pushed onto it with the current insertion point maintained in the stack pointer register.
(When a byte is pushed onto the stack, it will be stored in the address indicated by the value currently in the stack pointer, which will be then decremented by 1. Conversely, when a value is pulled from the stack, the stack pointer is incremented. The stack pointer is accessible by the TSX and TXS instructions.)

PHA
push accumulator
PHP
push processor status register (with break flag set)
PLA
pull accumulator
PLP
pull processor status register
Decrements & Increments

DEC
decrement (memory)
DEX
decrement X
DEY
decrement Y
INC
increment (memory)
INX
increment X
INY
increment Y
Arithmetic Operations

ADC
add with carry (prepare by CLC)
SBC
subtract with carry (prepare by SEC)
See the Primer of 6502 Arithmetic Instructions below for details.

Logical Operations

AND
and (with accumulator)
EOR
exclusive or (with accumulator)
ORA
(inclusive) or with accumulator
Shift & Rotate Instructions

All shift and rotate instructions preserve the bit shifted out in the carry flag.

ASL
arithmetic shift left (shifts in a zero bit on the right)
LSR
logical shift right (shifts in a zero bit on the left)
ROL
rotate left (shifts in carry bit on the right)
ROR
rotate right (shifts in zero bit on the left)
Flag Instructions

CLC
clear carry
CLD
clear decimal (BCD arithmetics disabled)
CLI
clear interrupt disable
CLV
clear overflow
SEC
set carry
SED
set decimal (BCD arithmetics enabled)
SEI
set interrupt disable
Comparisons

Generally, comparison instructions subtract the operand from the given register without affecting that register. Flags are still set as with a normal subtraction and thus the relation of the two values becomes accessible by the Zero, Carry and Negative flags.
(See the branch instructions below for how to evaluate flags.)

Relation R − Op	Z	C	N
Register < Operand	0	0	sign bit of result
Register = Operand	1	1	0
Register > Operand	0	1	sign bit of result
CMP
compare (with accumulator)
CPX
compare with X
CPY
compare with Y
Conditional Branch Instructions

Branch targets are relative, signed 8-bit address offsets.
(An offset of zero corresponds to the immedately following address. While it is perfectly feasible to calculate offsets by hand, more often these are computed by an assembler program from absoulte addresses or labels. In the latter case, branch instructions may look more like absolute address mode instructions, while taking in actuality just a relative offset as a single-byte operand.)

BCC
branch on carry clear
BCS
branch on carry set
BEQ
branch on equal            (zero flag set)
BMI
branch on minus            (negative flag set)
BNE
branch on not equal        (zero flag clear)
BPL
branch on plus             (negative flag clear)
BVC
branch on overflow clear
BVS
branch on overflow set
Jumps & Subroutines

JSR and RTS affect the stack as the return address is pushed onto or pulled from the stack, respectively.
(JSR will first push the high-byte of the return address [PC+2] onto the stack, then the low-byte. The stack will then contain, seen from the bottom or from the most recently added byte, [PC+2]-L [PC+2]-H.)

JMP
jump
JSR
jump subroutine
RTS
return from subroutine
Interrupts

A hardware interrupt (maskable IRQ and non-maskable NMI), will cause the processor to put first the address currently in the program counter onto the stack (in HB-LB order), followed by the value of the status register. (The stack will now contain, seen from the bottom or from the most recently added byte, SR PC-L PC-H with the stack pointer pointing to the address below the stored contents of status register.) Then, the processor will divert its control flow to the address provided in the two word-size interrupt vectors at $FFFA (IRQ) and $FFFE (NMI).
A set interrupt disable flag will inhibit the execution of an IRQ, but not of a NMI, which will be executed anyways.
The break instruction (BRK) behaves like a NMI, but will push the value of PC+2 onto the stack to be used as the return address. Also, as with any software initiated transfer of the status register to the stack, the break flag will be found set on the respective value pushed onto the stack. Then, control is transferred to the address in the NMI-vector at $FFFE.
In any way, the interrupt disable flag is set to inhibit any further IRQ as control is transferred to the interrupt handler specified by the respective interrupt vector.

The RTI instruction restores the status register from the stack and behaves otherwise like the JSR instruction. (The break flag is always ignored as the status is read from the stack, as it isn't a real processor flag anyway.)

BRK
break / software interrupt
RTI
return from interrupt
See the section Jump Vectors and Stack Operations below for operational details.

Other

BIT
bit test (accumulator & memory)
NOP
no operation
*/
