

#ifndef ADDRESSMODE_HPP
#define ADDRESSMODE_HPP

// #include <iostream>
// #include <iomanip>

#include "6502.hpp"

namespace m6502
{
    class CPU;

    // Addressing Modes

    // Instruction code chart
    // https://www.masswerk.at/6502/6502_instruction_set.html
    // enum class AddressModeKind
    // {
    //     IMPLICIT,           // Implicit
    //     ACCUMULATOR,        // Accumulator         A
    //     IMMEDIATE,          // Immediate           #$nn
    //     ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    //     ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6
    //     ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6
    //     RELATIVE,           // Relative            $nnnn
    //     ABSOLUTE,           // Absolute            $nnnn
    //     ABSOLUTE_X,         // Absolute, X         $nnnn, X
    //     ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y
    //     INDIRECT,           // Indirect            ($nnnn)
    //     INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
    //     INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
    // };

    // ============================================================================
    class CPU::AddressMode // TODO should be abstract
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
    };

    // ============================================================================
    //        Undefined,           // Catch illegal address mode
    class CPU::AddressModeUndefined : public AddressMode
    {
        // Constructors
        public:
            AddressModeUndefined(CPU &cpu) : AddressMode(cpu, "Undefined", "error") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        IMPLICIT,           // Implicit
    class CPU::AddressModeImplied : public AddressMode
    {
        public:
            // Constructors
            AddressModeImplied(CPU &cpu) : AddressMode(cpu, "Implied", "") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        ACCUMULATOR,        // Accumulator         A
    class CPU::AddressModeAccumulator : public AddressMode
    {
        public:
            // Constructors
        AddressModeAccumulator(CPU &cpu) : AddressMode(cpu, "Accumulator", "A") {}

        // Methods
            virtual hardware::Address execute()
            {
                // TODO return 'Accumulator'
                std::cout << "AddressModeAccumulator: Not yet implemented" << std::endl;
                return AddressMode::execute();
            }
    };

    // ============================================================================
    //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    class CPU::AddressModeZeroPage : public AddressMode
    {
        public:
            // Constructors
            AddressModeZeroPage(CPU &cpu) : AddressMode(cpu, "ZeroPage", "$nn") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        ZERO_PAGE_X,        // Zero Page, X        $nn,X      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedX : public AddressMode
    {
        public:
            // Constructors
            AddressModeZeroPageIndexedX(CPU &cpu) : AddressMode(cpu, "ZeroPage,X", "$nn,X") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        ZERO_PAGE_Y,        // Zero Page, Y        $nn,Y      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedY : public AddressMode
    {
        public:
            // Constructors
            AddressModeZeroPageIndexedY(CPU &cpu) : AddressMode(cpu, "ZeroPage, Y", "$nn,Y") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        RELATIVE,           // Relative            $nn
    class CPU::AddressModeRelative : public AddressMode
    {
        public:
            // Constructors
            AddressModeRelative(CPU &cpu) : AddressMode(cpu, "Relative", "$nn") {}

            // Methods
            virtual hardware::Address execute() override;
    };

    // ============================================================================
    //        ABSOLUTE,           // Absolute            $nnnn
    class CPU::AddressModeAbsolute : public AddressMode
    {
        public:
            // Constructors
            AddressModeAbsolute(CPU &cpu) : AddressMode(cpu, "Absolute", "$nnnn") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        ABSOLUTE_X,         // Absolute, X         $nnnn,X
    class CPU::AddressModeAbsoluteIndexedX : public AddressMode
    {
        // Constructors
        public:
            // Constructors
            AddressModeAbsoluteIndexedX(CPU &cpu) : AddressMode(cpu, "Absolute,X", "$nnnn,X") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        ABSOLUTE_Y,         // Absolute, Y         $nnnn,Y
    class CPU::AddressModeAbsoluteIndexedY : public AddressMode
    {
        public:
            // Constructors
            AddressModeAbsoluteIndexedY(CPU &cpu) : AddressMode(cpu, "Absolute,Y", "$nnnn,Y") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        INDIRECT,           // Indirect            ($nnnn)
    class CPU::AddressModeIndirect : public AddressMode
    {
        public:
            // Constructors
            AddressModeIndirect(CPU &cpu) : AddressMode(cpu, "Indirect", "($nnnn)") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn,X)    LO bit 1
    class CPU::AddressModeIndexedIndirectX : public AddressMode
    {
        public:
            // Constructors
            AddressModeIndexedIndirectX(CPU &cpu) : AddressMode(cpu, "X Indexed Indirect", "($nn,X)") {}

            // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn),Y    LO bit 1
    class CPU::AddressModeIndirectIndexedY : public AddressMode
    {
        public:
             // Constructors
           AddressModeIndirectIndexedY(CPU &cpu) : AddressMode(cpu, "Y Indirect Indexed", "($nn),Y") {}

           // Methods
            hardware::Address execute() override;
    };

    // ============================================================================
    //         IMMEDIATE,          // Immediate           #$nn
    class CPU::AddressModeImmediate : public AddressMode
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

    // ============================================================================
    //        Stack (pseudo),      // Stack               $01nn
    class CPU::AddressModeStack : public AddressMode // TODO should be abstract
    {
        public:
            const inline static hardware::Byte stackPage = 0x01;
            // const inline static hardware::Address stackMask = 0x01FF;

            // Constructors
            AddressModeStack(CPU &cpu, const char *name, const char *mnemonic) : AddressMode(cpu, name, mnemonic) {}

            // Methods
            hardware::Address execute() override;
    };

    class CPU::AddressModeStackPull : public AddressModeStack
    {
        // Constructors
        public:
            AddressModeStackPull(CPU &cpu) : AddressModeStack(cpu, "Stack Pull", "$01nn") {}

            // Methods
            hardware::Address execute() override;
    };

    class CPU::AddressModeStackPush : public AddressModeStack
    {
        // Constructors
        public:
            AddressModeStackPush(CPU &cpu) : AddressModeStack(cpu, "Stack Push", "$01nn") {}

            // Methods
            hardware::Address execute() override;
    };

}

#endif