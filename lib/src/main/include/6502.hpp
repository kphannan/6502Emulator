

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

    // class CPU;

    // // ----- Forward Declarations of Inner classes
    // // --- Address Modes
    // class CPU::AddressMode; // base
    // class CPU::AddressModeImplied;
    // class CPU::AddressModeAccumulator;
    // class CPU::AddressModeZeroPage;
    // class CPU::AddressModeZeroPageIndexedX;
    // class CPU::AddressModeZeroPageIndexedY;
    // class CPU::AddressModeRelative;
    // class CPU::AddressModeAbsolute;
    // class CPU::AddressModeAbsoluteIndexedX;
    // class CPU::AddressModeAbsoluteIndexedY;
    // class CPU::AddressModeIndirect;
    // class CPU::AddressModeIndexedIndirectX;
    // class CPU::AddressModeIndirectIndexedY;
    // class CPU::AddressModeImmediate;
    // class CPU::AddressModeStack;

    // Identifies the source & destination of an operation
    enum class InstructionTarget
    {
        Undefined,
        // Register
        A,      // Accumulator
        X,      // X Index
        Y,      // Y Index
        S,      // Stack  Pointer
        PC,     // Program Counter
        PSR,    // Status Register
        FLAG_N, // negative
        FLAG_V, // overflow
        FLAG_B, // break
        FLAG_D, // decimal
        FLAG_I, // interrupt disable
        FLAG_Z, // zero
        FLAG_C, // carry
                // Memory
        MEMORY, // Location in the address space (use addressing mode)
        STACK   // Stack memory
    };

    // http://www.6502.org/users/obelisk/6502/registers.html
    typedef struct GeneralPurposeRegisters
    {
        hardware::Byte A; // Accumulator

        hardware::Byte Y; // Index register Y
        hardware::Byte X; // Index register X

        hardware::Byte P; // Processor status register

        hardware::Address S; // Stack Pointer

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
        // ===== Inner Classes =====
    public:
        class AddressMode
        {
            // Constructors
        protected:
            AddressMode(CPU &cpu, const char *name, const char *mnemonic) : cpu(cpu), modeName(name), modeMnemonic(mnemonic) {}

            // Fields
        private:
            const char *modeName;
            const char *modeMnemonic;

        protected:
            CPU &cpu;

            // Methods
        public:
            const char *name() const { return modeName; }
            const char *mnemonic() const { return modeMnemonic; }
            // TODO  this should return an address, a literal byte.... (flag bit, register(A,X,Y,PC,SP))
            virtual hardware::Address execute();
            // {
            //     std::cout << name() << " ; " << mnemonic() << std::endl;
            //     return hardware::Address(0xDEADBEEF);
            // }
        };

        //        Undefined,           // Catch illegal address mode
        class AddressModeUndefined : public AddressMode
        {
            // Constructors
        public:
            AddressModeUndefined(CPU &cpu) : AddressMode(cpu, "Undefined", "error") {}

            // Methods
        public:
            hardware::Address execute() override;
            // {
            //     std::cout << "AddressMode(Undefined): Not yet implemented" << std::endl;
            //     return hardware::Address(0xFADEFACE);
            // }
        };

        //        IMPLICIT,           // Implicit
        class AddressModeImplied : public AddressMode
        {
            // Constructors
        public:
            AddressModeImplied(CPU &cpu) : AddressMode(cpu, "Implicit", "") {}
            // Methods
        public:
            hardware::Address execute() override;
            // {
            //     std::cout << "AddressMode(Implied): Not yet implemented" << std::endl;
            //     return AddressMode::execute();
            // };
        };

        //        ACCUMULATOR,        // Accumulator         A
        class AddressModeAccumulator : public AddressMode
        {
            // Constructors
        public:
            AddressModeAccumulator(CPU &cpu) : AddressMode(cpu, "Accumulator", "A") {}
            // Methods
        public:
            virtual hardware::Address execute()
            {
                // TODO return 'Accumulator'
                std::cout << "AddressModeAccumulator: Not yet implemented" << std::endl;
                return AddressMode::execute();
            }
        };

        //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
        class AddressModeZeroPage : public AddressMode
        {
            // Constructors
        public:
            AddressModeZeroPage(CPU &cpu) : AddressMode(cpu, "ZeroPage", "$nn") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        ZERO_PAGE_X,        // Zero Page, X        $nn,X      LO bits 4,5,6
        class AddressModeZeroPageIndexedX : public AddressMode
        {
            // Constructors
        public:
            AddressModeZeroPageIndexedX(CPU &cpu) : AddressMode(cpu, "ZeroPage,X", "$nn,X") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        ZERO_PAGE_Y,        // Zero Page, Y        $nn,Y      LO bits 4,5,6
        class AddressModeZeroPageIndexedY : public AddressMode
        {
            // Constructors
        public:
            AddressModeZeroPageIndexedY(CPU &cpu) : AddressMode(cpu, "ZeroPage, Y", "$nn,Y") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        RELATIVE,           // Relative            $nn
        class AddressModeRelative : public AddressMode
        {
            // Constructors
        public:
            AddressModeRelative(CPU &cpu) : AddressMode(cpu, "Relative", "$nn") {}
            // Methods
        public:
            virtual hardware::Address execute() override
            {
                std::cout << "AddressMode(Relative): Not yet implemented" << std::endl;
                // TODO return 'byte - offset'
                return AddressMode::execute();
            }
        };

        //        ABSOLUTE,           // Absolute            $nnnn
        class AddressModeAbsolute : public AddressMode
        {
            // Constructors
        public:
            AddressModeAbsolute(CPU &cpu) : AddressMode(cpu, "Absolute", "$nnnn") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        ABSOLUTE_X,         // Absolute, X         $nnnn,X
        class AddressModeAbsoluteIndexedX : public AddressMode
        {
            // Constructors
        public:
            AddressModeAbsoluteIndexedX(CPU &cpu) : AddressMode(cpu, "Absolute,X", "$nnnn,X") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        ABSOLUTE_Y,         // Absolute, Y         $nnnn,Y
        class AddressModeAbsoluteIndexedY : public AddressMode
        {
            // Constructors
        public:
            AddressModeAbsoluteIndexedY(CPU &cpu) : AddressMode(cpu, "Absolute,Y", "$nnnn,Y") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        INDIRECT,           // Indirect            ($nnnn)
        class AddressModeIndirect : public AddressMode
        {
            // Constructors
        public:
            AddressModeIndirect(CPU &cpu) : AddressMode(cpu, "Indirect", "($nnnn)") {}
            // Methods
        public:
            virtual hardware::Address execute() override;
            // {
            //     AddressMode::execute();
            //     hardware::Address indirectAddress = cpu.addressSpace.readWord(cpu.registers.PC++);

            //     cpu.decodePipeline().operand = cpu.addressSpace.readWord(indirectAddress);

            //     // TODO return 'address'
            //     return indirectAddress;
            // }
        };

        //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn,X)    LO bit 1
        class AddressModeIndexedIndirectX : public AddressMode
        {
            // Constructors
        public:
            AddressModeIndexedIndirectX(CPU &cpu) : AddressMode(cpu, "X Indexed Indirect", "($nn,X)") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn),Y    LO bit 1
        class AddressModeIndirectIndexedY : public AddressMode
        {
            // Constructors
        public:
            AddressModeIndirectIndexedY(CPU &cpu) : AddressMode(cpu, "Y Indirect Indexed", "($nn),Y") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        //         IMMEDIATE,          // Immediate           #$nn
        class AddressModeImmediate : public AddressMode
        {
            // Types
        private:
        protected:
        public:
            // Constants
        private:
        protected:
        public:
            // Constructors
        private:
        protected:
        public:
            AddressModeImmediate(CPU &cpu) : AddressMode(cpu, "Immediate", "$#BB") {}
            // Fields
        private:
        protected:
        public:
            // Methods
        private:
        protected:
        public:
            hardware::Address execute() override;
            // Operators
        private:
        protected:
        public:
        };

        //        Stack (pseudo),      // Stack               $01nn
        class AddressModeStack : public AddressMode
        {
        public:
            const static hardware::Address stackPage = 0x0100;
            const static hardware::Address stackMask = 0x00FF;

            // Constructors
        public:
            AddressModeStack(CPU &cpu, const char *name, const char *mnemonic) : AddressMode(cpu, name, mnemonic) {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        class AddressModeStackPull : public AddressModeStack
        {
            // Constructors
        public:
            AddressModeStackPull(CPU &cpu) : AddressModeStack(cpu, "Stack Pull", "$01nn") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        class AddressModeStackPush : public AddressModeStack
        {
            // Constructors
        public:
            AddressModeStackPush(CPU &cpu) : AddressModeStack(cpu, "Stack Push", "$01nn") {}
            // Methods
        public:
            hardware::Address execute() override;
        };

        // // ----- Forward Declarations of Inner classes
        // // --- Address Modes
        // class AddressMode; // base
        // class AddressModeImplied;
        // class AddressModeAccumulator;
        // class AddressModeZeroPage;
        // class AddressModeZeroPageIndexedX;
        // class AddressModeZeroPageIndexedY;
        // class AddressModeRelative;
        // class AddressModeAbsolute;
        // class AddressModeAbsoluteIndexedX;
        // class AddressModeAbsoluteIndexedY;
        // class AddressModeIndirect;
        // class AddressModeIndexedIndirectX;
        // class AddressModeIndirectIndexedY;
        // class AddressModeImmediate;

        // friend class AddressMode; // base
        // friend class AddressModeImplied;
        // friend class AddressModeAccumulator;
        // friend class AddressModeZeroPage;
        // friend class AddressModeZeroPageIndexedX;
        // friend class AddressModeZeroPageIndexedY;
        // friend class AddressModeRelative;
        // friend class AddressModeAbsolute;
        // friend class AddressModeAbsoluteIndexedX;
        // friend class AddressModeAbsoluteIndexedY;
        // friend class AddressModeIndirect;
        // friend class AddressModeIndexedIndirectX;
        // friend class AddressModeIndirectIndexedY;
        // friend class AddressModeImmediate;

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
        // --- SBC *
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
        // --- CLC
        // --- CLD
        // --- CLI
        // --- CLV
        // --- SEC
        // --- SED
        // --- SEI
        // ===== Comparison Instructions
        // --- CMP *
        // --- CPX *
        // --- CPY *
        // ===== Conditional Branch Instructions (fmt: zzy10000)
        // --- BCC
        // --- BCS
        // --- BEQ
        // --- BMI
        // --- BNE
        // --- BPL
        // --- BVC
        // --- BVS
        // ===== Jumps & Subroutines Instructions
        // --- JUMP *
        // --- JSR
        // --- RTS
        // ===== Interrupts Instructions
        // --- BRK
        // --- RTI
        // ===== Other Instructions
        // --- BIT *
        class InstructionLogicalBit;
        // --- NOP

    protected:
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

        // Addressing Modes

        // Instruction code chart
        // https://www.masswerk.at/6502/6502_instruction_set.html
        enum class AddressModeKind
        {
            IMPLICIT,           // Implicit
            ACCUMULATOR,        // Accumulator         A
            IMMEDIATE,          // Immediate           #$nn
            ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
            ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6
            ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6
            RELATIVE,           // Relative            $nnnn
            ABSOLUTE,           // Absolute            $nnnn
            ABSOLUTE_X,         // Absolute, X         $nnnn, X
            ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y
            INDIRECT,           // Indirect            ($nnnn)
            INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
            INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
        };

        // Instruction Pipeline
        // fetch OpCode (advance PC)
        // decode OpCode
        // fetch operand (advance PC as defined by operand)
        // evaluate OpCode (set status flags)

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

            // Decode  the opcode and determine the addressing mode and read the operand
            void fetchOperand(AddressMode &addressMode);
            void evaluate();

            //  Registers

            // Operators
        public:
        protected:
        private:
        };
        // End of Pipeline inner class

    protected:
    public:
        // ----- Constants -----
    public:
        const hardware::Address StackPointerDefault = 0x01FF;

    protected:
    private:
        // ----- Attributes -----
    public:
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
        // --- SBC *
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
        // --- CLC
        // --- CLD
        // --- CLI
        // --- CLV
        // --- SEC
        // --- SED
        // --- SEI
        // ===== Comparison Instructions
        // --- CMP *
        // --- CPX *
        // --- CPY *
        // ===== Conditional Branch Instructions (fmt: zzy10000)
        // --- BCC
        // --- BCS
        // --- BEQ
        // --- BMI
        // --- BNE
        // --- BPL
        // --- BVC
        // --- BVS
        // ===== Jumps & Subroutines Instructions
        // --- JUMP *
        // --- JSR
        // --- RTS
        // ===== Interrupts Instructions
        // --- BRK
        // --- RTI
        // ===== Other Instructions
        // --- BIT *
        CPU::InstructionLogicalBit *_instructionLogicalBit;
        // --- NOP

        // ----- Constructors -----
    public:
        CPU();
        CPU(memory::Memory &memory);

        // ----- Methods -----
    public:
        Pipeline &decodePipeline() { return *pipeline; }

        void reset();

        // Access to registers (pipeline only)
        hardware::Byte &A() { return registers.A; };
        hardware::Byte &X() { return registers.X; };
        hardware::Byte &Y() { return registers.Y; };
        hardware::Address &PC() { return registers.PC; };
        hardware::Address &S() { return registers.S; };
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

        void S(hardware::Address value)
        {
            // TODO address the fact the stack pointer is 8 bits and lives in page 1
            // TODO mask / set the value
            // TODO - may change from Address to Byte and let addresMode handle the page
            registers.S = value;
        };

        void PC(hardware::Address value) { registers.PC = value; };

        // Organize these constants better
        const uint8_t NegativeBit = 7;
        const uint8_t OverflowBit = 6;
        const uint8_t BrkBit = 4;
        const uint8_t DecimalModeBit = 3;
        const uint8_t IrqDisableBit = 2;
        const uint8_t ZeroBit = 1;
        const uint8_t CarryBit = 0;

        // Processor status flags
        bool isB() { return registers.P & 1 << BrkBit; };         // BRK
        bool isC() { return registers.P & 1 << CarryBit; };       // Carry
        bool isD() { return registers.P & 1 << DecimalModeBit; }; // Decimal mode
        bool isI() { return registers.P & 1 << IrqDisableBit; };  // IRQ disable
        bool isN() { return registers.P & 1 << NegativeBit; };    // Negative
        bool isV() { return registers.P & 1 << OverflowBit; };    // Overflow
        bool isZ() { return registers.P & 1 << ZeroBit; };        // Zero

        void setB() { setBit(registers.P, BrkBit); };
        void setC() { setBit(registers.P, CarryBit); };
        void setD() { setBit(registers.P, DecimalModeBit); };
        void setI() { setBit(registers.P, IrqDisableBit); };
        void setN() { setBit(registers.P, NegativeBit); };
        void setV() { setBit(registers.P, OverflowBit); };
        void setZ() { setBit(registers.P, ZeroBit); };

        void clearB() { clearBit(registers.P, BrkBit); };
        void clearC() { clearBit(registers.P, CarryBit); };
        void clearD() { clearBit(registers.P, DecimalModeBit); };
        void clearI() { clearBit(registers.P, IrqDisableBit); };
        void clearN() { clearBit(registers.P, NegativeBit); };
        void clearV() { clearBit(registers.P, OverflowBit); };
        void clearZ() { clearBit(registers.P, ZeroBit); };

        void showRegisters();

        void executeFromAddress(hardware::Address address, uint32_t stepCount = 1);
        void execute(int numberOfInstructions = -1);

        memory::Memory &currentMemory() { return addressSpace; }
        // void CPU::memoryBank(memory::Memory &memory);

    protected:
    private:
        void statusFlagCheck(int bit)
        {
            if (!(bit >= 0 && bit <= 7))
            {
                throw std::out_of_range("Status bit invalid");
            }
        }

        void setBit(hardware::Byte &value, int bit)
        {
            statusFlagCheck(bit);

            // bit value 0 to 7
            value |= 1 << bit;
        }

        void clearBit(hardware::Byte &value, int bit)
        {
            statusFlagCheck(bit);

            // bit value 0 to 7
            value &= ~(1 << bit);
        }

        // maybe remove these and only use set/clear methods with no args
        void setB(bool value) { value == 1 ? setBit(registers.P, BrkBit) : clearBit(registers.P, BrkBit); };
        void setC(bool value) { value == 1 ? setBit(registers.P, CarryBit) : clearBit(registers.P, CarryBit); };
        void setD(bool value) { value == 1 ? setBit(registers.P, DecimalModeBit) : clearBit(registers.P, DecimalModeBit); };
        void setI(bool value) { value == 1 ? setBit(registers.P, IrqDisableBit) : clearBit(registers.P, IrqDisableBit); };
        void setN(bool value) { value == 1 ? setBit(registers.P, NegativeBit) : clearBit(registers.P, NegativeBit); };
        void setV(bool value) { value == 1 ? setBit(registers.P, OverflowBit) : clearBit(registers.P, OverflowBit); };
        void setZ(bool value) { value == 1 ? setBit(registers.P, ZeroBit) : clearBit(registers.P, ZeroBit); };
    };

}

#endif
