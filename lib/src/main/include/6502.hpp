

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include <stdexcept>
// #include <cstddef>
#include <iostream>
#include <iomanip>

#include "hardware.hpp"
#include "memory.hpp"

namespace m6502
{
    // forward declarations
    // Identifies the source & destination of an operation
    enum class InstructionTarget;

    // http://www.6502.org/users/obelisk/6502/registers.html
    typedef struct GeneralPurposeRegisters
    {
        hardware::Byte A; // Accumulator

        hardware::Byte Y; // Index register Y
        hardware::Byte X; // Index register X

        hardware::Byte P; // Processor status register

        hardware::StackAddress S; // Stack Pointer

        hardware::Address PC; // Program Counter

    } REGISTERS;

    /*
        Status Register Flags (bit 7 to bit 0)

        N	Negative
        V	Overflow
        -	ignored
        B	Break
        D	Decimal (use BCD for arithmetics)
        I	Interrupt (IRQ disable)
        Z	Zero
        C	Carry
        The zero flag (Z) indicates a value of all zero bits and the negative flag (N)
        indicates the presence of a set sign bit in bit-position 7. These flags are
        always updated, whenever a value is transferred to a CPU register (A,X,Y) and
        as a result of any logical ALU operations. The Z and N flags are also updated
        by increment and decrement operations acting on a memory location.

        The carry flag (C) flag is used as a buffer and as a borrow in arithmetic
        operations. Any comparisons will update this additionally to the Z and N flags,
        as do shift and rotate operations.

        All arithmetic operations update the Z, N, C and V flags.

        The overflow flag (V) indicates overflow with signed binary arithmetics.
        As a signed byte represents a range of -128 to +127, an overflow can never occur
        when the operands are of opposite sign, since the result will never exceed this
        range. Thus, overflow may only occur, if both operands are of the same sign.
        Then, the result must be also of the same sign. Otherwise, overflow is detected
        and the overflow flag is set. (I.e., both operands have a zero in the sign
        position at bit 7, but bit 7 of the result is 1, or, both operands have the
        sign-bit set, but the result is positive.)

        The decimal flag (D) sets the ALU to binary coded decimal (BCD) mode for
        additions and subtractions (ADC, SBC).

        The interrupt inhibit flag (I) blocks any maskable interrupt requests (IRQ).

        The break flag (B) is not an actual flag implemented in a register, and rather
        appears only, when the status register is pushed onto or pulled from the stack.
        When pushed, it will be 1 when transfered by a BRK or PHP instruction, and zero
        otherwise (i.e., when pushed by a hardware interrupt). When pulled into the
        status register (by PLP or on RTI), it will be ignored.

        In other words, the break flag will be inserted, whenever the status register
        is transferred to the stack by software (BRK or PHP), and will be zero, when
        transferred by hardware. Since there is no actual slot for the break flag, it
        will be always ignored, when retrieved (PLP or RTI). The break flag is not
        accessed by the CPU at anytime and there is no internal representation.
        Its purpose is more for patching, to discern an interrupt caused by a BRK
        instruction from a normal interrupt initiated by hardware.

        Any of these flags (but the break flag) may be set or cleared by dedicated
        instructions. Moreover, there are branch instructions to conditionally divert
        the control flow depending on the respective state of the Z, N, C or V flag.
    */

    // Addressing Modes
    // Implicit
    // Accumulator         A
    // Immediate           #$nn
    // Zero Page           $nn        LO bits 4,5,6
    // Zero Page,X         $nn,X      LO bits 4,5,6
    // Zero Page,Y         $nn,Y      LO bits 4,5,6
    // Relative            $nnnn
    // Absolute            $nnnn
    // Absolute,X          $nnnn,X
    // Absolute,Y          $nnnn,Y
    // Indirect            ($nnnn)
    // X Indexed Indirect  ($nn,X)    LO bit 1
    // Y Indirect Indexed  ($nn),Y    LO bit 1

    // Hardware / Software Vectors
    enum class HardwareVector
    {
        NMI = 0xFFFA,
        RESET = 0xFFFC,
        IRQ = 0xFFFE
    };

    /**
     * @brief A model of the registers (programming model) of the 6502 microprocessor.
     *
     */
    class CPU
    {
        // Forward declarations
        protected:
            class Pipeline;

        // ===== Inner Classes =====
        public:
            class AddressMode;

            //        Undefined,           // Catch illegal address mode
            class AddressModeUndefined;

            //        IMPLICIT,           // Implicit
            class AddressModeImplied;

            //        ACCUMULATOR,        // Accumulator         A
            class AddressModeAccumulator;

            //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
            class AddressModeZeroPage;

            //        ZERO_PAGE_X,        // Zero Page, X        $nn,X      LO bits 4,5,6
            class AddressModeZeroPageIndexedX;

            //        ZERO_PAGE_Y,        // Zero Page, Y        $nn,Y      LO bits 4,5,6
            class AddressModeZeroPageIndexedY;

            //        RELATIVE,           // Relative            $nn
            class AddressModeRelative;

            //        ABSOLUTE,           // Absolute            $nnnn
            class AddressModeAbsolute;

            //        ABSOLUTE_X,         // Absolute, X         $nnnn,X
            class AddressModeAbsoluteIndexedX;

            //        ABSOLUTE_Y,         // Absolute, Y         $nnnn,Y
            class AddressModeAbsoluteIndexedY;

            //        INDIRECT,           // Indirect            ($nnnn)
            class AddressModeIndirect;

            //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn,X)    LO bit 1
            class AddressModeIndexedIndirectX;

            //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn),Y    LO bit 1
            class AddressModeIndirectIndexedY;

            //         IMMEDIATE,          // Immediate           #$nn
            class AddressModeImmediate;

            const inline static hardware::Byte stackPage = 0x01;      // TODO move
            // const inline static hardware::Address stackMask = 0x01FF; // TODO move
            //        Stack (pseudo),      // Stack               $01nn
            class AddressModeStack;

            class AddressModeStackPull;

            class AddressModeStackPush;

            // --- Instruction
            class Instruction; // base
            class InstructionIllegal;

            // ===== Transfer Instructions
            // ----- Load
            class InstructionLoad;
            // --- LDA *
            class InstructionLoadA;
            // --- LDX *
            class InstructionLoadX;
            // --- LDY *
            class InstructionLoadY;
            //

            // ----- Store
            class InstructionStore;
            // --- STA *
            // --- STX *
            // --- STY *

            // ----- Interregister transfer
            class InstructionTransfer;
            // --- TAX
            // --- TAY
            // --- TSX
            // --- TXA
            // --- TXS
            // --- TYA

            // ===== Stack Instructions
            class InstructionStack;
            // --- PHA
            // --- PHP
            // --- PLA
            // --- PLP
            // ===== Decrement & Increment
            class InstructionDecrement;
            class InstructionIncrement;
            // --- DEC *
            // --- DEX
            class InstructionDecrementX;
            // --- DEY
            class InstructionDecrementY;
            // --- INC *
            // --- INX
            class InstructionIncrementX;
            // --- INY
            class InstructionIncrementY;
            // ===== Arithmetic Instructions
            // --- ADC *
            class InstructionAdd;
            // --- SBC *
            class InstructionSubtract;
            // ===== Logical Instructions
            class InstructionLogical;
            // --- AND *
            class InstructionLogicalAnd;
            // --- EOR *
            class InstructionLogicalXor;
            // --- ORA *
            class InstructionLogicalOr;
            // ===== Shift & Rotate Instructions
            // --- ASL *
            class InstructionShiftLeft;
            // --- LSR *
            class InstructionShiftRight;
            // --- ROL *
            class InstructionRotateLeft;
            // --- ROR *
            class InstructionRotateRight;
            // ===== Flag Instructions
            class InstructionFlagClear;
            class InstructionFlagSet;
            // --- CLC
            // --- CLD
            // --- CLI
            // --- CLV
            // --- SEC
            // --- SED
            // --- SEI
            // ===== Comparison Instructions
            class InstructionCompare;
            // --- CMP *
            // --- CPX *
            // --- CPY *
            // ===== Conditional Branch Instructions (fmt: zzy10000)
            class InstructionBranch;
            // --- BCC
            class InstructionBranchCarryClear;
            // --- BCS
            class InstructionBranchCarrySet;
            // --- BEQ
            class InstructionBranchEqualToZero;
            // --- BMI
            class InstructionBranchMinus;
            // --- BNE
            class InstructionBranchNotEqualToZero;
            // --- BPL
            class InstructionBranchOnPlus;
            // --- BVC
            class InstructionBranchOverflowClear;
            // --- BVS
            class InstructionBranchOverflowSet;
            // ===== Jumps & Subroutines Instructions
            class InstructionChangeProgramCounter;
            // --- JUMP *
            class InstructionJump;
            // --- JSR
            class InstructionJumpSubroutine;
            // --- RTS
            class InstructionReturnFromSubroutine;
            // ===== Interrupts Instructions
            class InstructionInterrupt;
            // --- BRK
            class InstructionBreak;
            // --- RTI
            class InstructionReturnFromInterrupt;
            // ===== Other Instructions
            // --- BIT *
            class InstructionLogicalBit;
            // --- NOP
            class InstructionNoOp;

        private:
            // TODO maybe split to instruction set class
            union OpCode
            {
                // raw opcode byte
                hardware::Byte value;

                // format of memory access (r/w) instructions
                struct MemoryAccess
                {
                    unsigned c : 2; // 1..0
                    unsigned b : 3; // 4..2
                    unsigned a : 3; // 7..5
                    int s : 1;
                } memory;

                // Branching on flag bits follow this format
                struct BranchFormat
                {
                    unsigned z : 4; // 3..0
                    unsigned o : 1; // 4..4
                    unsigned y : 1; // 5..5
                    unsigned x : 2; // 7..6
                    int s : 1;
                } branch;
            };

            // Instruction Pipeline
            // fetch OpCode (advance PC)
            // decode OpCode
            // fetch operand (advance PC as defined by operand)
            // evaluate OpCode (set status flags)

            // ----- Constants -----
        public:
            const hardware::Address StackPointerDefault = hardware::Address( 0x01FF );

            // ----- Attributes -----
        protected:
            friend class CPU::InstructionTransfer;
            REGISTERS registers;

        private:
            memory::Memory &addressSpace; // make a reference

            hardware::Byte instruction;
            CPU::Pipeline *pipeline;

            // -----
            CPU::AddressMode *_addressModeUndefined;
            CPU::AddressModeImplied *_addressModeImplied;
            CPU::AddressModeAccumulator *_addressModeAccumulator;
            CPU::AddressModeZeroPage *_addressModeZeroPage;
            CPU::AddressModeZeroPageIndexedX *_addressModeZeroPageIndexedX;
            CPU::AddressModeZeroPageIndexedY *_addressModeZeroPageIndexedY;
            CPU::AddressModeRelative *_addressModeRelative;
            CPU::AddressModeAbsolute *_addressModeAbsolute;
            CPU::AddressModeAbsoluteIndexedX *_addressModeAbsoluteIndexedX;
            CPU::AddressModeAbsoluteIndexedY *_addressModeAbsoluteIndexedY;
            CPU::AddressModeIndirect *_addressModeIndirect;
            CPU::AddressModeIndexedIndirectX *_addressModeIndexedIndirectX;
            CPU::AddressModeIndirectIndexedY *_addressModeIndirectIndexedY;
            CPU::AddressModeImmediate *_addressModeImmediate;
            // CPU::AddressModeStack *_addressModeStack;
            CPU::AddressModeStackPull *_addressModeStackPull;
            CPU::AddressModeStackPush *_addressModeStackPush;

            // TODO get the actual operations
            CPU::Instruction *_instructionUndefined;
            // ----- Transfer

            // ===== Transfer Instructions
            // ----- Load
            CPU::InstructionLoad *_instructionLoad;
            // --- LDA *
            CPU::InstructionLoad *_instructionLoadA;
            // --- LDX *
            CPU::InstructionLoad *_instructionLoadX;
            // --- LDY *
            CPU::InstructionLoad *_instructionLoadY;
            //

            // ----- Store
            CPU::InstructionStore *_instructionStore;
            // --- STA *
            // --- STX *
            // --- STY *

            // ----- Interregister transfer
            CPU::InstructionTransfer *_instructionTransfer;
            // --- TAX
            CPU::InstructionTransfer *_instructionTransferAtoX;
            // --- TAY
            CPU::InstructionTransfer *_instructionTransferAtoY;
            // --- TSX
            CPU::InstructionTransfer *_instructionTransferStoX;
            // --- TXA
            CPU::InstructionTransfer *_instructionTransferXtoA;
            // --- TXS
            CPU::InstructionTransfer *_instructionTransferXtoS;
            // --- TYA
            CPU::InstructionTransfer *_instructionTransferYtoA;

            // ===== Stack Instructions
            CPU::InstructionStack *_instructionStack;
            // --- PHA
            // --- PHP
            // --- PLA
            // --- PLP
            // ===== Decrement & Increment
            CPU::InstructionDecrement *_instructionDecrement;
            CPU::InstructionIncrement *_instructionIncrement;
            // --- DEC *
            // --- DEX
            CPU::InstructionDecrement *_instructionDecrementX; // TODO use general Decrement
            // --- DEY
            CPU::InstructionDecrement *_instructionDecrementY; // TODO use general Decrement
            // --- INC *
            // --- INX
            CPU::InstructionIncrement *_instructionIncrementX; // TODO use general Increment
            // --- INY
            CPU::InstructionIncrement *_instructionIncrementY; // TODO use general Increment
            // ===== Arithmetic Instructions
            // --- ADC *
            CPU::InstructionAdd *_instructionAdd;
            // --- SBC *
            CPU::InstructionSubtract *_instructionSubtract;
            // ===== Logical Instructions
            CPU::InstructionLogical *_instructionLogical;
            // --- AND *
            CPU::InstructionLogicalAnd *_instructionLogicalAnd;
            // --- EOR *
            CPU::InstructionLogicalXor *_instructionLogicalXor;
            // --- ORA *
            CPU::InstructionLogicalOr *_instructionLogicalOr;
            // ===== Shift & Rotate Instructions
            // --- ASL *
            CPU::InstructionShiftLeft *_instructionShiftLeft;
            // --- LSR *
            CPU::InstructionShiftRight *_instructionShiftRight;
            // --- ROL *
            CPU::InstructionRotateLeft *_instructionRotateLeft;
            // --- ROR *
            CPU::InstructionRotateRight *_instructionRotateRight;
            // ===== Flag Instructions
            CPU::InstructionFlagClear *_instructionFlagClear;
            CPU::InstructionFlagSet *_instructionFlagSet;
            // --- CLC
            // --- CLD
            // --- CLI
            // --- CLV
            // --- SEC
            // --- SED
            // --- SEI
            // ===== Comparison Instructions
            CPU::InstructionCompare *_instructionCompare;
            // --- CMP *
            // --- CPX *
            // --- CPY *
            // ===== Conditional Branch Instructions (fmt: zzy10000)
            CPU::InstructionBranch *_instructionBranch;
            // --- BCC
            class CPU::InstructionBranchCarryClear *_instructionBranchCarryClear;
            // --- BCS
            class CPU::InstructionBranchCarrySet *_instructionBranchCarrySet;
            // --- BEQ
            class CPU::InstructionBranchEqualToZero *_instructionBranchEqualToZero;
            // --- BMI
            class CPU::InstructionBranchMinus *_instructionBranchMinus;
            // --- BNE
            class CPU::InstructionBranchNotEqualToZero *_instructionBranchNotEqualToZero;
            // --- BPL
            class CPU::InstructionBranchOnPlus *_instructionBranchOnPlus;
            // --- BVC
            class CPU::InstructionBranchOverflowClear *_instructionBranchOverflowClear;
            // --- BVS
            class CPU::InstructionBranchOverflowSet *_instructionBranchOverflowSet;

            // ===== Jumps & Subroutines Instructions
            // --- JUMP *
            CPU::InstructionJump *_instructionJump;
            // --- JSR
            CPU::InstructionJumpSubroutine *_instructionJumpSubroutine;
            // --- RTS
            CPU::InstructionReturnFromSubroutine *_instructionReturnFromSubroutine;
            // ===== Interrupts Instructions
            // --- BRK
            CPU::InstructionBreak *_instructionBreak;
            // --- RTI
            CPU::InstructionReturnFromInterrupt *_instructionReturnFromInterrupt;
            // ===== Other Instructions
            // --- BIT *
            CPU::InstructionLogicalBit *_instructionLogicalBit;
            // --- NOP
            CPU::InstructionNoOp *_instructionNoOp;

            // ----- Constructors -----
        public:
            CPU();
            // CPU(const CPU *other);
            CPU(memory::Memory &memory);

            ~CPU();

            // ----- Methods -----
        public:
            Pipeline &decodePipeline() { return *pipeline; }

            void reset();

            // Access to registers (pipeline only)
            hardware::Byte &A() { return registers.A; };
            hardware::Byte &X() { return registers.X; };
            hardware::Byte &Y() { return registers.Y; };
            //        hardware::Address &PC() { return registers.PC.address; };
            hardware::Address &PC() { return registers.PC; };
            hardware::Byte &PCH() { return registers.PC.value.hi; };
            hardware::Byte &PCL() { return registers.PC.value.lo; };
            //        Registers::ProgramCounter &PC() { return registers.PC; };
            hardware::StackAddress &S() { return registers.S; };
            // Processor status byte
            hardware::Byte &P() { return registers.P; };

            void A(hardware::Byte value)
            {
                registers.A = value;
                setZ(value == 0);
                setN((value & 0b10000000) != 0); // 2s compliment bit 7 is sign bit
            };

            void X(hardware::Byte value)
            {
                registers.X = value;
                setZ(value == 0);
                setN((value & 0b10000000) != 0); // 2s compliment bit 7 is sign bit
            };

            void Y(hardware::Byte value)
            {
                registers.Y = value;
                setZ(value == 0);
                setN((value & 0b10000000) != 0); // 2s compliment bit 7 is sign bit
            };

            void P(hardware::Byte value)
            {
                registers.P = value;
            };

            void S(const hardware::StackAddress& value)
            {
                registers.S = value;
            };

            void S(hardware::Byte value)
            {
                registers.S.value.lo = value;
            };

            void PC(const hardware::Address& value) { registers.PC.value.address = value.value.address; };
            void PC(const hardware::Word& value) { registers.PC.value.address.word = value.word; }
            void PC(const unsigned short value) { registers.PC.value.address.word = value; }
            // void PC(const unsigned int value) { registers.PC.address.word = value; }
            void PC(const hardware::Byte hi, const hardware::Byte lo)
            {
                registers.PC.value.address.hi = hi;
                registers.PC.value.address.lo = lo;
            };

            // This could be accomplished with a Union and bit fields.
            // Organize these constants better
            static const uint8_t NegativeBit = 7;
            static const uint8_t OverflowBit = 6;
            static const uint8_t BrkBit = 4;
            static const uint8_t DecimalModeBit = 3;
            static const uint8_t IrqDisableBit = 2;
            static const uint8_t ZeroBit = 1;
            static const uint8_t CarryBit = 0;
            // Masks for status bits
            static const uint8_t NegativeBitMask = 1 << NegativeBit;
            static const uint8_t OverflowBitMask = 1 << OverflowBit;
            static const uint8_t BrkBitMask = 1 << BrkBit;
            static const uint8_t DecimalModeBitMask = 1 << DecimalModeBit;
            static const uint8_t IrqDisableBitMask = 1 << IrqDisableBit;
            static const uint8_t ZeroBitMask = 1 << ZeroBit;
            static const uint8_t CarryBitMask = 1 << CarryBit;

            // Processor status flags
            bool isB() const { return registers.P & BrkBitMask; };         // BRK
            bool isC() const { return registers.P & CarryBitMask; };       // Carry
            bool isD() const { return registers.P & DecimalModeBitMask; }; // Decimal mode
            bool isI() const { return registers.P & IrqDisableBitMask; };  // IRQ disable
            bool isN() const { return registers.P & NegativeBitMask; };    // Negative
            bool isV() const { return registers.P & OverflowBitMask; };    // Overflow
            bool isZ() const { return registers.P & ZeroBitMask; };        // Zero

            hardware::Byte setB() { return setBit(registers.P, BrkBit); };
            hardware::Byte setC() { return setBit(registers.P, CarryBit); };
            hardware::Byte setD() { return setBit(registers.P, DecimalModeBit); };
            hardware::Byte setI() { return setBit(registers.P, IrqDisableBit); };
            hardware::Byte setN() { return setBit(registers.P, NegativeBit); };
            hardware::Byte setV() { return setBit(registers.P, OverflowBit); };
            hardware::Byte setZ() { return setBit(registers.P, ZeroBit); };

            hardware::Byte clearB() { return clearBit(registers.P, BrkBit); };
            hardware::Byte clearC() { return clearBit(registers.P, CarryBit); };
            hardware::Byte clearD() { return clearBit(registers.P, DecimalModeBit); };
            hardware::Byte clearI() { return clearBit(registers.P, IrqDisableBit); };
            hardware::Byte clearN() { return clearBit(registers.P, NegativeBit); };
            hardware::Byte clearV() { return clearBit(registers.P, OverflowBit); };
            hardware::Byte clearZ() { return clearBit(registers.P, ZeroBit); };

            void showRegisters();

            void executeFromAddress(const hardware::Address& address, uint32_t stepCount = 1);
            void executeFromAddress(const unsigned int address, uint32_t stepCount = 1);
            void execute(int numberOfInstructions = -1);

            memory::Memory &currentMemory() { return addressSpace; }
            // void CPU::memoryBank(memory::Memory &memory);

        private:
            void statusFlagCheck(int bit) const
            {
                if (!(bit >= 0 && bit <= 7))
                {
                    throw std::out_of_range("Status bit invalid");
                }
            }

            hardware::Byte setBit(const hardware::Byte &value, int bit) const
            {
                statusFlagCheck(bit);

                hardware::Byte tmp(value);

                // bit value 0 to 7
                tmp |= 1 << bit;

                return tmp;
            }

            hardware::Byte setBit(hardware::Byte &value, int bit) const
            {
                statusFlagCheck(bit);

                // bit value 0 to 7
                value |= 1 << bit;

                return value;
            }

            hardware::Byte clearBit(const hardware::Byte &value, int bit) const
            {
                statusFlagCheck(bit);

                hardware::Byte tmp(value);
                // bit value 0 to 7
                tmp &= ~(1 << bit);

                return tmp;
            }

            hardware::Byte clearBit(hardware::Byte &value, int bit) const
            {
                statusFlagCheck(bit);

                // bit value 0 to 7
                value &= ~(1 << bit);

                return value;
            }

            // maybe remove these and only use set/clear methods with no args
            hardware::Byte setB(bool value) { return value == 1 ? setBit(registers.P, BrkBit) : clearBit(registers.P, BrkBit); };
            hardware::Byte setC(bool value) { return value == 1 ? setBit(registers.P, CarryBit) : clearBit(registers.P, CarryBit); };
            hardware::Byte setD(bool value) { return value == 1 ? setBit(registers.P, DecimalModeBit) : clearBit(registers.P, DecimalModeBit); };
            hardware::Byte setI(bool value) { return value == 1 ? setBit(registers.P, IrqDisableBit) : clearBit(registers.P, IrqDisableBit); };
            hardware::Byte setN(bool value) { return value == 1 ? setBit(registers.P, NegativeBit) : clearBit(registers.P, NegativeBit); };
            hardware::Byte setV(bool value) { return value == 1 ? setBit(registers.P, OverflowBit) : clearBit(registers.P, OverflowBit); };
            hardware::Byte setZ(bool value) { return value == 1 ? setBit(registers.P, ZeroBit) : clearBit(registers.P, ZeroBit); };

            void push(const hardware::Byte &value);
            void push(const hardware::Word &value);
            void push(const hardware::Address &value);
            hardware::Byte pop();
            hardware::Word popWord();
            hardware::Address popAddress();


        protected:

            class Pipeline
            {
                    // Constants
                private:
                protected:
                public:
                    // Fields
                public:
                    // TODO not good to be public
                    int operand; // struct/union/class (register,implied,Byte,Word)
                    Instruction *cpuInstruction;
                    CPU::AddressMode *addressMode;

                    InstructionTarget src;
                    InstructionTarget dst;

                    // Lambdas....
                    // source;
                    // destination;

                protected:
                private:
                    CPU &cpu;

                    OpCode opCode;
                    // int opcode;
                    // AddressModeKind addressModeKind;

                    // Constructors
                public:
                    /** Configure the instruction pipeline with the address space and program counter. */
                    // Pipeline(memory::Memory &memory, hardware::Address &address);
                    Pipeline(CPU &cpu);

                    // ~Pipeline();
                protected:
                private:
                    // Methods
                public:
                    void reset(hardware::Address address);
                    // Reset the pipeline, removing any instructions being decoded.
                    void clear();
                    void execute();
                    void execute(int numberOfSteps);
                    void showPipeline() const;

                protected:
                private:
                    // Read an opcode from the cuurrent memory address
                    void fetchOpCode();
                    AddressMode &decodeAddressMode(const OpCode instruction);
                    void showAddressMode(const AddressMode &addressMode) const;
                    void decodeOperation(const OpCode instruction);

                    void evaluate();

                    //  Registers

                    // Operators
                public:
                protected:
                private:
            };
            // End of Pipeline inner class


    };

    // ===== Address Modes =====

    // ===== Instructions =====

}

#endif
